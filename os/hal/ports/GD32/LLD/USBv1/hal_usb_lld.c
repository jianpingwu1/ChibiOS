/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

/**
 * @file    USBv1/hal_usb_lld.c
 * @brief   GD32 USB subsystem low level driver source.
 *
 * @addtogroup USB
 * @{
 */

#include <string.h>

#include "hal.h"

#if HAL_USE_USB || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* USB turnaround time, it depends on the AHB clock and on the selected
   PHY.*/
#define UTT_VALUE_FS            5
#define UTT_VALUE_HS            9

#define EP0_MAX_INSIZE          64
#define EP0_MAX_OUTSIZE         64

/* Interrupt sources that can be re-triggered while being served.*/
#define IRQ_RETRY_MASK          (GINTF_NPTXFEIF | GINTF_PTXFEIF |           \
                                 GINTF_RXFNEIF)

/* Interrupt sources reflecting FIFO or endpoint state, not W1C events.*/
#define IRQ_STATUS_MASK         (GINTF_NPTXFEIF | GINTF_PTXFEIF |           \
                                 GINTF_RXFNEIF | GINTF_IEPIF | GINTF_OEPIF)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USBHS0 driver identifier.*/
#if GD32_USB_USE_USBHS0 || defined(__DOXYGEN__)
USBDriver USBD0;
#endif

/** @brief USBHS1 driver identifier.*/
#if GD32_USB_USE_USBHS1 || defined(__DOXYGEN__)
USBDriver USBD1;
#endif

/** @brief Interrupt tracing counters, debug aid only.*/
#if GD32_USB_DEBUG_COUNTERS == TRUE
gd32_usb_debug_t gd32_usb_debug;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   EP0 state.
 * @note    It is an union because IN and OUT endpoints are never used at the
 *          same time for EP0.
 */
static union {
  /**
   * @brief   IN EP0 state.
   */
  USBInEndpointState in;
  /**
   * @brief   OUT EP0 state.
   */
  USBOutEndpointState out;
} ep0_state;

/**
 * @brief   Buffer for the EP0 setup packets.
 */
static uint8_t ep0setup_buffer[8];

/**
 * @brief   EP0 initialization structure.
 */
static const USBEndpointConfig ep0config = {
  USB_EP_MODE_TYPE_CTRL,
  _usb_ep0setup,
  _usb_ep0in,
  _usb_ep0out,
  0x40,
  0x40,
  &ep0_state.in,
  &ep0_state.out,
  1,
  ep0setup_buffer
};

#if GD32_USB_USE_USBHS0 || defined(__DOXYGEN__)
static const gd32_otg_params_t usbhs0params = {
  GD32_USB_USBHS0_RX_FIFO_SIZE / 4,
  GD32_USBHS0_FIFO_MEM_SIZE,
  GD32_USBHS0_ENDPOINTS,
  GD32_USB_USBHS0_HS
};
#endif

#if GD32_USB_USE_USBHS1 || defined(__DOXYGEN__)
static const gd32_otg_params_t usbhs1params = {
  GD32_USB_USBHS1_RX_FIFO_SIZE / 4,
  GD32_USBHS1_FIFO_MEM_SIZE,
  GD32_USBHS1_ENDPOINTS,
  GD32_USB_USBHS1_HS
};
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Waits for a hardware flag, the polling loop is always bounded.
 *
 * @param[in] regp      pointer to the register to be polled
 * @param[in] mask      mask of the flag to be tested
 * @param[in] set       expected flag state
 *
 * @notapi
 */
static bool otg_wait_flag(volatile uint32_t *regp, uint32_t mask, bool set) {
  uint32_t timeout = GD32_USB_FLAG_TIMEOUT;

  while ((((*regp & mask) != 0U) != set) && (timeout > 0U)) {
    timeout--;
  }

#if GD32_USB_DEBUG_COUNTERS == TRUE
  if (timeout == 0U) {
    gd32_usb_debug.timeout++;
  }
#endif
  return timeout > 0U;
}

static void otg_core_reset(USBDriver *usbp) {
  gd32_otg_t *otgp = usbp->otg;

  /* Wait for the AHB master idle condition.*/
  otg_wait_flag(&otgp->GRSTCTL, GRSTCTL_DMAIDL, true);

  /* Core reset and delay of at least 3 PHY cycles.*/
  otgp->GRSTCTL = GRSTCTL_CSRST;
  osalSysPolledDelayX(12);
  otg_wait_flag(&otgp->GRSTCTL, GRSTCTL_CSRST, false);

  osalSysPolledDelayX(18);

  /* Wait for the AHB master idle condition again.*/
  otg_wait_flag(&otgp->GRSTCTL, GRSTCTL_DMAIDL, true);
}

static void otg_disable_ep(USBDriver *usbp) {
  gd32_otg_t *otgp = usbp->otg;
  unsigned i;

  for (i = 0; i <= usbp->otgparams->num_endpoints; i++) {

    if ((otgp->ie[i].DIEPCTL & DEPCTL_EPEN) != 0U) {
      otgp->ie[i].DIEPCTL |= DEPCTL_EPD;
    }

    if ((otgp->oe[i].DOEPCTL & DEPCTL_EPEN) != 0U) {
      otgp->oe[i].DOEPCTL |= DEPCTL_EPD;
    }

    otgp->ie[i].DIEPINTF = 0xFFFFFFFF;
    otgp->oe[i].DOEPINTF = 0xFFFFFFFF;
  }
  otgp->DAEPINTEN = DAEPINTEN_OEPIE(0) | DAEPINTEN_IEPIE(0);
}

static void otg_rxfifo_flush(USBDriver *usbp) {
  gd32_otg_t *otgp = usbp->otg;

  otgp->GRSTCTL = GRSTCTL_RXFF;
  otg_wait_flag(&otgp->GRSTCTL, GRSTCTL_RXFF, false);

  /* Wait for 3 PHY clocks.*/
  osalSysPolledDelayX(18);
}

static void otg_txfifo_flush(USBDriver *usbp, uint32_t fifo) {
  gd32_otg_t *otgp = usbp->otg;

  otgp->GRSTCTL = GRSTCTL_TXFNUM(fifo) | GRSTCTL_TXFF;
  otg_wait_flag(&otgp->GRSTCTL, GRSTCTL_TXFF, false);

  /* Wait for 3 PHY clocks.*/
  osalSysPolledDelayX(18);
}

static void otg_device_prepare(USBDriver *usbp) {
  gd32_otg_t *otgp = usbp->otg;
  uint32_t address = usbp->otgparams->rx_fifo_size;
  unsigned i;

  /* Match the vendor FS device-core layout used before bus connection.*/
  otgp->GRFLEN = usbp->otgparams->rx_fifo_size;
  for (i = 0U; i <= usbp->otgparams->num_endpoints; i++) {
    uint32_t depth = i < 4U ? 128U : 0U;

    if ((address + depth) > usbp->otgparams->otg_ram_size) {
      depth = 0U;
    }
    if (i == 0U) {
      otgp->DIEP0TFLEN = DIEP0TFLEN_IEP0TXFD(depth) |
                         DIEP0TFLEN_IEP0TXRSAR(address);
    }
    else {
      otgp->DIEPTFLEN[i - 1U] = DIEPTFLEN_IEPTXFD(depth) |
                                DIEPTFLEN_IEPTXRSAR(address);
    }
    address += depth;
  }

  otg_txfifo_flush(usbp, 0x10U);
  otg_rxfifo_flush(usbp);

  otgp->DIEPINTEN = 0U;
  otgp->DOEPINTEN = 0U;
  otgp->DAEPINT = 0xFFFFFFFFU;
  otgp->DAEPINTEN = 0U;
  for (i = 0U; i <= usbp->otgparams->num_endpoints; i++) {
    if ((otgp->ie[i].DIEPCTL & DEPCTL_EPEN) != 0U) {
      otgp->ie[i].DIEPCTL |= DEPCTL_EPD | DEPCTL_SNAK;
    }
    else {
      otgp->ie[i].DIEPCTL = 0U;
    }
    otgp->ie[i].DIEPLEN = 0U;
    otgp->ie[i].DIEPINTF = 0xFFU;

    if ((otgp->oe[i].DOEPCTL & DEPCTL_EPEN) != 0U) {
      otgp->oe[i].DOEPCTL |= DEPCTL_EPD | DEPCTL_SNAK;
    }
    else {
      otgp->oe[i].DOEPCTL = 0U;
    }
    otgp->oe[i].DOEPLEN = 0U;
    otgp->oe[i].DOEPINTF = 0xFFU;
  }

  otgp->DIEPINTEN = DIEPINTEN_EPTXFUDEN;
  otgp->GOTGINTF = 0xFFFFFFFFU;
  otgp->GINTF = 0xBFFFFFFFU;

  if (usbp->config->sof_cb != NULL) {
    otgp->GINTEN = GINTEN_WKUPIE | GINTEN_SPIE | GINTEN_RXFNEIE |
                   GINTEN_RSTIE | GINTEN_ENUMFIE | GINTEN_IEPIE |
                   GINTEN_OEPIE | GINTEN_ISOONCIE | GINTEN_ISOINCIE |
                   GINTEN_SOFIE;
  } else {
    otgp->GINTEN = GINTEN_WKUPIE | GINTEN_SPIE | GINTEN_RXFNEIE |
                   GINTEN_RSTIE | GINTEN_ENUMFIE | GINTEN_IEPIE |
                   GINTEN_OEPIE | GINTEN_ISOONCIE | GINTEN_ISOINCIE;
  }
}

/**
 * @brief   Resets the FIFO RAM memory allocator.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
static void otg_ram_reset(USBDriver *usbp) {

  usbp->pmnext = usbp->otgparams->rx_fifo_size;
}

/**
 * @brief   Allocates a block from the FIFO RAM memory.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] size      size of the packet buffer to allocate in words
 *
 * @notapi
 */
static uint32_t otg_ram_alloc(USBDriver *usbp, size_t size) {
  uint32_t next;

  next = usbp->pmnext;
  usbp->pmnext += size;
  osalDbgAssert(usbp->pmnext <= usbp->otgparams->otg_ram_size,
                "USB FIFO memory overflow");
  return next;
}

/**
 * @brief   Writes to a TX FIFO.
 *
 * @param[in] fifop     pointer to the FIFO register
 * @param[in] buf       buffer where to copy the endpoint data
 * @param[in] n         maximum number of bytes to copy
 *
 * @notapi
 */
static void otg_fifo_write_from_buffer(volatile uint32_t *fifop,
                                       const uint8_t *buf,
                                       size_t n) {

  osalDbgAssert(n > 0, "is zero");

  while (n >= 4U) {
    uint32_t w;

    w  = (uint32_t)buf[0];
    w |= (uint32_t)buf[1] << 8;
    w |= (uint32_t)buf[2] << 16;
    w |= (uint32_t)buf[3] << 24;
    *fifop = w;
    buf += 4;
    n -= 4U;
  }

  if (n != 0U) {
    uint32_t w = 0U;

    switch (n) {
    case 3:
      w |= (uint32_t)buf[2] << 16;
      /* Falls through.*/
    case 2:
      w |= (uint32_t)buf[1] << 8;
      /* Falls through.*/
    case 1:
      w |= (uint32_t)buf[0];
      break;
    default:
      break;
    }
    *fifop = w;
  }
}

/**
 * @brief   Reads a packet from the RX FIFO.
 *
 * @param[in] fifop     pointer to the FIFO register
 * @param[out] buf      buffer where to copy the endpoint data
 * @param[in] n         number of bytes to pull from the FIFO
 * @param[in] max       number of bytes to copy into the buffer
 *
 * @notapi
 */
static void otg_fifo_read_to_buffer(volatile uint32_t *fifop,
                                    uint8_t *buf,
                                    size_t n,
                                    size_t max) {
  uint32_t w = 0;
  size_t i = 0;

  while (i < n) {
    if ((i & 3) == 0) {
      w = *fifop;
    }
    if (i < max) {
      *buf++ = (uint8_t)w;
      w >>= 8;
    }
    i++;
  }
}

/**
 * @brief   Incoming packets handler.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
static void otg_rxfifo_handler(USBDriver *usbp) {
  uint32_t sts, ep;
  size_t n, max;
  uint8_t discard;

  /* Popping the event word out of the RX FIFO.*/
  sts = usbp->otg->GRSTATP;

  /* Event details.*/
  n  = (size_t)((sts & GRSTATP_BCOUNT_MASK) >> GRSTATP_BCOUNT_OFF);
  ep = (sts & GRSTATP_EPNUM_MASK) >> GRSTATP_EPNUM_OFF;

  /* RX status can be reported before the first bus reset. ChibiOS has no
     endpoint configuration at that point, consume the payload safely.*/
  if ((ep > usbp->otgparams->num_endpoints) || (usbp->epc[ep] == NULL)) {
    if (n > 0U) {
      otg_fifo_read_to_buffer(usbp->otg->FIFO[0], &discard, n, 0U);
    }
    return;
  }

  switch (sts & GRSTATP_RPCKST_MASK) {
  case GRSTATP_SETUP_DATA:
#if GD32_USB_DEBUG_COUNTERS == TRUE
    gd32_usb_debug.setup++;
#endif
    otg_fifo_read_to_buffer(usbp->otg->FIFO[0], usbp->epc[ep]->setup_buf,
                            n, 8);
  #if GD32_USB_DEBUG_COUNTERS == TRUE
    memcpy(&gd32_usb_debug.last_setup0, &usbp->epc[ep]->setup_buf[0], 4);
    memcpy(&gd32_usb_debug.last_setup1, &usbp->epc[ep]->setup_buf[4], 4);
  #endif
    break;
  case GRSTATP_SETUP_COMP:
    break;
  case GRSTATP_OUT_DATA:
    max = usbp->epc[ep]->out_state->rxsize - usbp->epc[ep]->out_state->rxcnt;
    otg_fifo_read_to_buffer(usbp->otg->FIFO[0],
                            usbp->epc[ep]->out_state->rxbuf,
                            n, max);
    if (n < max) {
      usbp->epc[ep]->out_state->rxbuf += n;
      usbp->epc[ep]->out_state->rxcnt += n;
    }
    else {
      usbp->epc[ep]->out_state->rxbuf += max;
      usbp->epc[ep]->out_state->rxcnt += max;
    }
    break;
  case GRSTATP_OUT_COMP:
    break;
  case GRSTATP_OUT_GLOBAL_NAK:
    break;
  default:
    break;
  }
}

/**
 * @brief   Outgoing packets handler.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
static bool otg_txfifo_handler(USBDriver *usbp, usbep_t ep) {

  /* The TX FIFO is filled until there is space and data to be transmitted.*/
  while (true) {
    uint32_t n;

    /* Transaction end condition.*/
    if (usbp->epc[ep]->in_state->txcnt >= usbp->epc[ep]->in_state->txsize) {
      usbp->otg->DIEPFEINTEN &= ~DIEPFEINTEN_IEPTXFEIE(ep);
      return true;
    }

    /* Number of bytes remaining in current transaction.*/
    n = usbp->epc[ep]->in_state->txsize - usbp->epc[ep]->in_state->txcnt;
    if (n > usbp->epc[ep]->in_maxsize)
      n = usbp->epc[ep]->in_maxsize;

    /* Checks if in the TX FIFO there is enough space to accommodate the
       next packet.*/
    if (((usbp->otg->ie[ep].DIEPTFSTAT & DIEPTFSTAT_IEPTFS_MASK) * 4) < n)
      return false;

#if GD32_USB_USBFIFO_FILL_BASEPRI
    __set_BASEPRI(CORTEX_PRIO_MASK(GD32_USB_USBFIFO_FILL_BASEPRI));
#endif
    otg_fifo_write_from_buffer(usbp->otg->FIFO[ep],
                               usbp->epc[ep]->in_state->txbuf,
                               n);
    usbp->epc[ep]->in_state->txbuf += n;
    usbp->epc[ep]->in_state->txcnt += n;
#if GD32_USB_USBFIFO_FILL_BASEPRI
    __set_BASEPRI(0);
#endif
  }
}

/**
 * @brief   Generic endpoint IN handler.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
static void otg_epin_handler(USBDriver *usbp, usbep_t ep) {
  gd32_otg_t *otgp = usbp->otg;
  uint32_t epint = otgp->ie[ep].DIEPINTF & otgp->DIEPINTEN;

  if ((otgp->DIEPFEINTEN & DIEPFEINTEN_IEPTXFEIE(ep)) != 0U) {
    epint |= otgp->ie[ep].DIEPINTF & DIEPINTF_TXFE;
  }

  otgp->ie[ep].DIEPINTF = epint;

  if ((epint & DIEPINTF_TF) && (otgp->DIEPINTEN & DIEPINTEN_TFEN)) {
    /* Transmit transfer finished.*/
    USBInEndpointState *isp = usbp->epc[ep]->in_state;

    if (isp->txsize < isp->totsize) {
      /* In case the transaction covered only part of the total transfer
         then another transaction is immediately started in order to
         cover the remaining.*/
      isp->txsize = isp->totsize - isp->txsize;
      isp->txcnt  = 0;
      osalSysLockFromISR();
      usb_lld_start_in(usbp, ep);
      osalSysUnlockFromISR();
    }
    else {
      /* End of the IN transfer.*/
      _usb_isr_invoke_in_cb(usbp, ep);
    }
  }

  if ((epint & DIEPINTF_TXFE) &&
      (otgp->DIEPFEINTEN & DIEPFEINTEN_IEPTXFEIE(ep))) {
    /* TX FIFO empty or emptying.*/
    otg_txfifo_handler(usbp, ep);
  }
}

/**
 * @brief   Generic endpoint OUT handler.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
static void otg_epout_handler(USBDriver *usbp, usbep_t ep) {
  gd32_otg_t *otgp = usbp->otg;
  uint32_t epint = otgp->oe[ep].DOEPINTF & otgp->DOEPINTEN;

#if GD32_USB_DEBUG_COUNTERS == TRUE
  gd32_usb_debug.last_oepint = epint;
#endif

  /* Resets all EP IRQ sources.*/
  otgp->oe[ep].DOEPINTF = epint;

  if ((epint & DOEPINTF_STPF) && (otgp->DOEPINTEN & DOEPINTEN_STPFEN)) {
#if GD32_USB_DEBUG_COUNTERS == TRUE
    gd32_usb_debug.setup_done++;
#endif
    /* Setup packets handling, setup packets are handled using a
       specific callback.*/
    _usb_isr_invoke_setup_cb(usbp, ep);
  }

  if ((epint & DOEPINTF_TF) && (otgp->DOEPINTEN & DOEPINTEN_TFEN)) {
    USBOutEndpointState *osp;

    /* OUT state structure pointer for this endpoint.*/
    osp = usbp->epc[ep]->out_state;

    /* EP0 requires special handling.*/
    if (ep == 0) {

      /* GD32 can report the SETUP transaction's OUT completion before the
         setup phase interrupt. It is not an OUT data/status completion.*/
      if ((usbp->ep0state & USB_OUT_STATE) == 0U) {
        return;
      }

      /* In case the transaction covered only part of the total transfer
         then another transaction is immediately started in order to
         cover the remaining.*/
      if (((osp->rxcnt % usbp->epc[ep]->out_maxsize) == 0) &&
          (osp->rxsize < osp->totsize)) {
        osp->rxsize = osp->totsize - osp->rxsize;
        osp->rxcnt  = 0;
        osalSysLockFromISR();
        usb_lld_start_out(usbp, ep);
        osalSysUnlockFromISR();
        return;
      }
    }

    /* End of the OUT transfer.*/
    _usb_isr_invoke_out_cb(usbp, ep);
  }
}

/**
 * @brief   Isochronous IN transfer failed handler.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
static void otg_isoc_in_failed_handler(USBDriver *usbp) {
  usbep_t ep;
  gd32_otg_t *otgp = usbp->otg;

  for (ep = 0; ep <= usbp->otgparams->num_endpoints; ep++) {
    if (((otgp->ie[ep].DIEPCTL & DEPCTL_EPTYPE_MASK) == DEPCTL_EPTYPE_ISO) &&
        ((otgp->ie[ep].DIEPCTL & DEPCTL_EPEN) != 0)) {
      /* Endpoint enabled, the ISO IN transfer failed, the endpoint is
         disabled before preparing the next frame.*/
      otgp->ie[ep].DIEPCTL |= (DEPCTL_EPD | DEPCTL_SNAK);
      otg_wait_flag(&otgp->ie[ep].DIEPCTL, DEPCTL_EPEN, false);

      /* Flush the FIFO.*/
      otg_txfifo_flush(usbp, ep);

      /* Prepare data for next frame.*/
      _usb_isr_invoke_in_cb(usbp, ep);
    }
  }
}

/**
 * @brief   Isochronous OUT transfer failed handler.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
static void otg_isoc_out_failed_handler(USBDriver *usbp) {
  usbep_t ep;
  gd32_otg_t *otgp = usbp->otg;

  for (ep = 0; ep <= usbp->otgparams->num_endpoints; ep++) {
    if (((otgp->oe[ep].DOEPCTL & DEPCTL_EPTYPE_MASK) == DEPCTL_EPTYPE_ISO) &&
        ((otgp->oe[ep].DOEPCTL & DEPCTL_EPEN) != 0)) {
      /* Prepare transfer for next frame, the endpoint is not disabled
         because the core does not always report the disabled state.*/
      _usb_isr_invoke_out_cb(usbp, ep);
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   USBHS shared ISR.
 * @note    The interrupt handlers are located in the shared
 *          GD32/LLD/USB/gd32_usbhsN.inc files.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_serve_interrupt(USBDriver *usbp) {
  gd32_otg_t *otgp = usbp->otg;
  uint32_t sts, src;
  unsigned i, retry = 128U;

irq_retry:

  sts  = otgp->GINTF;
  sts &= otgp->GINTEN;
  otgp->GINTF = sts & ~IRQ_STATUS_MASK;

#if GD32_USB_DEBUG_COUNTERS == TRUE
  gd32_usb_debug.isr++;
  gd32_usb_debug.last_sts |= sts;
  if (gd32_usb_debug.nhist <
      (sizeof gd32_usb_debug.hist / sizeof gd32_usb_debug.hist[0])) {
    gd32_usb_debug.hist[gd32_usb_debug.nhist++] = sts;
  }
  if (sts & GINTF_RST) {
    gd32_usb_debug.rst++;
  }
  if (sts & GINTF_ENUMFIF) {
    gd32_usb_debug.enumf++;
  }
  if (sts & GINTF_SP) {
    gd32_usb_debug.sp++;
  }
  if (sts & GINTF_WKUPIF) {
    gd32_usb_debug.wkup++;
  }
  if (sts & GINTF_SOF) {
    gd32_usb_debug.sof++;
  }
  if (sts & GINTF_RXFNEIF) {
    gd32_usb_debug.rxfne++;
    gd32_usb_debug.last_rstat = otgp->GRSTATR;
  }
  if (sts & GINTF_OEPIF) {
    gd32_usb_debug.epout++;
  }
  if (sts & GINTF_IEPIF) {
    gd32_usb_debug.epin++;
  }
#endif

  /* Reset interrupt handling.*/
  if (sts & GINTF_RST) {
    /* Default reset action.*/
    _usb_reset(usbp);

    /* Preventing execution of more handlers, the core has been reset.*/
    return;
  }

  /* Wake-up handling.*/
  if (sts & GINTF_WKUPIF) {
    /* If clocks are gated off, turn them back on (may be the case if
       coming out of suspend mode).*/
    if (otgp->PWRCLKCTL & (PWRCLKCTL_SUCLK | PWRCLKCTL_SHCLK)) {
      /* Set to zero to un-gate the USB core clocks.*/
      otgp->PWRCLKCTL &= ~(PWRCLKCTL_SUCLK | PWRCLKCTL_SHCLK);
    }

    /* Clear the remote wakeup signaling.*/
    otgp->DCTL &= ~DCTL_RWKUP;

    _usb_wakeup(usbp);
  }

  /* Suspend handling.*/
  if (sts & GINTF_SP) {
    /* Default suspend action.*/
    _usb_suspend(usbp);
  }

  /* Enumeration finished.*/
  if (sts & GINTF_ENUMFIF) {
    /* Any pending global IN NAK must be released before the control
       transfers can start.*/
    otgp->DCTL |= DCTL_CGINAK;

    /* Full or high speed timing selection.*/
    if ((otgp->DSTAT & DSTAT_ES_MASK) == DSTAT_ES_HS) {
      otgp->GUSBCS = (otgp->GUSBCS & ~GUSBCS_UTT_MASK) |
                     GUSBCS_UTT(UTT_VALUE_HS);
    }
    else {
      otgp->GUSBCS = (otgp->GUSBCS & ~GUSBCS_UTT_MASK) |
                     GUSBCS_UTT(UTT_VALUE_FS);
    }
  }

  /* SOF interrupt handling.*/
  if (sts & GINTF_SOF) {
    /* The SOF interrupt is also used to detect the resume of the USB bus
       after issuing a remote wakeup to the host, in that case it is
       disabled again.*/
    if (usbp->config->sof_cb == NULL) {
      otgp->GINTEN &= ~GINTEN_SOFIE;
    }
    if (usbp->state == USB_SUSPENDED) {
      /* Set to zero to un-gate the USB core clocks.*/
      otgp->PWRCLKCTL &= ~(PWRCLKCTL_SUCLK | PWRCLKCTL_SHCLK);
      _usb_wakeup(usbp);
    }

    _usb_isr_invoke_sof_cb(usbp);
  }

  /* Isochronous IN failed handling.*/
  if (sts & GINTF_ISOINCIF) {
    otg_isoc_in_failed_handler(usbp);
  }

  /* Isochronous OUT failed handling.*/
  if (sts & GINTF_ISOONCIF) {
    otg_isoc_out_failed_handler(usbp);
  }

  /* Drain the RX status queue before endpoint callbacks. Multiple OUT packets
     can be queued when RXFNE and OEP are asserted together.*/
  while (((otgp->GINTF & otgp->GINTEN) & GINTF_RXFNEIF) != 0U) {
    otg_rxfifo_handler(usbp);
    if (--retry == 0U) {
      return;
    }
  }

  /* IN/OUT endpoints event handling.*/
  src = otgp->DAEPINT & otgp->DAEPINTEN;
  if (sts & GINTF_OEPIF) {
    for (i = 0; i <= usbp->otgparams->num_endpoints; i++) {
      if (src & (1U << (DAEPINT_OEPITB_OFF + i))) {
        otg_epout_handler(usbp, (usbep_t)i);
      }
    }
  }
  if (sts & GINTF_IEPIF) {
    for (i = 0; i <= usbp->otgparams->num_endpoints; i++) {
      if (src & (1U << (DAEPINT_IEPITB_OFF + i))) {
        otg_epin_handler(usbp, (usbep_t)i);
      }
    }
  }

  if ((sts & IRQ_RETRY_MASK) && (--retry > 0U))
    goto irq_retry;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level USB driver initialization.
 *
 * @notapi
 */
void usb_lld_init(void) {

  /* Driver initialization.*/
#if GD32_USB_USE_USBHS0
  usbObjectInit(&USBD0);
  USBD0.otg       = USBHSD0;
  USBD0.otgparams = &usbhs0params;
#endif

#if GD32_USB_USE_USBHS1
  usbObjectInit(&USBD1);
  USBD1.otg       = USBHSD1;
  USBD1.otgparams = &usbhs1params;
#endif
}

/**
 * @brief   Configures and activates the USB peripheral.
 * @note    Starting the USBHS cell can be a slow operation carried out with
 *          interrupts disabled, perform it before starting time-critical
 *          operations.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t usb_lld_start(USBDriver *usbp) {
  gd32_otg_t *otgp = usbp->otg;

  if (usbp->state == USB_STOP) {
    /* The USB 3.3V regulator and its detector must be ready before the
       transceiver is powered on.*/
    PMU_CTL2 |= PMU_CTL2_USBSEN | PMU_CTL2_VUSB33DEN;
    if (!otg_wait_flag(&PMU_CTL2, PMU_CTL2_USB33RF, true)) {
      return HAL_RET_HW_FAILURE;
    }

    /* Clock activation, the 48MHz domain is sourced from CK_PLL1Q. An
       on-chip HS PHY requires PLLUSBHS instead, the source is switched
       after the core initialization.*/
#if GD32_USB_USE_USBHS0
    if (&USBD0 == usbp) {
#if GD32_USB_USE_IRC48M
      RCU_ADDCTL0 |= RCU_ADDCTL0_IRC48MEN;
      if (!otg_wait_flag(&RCU_ADDCTL0, RCU_ADDCTL0_IRC48MSTB, true)) {
        return HAL_RET_HW_FAILURE;
      }
      RCU_USBCLKCTL = (RCU_USBCLKCTL & ~(RCU_USBCLKCTL_USBHS0PSC |
                                         RCU_USBCLKCTL_USBHS048MSEL)) |
                      RCU_USB48MSRC_IRC48M;
#else
      RCU_USBCLKCTL = (RCU_USBCLKCTL & ~(RCU_USBCLKCTL_USBHS0PSC |
                                         RCU_USBCLKCTL_USBHS048MSEL)) |
                      USBCLKCTL_USBHSPSC(GD32_USB_PLL1Q_PSC - 1U) |
                      RCU_USB48MSRC_PLL1Q;
#endif
      rcuEnableUSBHS0(true);
    }
#endif

#if GD32_USB_USE_USBHS1
    if (&USBD1 == usbp) {
#if GD32_USB_USE_IRC48M
      RCU_ADDCTL0 |= RCU_ADDCTL0_IRC48MEN;
      if (!otg_wait_flag(&RCU_ADDCTL0, RCU_ADDCTL0_IRC48MSTB, true)) {
        return HAL_RET_HW_FAILURE;
      }

      RCU_USBCLKCTL =
        (RCU_USBCLKCTL & ~(RCU_USBCLKCTL_USBHS1PSC |
                           RCU_USBCLKCTL_USBHS148MSEL)) |
        (RCU_USB48MSRC_IRC48M << 8U);
#else
      RCU_USBCLKCTL =
        (RCU_USBCLKCTL & ~(RCU_USBCLKCTL_USBHS1PSC |
                           RCU_USBCLKCTL_USBHS148MSEL)) |
        (USBCLKCTL_USBHSPSC(GD32_USB_PLL1Q_PSC - 1U) << 3U) |
        (RCU_USB48MSRC_PLL1Q << 8U);
#endif
      rcuEnableUSBHS1(true);
    }
#endif

    /* PHY selection before the core reset, matching usb_core_init().*/
    if (usbp->otgparams->phy_hs) {
      otgp->GUSBCS = (otgp->GUSBCS & ~GUSBCS_EMBPHY_FS) |
                     GUSBCS_EMBPHY_HS;
    }
    else {
      otgp->GUSBCS = (otgp->GUSBCS & ~GUSBCS_EMBPHY_HS) |
                     GUSBCS_EMBPHY_FS;
    }

    /* Soft core reset.*/
    otg_core_reset(usbp);

    /* The working vendor FS binary clears GCCFG after the core reset. This
       also clears the undocumented reset-state bit 8 before PHY power-up.*/
    otgp->GCCFG = 0U;

    /* VBUS sensing is not used, the session is forced as valid before the
       transceiver is powered on.*/
    otgp->GOTGCS |= GOTGCS_BVOE | GOTGCS_BVOV;
    otgp->GCCFG  |= GCCFG_PWRON;

    /* The transceiver needs time to settle after being powered on.*/
    osalSysPolledDelayX(OSAL_MS2RTC(GD32_SYSCLK, 20));

    /* Match the vendor device sequence: disconnect only after the PHY has
       been powered and allowed to settle.*/
    otgp->DCTL |= DCTL_SD;

    /* Forced device mode, the mode bits are cleared by the core reset. The
       core needs time to switch the internal state machine.*/
    otgp->GUSBCS = (otgp->GUSBCS & ~GUSBCS_FHM) | GUSBCS_FDM;
    osalSysPolledDelayX(OSAL_MS2RTC(GD32_SYSCLK, 50));

    /* PHY clocks enabled, frame interval and device speed selection. Only
       the related fields are touched, the other defaults must be
       preserved.*/
    otgp->PWRCLKCTL = 0;
    if (usbp->otgparams->phy_hs) {
      otgp->DCFG = (otgp->DCFG & ~(DCFG_EOPFT_MASK | DCFG_DS_MASK)) |
                   DCFG_EOPFT_80 | DCFG_DS_HS;
    }
    else {
      otgp->DCFG = (otgp->DCFG & ~(DCFG_EOPFT_MASK | DCFG_DS_MASK)) |
                   DCFG_EOPFT_80 | DCFG_DS_FS;
    }

    /* Interrupts on TX FIFOs half empty.*/
    otgp->GAHBCS = 0;

    /* FIFO allocator and device-core initialization. The hardware layout
       is refined again by the ChibiOS endpoint setup after bus reset.*/
    otg_ram_reset(usbp);
    otg_device_prepare(usbp);

#if GD32_USB_USE_EMBPHY_HS
    /* The core is initialized from PLL1Q first, matching the vendor
       sequence. The embedded HS PHY clock is switched to PLLUSBHS only
       after the device core has been prepared.*/
    if (usbp->otgparams->phy_hs) {
#if GD32_USB_USE_USBHS0
      if (&USBD0 == usbp) {
        uint32_t pllcfg;

        /* PLLUSBHS0 source is HXTAL.*/
        RCU_USBCLKCTL &= ~RCU_USBCLKCTL_PLLUSBHS0PRESEL;

        /* Configure only the PLLUSBHS0 fields.*/
        pllcfg = RCU_PLLUSBCFG;
        pllcfg &= ~(RCU_PLLUSBCFG_PLLUSBHS0PREDV |
                    RCU_PLLUSBCFG_PLLUSBHS0MF |
                    RCU_PLLUSBCFG_USBHS0DV);
        pllcfg |= PLLUSBCFG_PLLUSBHSPREDV(GD32_USB_PLLUSBHS_PREDV) |
                  PLLUSBCFG_PLLUSBHSMF(GD32_USB_PLLUSBHS_MF) |
                  PLLUSBCFG_USBHSDV((GD32_USB_PLLUSBHS_DV / 2U) - 1U);
        RCU_PLLUSBCFG = pllcfg;

        RCU_ADDCTL1 |= RCU_ADDCTL1_PLLUSBHS0EN;
        if (!otg_wait_flag(&RCU_ADDCTL1,
                           RCU_ADDCTL1_PLLUSBHS0STB,
                           true)) {
          RCU_ADDCTL1 &= ~RCU_ADDCTL1_PLLUSBHS0EN;
          rcuDisableUSBHS0();
          return HAL_RET_HW_FAILURE;
        }

        /* Switch USBHS0 48MHz domain to PLLUSBHS.*/
        RCU_USBCLKCTL =
          (RCU_USBCLKCTL & ~RCU_USBCLKCTL_USBHS048MSEL) |
          RCU_USB48MSRC_PLLUSBHS;

        /* Allow the new clock to propagate through the USB core.*/
        osalSysPolledDelayX(18);
      }
#endif

#if GD32_USB_USE_USBHS1
      if (&USBD1 == usbp) {
        uint32_t pllcfg;

        /* PLLUSBHS1 source is HXTAL.*/
        RCU_USBCLKCTL &= ~RCU_USBCLKCTL_PLLUSBHS1PRESEL;

        /* Configure only the PLLUSBHS1 fields.*/
        pllcfg = RCU_PLLUSBCFG;
        pllcfg &= ~(RCU_PLLUSBCFG_PLLUSBHS1PREDV |
                    RCU_PLLUSBCFG_PLLUSBHS1MF |
                    RCU_PLLUSBCFG_USBHS1DV);
        pllcfg |= PLLUSBCFG_PLLUSBHSPREDV(GD32_USB_PLLUSBHS_PREDV) << 16U |
                  PLLUSBCFG_PLLUSBHSMF(GD32_USB_PLLUSBHS_MF) << 16U |
                  PLLUSBCFG_USBHSDV(
                    (GD32_USB_PLLUSBHS_DV / 2U) - 1U) << 16U;
        RCU_PLLUSBCFG = pllcfg;

        RCU_ADDCTL1 |= RCU_ADDCTL1_PLLUSBHS1EN;
        if (!otg_wait_flag(&RCU_ADDCTL1,
                           RCU_ADDCTL1_PLLUSBHS1STB,
                           true)) {
          RCU_ADDCTL1 &= ~RCU_ADDCTL1_PLLUSBHS1EN;
          rcuDisableUSBHS1();
          return HAL_RET_HW_FAILURE;
        }

        RCU_USBCLKCTL =
          (RCU_USBCLKCTL & ~RCU_USBCLKCTL_USBHS148MSEL) |
          (RCU_USB48MSRC_PLLUSBHS << 8U);

        osalSysPolledDelayX(18);
      }
#endif
    }
#endif /* GD32_USB_USE_EMBPHY_HS */
    /* Global interrupts enable.*/
    otgp->GAHBCS |= GAHBCS_GINTEN;
  }

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the USB peripheral.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_stop(USBDriver *usbp) {
  gd32_otg_t *otgp = usbp->otg;

  /* If in ready state then disables the USB clock.*/
  if (usbp->state != USB_STOP) {

    /* Disabling all endpoints in case the driver has been stopped while
       active.*/
    otg_disable_ep(usbp);

    otgp->DAEPINTEN = 0;
    otgp->GAHBCS    = 0;
    otgp->GCCFG     = 0;

#if GD32_USB_USE_USBHS0
    if (&USBD0 == usbp) {
      rcuDisableUSBHS0();
      if (usbp->otgparams->phy_hs) {
        RCU_ADDCTL1 &= ~RCU_ADDCTL1_PLLUSBHS0EN;
        (void)otg_wait_flag(&RCU_ADDCTL1,
                            RCU_ADDCTL1_PLLUSBHS0STB,
                            false);
      }
    }
#endif

#if GD32_USB_USE_USBHS1
    if (&USBD1 == usbp) {
      rcuDisableUSBHS1();
      if (usbp->otgparams->phy_hs) {
        RCU_ADDCTL1 &= ~RCU_ADDCTL1_PLLUSBHS1EN;
        (void)otg_wait_flag(&RCU_ADDCTL1,
                            RCU_ADDCTL1_PLLUSBHS1STB,
                            false);
      }
    }
#endif
  }
}

/**
 * @brief   USB low level reset routine.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_reset(USBDriver *usbp) {
  unsigned i;
  gd32_otg_t *otgp = usbp->otg;

  /* Clear the remote wakeup signaling.*/
  otgp->DCTL &= ~DCTL_RWKUP;

  /* Flush the TX FIFO.*/
  otg_txfifo_flush(usbp, 0);

  /* Endpoint interrupts all disabled and cleared.*/
  otgp->DIEPFEINTEN = 0;
  otgp->DAEPINT      = 0xFFFFFFFF;
  otgp->DAEPINTEN   = DAEPINTEN_OEPIE(0) | DAEPINTEN_IEPIE(0);

  /* All endpoints in NAK mode, interrupts cleared.*/
  for (i = 0; i <= usbp->otgparams->num_endpoints; i++) {
    otgp->ie[i].DIEPCTL  = DEPCTL_SNAK;
    otgp->oe[i].DOEPCTL  = DEPCTL_SNAK;
    otgp->ie[i].DIEPINTF = 0xFFFFFFFF;
    otgp->oe[i].DOEPINTF = 0xFFFFFFFF;
  }

  /* Resets the FIFO memory allocator.*/
  otg_ram_reset(usbp);

  /* Receive FIFO size initialization, the address is always zero.*/
  otgp->GRFLEN = usbp->otgparams->rx_fifo_size;
  otg_rxfifo_flush(usbp);

  /* Resets the device address to zero.*/
  otgp->DCFG = (otgp->DCFG & ~DCFG_DAR_MASK) | DCFG_DAR(0);

  /* Enables also EP-related interrupt sources.*/
  otgp->GINTEN    |= GINTEN_RXFNEIE | GINTEN_OEPIE | GINTEN_IEPIE;
  otgp->DIEPINTEN  = DIEPINTEN_TFEN;
  otgp->DOEPINTEN  = DOEPINTEN_STPFEN | DOEPINTEN_TFEN;

  /* EP0 initialization, it is a special case. The OUT length register is
     armed for three back to back SETUP packets.*/
  usbp->epc[0] = &ep0config;
  otgp->oe[0].DOEPLEN = DOEPLEN_STPCNT(3) | DEPLEN_PCNT(1) |
                        DEPLEN_TLEN(3 * 8);
  otgp->oe[0].DOEPCTL = DEPCTL_SD0PID | DEPCTL_EPACT | DEPCTL_EPTYPE_CTRL |
                        DEP0CTL_MPL_64;
  otgp->ie[0].DIEPLEN = 0;
  otgp->ie[0].DIEPCTL = DEPCTL_SD0PID | DEPCTL_EPACT | DEPCTL_EPTYPE_CTRL |
                        DIEPCTL_TXFNUM(0) | DEP0CTL_MPL_64;
  otgp->DIEP0TFLEN = DIEP0TFLEN_IEP0TXFD(ep0config.in_maxsize / 4) |
                     DIEP0TFLEN_IEP0TXRSAR(otg_ram_alloc(usbp,
                                             ep0config.in_maxsize / 4));
}

/**
 * @brief   Sets the USB address.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_set_address(USBDriver *usbp) {
  gd32_otg_t *otgp = usbp->otg;

  otgp->DCFG = (otgp->DCFG & ~DCFG_DAR_MASK) | DCFG_DAR(usbp->address);
}

/**
 * @brief   Enables an endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_init_endpoint(USBDriver *usbp, usbep_t ep) {
  uint32_t ctl, fsize;
  gd32_otg_t *otgp = usbp->otg;

  /* IN and OUT common parameters.*/
  switch (usbp->epc[ep]->ep_mode & USB_EP_MODE_TYPE) {
  case USB_EP_MODE_TYPE_CTRL:
    ctl = DEPCTL_SD0PID | DEPCTL_EPACT | DEPCTL_EPTYPE_CTRL;
    break;
  case USB_EP_MODE_TYPE_ISOC:
    ctl = DEPCTL_SD0PID | DEPCTL_EPACT | DEPCTL_EPTYPE_ISO;
    break;
  case USB_EP_MODE_TYPE_BULK:
    ctl = DEPCTL_SD0PID | DEPCTL_EPACT | DEPCTL_EPTYPE_BULK;
    break;
  case USB_EP_MODE_TYPE_INTR:
    ctl = DEPCTL_SD0PID | DEPCTL_EPACT | DEPCTL_EPTYPE_INTR;
    break;
  default:
    return;
  }

  /* OUT endpoint activation or deactivation.*/
  otgp->oe[ep].DOEPLEN = 0;
  if (usbp->epc[ep]->out_state != NULL) {
    otgp->oe[ep].DOEPCTL = ctl | DEPCTL_MPL(usbp->epc[ep]->out_maxsize);
    otgp->DAEPINTEN |= DAEPINTEN_OEPIE(ep);
  }
  else {
    otgp->oe[ep].DOEPCTL &= ~DEPCTL_EPACT;
    otgp->DAEPINTEN &= ~DAEPINTEN_OEPIE(ep);
  }

  /* IN endpoint activation or deactivation.*/
  otgp->ie[ep].DIEPLEN = 0;
  if (usbp->epc[ep]->in_state != NULL) {
    /* FIFO allocation for the IN endpoint.*/
    fsize = usbp->epc[ep]->in_maxsize / 4;
    if (usbp->epc[ep]->in_multiplier > 1)
      fsize *= usbp->epc[ep]->in_multiplier;
    otgp->DIEPTFLEN[ep - 1] = DIEPTFLEN_IEPTXFD(fsize) |
                              DIEPTFLEN_IEPTXRSAR(otg_ram_alloc(usbp, fsize));
    otg_txfifo_flush(usbp, ep);

    otgp->ie[ep].DIEPCTL = ctl |
                           DIEPCTL_TXFNUM(ep) |
                           DEPCTL_MPL(usbp->epc[ep]->in_maxsize);
    otgp->DAEPINTEN |= DAEPINTEN_IEPIE(ep);
  }
  else {
    otgp->DIEPTFLEN[ep - 1] = DIEPTFLEN_IEPTXFD(0x200) |
                              DIEPTFLEN_IEPTXRSAR(0x400); /* Reset value.*/
    otg_txfifo_flush(usbp, ep);
    otgp->ie[ep].DIEPCTL &= ~DEPCTL_EPACT;
    otgp->DAEPINTEN &= ~DAEPINTEN_IEPIE(ep);
  }
}

/**
 * @brief   Disables all the active endpoints except the endpoint zero.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 *
 * @notapi
 */
void usb_lld_disable_endpoints(USBDriver *usbp) {

  /* Resets the FIFO memory allocator.*/
  otg_ram_reset(usbp);

  /* Disabling all endpoints.*/
  otg_disable_ep(usbp);
}

/**
 * @brief   Returns the status of an OUT endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              The endpoint status.
 * @retval EP_STATUS_DISABLED The endpoint is not active.
 * @retval EP_STATUS_STALLED  The endpoint is stalled.
 * @retval EP_STATUS_ACTIVE   The endpoint is active.
 *
 * @notapi
 */
usbepstatus_t usb_lld_get_status_out(USBDriver *usbp, usbep_t ep) {
  uint32_t ctl;

  ctl = usbp->otg->oe[ep].DOEPCTL;
  if (!(ctl & DEPCTL_EPACT))
    return EP_STATUS_DISABLED;
  if (ctl & DEPCTL_STALL)
    return EP_STATUS_STALLED;
  return EP_STATUS_ACTIVE;
}

/**
 * @brief   Returns the status of an IN endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              The endpoint status.
 * @retval EP_STATUS_DISABLED The endpoint is not active.
 * @retval EP_STATUS_STALLED  The endpoint is stalled.
 * @retval EP_STATUS_ACTIVE   The endpoint is active.
 *
 * @notapi
 */
usbepstatus_t usb_lld_get_status_in(USBDriver *usbp, usbep_t ep) {
  uint32_t ctl;

  ctl = usbp->otg->ie[ep].DIEPCTL;
  if (!(ctl & DEPCTL_EPACT))
    return EP_STATUS_DISABLED;
  if (ctl & DEPCTL_STALL)
    return EP_STATUS_STALLED;
  return EP_STATUS_ACTIVE;
}

/**
 * @brief   Reads a setup packet from the dedicated packet buffer.
 * @details This function must be invoked in the context of the @p setup_cb
 *          callback in order to read the received setup packet.
 * @pre     In order to use this function the endpoint must have been
 *          initialized as a control endpoint.
 * @post    The endpoint is ready to accept another packet.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @param[out] buf      buffer where to copy the packet data
 *
 * @notapi
 */
void usb_lld_read_setup(USBDriver *usbp, usbep_t ep, uint8_t *buf) {

  memcpy(buf, usbp->epc[ep]->setup_buf, 8);
}

/**
 * @brief   Starts a receive operation on an OUT endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_start_out(USBDriver *usbp, usbep_t ep) {
  uint32_t pcnt, rxsize;
  USBOutEndpointState *osp = usbp->epc[ep]->out_state;

  /* Transfer initialization.*/
  osp->totsize = osp->rxsize;
  if ((ep == 0) && (osp->rxsize > EP0_MAX_OUTSIZE))
    osp->rxsize = EP0_MAX_OUTSIZE;

  /* Transaction size is rounded to a multiple of the packet size because
     the transfer length field must be a multiple of the maximum packet
     size of the endpoint, adjusted to the word boundary.*/
  pcnt   = (osp->rxsize + usbp->epc[ep]->out_maxsize - 1U) /
           usbp->epc[ep]->out_maxsize;
  rxsize = (pcnt * usbp->epc[ep]->out_maxsize + 3U) & 0xFFFFFFFCU;

  /* A zero-length OUT status packet still requires one packet slot on
     this core, the transfer length itself remains zero.*/
  if (pcnt == 0U) {
    pcnt = 1U;
  }

  /* Setting up transaction parameters in DOEPLEN. Setup packet count is
     meaningful only for EP0; on data OUT endpoints bits 29..30 report the
     received data PID and must not be programmed as STPCNT.*/
  usbp->otg->oe[ep].DOEPLEN = DEPLEN_PCNT(pcnt) | DEPLEN_TLEN(rxsize);
  if (ep == 0U) {
    usbp->otg->oe[ep].DOEPLEN |= DOEPLEN_STPCNT(3);
  }

  /* Special case of isochronous endpoint.*/
  if ((usbp->epc[ep]->ep_mode & USB_EP_MODE_TYPE) == USB_EP_MODE_TYPE_ISOC) {
    /* Odd/even bit toggling for isochronous endpoint.*/
    if (usbp->otg->DSTAT & DSTAT_FNRSOF_ODD)
      usbp->otg->oe[ep].DOEPCTL |= DEPCTL_SEVENFRM;
    else
      usbp->otg->oe[ep].DOEPCTL |= DEPCTL_SODDFRM;
  }

  /* Starting operation.*/
  usbp->otg->oe[ep].DOEPCTL |= DEPCTL_EPEN | DEPCTL_CNAK;
}

/**
 * @brief   Starts a transmit operation on an IN endpoint.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_start_in(USBDriver *usbp, usbep_t ep) {
  USBInEndpointState *isp = usbp->epc[ep]->in_state;

  /* Transfer initialization.*/
  isp->totsize = isp->txsize;
  if (isp->txsize == 0) {
    /* Special case, sending zero size packet.*/
    usbp->otg->ie[ep].DIEPLEN = DEPLEN_PCNT(1) | DEPLEN_TLEN(0);
  }
  else {
    if ((ep == 0) && (isp->txsize > EP0_MAX_INSIZE))
      isp->txsize = EP0_MAX_INSIZE;

    /* Normal case.*/
    uint32_t pcnt = (isp->txsize + usbp->epc[ep]->in_maxsize - 1) /
                    usbp->epc[ep]->in_maxsize;
    usbp->otg->ie[ep].DIEPLEN = DIEPLEN_MCNT(1) | DEPLEN_PCNT(pcnt) |
                                DEPLEN_TLEN(isp->txsize);
  }

  /* Special case of isochronous endpoint.*/
  if ((usbp->epc[ep]->ep_mode & USB_EP_MODE_TYPE) == USB_EP_MODE_TYPE_ISOC) {
    /* Odd/even bit toggling.*/
    if (usbp->otg->DSTAT & DSTAT_FNRSOF_ODD)
      usbp->otg->ie[ep].DIEPCTL |= DEPCTL_SEVENFRM;
    else
      usbp->otg->ie[ep].DIEPCTL |= DEPCTL_SODDFRM;
  }

  /* Starting operation.*/
  usbp->otg->ie[ep].DIEPCTL |= DEPCTL_EPEN | DEPCTL_CNAK;
  usbp->otg->DIEPFEINTEN |= DIEPFEINTEN_IEPTXFEIE(ep);
#if GD32_USB_DEBUG_COUNTERS == TRUE
  if (ep == 0U) {
    gd32_usb_debug.start_in++;
    gd32_usb_debug.last_iepctl = usbp->otg->ie[ep].DIEPCTL;
    gd32_usb_debug.last_ieplen = usbp->otg->ie[ep].DIEPLEN;
  }
#endif
}

/**
 * @brief   Brings an OUT endpoint in the stalled state.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_stall_out(USBDriver *usbp, usbep_t ep) {

  usbp->otg->oe[ep].DOEPCTL |= DEPCTL_STALL;
}

/**
 * @brief   Brings an IN endpoint in the stalled state.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_stall_in(USBDriver *usbp, usbep_t ep) {

  usbp->otg->ie[ep].DIEPCTL |= DEPCTL_STALL;
}

/**
 * @brief   Brings an OUT endpoint in the active state.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_clear_out(USBDriver *usbp, usbep_t ep) {

  usbp->otg->oe[ep].DOEPCTL &= ~DEPCTL_STALL;
}

/**
 * @brief   Brings an IN endpoint in the active state.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 *
 * @notapi
 */
void usb_lld_clear_in(USBDriver *usbp, usbep_t ep) {

  usbp->otg->ie[ep].DIEPCTL &= ~DEPCTL_STALL;
}

#endif /* HAL_USE_USB */

/** @} */
