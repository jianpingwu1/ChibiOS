/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    FDCANv2/hal_can_lld.c
 * @brief   GD32H73x/75x CAN-FD subsystem low level driver source.
 *
 * @addtogroup CAN
 * @{
 */

#include "hal.h"

#if HAL_USE_CAN || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define GD32_CAN_MODE_TIMEOUT             0x00100000U
#define GD32_CAN_MDES0_CODE(code)         (((uint32_t)(code) << 24U) &       \
                                            CAN_MDES0_CODE)
#define GD32_CAN_MDES0_DLC(dlc)           (((uint32_t)(dlc) << 16U) &        \
                                            CAN_MDES0_DLC)
#define GD32_CAN_MDES1_STD(id)            (((uint32_t)(id) << 18U) &         \
                                            CAN_MDES1_ID_STD)
#define GD32_CAN_MDES1_EXT(id)            ((uint32_t)(id) &                 \
                                            (CAN_MDES1_ID_EXD |             \
                                             CAN_MDES1_ID_STD))

#define GD32_CAN_ERROR_CLEAR_MASK         (CAN_ERR1_ERRSF | CAN_ERR1_BOF |   \
                                            CAN_ERR1_RWERRIF |              \
                                            CAN_ERR1_TWERRIF |              \
                                            CAN_ERR1_BORF | CAN_ERR1_ERRFSF)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if GD32_CAN_USE_CAN0 || defined(__DOXYGEN__)
CANDriver CAND0;
#endif

#if GD32_CAN_USE_CAN1 || defined(__DOXYGEN__)
CANDriver CAND1;
#endif

#if GD32_CAN_USE_CAN2 || defined(__DOXYGEN__)
CANDriver CAND2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const uint8_t dlc_to_bytes[16] = {
  0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U,
  8U, 12U, 16U, 20U, 24U, 32U, 48U, 64U
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void gd32_can_clock_enable(CANDriver *canp) {

#if GD32_CAN_USE_CAN0
  if (canp == &CAND0) {
    rcuEnableCAN0(TRUE);
    rcuResetCAN0();
    return;
  }
#endif
#if GD32_CAN_USE_CAN1
  if (canp == &CAND1) {
    rcuEnableCAN1(TRUE);
    rcuResetCAN1();
    return;
  }
#endif
#if GD32_CAN_USE_CAN2
  if (canp == &CAND2) {
    rcuEnableCAN2(TRUE);
    rcuResetCAN2();
  }
#endif
}

static void gd32_can_clock_disable(CANDriver *canp) {

#if GD32_CAN_USE_CAN0
  if (canp == &CAND0) {
    rcuDisableCAN0();
    return;
  }
#endif
#if GD32_CAN_USE_CAN1
  if (canp == &CAND1) {
    rcuDisableCAN1();
    return;
  }
#endif
#if GD32_CAN_USE_CAN2
  if (canp == &CAND2) {
    rcuDisableCAN2();
  }
#endif
}

static bool gd32_can_software_reset(CANDriver *canp) {
  uint32_t timeout;

  CAN_CTL0(canp->can) |= CAN_CTL0_SWRST;
  timeout = GD32_CAN_MODE_TIMEOUT;
  while (((CAN_CTL0(canp->can) & CAN_CTL0_SWRST) != 0U) &&
         (timeout > 0U)) {
    timeout--;
  }

  return (CAN_CTL0(canp->can) & CAN_CTL0_SWRST) != 0U;
}

static bool gd32_can_enter_inactive(CANDriver *canp) {
  uint32_t timeout;

  /* CANDIS and PNEN can be changed before the inactive state is entered.
     PNMOD is protected and is cleared only after INAS is asserted.*/
  CAN_CTL0(canp->can) &= ~(CAN_CTL0_CANDIS | CAN_CTL0_PNEN);
  CAN_CTL0(canp->can) |= CAN_CTL0_HALT | CAN_CTL0_INAMOD;

  timeout = GD32_CAN_MODE_TIMEOUT;
  while (((CAN_CTL0(canp->can) & (CAN_CTL0_NRDY | CAN_CTL0_INAS)) !=
          (CAN_CTL0_NRDY | CAN_CTL0_INAS)) && (timeout > 0U)) {
    timeout--;
  }

  if (timeout == 0U) {
    return true;
  }

  CAN_CTL0(canp->can) &= ~CAN_CTL0_PNMOD;
  return false;
}

static bool gd32_can_enter_active(CANDriver *canp) {
  uint32_t timeout;

  CAN_CTL0(canp->can) &= ~(CAN_CTL0_HALT | CAN_CTL0_INAMOD);
  timeout = GD32_CAN_MODE_TIMEOUT;
  while (((CAN_CTL0(canp->can) & CAN_CTL0_INAS) != 0U) &&
         (timeout > 0U)) {
    timeout--;
  }

  return timeout == 0U;
}

static uint32_t *gd32_can_mailbox(CANDriver *canp, uint32_t index) {

  return (uint32_t *)GD32_CAN_MAILBOX_ADDRESS(canp->can, index);
}

static uint32_t gd32_can_mailbox_code(CANDriver *canp, uint32_t index) {

  return GET_MDES0_CODE(*gd32_can_mailbox(canp, index));
}

static void gd32_can_swap_words(uint32_t *dst,
                                const uint32_t *src,
                                uint32_t bytes) {
  uint32_t words = (bytes + 3U) / 4U;

  for (uint32_t i = 0U; i < words; i++) {
    uint32_t value = src[i];

    dst[i] = ((value >> 24U) & 0x000000FFU) |
             ((value >> 8U)  & 0x0000FF00U) |
             ((value << 8U)  & 0x00FF0000U) |
             ((value << 24U) & 0xFF000000U);
  }
}

static void gd32_can_configure_rx_mailbox(CANDriver *canp, uint32_t index) {
  uint32_t *mailbox = gd32_can_mailbox(canp, index);

  CAN_STAT(canp->can) = (1UL << index);
  mailbox[1] = 0U;
  mailbox[0] = GD32_CAN_MDES0_CODE(CAN_MB_RX_STATUS_EMPTY);
  CAN_RFIFOMPF(canp->can, index) = GD32_CAN_ACCEPT_ALL_FILTER;
}

static int32_t gd32_can_find_rx_mailbox(CANDriver *canp) {

  for (uint32_t index = 0U; index < GD32_CAN_RX_MAILBOX_COUNT; index++) {
    uint32_t code = gd32_can_mailbox_code(canp, index);

    if ((code == CAN_MB_RX_STATUS_FULL) ||
        (code == CAN_MB_RX_STATUS_OVERRUN)) {
      return (int32_t)index;
    }
  }

  return -1;
}

static int32_t gd32_can_find_tx_mailbox(CANDriver *canp) {

  for (uint32_t index = GD32_CAN_TX_MAILBOX_FIRST;
       index < GD32_CAN_MAILBOX_COUNT; index++) {
    if (gd32_can_mailbox_code(canp, index) ==
        CAN_MB_TX_STATUS_INACTIVE) {
      return (int32_t)index;
    }
  }

  return -1;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

void can_lld_init(void) {

#if GD32_CAN_USE_CAN0
  canObjectInit(&CAND0);
  CAND0.can = CAN0;
  CAND0.clock = GD32_CAN0CLK;
#endif
#if GD32_CAN_USE_CAN1
  canObjectInit(&CAND1);
  CAND1.can = CAN1;
  CAND1.clock = GD32_CAN1CLK;
#endif
#if GD32_CAN_USE_CAN2
  canObjectInit(&CAND2);
  CAND2.can = CAN2;
  CAND2.clock = GD32_CAN2CLK;
#endif
}

bool can_lld_start(CANDriver *canp) {
  uint32_t ctl1;
  uint32_t ctl2;
  uint32_t fdctl;

  osalDbgCheck((canp != NULL) && (canp->config != NULL));

  if ((canp->config->op_mode != OPMODE_FDCAN) &&
      (canp->config->op_mode != OPMODE_CAN)) {
    return true;
  }
  gd32_can_clock_enable(canp);
  if (gd32_can_software_reset(canp) || gd32_can_enter_inactive(canp)) {
    gd32_can_clock_disable(canp);
    return true;
  }

  for (uint32_t index = 0U; index < GD32_CAN_MAILBOX_COUNT; index++) {
    uint32_t *mailbox = gd32_can_mailbox(canp, index);

    for (uint32_t word = 0U; word < GD32_CAN_MAILBOX_WORDS; word++) {
      mailbox[word] = 0U;
    }
    CAN_RFIFOMPF(canp->can, index) = 0U;
  }

  CAN_INTEN(canp->can) = 0U;
  CAN_STAT(canp->can) = GD32_CAN_MAILBOX_INT_MASK;
  CAN_RMPUBF(canp->can) = canp->config->rx_public_filter;
  /* RX FIFO is unused (RFEN stays cleared), keep its filter mask reset.*/
  CAN_RFIFOPUBF(canp->can) = 0U;
  CAN_BT(canp->can) = canp->config->bt;

  /* BSPMOD must stay cleared, the FD engine only supports single
     sampling.*/
  ctl1 = canp->config->ctl1 &
         (CAN_CTL1_MMOD | CAN_CTL1_MTO | CAN_CTL1_TSYNC |
          CAN_CTL1_ABORDIS | CAN_CTL1_LSCMOD);
  ctl1 |= CAN_CTL1_RWERRIE | CAN_CTL1_TWERRIE |
          CAN_CTL1_ERRSIE | CAN_CTL1_BOIE;
  CAN_CTL1(canp->can) = ctl1;

  /* Use the validated CTL2 baseline from the GD32 loopback reference. */
  ctl2 = canp->config->ctl2 & (CAN_CTL2_PREEN | CAN_CTL2_ITSRC);
  ctl2 |= CAN_CTL2_ISO | CAN_CTL2_EFDIS | CAN_CTL2_IDERTR_RMF |
          CAN_CTL2_RRFRMS | CAN_CTL2_RFO | CTL2_ASD(0x14U);
  CAN_CTL2(canp->can) = ctl2 | CAN_CTL2_BORIE | CAN_CTL2_ERRFSIE;

  CAN_CTL0(canp->can) &= ~(CAN_CTL0_RFEN | CAN_CTL0_DMAEN |
                           CAN_CTL0_RPFQEN | CAN_CTL0_MSZ);
  /* MSZ units are a fixed 16-byte granule, always request the full
     512-byte CAN RAM regardless of the mailbox data size in use.*/
  /* RPFQEN enables the per-mailbox private filters configured below. */
  CAN_CTL0(canp->can) |= CAN_CTL0_FDEN | CAN_CTL0_MST |
                         CAN_CTL0_SRDIS | CAN_CTL0_RPFQEN |
                         CAN_MEMSIZE_32_UNIT;

  /* FD timing and mailbox data size are only programmed once FDEN is
     asserted.*/
  CAN_FDBT(canp->can) = canp->config->fdbt &
                         (CAN_FDBT_DBAUDPSC | CAN_FDBT_DSJW |
                          CAN_FDBT_DPTS | CAN_FDBT_DPBS1 | CAN_FDBT_DPBS2);
  fdctl = CAN_FDCTL(canp->can);
  fdctl &= ~(CAN_FDCTL_TDCO | CAN_FDCTL_TDCEN |
             CAN_FDCTL_MDSZ | CAN_FDCTL_BRSEN);
  fdctl |= CAN_MAILBOX_DATA_SIZE_64_BYTES |
           (canp->config->fdctl &
            (CAN_FDCTL_TDCO | CAN_FDCTL_TDCEN | CAN_FDCTL_BRSEN));
  CAN_FDCTL(canp->can) = fdctl;

  for (uint32_t index = 0U; index < GD32_CAN_RX_MAILBOX_COUNT; index++) {
    gd32_can_configure_rx_mailbox(canp, index);
  }
  for (uint32_t index = GD32_CAN_TX_MAILBOX_FIRST;
       index < GD32_CAN_MAILBOX_COUNT; index++) {
    uint32_t *mailbox = gd32_can_mailbox(canp, index);

    mailbox[0] = GD32_CAN_MDES0_CODE(CAN_MB_TX_STATUS_INACTIVE);
  }

  CAN_STAT(canp->can) = GD32_CAN_MAILBOX_INT_MASK;
  CAN_INTEN(canp->can) = GD32_CAN_MAILBOX_INT_MASK;

  if (gd32_can_enter_active(canp)) {
    CAN_INTEN(canp->can) = 0U;
    gd32_can_clock_disable(canp);
    return true;
  }

  return false;
}

void can_lld_stop(CANDriver *canp) {

  if (canp->state == CAN_READY) {
    CAN_INTEN(canp->can) = 0U;
    CAN_CTL1(canp->can) &= ~(CAN_CTL1_RWERRIE | CAN_CTL1_TWERRIE |
                             CAN_CTL1_ERRSIE | CAN_CTL1_BOIE);
    CAN_CTL2(canp->can) &= ~(CAN_CTL2_BORIE | CAN_CTL2_ERRFSIE);
    (void)gd32_can_enter_inactive(canp);
    CAN_STAT(canp->can) = GD32_CAN_MAILBOX_INT_MASK;
    gd32_can_clock_disable(canp);
  }
}

bool can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox) {

  (void)mailbox;
  return gd32_can_find_tx_mailbox(canp) >= 0;
}

void can_lld_transmit(CANDriver *canp,
                      canmbx_t mailbox,
                      const CANTxFrame *ctfp) {
  int32_t index;
  uint32_t bytes;
  uint32_t descriptor0;
  uint32_t descriptor1;
  uint32_t *address;

  (void)mailbox;
  osalDbgCheck((canp != NULL) && (ctfp != NULL));
  osalDbgAssert(ctfp->DLC < 16U, "invalid CAN-FD DLC");
  osalDbgAssert((canp->config->op_mode == OPMODE_FDCAN) ||
                (ctfp->FDF == 0U), "FD frame in CAN mode");

  index = gd32_can_find_tx_mailbox(canp);
  osalDbgAssert(index >= 0, "no CAN-FD transmit mailbox");
  if (index < 0) {
    return;
  }

  bytes = dlc_to_bytes[ctfp->DLC];
  descriptor0 = GD32_CAN_MDES0_DLC(ctfp->DLC) |
                GD32_CAN_MDES0_CODE(CAN_MB_TX_STATUS_DATA);
  descriptor1 = 0U;

  if (ctfp->common.RTR != 0U) {
    descriptor0 |= CAN_MDES0_RTR;
  }
  if (ctfp->common.XTD != 0U) {
    descriptor0 |= CAN_MDES0_IDE | CAN_MDES0_SRR;
    descriptor1 = GD32_CAN_MDES1_EXT(ctfp->ext.EID);
  }
  else {
    descriptor1 = GD32_CAN_MDES1_STD(ctfp->std.SID);
  }
  if (ctfp->common.ESI != 0U) {
    descriptor0 |= CAN_MDES0_ESI;
  }
  if (ctfp->FDF != 0U) {
    descriptor0 |= CAN_MDES0_FDF;
    descriptor0 &= ~CAN_MDES0_RTR;
    if (ctfp->BPS != 0U) {
      descriptor0 |= CAN_MDES0_BRS;
    }
  }

  address = gd32_can_mailbox(canp, (uint32_t)index);
  CAN_STAT(canp->can) = 1UL << (uint32_t)index;
  for (uint32_t word = 2U; word < GD32_CAN_MAILBOX_WORDS; word++) {
    address[word] = 0U;
  }
  gd32_can_swap_words(&address[2], ctfp->data32, bytes);
  address[1] = descriptor1;
  address[0] = descriptor0;
}

bool can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox) {

  if ((mailbox != CAN_ANY_MAILBOX) && (mailbox != 1U)) {
    return false;
  }

  return gd32_can_find_rx_mailbox(canp) >= 0;
}

void can_lld_receive(CANDriver *canp,
                     canmbx_t mailbox,
                     CANRxFrame *crfp) {
  int32_t index;
  uint32_t descriptor0;
  uint32_t descriptor1;
  uint32_t bytes;
  uint32_t *address;

  osalDbgCheck((canp != NULL) && (crfp != NULL));
  if ((mailbox != CAN_ANY_MAILBOX) && (mailbox != 1U)) {
    return;
  }

  index = gd32_can_find_rx_mailbox(canp);
  if (index < 0) {
    return;
  }

  address = gd32_can_mailbox(canp, (uint32_t)index);
  descriptor0 = address[0];
  descriptor1 = address[1];
  bytes = dlc_to_bytes[GET_MDES0_DLC(descriptor0)];

  crfp->header32[0] = 0U;
  crfp->header32[1] = 0U;
  if ((descriptor0 & CAN_MDES0_IDE) != 0U) {
    crfp->common.XTD = 1U;
    crfp->ext.EID = GET_MDES1_ID_EXD(descriptor1);
  }
  else {
    crfp->std.SID = GET_MDES1_ID_STD(descriptor1);
  }
  crfp->common.RTR = (descriptor0 & CAN_MDES0_RTR) != 0U;
  crfp->common.ESI = (descriptor0 & CAN_MDES0_ESI) != 0U;
  crfp->RXTS = descriptor0 & CAN_MDES0_TIMESTAMP;
  crfp->DLC = GET_MDES0_DLC(descriptor0);
  crfp->BRS = (descriptor0 & CAN_MDES0_BRS) != 0U;
  crfp->FDF = (descriptor0 & CAN_MDES0_FDF) != 0U;
  crfp->FIDX = (uint32_t)index;

  gd32_can_swap_words(crfp->data32, &address[2], bytes);
  CAN_STAT(canp->can) = 1UL << (uint32_t)index;
  (void)CAN_TIMER(canp->can);
  gd32_can_configure_rx_mailbox(canp, (uint32_t)index);
}

void can_lld_abort(CANDriver *canp, canmbx_t mailbox) {

  (void)mailbox;
  for (uint32_t index = GD32_CAN_TX_MAILBOX_FIRST;
       index < GD32_CAN_MAILBOX_COUNT; index++) {
    uint32_t *address = gd32_can_mailbox(canp, index);
    uint32_t descriptor0 = address[0];

    if (GET_MDES0_CODE(descriptor0) == CAN_MB_TX_STATUS_DATA) {
      descriptor0 &= ~CAN_MDES0_CODE;
      address[0] = descriptor0 |
                   GD32_CAN_MDES0_CODE(CAN_MB_TX_STATUS_ABORT);
    }
  }
}

#if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
void can_lld_sleep(CANDriver *canp) {
  uint32_t timeout;

  CAN_CTL0(canp->can) |= CAN_CTL0_CANDIS;
  timeout = GD32_CAN_MODE_TIMEOUT;
  while (((CAN_CTL0(canp->can) & (CAN_CTL0_NRDY | CAN_CTL0_LPS)) !=
          (CAN_CTL0_NRDY | CAN_CTL0_LPS)) && (timeout > 0U)) {
    timeout--;
  }
  osalDbgAssert(timeout > 0U, "CAN sleep timeout");
}

void can_lld_wakeup(CANDriver *canp) {
  uint32_t timeout;

  CAN_CTL0(canp->can) &= ~CAN_CTL0_CANDIS;
  timeout = GD32_CAN_MODE_TIMEOUT;
  while (((CAN_CTL0(canp->can) & CAN_CTL0_LPS) != 0U) &&
         (timeout > 0U)) {
    timeout--;
  }
  osalDbgAssert(timeout > 0U, "CAN wakeup timeout");
}
#endif

void can_lld_serve_interrupt(CANDriver *canp) {
  uint32_t status = CAN_STAT(canp->can) & CAN_INTEN(canp->can);
  uint32_t errors = CAN_ERR1(canp->can);
  eventflags_t flags = 0U;

  if ((status & GD32_CAN_RX_INT_MASK) != 0U) {
    CAN_STAT(canp->can) = status & GD32_CAN_RX_INT_MASK;
    _can_rx_full_isr(canp, CAN_MAILBOX_TO_MASK(1U));
  }
  if ((status & GD32_CAN_TX_INT_MASK) != 0U) {
    CAN_STAT(canp->can) = status & GD32_CAN_TX_INT_MASK;
    for (uint32_t index = GD32_CAN_TX_MAILBOX_FIRST;
         index < GD32_CAN_MAILBOX_COUNT; index++) {
      if ((status & (1UL << index)) != 0U) {
        uint32_t *address = gd32_can_mailbox(canp, index);

        address[0] = (address[0] & ~CAN_MDES0_CODE) |
                     GD32_CAN_MDES0_CODE(CAN_MB_TX_STATUS_INACTIVE);
      }
    }
    _can_tx_empty_isr(canp, CAN_MAILBOX_TO_MASK(1U));
  }

  if ((errors & (CAN_ERR1_RWERRIF | CAN_ERR1_TWERRIF)) != 0U) {
    flags |= CAN_LIMIT_WARNING;
  }
  if ((errors & CAN_ERR1_ERRSI) != 0U) {
    flags |= CAN_LIMIT_ERROR;
  }
  if ((errors & CAN_ERR1_BOF) != 0U) {
    flags |= CAN_BUS_OFF_ERROR;
  }
  if ((errors & (CAN_ERR1_ERRSF | CAN_ERR1_ERRFSF |
                 CAN_ERR1_ERROVR)) != 0U) {
    flags |= CAN_FRAMING_ERROR;
  }
  if (flags != 0U) {
    CAN_ERR1(canp->can) = errors & GD32_CAN_ERROR_CLEAR_MASK;
    _can_error_isr(canp, flags);
  }
}

void canGD32SetFilters(CANDriver *canp,
                       uint8_t num,
                       const CANFilter *cfp) {

  (void)canp;
  (void)cfp;
  osalDbgAssert(num == 0U, "GD32 CAN-FD filters require UM validation");
}

#endif /* HAL_USE_CAN */

/** @} */