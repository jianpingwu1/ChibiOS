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
 * @file    USARTv1/hal_serial_lld.c
 * @brief   GD32H73x_75x low level serial driver code.
 *
 * @addtogroup SERIAL
 * @{
 */

#include "hal.h"

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USART0 serial driver identifier.*/
#if GD32_SERIAL_USE_USART0 || defined(__DOXYGEN__)
SerialDriver SD0;
#endif

/** @brief USART1 serial driver identifier.*/
#if GD32_SERIAL_USE_USART1 || defined(__DOXYGEN__)
SerialDriver SD1;
#endif

/** @brief USART2 serial driver identifier.*/
#if GD32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
SerialDriver SD2;
#endif

/** @brief UART3 serial driver identifier.*/
#if GD32_SERIAL_USE_UART3 || defined(__DOXYGEN__)
SerialDriver SD3;
#endif

/** @brief UART4 serial driver identifier.*/
#if GD32_SERIAL_USE_UART4 || defined(__DOXYGEN__)
SerialDriver SD4;
#endif

/** @brief USART5 serial driver identifier.*/
#if GD32_SERIAL_USE_USART5 || defined(__DOXYGEN__)
SerialDriver SD5;
#endif

/** @brief UART6 serial driver identifier.*/
#if GD32_SERIAL_USE_UART6 || defined(__DOXYGEN__)
SerialDriver SD6;
#endif

/** @brief UART7 serial driver identifier.*/
#if GD32_SERIAL_USE_UART7 || defined(__DOXYGEN__)
SerialDriver SD7;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/** @brief Driver default configuration.*/
static const SerialConfig default_config =
{
  SERIAL_DEFAULT_BITRATE,
  0,
  USART_STB_1BIT,
  0,
  NULL,
  NULL
};

#if defined(SERIAL_ADVANCED_BUFFERING_SUPPORT) && \
    (SERIAL_ADVANCED_BUFFERING_SUPPORT == TRUE) || defined(__DOXYGEN__)
#if GD32_SERIAL_USE_USART0 || defined(__DOXYGEN__)
/** @brief Input buffer for SD0.*/
static uint8_t sd_in_buf0[GD32_SERIAL_USART0_IN_BUF_SIZE];

/** @brief Output buffer for SD0.*/
static uint8_t sd_out_buf1[GD32_SERIAL_USART0_OUT_BUF_SIZE];
#endif

#if GD32_SERIAL_USE_USART1 || defined(__DOXYGEN__)
/** @brief Input buffer for SD1.*/
static uint8_t sd_in_buf1[GD32_SERIAL_USART1_IN_BUF_SIZE];

/** @brief Output buffer for SD1.*/
static uint8_t sd_out_buf2[GD32_SERIAL_USART1_OUT_BUF_SIZE];
#endif

#if GD32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
/** @brief Input buffer for SD2.*/
static uint8_t sd_in_buf2[GD32_SERIAL_USART2_IN_BUF_SIZE];

/** @brief Output buffer for SD2.*/
static uint8_t sd_out_buf3[GD32_SERIAL_USART2_OUT_BUF_SIZE];
#endif

#if GD32_SERIAL_USE_UART3 || defined(__DOXYGEN__)
/** @brief Input buffer for SD3.*/
static uint8_t sd_in_buf3[GD32_SERIAL_UART3_IN_BUF_SIZE];

/** @brief Output buffer for SD3.*/
static uint8_t sd_out_buf4[GD32_SERIAL_UART3_OUT_BUF_SIZE];
#endif

#if GD32_SERIAL_USE_UART4 || defined(__DOXYGEN__)
/** @brief Input buffer for SD4.*/
static uint8_t sd_in_buf4[GD32_SERIAL_UART4_IN_BUF_SIZE];

/** @brief Output buffer for SD4.*/
static uint8_t sd_out_buf5[GD32_SERIAL_UART4_OUT_BUF_SIZE];
#endif

#if GD32_SERIAL_USE_USART5 || defined(__DOXYGEN__)
/** @brief Input buffer for SD5.*/
static uint8_t sd_in_buf5[GD32_SERIAL_USART5_IN_BUF_SIZE];

/** @brief Output buffer for SD5.*/
static uint8_t sd_out_buf6[GD32_SERIAL_USART5_OUT_BUF_SIZE];
#endif

#if GD32_SERIAL_USE_UART6 || defined(__DOXYGEN__)
/** @brief Input buffer for SD6.*/
static uint8_t sd_in_buf6[GD32_SERIAL_UART6_IN_BUF_SIZE];

/** @brief Output buffer for SD6.*/
static uint8_t sd_out_buf7[GD32_SERIAL_UART6_OUT_BUF_SIZE];
#endif

#if GD32_SERIAL_USE_UART7 || defined(__DOXYGEN__)
/** @brief Input buffer for SD7.*/
static uint8_t sd_in_buf7[GD32_SERIAL_UART7_IN_BUF_SIZE];

/** @brief Output buffer for SD7.*/
static uint8_t sd_out_buf8[GD32_SERIAL_UART7_OUT_BUF_SIZE];
#endif
#endif /* SERIAL_ADVANCED_BUFFERING_SUPPORT == TRUE */

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   USART initialization.
 * @details This function must be invoked with interrupts disabled.
 * @note    The gd32_ prefix keeps these helpers out of the vendor peripheral
 *          library namespace: gd32h73x_75x_usart.h is pulled in through ch.h
 *          and already owns every usart_* symbol.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration
 */
static void gd32_usart_init(SerialDriver *sdp, const SerialConfig *config) {
  uint32_t baud;
  uint32_t u = sdp->usart;

  baud = (uint32_t)((sdp->clock + config->speed / 2) / config->speed);

  /* Correcting the BAUD value when oversampling by 8 instead of 16.
     Fraction is still 4 bits wide, but only the lower 3 bits are used.
     Mantissa is doubled, but Fraction is left the same.*/
  if (config->ctl0 & USART_CTL0_OVSMOD) {
    baud = ((baud & ~7U) * 2U) | (baud & 7U);
  }

  osalDbgAssert(baud < 0x10000U, "invalid BAUD value");

  USART_BAUD(u) = baud;

  /* Note that some bits are enforced.*/
  USART_CTL1(u) = config->ctl1 | USART_CTL1_LBDIE;
  USART_CTL2(u) = config->ctl2 | USART_CTL2_ERRIE;
  USART_CTL0(u) = config->ctl0 | USART_CTL0_UEN | USART_CTL0_PERRIE |
                                  USART_CTL0_RBNEIE | USART_CTL0_TEN |
                                  USART_CTL0_REN;

  /* Clearing any pending clearable status flags and flushing the
     RX/TX data paths.*/
  USART_INTC(u) = USART_INTC_PEC  | USART_INTC_FEC  | USART_INTC_NEC |
                  USART_INTC_OREC | USART_INTC_IDLEC | USART_INTC_TCC |
                  USART_INTC_LBDC;
  (void)USART_RDATA(u);

  /* Deciding mask to be applied on the data register on receive, this is
     required in order to mask out the parity bit. Only the WL0 bit is
     taken into account here (8-bit dataframes with/without parity, or
     9-bit dataframes without parity); the WL1 combinations (7-bit
     dataframes) are not handled by this simplified mask logic.*/
  if ((config->ctl0 & (USART_CTL0_WL0 | USART_CTL0_PCEN)) == USART_CTL0_PCEN) {
    sdp->rxmask = 0x7F;
  }
  else {
    sdp->rxmask = 0xFF;
  }
}

/**
 * @brief   USART de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] u         base address of an USART/UART registers block
 */
static void gd32_usart_deinit(uint32_t u) {

  USART_CTL0(u) = 0;
  USART_CTL1(u) = 0;
  USART_CTL2(u) = 0;
}

/**
 * @brief   Error handling routine.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] stat      USART STAT register value
 */
static void set_error(SerialDriver *sdp, uint32_t stat) {
  eventflags_t sts = 0;

  if (stat & USART_STAT_ORERR)
    sts |= SD_OVERRUN_ERROR;
  if (stat & USART_STAT_PERR)
    sts |= SD_PARITY_ERROR;
  if (stat & USART_STAT_FERR)
    sts |= SD_FRAMING_ERROR;
  if (stat & USART_STAT_NERR)
    sts |= SD_NOISE_ERROR;
  osalSysLockFromISR();
  chnAddFlagsI(sdp, sts);
  osalSysUnlockFromISR();
}

#if GD32_SERIAL_USE_USART0 || defined(__DOXYGEN__)
static void notify0(io_queue_t *qp) {

  (void)qp;
  USART_CTL0(USART0) |= USART_CTL0_TBEIE | USART_CTL0_TCIE;
}
#endif

#if GD32_SERIAL_USE_USART1 || defined(__DOXYGEN__)
static void notify1(io_queue_t *qp) {

  (void)qp;
  USART_CTL0(USART1) |= USART_CTL0_TBEIE | USART_CTL0_TCIE;
}
#endif

#if GD32_SERIAL_USE_USART2 || defined(__DOXYGEN__)
static void notify2(io_queue_t *qp) {

  (void)qp;
  USART_CTL0(USART2) |= USART_CTL0_TBEIE | USART_CTL0_TCIE;
}
#endif

#if GD32_SERIAL_USE_UART3 || defined(__DOXYGEN__)
static void notify3(io_queue_t *qp) {

  (void)qp;
  USART_CTL0(UART3) |= USART_CTL0_TBEIE | USART_CTL0_TCIE;
}
#endif

#if GD32_SERIAL_USE_UART4 || defined(__DOXYGEN__)
static void notify4(io_queue_t *qp) {

  (void)qp;
  USART_CTL0(UART4) |= USART_CTL0_TBEIE | USART_CTL0_TCIE;
}
#endif

#if GD32_SERIAL_USE_USART5 || defined(__DOXYGEN__)
static void notify5(io_queue_t *qp) {

  (void)qp;
  USART_CTL0(USART5) |= USART_CTL0_TBEIE | USART_CTL0_TCIE;
}
#endif

#if GD32_SERIAL_USE_UART6 || defined(__DOXYGEN__)
static void notify6(io_queue_t *qp) {

  (void)qp;
  USART_CTL0(UART6) |= USART_CTL0_TBEIE | USART_CTL0_TCIE;
}
#endif

#if GD32_SERIAL_USE_UART7 || defined(__DOXYGEN__)
static void notify7(io_queue_t *qp) {

  (void)qp;
  USART_CTL0(UART7) |= USART_CTL0_TBEIE | USART_CTL0_TCIE;
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level serial driver initialization.
 *
 * @notapi
 */
void sd_lld_init(void) {

#if GD32_SERIAL_USE_USART0
#if (SERIAL_ADVANCED_BUFFERING_SUPPORT == TRUE)
  sdObjectInit(&SD0);
  iqObjectInit(&SD0.iqueue, sd_in_buf0, sizeof sd_in_buf0, NULL, &SD0);
  oqObjectInit(&SD0.oqueue, sd_out_buf1, sizeof sd_out_buf1, notify0, &SD0);
#else
  sdObjectInit(&SD0, NULL, notify0);
#endif
  SD0.usart = USART0;
  SD0.clock = GD32_USART0CLK;
#endif

#if GD32_SERIAL_USE_USART1
#if (SERIAL_ADVANCED_BUFFERING_SUPPORT == TRUE)
  sdObjectInit(&SD1);
  iqObjectInit(&SD1.iqueue, sd_in_buf1, sizeof sd_in_buf1, NULL, &SD1);
  oqObjectInit(&SD1.oqueue, sd_out_buf2, sizeof sd_out_buf2, notify1, &SD1);
#else
  sdObjectInit(&SD1, NULL, notify1);
#endif
  SD1.usart = USART1;
  SD1.clock = GD32_USART1CLK;
#endif

#if GD32_SERIAL_USE_USART2
#if (SERIAL_ADVANCED_BUFFERING_SUPPORT == TRUE)
  sdObjectInit(&SD2);
  iqObjectInit(&SD2.iqueue, sd_in_buf2, sizeof sd_in_buf2, NULL, &SD2);
  oqObjectInit(&SD2.oqueue, sd_out_buf3, sizeof sd_out_buf3, notify2, &SD2);
#else
  sdObjectInit(&SD2, NULL, notify2);
#endif
  SD2.usart = USART2;
  SD2.clock = GD32_USART2CLK;
#endif

#if GD32_SERIAL_USE_UART3
#if (SERIAL_ADVANCED_BUFFERING_SUPPORT == TRUE)
  sdObjectInit(&SD3);
  iqObjectInit(&SD3.iqueue, sd_in_buf3, sizeof sd_in_buf3, NULL, &SD3);
  oqObjectInit(&SD3.oqueue, sd_out_buf4, sizeof sd_out_buf4, notify3, &SD3);
#else
  sdObjectInit(&SD3, NULL, notify3);
#endif
  SD3.usart = UART3;
  SD3.clock = GD32_UART3CLK;
#endif

#if GD32_SERIAL_USE_UART4
#if (SERIAL_ADVANCED_BUFFERING_SUPPORT == TRUE)
  sdObjectInit(&SD4);
  iqObjectInit(&SD4.iqueue, sd_in_buf4, sizeof sd_in_buf4, NULL, &SD4);
  oqObjectInit(&SD4.oqueue, sd_out_buf5, sizeof sd_out_buf5, notify4, &SD4);
#else
  sdObjectInit(&SD4, NULL, notify4);
#endif
  SD4.usart = UART4;
  SD4.clock = GD32_UART4CLK;
#endif

#if GD32_SERIAL_USE_USART5
#if (SERIAL_ADVANCED_BUFFERING_SUPPORT == TRUE)
  sdObjectInit(&SD5);
  iqObjectInit(&SD5.iqueue, sd_in_buf5, sizeof sd_in_buf5, NULL, &SD5);
  oqObjectInit(&SD5.oqueue, sd_out_buf6, sizeof sd_out_buf6, notify5, &SD5);
#else
  sdObjectInit(&SD5, NULL, notify5);
#endif
  SD5.usart = USART5;
  SD5.clock = GD32_USART5CLK;
#endif

#if GD32_SERIAL_USE_UART6
#if (SERIAL_ADVANCED_BUFFERING_SUPPORT == TRUE)
  sdObjectInit(&SD6);
  iqObjectInit(&SD6.iqueue, sd_in_buf6, sizeof sd_in_buf6, NULL, &SD6);
  oqObjectInit(&SD6.oqueue, sd_out_buf7, sizeof sd_out_buf7, notify6, &SD6);
#else
  sdObjectInit(&SD6, NULL, notify6);
#endif
  SD6.usart = UART6;
  SD6.clock = GD32_UART6CLK;
#endif

#if GD32_SERIAL_USE_UART7
#if (SERIAL_ADVANCED_BUFFERING_SUPPORT == TRUE)
  sdObjectInit(&SD7);
  iqObjectInit(&SD7.iqueue, sd_in_buf7, sizeof sd_in_buf7, NULL, &SD7);
  oqObjectInit(&SD7.oqueue, sd_out_buf8, sizeof sd_out_buf8, notify7, &SD7);
#else
  sdObjectInit(&SD7, NULL, notify7);
#endif
  SD7.usart = UART7;
  SD7.clock = GD32_UART7CLK;
#endif
}

/**
 * @brief   Low level serial driver configuration and (re)start.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 * @param[in] config    the architecture-dependent serial driver configuration.
 *                      If this parameter is set to @p NULL then a default
 *                      configuration is used.
 *
 * @notapi
 */
void sd_lld_start(SerialDriver *sdp, const SerialConfig *config) {

  if (config == NULL)
    config = &default_config;

  sdp->config = config;

  if (sdp->state == SD_STOP) {
#if GD32_SERIAL_USE_USART0
    if (&SD0 == sdp) {
      rcuEnableUSART0(true);
    }
#endif
#if GD32_SERIAL_USE_USART1
    if (&SD1 == sdp) {
      rcuEnableUSART1(true);
    }
#endif
#if GD32_SERIAL_USE_USART2
    if (&SD2 == sdp) {
      rcuEnableUSART2(true);
    }
#endif
#if GD32_SERIAL_USE_UART3
    if (&SD3 == sdp) {
      rcuEnableUART3(true);
    }
#endif
#if GD32_SERIAL_USE_UART4
    if (&SD4 == sdp) {
      rcuEnableUART4(true);
    }
#endif
#if GD32_SERIAL_USE_USART5
    if (&SD5 == sdp) {
      rcuEnableUSART5(true);
    }
#endif
#if GD32_SERIAL_USE_UART6
    if (&SD6 == sdp) {
      rcuEnableUART6(true);
    }
#endif
#if GD32_SERIAL_USE_UART7
    if (&SD7 == sdp) {
      rcuEnableUART7(true);
    }
#endif
  }
  gd32_usart_init(sdp, config);
}

/**
 * @brief   Low level serial driver stop.
 * @details De-initializes the USART, stops the associated clock, resets the
 *          interrupt vector.
 *
 * @param[in] sdp       pointer to a @p SerialDriver object
 *
 * @notapi
 */
void sd_lld_stop(SerialDriver *sdp) {

  if (sdp->state == SD_READY) {
    gd32_usart_deinit(sdp->usart);
#if GD32_SERIAL_USE_USART0
    if (&SD0 == sdp) {
      rcuDisableUSART0();
      return;
    }
#endif
#if GD32_SERIAL_USE_USART1
    if (&SD1 == sdp) {
      rcuDisableUSART1();
      return;
    }
#endif
#if GD32_SERIAL_USE_USART2
    if (&SD2 == sdp) {
      rcuDisableUSART2();
      return;
    }
#endif
#if GD32_SERIAL_USE_UART3
    if (&SD3 == sdp) {
      rcuDisableUART3();
      return;
    }
#endif
#if GD32_SERIAL_USE_UART4
    if (&SD4 == sdp) {
      rcuDisableUART4();
      return;
    }
#endif
#if GD32_SERIAL_USE_USART5
    if (&SD5 == sdp) {
      rcuDisableUSART5();
      return;
    }
#endif
#if GD32_SERIAL_USE_UART6
    if (&SD6 == sdp) {
      rcuDisableUART6();
      return;
    }
#endif
#if GD32_SERIAL_USE_UART7
    if (&SD7 == sdp) {
      rcuDisableUART7();
      return;
    }
#endif
  }
}

/**
 * @brief   Common IRQ handler.
 *
 * @param[in] sdp       communication channel associated to the USART
 */
void sd_lld_serve_interrupt(SerialDriver *sdp) {
  uint32_t u = sdp->usart;
  uint32_t ctl0;
  uint32_t stat = USART_STAT(u);

  /* Special case, LIN break detection.*/
  if (stat & USART_STAT_LBDF) {
    osalSysLockFromISR();
    chnAddFlagsI(sdp, SD_BREAK_DETECTED);
    USART_INTC(u) = USART_INTC_LBDC;
    osalSysUnlockFromISR();
  }

  /* Data available.*/
  while (stat & (USART_STAT_RBNE | USART_STAT_ORERR | USART_STAT_NERR |
                 USART_STAT_FERR | USART_STAT_PERR)) {
    uint8_t b;

    /* Error condition detection.*/
    if (stat & (USART_STAT_ORERR | USART_STAT_NERR | USART_STAT_FERR |
                USART_STAT_PERR)) {
      set_error(sdp, stat);
      USART_INTC(u) = USART_INTC_PEC | USART_INTC_FEC | USART_INTC_NEC |
                      USART_INTC_OREC;
    }
    b = (uint8_t)USART_RDATA(u) & sdp->rxmask;
    if (stat & USART_STAT_RBNE) {
      osalSysLockFromISR();
      sdIncomingDataI(sdp, b);
      osalSysUnlockFromISR();
    }
    stat = USART_STAT(u);
  }

  /* Caching CTL0.*/
  ctl0 = USART_CTL0(u);

  /* Transmission buffer empty.*/
  if ((ctl0 & USART_CTL0_TBEIE) && (stat & USART_STAT_TBE)) {
    msg_t b;
    osalSysLockFromISR();
    b = oqGetI(&sdp->oqueue);
    if (b < MSG_OK) {
      chnAddFlagsI(sdp, CHN_OUTPUT_EMPTY);
      ctl0 &= ~USART_CTL0_TBEIE;
    }
    else {
      USART_TDATA(u) = (uint32_t)b;
    }
    osalSysUnlockFromISR();
  }

  /* Physical transmission end.*/
  if ((ctl0 & USART_CTL0_TCIE) && (stat & USART_STAT_TC)) {
    osalSysLockFromISR();
    if (oqIsEmptyI(&sdp->oqueue)) {
      chnAddFlagsI(sdp, CHN_TRANSMISSION_END);
      ctl0 &= ~USART_CTL0_TCIE;
    }
    USART_INTC(u) = USART_INTC_TCC;
    osalSysUnlockFromISR();
  }

  /* Writing CTL0 once.*/
  USART_CTL0(u) = ctl0;

  /* Idle line detected. The flag must be cleared here, otherwise it stays
     set and the interrupt keeps re-triggering. */
  if (stat & USART_STAT_IDLEF) {
    USART_INTC(u) = USART_INTC_IDLEC;
    _serial_irq_code(sdp);
  }
}

#endif /* HAL_USE_SERIAL */

/** @} */
