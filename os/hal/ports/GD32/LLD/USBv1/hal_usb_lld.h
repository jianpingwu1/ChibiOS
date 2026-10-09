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
 * @file    USBv1/hal_usb_lld.h
 * @brief   GD32 USB subsystem low level driver header.
 *
 * @addtogroup USB
 * @{
 */

#ifndef HAL_USB_LLD_H
#define HAL_USB_LLD_H

#if HAL_USE_USB || defined(__DOXYGEN__)

#include "gd32_usb.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Enables the enhanced start API.
 */
#define USB_LLD_ENHANCED_API

/**
 * @brief   Status stage handling method.
 */
#define USB_EP0_STATUS_STAGE                USB_EP0_STATUS_STAGE_SW

/**
 * @brief   The address can be changed immediately upon packet reception.
 */
#define USB_SET_ADDRESS_MODE                USB_EARLY_SET_ADDRESS

/**
 * @brief   Method for set address acknowledge.
 */
#define USB_SET_ADDRESS_ACK_HANDLING        USB_SET_ADDRESS_ACK_SW

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   USBHS0 driver enable switch.
 * @details If set to @p TRUE the support for USBHS0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_USB_USE_USBHS0) || defined(__DOXYGEN__)
#define GD32_USB_USE_USBHS0                 FALSE
#endif

/**
 * @brief   USBHS1 driver enable switch.
 * @details If set to @p TRUE the support for USBHS1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_USB_USE_USBHS1) || defined(__DOXYGEN__)
#define GD32_USB_USE_USBHS1                 FALSE
#endif

/**
 * @brief   Enables the high speed mode on USBHS0, else full speed mode.
 * @note    Only the on-chip PHYs are supported, an external ULPI PHY is not
 *          handled by this driver.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_USB_USBHS0_HS) || defined(__DOXYGEN__)
#define GD32_USB_USBHS0_HS                  FALSE
#endif

/**
 * @brief   Enables the high speed mode on USBHS1, else full speed mode.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_USB_USBHS1_HS) || defined(__DOXYGEN__)
#define GD32_USB_USBHS1_HS                  FALSE
#endif

/**
 * @brief   USBHS0 RX shared FIFO size in bytes.
 * @note    Must be a multiple of 4.
 */
#if !defined(GD32_USB_USBHS0_RX_FIFO_SIZE) || defined(__DOXYGEN__)
#define GD32_USB_USBHS0_RX_FIFO_SIZE        2048
#endif

/**
 * @brief   USBHS1 RX shared FIFO size in bytes.
 * @note    Must be a multiple of 4.
 */
#if !defined(GD32_USB_USBHS1_RX_FIFO_SIZE) || defined(__DOXYGEN__)
#define GD32_USB_USBHS1_RX_FIFO_SIZE        2048
#endif

/**
 * @brief   CK_PLL1Q prescaler feeding the USB 48MHz domain.
 * @note    Allowed values are 1..8.
 */
#if !defined(GD32_USB_PLL1Q_PSC) || defined(__DOXYGEN__)
#define GD32_USB_PLL1Q_PSC                  1
#endif

/**
 * @brief   PLLUSBHS pre-divider, the clock source is always HXTAL.
 * @note    Only used when an on-chip HS PHY is enabled, in that case the
 *          48MHz domain must be sourced from PLLUSBHS.
 * @note    Allowed values are 1..15.
 */
#if !defined(GD32_USB_PLLUSBHS_PREDV) || defined(__DOXYGEN__)
#define GD32_USB_PLLUSBHS_PREDV             5
#endif

/**
 * @brief   PLLUSBHS multiplication factor.
 * @note    Allowed values are 16..127.
 */
#if !defined(GD32_USB_PLLUSBHS_MF) || defined(__DOXYGEN__)
#define GD32_USB_PLLUSBHS_MF                96
#endif

/**
 * @brief   PLLUSBHS output divider feeding the 48MHz domain.
 * @note    Allowed values are the even numbers in the 2..16 range.
 */
#if !defined(GD32_USB_PLLUSBHS_DV) || defined(__DOXYGEN__)
#define GD32_USB_PLLUSBHS_DV                10
#endif

/**
 * @brief   Exception priority level during TXFIFOs operations.
 * @details Copying a packet into a TX FIFO must not be interrupted by any
 *          other operation on the same core, this parameter is the priority
 *          mask applied during the copy.
 * @note    The value zero means disabled, when disabled calling USB
 *          functions is only safe from thread level or from USB callbacks.
 */
#if !defined(GD32_USB_USBFIFO_FILL_BASEPRI) || defined(__DOXYGEN__)
#define GD32_USB_USBFIFO_FILL_BASEPRI       0
#endif

/**
 * @brief   Host wake-up procedure duration.
 */
#if !defined(GD32_USB_HOST_WAKEUP_DURATION) || defined(__DOXYGEN__)
#define GD32_USB_HOST_WAKEUP_DURATION       2
#endif

/**
 * @brief   Allowed deviation for the 48MHz clock.
 */
#if !defined(GD32_USB_48MHZ_DELTA) || defined(__DOXYGEN__)
#define GD32_USB_48MHZ_DELTA                120000
#endif

/**
 * @brief   Polling cycles before giving up on a core status flag.
 */
#if !defined(GD32_USB_FLAG_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_USB_FLAG_TIMEOUT               200000
#endif

/**
 * @brief   Enables the interrupt tracing counters.
 * @details Debug aid only, it can be switched off once the port is
 *          validated on the target board.
 */
#if !defined(GD32_USB_DEBUG_COUNTERS) || defined(__DOXYGEN__)
#define GD32_USB_DEBUG_COUNTERS             FALSE
#endif

/**
 * @brief   Uses the internal 48MHz oscillator for the USB FS PHY.
 */
#if !defined(GD32_USB_USE_IRC48M) || defined(__DOXYGEN__)
#define GD32_USB_USE_IRC48M                 FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks.*/
#if !defined(GD32_HAS_USBHS0) || !defined(GD32_HAS_USBHS1)
#error "GD32_HAS_USBHSx not defined in registry"
#endif

#if GD32_HAS_USBHS0 && !defined(GD32_USBHS0_ENDPOINTS)
#error "GD32_USBHS0_ENDPOINTS not defined in registry"
#endif

#if GD32_HAS_USBHS1 && !defined(GD32_USBHS1_ENDPOINTS)
#error "GD32_USBHS1_ENDPOINTS not defined in registry"
#endif

#if GD32_HAS_USBHS0 && !defined(GD32_USBHS0_FIFO_MEM_SIZE)
#error "GD32_USBHS0_FIFO_MEM_SIZE not defined in registry"
#endif

#if GD32_HAS_USBHS1 && !defined(GD32_USBHS1_FIFO_MEM_SIZE)
#error "GD32_USBHS1_FIFO_MEM_SIZE not defined in registry"
#endif

#if GD32_USB_USE_USBHS0 && !GD32_HAS_USBHS0
#error "USBHS0 not present in the selected device"
#endif

#if GD32_USB_USE_USBHS1 && !GD32_HAS_USBHS1
#error "USBHS1 not present in the selected device"
#endif

#if !GD32_USB_USE_USBHS0 && !GD32_USB_USE_USBHS1
#error "USB driver activated but no USB peripheral assigned"
#endif

/* Maximum endpoint address.*/
#if (GD32_USB_USE_USBHS0 && GD32_USB_USE_USBHS1) || defined(__DOXYGEN__)
#if GD32_USBHS0_ENDPOINTS < GD32_USBHS1_ENDPOINTS
#define USB_MAX_ENDPOINTS                   GD32_USBHS1_ENDPOINTS
#else
#define USB_MAX_ENDPOINTS                   GD32_USBHS0_ENDPOINTS
#endif
#elif GD32_USB_USE_USBHS0
#define USB_MAX_ENDPOINTS                   GD32_USBHS0_ENDPOINTS
#else
#define USB_MAX_ENDPOINTS                   GD32_USBHS1_ENDPOINTS
#endif

#if GD32_USB_USE_USBHS0 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_USBHS0_PRIORITY)
#error "Invalid IRQ priority assigned to USBHS0"
#endif

#if GD32_USB_USE_USBHS1 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_USBHS1_PRIORITY)
#error "Invalid IRQ priority assigned to USBHS1"
#endif

/* Checks on allocation of USBHSx units, the same core cannot be claimed
   by more than one driver.*/
#if GD32_USB_USE_USBHS0
#if defined(GD32_USBHS0_IS_USED)
#error "USBD0 requires USBHS0 but it is already used"
#else
#define GD32_USBHS0_IS_USED
#endif
#endif

#if GD32_USB_USE_USBHS1
#if defined(GD32_USBHS1_IS_USED)
#error "USBD1 requires USBHS1 but it is already used"
#else
#define GD32_USBHS1_IS_USED
#endif
#endif

#if (GD32_USB_USBHS0_RX_FIFO_SIZE & 3) != 0
#error "USBHS0 RX FIFO size must be a multiple of 4"
#endif

#if (GD32_USB_USBHS1_RX_FIFO_SIZE & 3) != 0
#error "USBHS1 RX FIFO size must be a multiple of 4"
#endif

#if (GD32_USB_PLL1Q_PSC < 1) || (GD32_USB_PLL1Q_PSC > 8)
#error "invalid GD32_USB_PLL1Q_PSC setting, it must be between 1 and 8"
#endif

#if (GD32_USB_HOST_WAKEUP_DURATION < 2) || (GD32_USB_HOST_WAKEUP_DURATION > 15)
#error "invalid GD32_USB_HOST_WAKEUP_DURATION setting, it must be between 2 and 15"
#endif

/* Allowing for a small tolerance.*/
#if (GD32_USB_48MHZ_DELTA < 0) || (GD32_USB_48MHZ_DELTA > 120000)
#error "invalid GD32_USB_48MHZ_DELTA setting, it must not exceed 120000"
#endif

/* CK_PLL1Q is gated off unless the PLL1 Q output is explicitly enabled.*/
#if !GD32_USB_USE_IRC48M
#if (GD32_PLL1_ENABLED != TRUE) || (GD32_PLL1_Q_ENABLED != TRUE)
#error "the USB driver requires GD32_PLL1_ENABLED and GD32_PLL1_Q_ENABLED"
#endif

/**
 * @brief   USB 48MHz domain frequency, sourced from CK_PLL1Q.
 */
#define GD32_USBCLK                         (GD32_PLL1Q_CLK / GD32_USB_PLL1Q_PSC)

#if (GD32_USBCLK < (48000000 - GD32_USB_48MHZ_DELTA)) ||                    \
    (GD32_USBCLK > (48000000 + GD32_USB_48MHZ_DELTA))
#error "the USB driver requires a 48MHz clock on CK_PLL1Q / GD32_USB_PLL1Q_PSC"
#endif
#else
#define GD32_USBCLK                         48000000U
#endif

/**
 * @brief   TRUE if at least one core is configured for the on-chip HS PHY.
 */
#if (GD32_USB_USE_USBHS0 && GD32_USB_USBHS0_HS) ||                          \
    (GD32_USB_USE_USBHS1 && GD32_USB_USBHS1_HS) || defined(__DOXYGEN__)
#define GD32_USB_USE_EMBPHY_HS              TRUE
#else
#define GD32_USB_USE_EMBPHY_HS              FALSE
#endif

#if GD32_USB_USE_EMBPHY_HS
#if (GD32_USB_PLLUSBHS_PREDV < 1) || (GD32_USB_PLLUSBHS_PREDV > 15)
#error "invalid GD32_USB_PLLUSBHS_PREDV setting, it must be between 1 and 15"
#endif

#if (GD32_USB_PLLUSBHS_MF < 16) || (GD32_USB_PLLUSBHS_MF > 127)
#error "invalid GD32_USB_PLLUSBHS_MF setting, it must be between 16 and 127"
#endif

#if (GD32_USB_PLLUSBHS_DV < 2) || (GD32_USB_PLLUSBHS_DV > 16) ||            \
    ((GD32_USB_PLLUSBHS_DV & 1) != 0)
#error "invalid GD32_USB_PLLUSBHS_DV setting, it must be an even value between 2 and 16"
#endif

/**
 * @brief   PLLUSBHS output frequency feeding the 48MHz domain.
 */
#define GD32_PLLUSBHSCLK                                                    \
  ((GD32_HXTALCLK / GD32_USB_PLLUSBHS_PREDV) * GD32_USB_PLLUSBHS_MF /       \
   GD32_USB_PLLUSBHS_DV)

#if (GD32_PLLUSBHSCLK < (48000000 - GD32_USB_48MHZ_DELTA)) ||               \
    (GD32_PLLUSBHSCLK > (48000000 + GD32_USB_48MHZ_DELTA))
#error "the on-chip HS PHY requires a 48MHz clock out of PLLUSBHS"
#endif
#endif /* GD32_USB_USE_EMBPHY_HS */

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Peripheral-specific parameters block.
 */
typedef struct {
  uint32_t                      rx_fifo_size;
  uint32_t                      otg_ram_size;
  uint32_t                      num_endpoints;
  uint32_t                      phy_hs;
} gd32_otg_params_t;

/**
 * @brief   Type of an IN endpoint state structure.
 */
typedef struct {
  /**
   * @brief   Requested transmit transfer size.
   */
  size_t                        txsize;
  /**
   * @brief   Transmitted bytes so far.
   */
  size_t                        txcnt;
  /**
   * @brief   Pointer to the transmission linear buffer.
   */
  const uint8_t                 *txbuf;
#if (USB_USE_WAIT == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Waiting thread.
   */
  thread_reference_t            thread;
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Total transmit transfer size.
   */
  size_t                        totsize;
} USBInEndpointState;

/**
 * @brief   Type of an OUT endpoint state structure.
 */
typedef struct {
  /**
   * @brief   Requested receive transfer size.
   */
  size_t                        rxsize;
  /**
   * @brief   Received bytes so far.
   */
  size_t                        rxcnt;
  /**
   * @brief   Pointer to the receive linear buffer.
   */
  uint8_t                       *rxbuf;
#if (USB_USE_WAIT == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Waiting thread.
   */
  thread_reference_t            thread;
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Total receive transfer size.
   */
  size_t                        totsize;
} USBOutEndpointState;

/**
 * @brief   Type of an USB endpoint configuration structure.
 * @note    Platform specific restrictions may apply to endpoints.
 */
typedef struct {
  /**
   * @brief   Type and mode of the endpoint.
   */
  uint32_t                      ep_mode;
  /**
   * @brief   Setup packet notification callback.
   * @details This callback is invoked when a setup packet has been
   *          received.
   * @post    The application must immediately call @p usbReadPacket() in
   *          order to access the received packet.
   * @note    This field is only valid for @p USB_EP_MODE_TYPE_CTRL
   *          endpoints, it should be set to @p NULL for other endpoint
   *          types.
   */
  usbepcallback_t               setup_cb;
  /**
   * @brief   IN endpoint notification callback.
   * @details This field must be set to @p NULL if callback is not required.
   */
  usbepcallback_t               in_cb;
  /**
   * @brief   OUT endpoint notification callback.
   * @details This field must be set to @p NULL if callback is not required.
   */
  usbepcallback_t               out_cb;
  /**
   * @brief   IN endpoint maximum packet size.
   * @details This field must be set to zero if the IN endpoint is not used.
   */
  uint16_t                      in_maxsize;
  /**
   * @brief   OUT endpoint maximum packet size.
   * @details This field must be set to zero if the OUT endpoint is not used.
   */
  uint16_t                      out_maxsize;
  /**
   * @brief   @p USBEndpointState associated to the IN endpoint.
   * @details This field must be set to @p NULL if the IN endpoint is not
   *          used.
   */
  USBInEndpointState            *in_state;
  /**
   * @brief   @p USBEndpointState associated to the OUT endpoint.
   * @details This field must be set to @p NULL if the OUT endpoint is not
   *          used.
   */
  USBOutEndpointState           *out_state;
  /* End of the mandatory fields.*/
  /**
   * @brief   Determines the space allocated for the TXFIFO as multiples of
   *          the packet size (@p in_maxsize). Note that zero is interpreted
   *          as one for simplicity and robustness.
   */
  uint16_t                      in_multiplier;
  /**
   * @brief   Pointer to a buffer for setup packets.
   * @details Setup packets require a dedicated 8-bytes buffer, set this
   *          field to @p NULL for non-control endpoints.
   */
  uint8_t                       *setup_buf;
} USBEndpointConfig;

/**
 * @brief   Type of an USB driver configuration structure.
 */
typedef struct {
  /**
   * @brief   USB events callback.
   * @details This callback is invoked when an USB driver event is registered.
   */
  usbeventcb_t                  event_cb;
  /**
   * @brief   Device GET_DESCRIPTOR request callback.
   * @note    This callback is mandatory and cannot be set to @p NULL.
   */
  usbgetdescriptor_t            get_descriptor_cb;
  /**
   * @brief   Requests hook callback.
   * @details This hook allows to be notified of standard requests or to
   *          handle non standard requests.
   */
  usbreqhandler_t               requests_hook_cb;
  /**
   * @brief   Start Of Frame callback.
   */
  usbcallback_t                 sof_cb;
  /* End of the mandatory fields.*/
} USBConfig;

/**
 * @brief   Structure representing an USB driver.
 */
struct USBDriver {
  /**
   * @brief   Driver state.
   */
  usbstate_t                    state;
  /**
   * @brief   Current configuration data.
   */
  const USBConfig               *config;
  /**
   * @brief   Bit map of the transmitting IN endpoints.
   */
  uint16_t                      transmitting;
  /**
   * @brief   Bit map of the receiving OUT endpoints.
   */
  uint16_t                      receiving;
  /**
   * @brief   Active endpoints configurations.
   */
  const USBEndpointConfig       *epc[USB_MAX_ENDPOINTS + 1];
  /**
   * @brief   Fields available to user, it can be used to associate an
   *          application-defined handler to an IN endpoint.
   * @note    The base index is one, the endpoint zero does not have a
   *          reserved element in this array.
   */
  void                          *in_params[USB_MAX_ENDPOINTS];
  /**
   * @brief   Fields available to user, it can be used to associate an
   *          application-defined handler to an OUT endpoint.
   * @note    The base index is one, the endpoint zero does not have a
   *          reserved element in this array.
   */
  void                          *out_params[USB_MAX_ENDPOINTS];
  /**
   * @brief   Endpoint 0 state.
   */
  usbep0state_t                 ep0state;
  /**
   * @brief   Next position in the buffer to be transferred through endpoint 0.
   */
  uint8_t                       *ep0next;
  /**
   * @brief   Number of bytes yet to be transferred through endpoint 0.
   */
  size_t                        ep0n;
  /**
   * @brief   Endpoint 0 end transaction callback.
   */
  usbcallback_t                 ep0endcb;
  /**
   * @brief   Setup packet buffer.
   */
  uint8_t                       setup[8];
  /**
   * @brief   Current USB device status.
   */
  uint16_t                      status;
  /**
   * @brief   Assigned USB address.
   */
  uint8_t                       address;
  /**
   * @brief   Current USB device configuration.
   */
  uint8_t                       configuration;
  /**
   * @brief   State of the driver when a suspend happened.
   */
  usbstate_t                    saved_state;
#if defined(USB_DRIVER_EXT_FIELDS)
  USB_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Pointer to the USBHS core associated to this driver.
   */
  gd32_otg_t                    *otg;
  /**
   * @brief   Peripheral-specific parameters.
   */
  const gd32_otg_params_t       *otgparams;
  /**
   * @brief   Pointer to the next address in the packet memory.
   */
  uint32_t                      pmnext;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the exact size of a receive transaction.
 * @details The received size can be different from the size specified in
 *          @p usbStartReceiveI() because the last packet could have a size
 *          different from the expected one.
 * @pre     The OUT endpoint must have been configured in transaction mode
 *          in order to use this function.
 *
 * @param[in] usbp      pointer to the @p USBDriver object
 * @param[in] ep        endpoint number
 * @return              Received data size.
 *
 * @notapi
 */
#define usb_lld_get_transaction_size(usbp, ep)                              \
  ((usbp)->epc[ep]->out_state->rxcnt)

/**
 * @brief   Connects the USB device.
 *
 * @notapi
 */
#define usb_lld_connect_bus(usbp) ((usbp)->otg->DCTL &= ~DCTL_SD)

/**
 * @brief   Disconnect the USB device.
 *
 * @notapi
 */
#define usb_lld_disconnect_bus(usbp) ((usbp)->otg->DCTL |= DCTL_SD)

/**
 * @brief   Start of host wake-up procedure.
 *
 * @notapi
 */
#define usb_lld_wakeup_host(usbp)                                           \
  do {                                                                      \
    /* Turning clocks back on, they may be off when coming out of the
       suspend state.*/                                                     \
    (usbp)->otg->PWRCLKCTL &= ~(PWRCLKCTL_SUCLK | PWRCLKCTL_SHCLK);         \
    (usbp)->otg->DCTL |= DCTL_RWKUP;                                        \
    /* A remote wakeup does not trigger the wakeup interrupt, the SOF
       interrupt is used instead to detect the bus resume.*/                \
    (usbp)->otg->GINTF |= GINTF_SOF;                                        \
    (usbp)->otg->GINTEN |= GINTEN_SOFIE;                                    \
    osalThreadSleepMilliseconds(GD32_USB_HOST_WAKEUP_DURATION);             \
    (usbp)->otg->DCTL &= ~DCTL_RWKUP;                                       \
  } while (false)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (GD32_USB_DEBUG_COUNTERS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Interrupt tracing counters, debug aid only.
 */
typedef struct {
  uint32_t              isr;
  uint32_t              rst;
  uint32_t              enumf;
  uint32_t              sp;
  uint32_t              wkup;
  uint32_t              sof;
  uint32_t              rxfne;
  uint32_t              setup;
  uint32_t              setup_done;
  uint32_t              start_in;
  uint32_t              timeout;
  uint32_t              epin;
  uint32_t              epout;
  uint32_t              last_sts;
  uint32_t              last_rstat;
  uint32_t              last_setup0;
  uint32_t              last_setup1;
  uint32_t              last_oepint;
  uint32_t              last_iepctl;
  uint32_t              last_ieplen;
  /* First interrupt events in arrival order, it tells if the host really
     reset the device or the bus just went idle.*/
  uint32_t              hist[8];
  uint32_t              nhist;
} gd32_usb_debug_t;

extern gd32_usb_debug_t gd32_usb_debug;
#endif

#if GD32_USB_USE_USBHS0 && !defined(__DOXYGEN__)
extern USBDriver USBD0;
#endif

#if GD32_USB_USE_USBHS1 && !defined(__DOXYGEN__)
extern USBDriver USBD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void usb_lld_init(void);
  msg_t usb_lld_start(USBDriver *usbp);
  void usb_lld_stop(USBDriver *usbp);
  void usb_lld_reset(USBDriver *usbp);
  void usb_lld_set_address(USBDriver *usbp);
  void usb_lld_init_endpoint(USBDriver *usbp, usbep_t ep);
  void usb_lld_disable_endpoints(USBDriver *usbp);
  usbepstatus_t usb_lld_get_status_in(USBDriver *usbp, usbep_t ep);
  usbepstatus_t usb_lld_get_status_out(USBDriver *usbp, usbep_t ep);
  void usb_lld_read_setup(USBDriver *usbp, usbep_t ep, uint8_t *buf);
  void usb_lld_start_out(USBDriver *usbp, usbep_t ep);
  void usb_lld_start_in(USBDriver *usbp, usbep_t ep);
  void usb_lld_stall_out(USBDriver *usbp, usbep_t ep);
  void usb_lld_stall_in(USBDriver *usbp, usbep_t ep);
  void usb_lld_clear_out(USBDriver *usbp, usbep_t ep);
  void usb_lld_clear_in(USBDriver *usbp, usbep_t ep);
  void usb_lld_serve_interrupt(USBDriver *usbp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_USB */

#endif /* HAL_USB_LLD_H */

/** @} */
