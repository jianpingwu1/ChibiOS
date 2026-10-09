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
 * @file    USARTv1/hal_sio_lld.c
 * @brief   GD32H73x_75x SIO subsystem low level driver source.
 *
 * @addtogroup SIO
 * @{
 */

#include "hal.h"

#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Bit positions, the vendor header only provides the bit masks.*/
#define GD32_USART_CTL0_IDLEIE_POS      4U
#define GD32_USART_CTL0_RBNEIE_POS      5U
#define GD32_USART_CTL0_TCIE_POS        6U
#define GD32_USART_CTL0_TBEIE_POS       7U
#define GD32_USART_CTL0_PERRIE_POS      8U
#define GD32_USART_CTL1_LBDIE_POS       6U
#define GD32_USART_STAT_PERR_POS        0U
#define GD32_USART_STAT_ORERR_POS       3U
#define GD32_USART_STAT_IDLEF_POS       4U
#define GD32_USART_STAT_RBNE_POS        5U
#define GD32_USART_STAT_TC_POS          6U
#define GD32_USART_STAT_TBE_POS         7U
#define GD32_USART_STAT_LBDF_POS        8U

/* This mask includes IDLEF, ORERR, NERR, FERR, PERR bits.*/
#define GD32_USART_STAT_IONFP_POS       GD32_USART_STAT_PERR_POS
#define GD32_USART_STAT_IONFP_MASK      (0x1FU << GD32_USART_STAT_IONFP_POS)

/* This mask includes ORERR, NERR, FERR, PERR bits.*/
#define GD32_USART_STAT_ONFP_POS        GD32_USART_STAT_PERR_POS
#define GD32_USART_STAT_ONFP_MASK       (0x0FU << GD32_USART_STAT_ONFP_POS)

/* Clearable flags handled by this driver, INTC and STAT share the same bit
   positions for all of them.*/
#define GD32_USART_INTC_ALL_FLAGS       (USART_INTC_PEC   | USART_INTC_FEC  |\
                                         USART_INTC_NEC   | USART_INTC_OREC |\
                                         USART_INTC_IDLEC | USART_INTC_TCC  |\
                                         USART_INTC_LBDC)

/* Interrupt sources and operation enable bits are owned by the driver.*/
#define GD32_USART_CTL0_CFG_FORBIDDEN   (USART_CTL0_AMIE1  | USART_CTL0_EBIE  |\
                                         USART_CTL0_RTIE   | USART_CTL0_AMIE0 |\
                                         USART_CTL0_PERRIE | USART_CTL0_TBEIE |\
                                         USART_CTL0_TCIE   | USART_CTL0_RBNEIE|\
                                         USART_CTL0_IDLEIE | USART_CTL0_TEN   |\
                                         USART_CTL0_REN    | USART_CTL0_UEN)
#define GD32_USART_CTL1_CFG_FORBIDDEN   (USART_CTL1_LBDIE)
#define GD32_USART_CTL2_CFG_FORBIDDEN   (USART_CTL2_WUIE  | USART_CTL2_CTSIE |\
                                         USART_CTL2_ERRIE)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USART0 SIO driver identifier.*/
#if (GD32_SIO_USE_USART0 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD0;
#endif

/** @brief USART1 SIO driver identifier.*/
#if (GD32_SIO_USE_USART1 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD1;
#endif

/** @brief USART2 SIO driver identifier.*/
#if (GD32_SIO_USE_USART2 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD2;
#endif

/** @brief UART3 SIO driver identifier.*/
#if (GD32_SIO_USE_UART3 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD3;
#endif

/** @brief UART4 SIO driver identifier.*/
#if (GD32_SIO_USE_UART4 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD4;
#endif

/** @brief USART5 SIO driver identifier.*/
#if (GD32_SIO_USE_USART5 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD5;
#endif

/** @brief UART6 SIO driver identifier.*/
#if (GD32_SIO_USE_UART6 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD6;
#endif

/** @brief UART7 SIO driver identifier.*/
#if (GD32_SIO_USE_UART7 == TRUE) || defined(__DOXYGEN__)
SIODriver SIOD7;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Driver default configuration.
 * @note    In this implementation it is: 38400-8-N-1.
 */
static const SIOConfig default_config = {
  .baud = SIO_DEFAULT_BITRATE,
  .ctl0 = USART_WL_8BIT,
  .ctl1 = USART_STB_1BIT,
  .ctl2 = 0U
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Enables the RX related interrupt sources.
 * @note    The gd32_ prefix keeps these helpers out of the vendor peripheral
 *          library namespace, gd32h73x_75x_usart.h already owns every
 *          usart_* symbol.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 */
static inline void gd32_sio_enable_rx_irq(SIODriver *siop) {
  uint32_t ctl0 = USART_CTL0(siop->usart);

  if ((siop->enabled & SIO_EV_RXNOTEMPY) != 0U) {
    ctl0 |= USART_CTL0_RBNEIE;
  }
  if ((siop->enabled & SIO_EV_RXIDLE) != 0U) {
    ctl0 |= USART_CTL0_IDLEIE;
  }
  USART_CTL0(siop->usart) = ctl0;
}

/**
 * @brief   Enables the RX error related interrupt sources.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 */
static inline void gd32_sio_enable_rx_errors_irq(SIODriver *siop) {
  uint32_t u = siop->usart;

  if ((siop->enabled & SIO_EV_PARITY_ERR) != 0U) {
    USART_CTL0(u) |= USART_CTL0_PERRIE;
  }
  if ((siop->enabled & SIO_EV_RXBREAK) != 0U) {
    USART_CTL1(u) |= USART_CTL1_LBDIE;
  }

  /* The following 3 are grouped.*/
  if ((siop->enabled & (SIO_EV_FRAMING_ERR |
                        SIO_EV_OVERRUN_ERR |
                        SIO_EV_NOISE_ERR)) != 0U) {
    USART_CTL2(u) |= USART_CTL2_ERRIE;
  }
}

/**
 * @brief   Enables the TX buffer interrupt source.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 */
static inline void gd32_sio_enable_tx_irq(SIODriver *siop) {

  if ((siop->enabled & SIO_EV_TXNOTFULL) != 0U) {
    USART_CTL0(siop->usart) |= USART_CTL0_TBEIE;
  }
}

/**
 * @brief   Enables the transmission end interrupt source.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 */
static inline void gd32_sio_enable_tx_end_irq(SIODriver *siop) {

  if ((siop->enabled & SIO_EV_TXDONE) != 0U) {
    USART_CTL0(siop->usart) |= USART_CTL0_TCIE;
  }
}

/**
 * @brief   USART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 */
static void gd32_sio_usart_init(SIODriver *siop) {
  uint32_t u = siop->usart;
  uint32_t baud;

  baud = (siop->clock + siop->config->baud / 2U) / siop->config->baud;

  /* Correcting the BAUD value when oversampling by 8 instead of 16.
     Fraction is still 4 bits wide, but only the lower 3 bits are used.
     Mantissa is doubled, but Fraction is left the same.*/
  if ((siop->config->ctl0 & USART_CTL0_OVSMOD) != 0U) {
    baud = ((baud & ~7U) * 2U) | (baud & 7U);
  }

  osalDbgAssert(baud < 0x10000U, "invalid BAUD value");

  /* The peripheral is kept disabled while reprogramming.*/
  USART_CTL0(u) = 0U;
  USART_CTL1(u) = siop->config->ctl1 & ~GD32_USART_CTL1_CFG_FORBIDDEN;
  USART_CTL2(u) = siop->config->ctl2 & ~GD32_USART_CTL2_CFG_FORBIDDEN;
  USART_BAUD(u) = baud;

  /* Starting operations.*/
  USART_INTC(u) = GD32_USART_INTC_ALL_FLAGS;
  USART_CTL0(u) = (siop->config->ctl0 & ~GD32_USART_CTL0_CFG_FORBIDDEN) |
                  USART_CTL0_UEN | USART_CTL0_TEN | USART_CTL0_REN;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SIO driver initialization.
 *
 * @notapi
 */
void sio_lld_init(void) {

#if GD32_SIO_USE_USART0 == TRUE
  sioObjectInit(&SIOD0);
  SIOD0.usart = USART0;
  SIOD0.clock = GD32_USART0CLK;
#endif
#if GD32_SIO_USE_USART1 == TRUE
  sioObjectInit(&SIOD1);
  SIOD1.usart = USART1;
  SIOD1.clock = GD32_USART1CLK;
#endif
#if GD32_SIO_USE_USART2 == TRUE
  sioObjectInit(&SIOD2);
  SIOD2.usart = USART2;
  SIOD2.clock = GD32_USART2CLK;
#endif
#if GD32_SIO_USE_UART3 == TRUE
  sioObjectInit(&SIOD3);
  SIOD3.usart = UART3;
  SIOD3.clock = GD32_UART3CLK;
#endif
#if GD32_SIO_USE_UART4 == TRUE
  sioObjectInit(&SIOD4);
  SIOD4.usart = UART4;
  SIOD4.clock = GD32_UART4CLK;
#endif
#if GD32_SIO_USE_USART5 == TRUE
  sioObjectInit(&SIOD5);
  SIOD5.usart = USART5;
  SIOD5.clock = GD32_USART5CLK;
#endif
#if GD32_SIO_USE_UART6 == TRUE
  sioObjectInit(&SIOD6);
  SIOD6.usart = UART6;
  SIOD6.clock = GD32_UART6CLK;
#endif
#if GD32_SIO_USE_UART7 == TRUE
  sioObjectInit(&SIOD7);
  SIOD7.usart = UART7;
  SIOD7.clock = GD32_UART7CLK;
#endif
}

/**
 * @brief   Configures and activates the SIO peripheral.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t sio_lld_start(SIODriver *siop) {

  /* Using the default configuration if the application passed a
     NULL pointer.*/
  if (siop->config == NULL) {
    siop->config = &default_config;
  }

  if (siop->state == SIO_STOP) {

    /* Enables the peripheral.*/
    if (false) {
    }
#if GD32_SIO_USE_USART0 == TRUE
    else if (&SIOD0 == siop) {
      rcuResetUSART0();
      rcuEnableUSART0(true);
    }
#endif
#if GD32_SIO_USE_USART1 == TRUE
    else if (&SIOD1 == siop) {
      rcuResetUSART1();
      rcuEnableUSART1(true);
    }
#endif
#if GD32_SIO_USE_USART2 == TRUE
    else if (&SIOD2 == siop) {
      rcuResetUSART2();
      rcuEnableUSART2(true);
    }
#endif
#if GD32_SIO_USE_UART3 == TRUE
    else if (&SIOD3 == siop) {
      rcuResetUART3();
      rcuEnableUART3(true);
    }
#endif
#if GD32_SIO_USE_UART4 == TRUE
    else if (&SIOD4 == siop) {
      rcuResetUART4();
      rcuEnableUART4(true);
    }
#endif
#if GD32_SIO_USE_USART5 == TRUE
    else if (&SIOD5 == siop) {
      rcuResetUSART5();
      rcuEnableUSART5(true);
    }
#endif
#if GD32_SIO_USE_UART6 == TRUE
    else if (&SIOD6 == siop) {
      rcuResetUART6();
      rcuEnableUART6(true);
    }
#endif
#if GD32_SIO_USE_UART7 == TRUE
    else if (&SIOD7 == siop) {
      rcuResetUART7();
      rcuEnableUART7(true);
    }
#endif
    else {
      osalDbgAssert(false, "invalid SIO instance");
      return HAL_RET_IS_INVALID;
    }
  }

  /* Configures the peripheral.*/
  gd32_sio_usart_init(siop);

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the SIO peripheral.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @notapi
 */
void sio_lld_stop(SIODriver *siop) {

  if (siop->state == SIO_READY) {

    /* Resets and disables the peripheral.*/
    if (false) {
    }
#if GD32_SIO_USE_USART0 == TRUE
    else if (&SIOD0 == siop) {
      rcuResetUSART0();
      rcuDisableUSART0();
    }
#endif
#if GD32_SIO_USE_USART1 == TRUE
    else if (&SIOD1 == siop) {
      rcuResetUSART1();
      rcuDisableUSART1();
    }
#endif
#if GD32_SIO_USE_USART2 == TRUE
    else if (&SIOD2 == siop) {
      rcuResetUSART2();
      rcuDisableUSART2();
    }
#endif
#if GD32_SIO_USE_UART3 == TRUE
    else if (&SIOD3 == siop) {
      rcuResetUART3();
      rcuDisableUART3();
    }
#endif
#if GD32_SIO_USE_UART4 == TRUE
    else if (&SIOD4 == siop) {
      rcuResetUART4();
      rcuDisableUART4();
    }
#endif
#if GD32_SIO_USE_USART5 == TRUE
    else if (&SIOD5 == siop) {
      rcuResetUSART5();
      rcuDisableUSART5();
    }
#endif
#if GD32_SIO_USE_UART6 == TRUE
    else if (&SIOD6 == siop) {
      rcuResetUART6();
      rcuDisableUART6();
    }
#endif
#if GD32_SIO_USE_UART7 == TRUE
    else if (&SIOD7 == siop) {
      rcuResetUART7();
      rcuDisableUART7();
    }
#endif
    else {
      osalDbgAssert(false, "invalid SIO instance");
    }
  }
}

/**
 * @brief   Enable flags change notification.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 */
void sio_lld_update_enable_flags(SIODriver *siop) {
  uint32_t u = siop->usart;
  uint32_t ctl0, ctl1, ctl2;

  ctl0 = USART_CTL0(u) & ~(USART_CTL0_TBEIE  | USART_CTL0_RBNEIE |
                           USART_CTL0_IDLEIE | USART_CTL0_TCIE   |
                           USART_CTL0_PERRIE);
  ctl1 = USART_CTL1(u) & ~(USART_CTL1_LBDIE);
  ctl2 = USART_CTL2(u) & ~(USART_CTL2_ERRIE);

  ctl0 |= __sio_reloc_field(siop->enabled, SIO_EV_RXNOTEMPY,  SIO_EV_RXNOTEMPY_POS,  GD32_USART_CTL0_RBNEIE_POS) |
          __sio_reloc_field(siop->enabled, SIO_EV_TXNOTFULL,  SIO_EV_TXNOTFULL_POS,  GD32_USART_CTL0_TBEIE_POS)  |
          __sio_reloc_field(siop->enabled, SIO_EV_TXDONE,     SIO_EV_TXDONE_POS,     GD32_USART_CTL0_TCIE_POS)   |
          __sio_reloc_field(siop->enabled, SIO_EV_RXIDLE,     SIO_EV_RXIDLE_POS,     GD32_USART_CTL0_IDLEIE_POS) |
          __sio_reloc_field(siop->enabled, SIO_EV_PARITY_ERR, SIO_EV_PARITY_ERR_POS, GD32_USART_CTL0_PERRIE_POS);
  ctl1 |= __sio_reloc_field(siop->enabled, SIO_EV_RXBREAK,    SIO_EV_RXBREAK_POS,    GD32_USART_CTL1_LBDIE_POS);

  /* The following 3 are grouped.*/
  if ((siop->enabled & (SIO_EV_FRAMING_ERR |
                        SIO_EV_OVERRUN_ERR |
                        SIO_EV_NOISE_ERR)) != 0U) {
    ctl2 |= USART_CTL2_ERRIE;
  }

  /* Setting up the operation.*/
  USART_CTL0(u) = ctl0;
  USART_CTL1(u) = ctl1;
  USART_CTL2(u) = ctl2;
}

/**
 * @brief   Get and clears SIO error event flags.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The pending event flags.
 *
 * @notapi
 */
sioevents_t sio_lld_get_and_clear_errors(SIODriver *siop) {
  uint32_t u = siop->usart;
  uint32_t stat;
  sioevents_t errors;

  stat = USART_STAT(u) & SIO_LLD_STAT_RX_ERRORS;

  /* Clearing captured events, the handled STAT flags share their bit
     positions with the matching INTC clear bits.*/
  USART_INTC(u) = stat & GD32_USART_INTC_ALL_FLAGS;

  /* Status flags cleared, now the error-related interrupts can be
     enabled again.*/
  gd32_sio_enable_rx_errors_irq(siop);

  /* Translating the status flags in SIO events.*/
  errors = __sio_reloc_field(stat, GD32_USART_STAT_ONFP_MASK, GD32_USART_STAT_ONFP_POS, SIO_EV_ALL_ERRORS_POS) |
           __sio_reloc_field(stat, USART_STAT_LBDF,           GD32_USART_STAT_LBDF_POS, SIO_EV_RXBREAK_POS);

  return errors;
}

/**
 * @brief   Get and clears SIO event flags.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The pending event flags.
 *
 * @notapi
 */
sioevents_t sio_lld_get_and_clear_events(SIODriver *siop) {
  uint32_t u = siop->usart;
  uint32_t stat;
  sioevents_t events;

  stat = USART_STAT(u) & (SIO_LLD_STAT_RX_ERRORS |
                          USART_STAT_RBNE        |
                          USART_STAT_IDLEF       |
                          USART_STAT_TBE         |
                          USART_STAT_TC);

  /* Clearing captured events, RBNE and TBE have no INTC counterpart.*/
  USART_INTC(u) = stat & GD32_USART_INTC_ALL_FLAGS;

  /* Status flags cleared, now the RX-related interrupts can be
     enabled again.*/
  gd32_sio_enable_rx_irq(siop);
  gd32_sio_enable_rx_errors_irq(siop);

  /* Translating the status flags in SIO events.*/
  events = __sio_reloc_field(stat, USART_STAT_RBNE,            GD32_USART_STAT_RBNE_POS,  SIO_EV_RXNOTEMPY_POS)  |
           __sio_reloc_field(stat, USART_STAT_TBE,             GD32_USART_STAT_TBE_POS,   SIO_EV_TXNOTFULL_POS)  |
           __sio_reloc_field(stat, USART_STAT_TC,              GD32_USART_STAT_TC_POS,    SIO_EV_TXDONE_POS)     |
           __sio_reloc_field(stat, GD32_USART_STAT_IONFP_MASK, GD32_USART_STAT_IONFP_POS, SIO_EV_ALL_ERRORS_POS) |
           __sio_reloc_field(stat, USART_STAT_LBDF,            GD32_USART_STAT_LBDF_POS,  SIO_EV_RXBREAK_POS);

  return events;
}

/**
 * @brief   Returns the pending SIO event flags.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The pending event flags.
 *
 * @notapi
 */
sioevents_t sio_lld_get_events(SIODriver *siop) {
  uint32_t stat;
  sioevents_t events;

  stat = USART_STAT(siop->usart) & (SIO_LLD_STAT_RX_ERRORS |
                                    USART_STAT_RBNE        |
                                    USART_STAT_IDLEF       |
                                    USART_STAT_TBE         |
                                    USART_STAT_TC);

  /* Translating the status flags in SIO events.*/
  events = __sio_reloc_field(stat, USART_STAT_RBNE,            GD32_USART_STAT_RBNE_POS,  SIO_EV_RXNOTEMPY_POS)  |
           __sio_reloc_field(stat, USART_STAT_TBE,             GD32_USART_STAT_TBE_POS,   SIO_EV_TXNOTFULL_POS)  |
           __sio_reloc_field(stat, USART_STAT_TC,              GD32_USART_STAT_TC_POS,    SIO_EV_TXDONE_POS)     |
           __sio_reloc_field(stat, GD32_USART_STAT_IONFP_MASK, GD32_USART_STAT_IONFP_POS, SIO_EV_ALL_ERRORS_POS) |
           __sio_reloc_field(stat, USART_STAT_LBDF,            GD32_USART_STAT_LBDF_POS,  SIO_EV_RXBREAK_POS);

  return events;
}

/**
 * @brief   Reads data from the RX FIFO.
 * @details The function is not blocking, it reads frames until there
 *          are frames available without waiting.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 * @param[in] buffer        pointer to the buffer for read frames
 * @param[in] n             maximum number of frames to be read
 * @return                  The number of frames copied from the buffer.
 * @retval 0                if the RX FIFO is empty.
 */
size_t sio_lld_read(SIODriver *siop, uint8_t *buffer, size_t n) {
  size_t rd;

  rd = 0U;
  while (true) {

    /* If the RX FIFO has been emptied then the RX FIFO and IDLE interrupts
       are enabled again.*/
    if (sio_lld_is_rx_empty(siop)) {
      gd32_sio_enable_rx_irq(siop);
      break;
    }

    /* Buffer filled condition.*/
    if (rd >= n) {
      break;
    }

    *buffer++ = (uint8_t)USART_RDATA(siop->usart);
    rd++;
  }

  return rd;
}

/**
 * @brief   Writes data into the TX FIFO.
 * @details The function is not blocking, it writes frames until there
 *          is space available without waiting.
 *
 * @param[in] siop          pointer to an @p SIODriver structure
 * @param[in] buffer        pointer to the buffer for read frames
 * @param[in] n             maximum number of frames to be written
 * @return                  The number of frames copied from the buffer.
 * @retval 0                if the TX FIFO is full.
 */
size_t sio_lld_write(SIODriver *siop, const uint8_t *buffer, size_t n) {
  size_t wr;

  wr = 0U;
  while (true) {

    /* If the TX FIFO has been filled then the interrupt is enabled again.*/
    if (sio_lld_is_tx_full(siop)) {
      gd32_sio_enable_tx_irq(siop);
      break;
    }

    /* Buffer emptied condition.*/
    if (wr >= n) {
      break;
    }

    USART_TDATA(siop->usart) = (uint32_t)*buffer++;
    wr++;
  }

  /* The transmit complete interrupt is always re-enabled on write.*/
  gd32_sio_enable_tx_end_irq(siop);

  return wr;
}

/**
 * @brief   Returns one frame from the RX FIFO.
 * @note    If the FIFO is empty then the returned value is unpredictable.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The frame from RX FIFO.
 *
 * @notapi
 */
msg_t sio_lld_get(SIODriver *siop) {
  msg_t msg;

  msg = (msg_t)USART_RDATA(siop->usart);

  /* If the RX FIFO has been emptied then the interrupt is enabled again.*/
  if (sio_lld_is_rx_empty(siop)) {
    gd32_sio_enable_rx_irq(siop);
  }

  return msg;
}

/**
 * @brief   Pushes one frame into the TX FIFO.
 * @note    If the FIFO is full then the behavior is unpredictable.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] data      frame to be written
 *
 * @notapi
 */
void sio_lld_put(SIODriver *siop, uint_fast16_t data) {

  USART_TDATA(siop->usart) = (uint32_t)data;

  /* If the TX FIFO has been filled then the interrupt is enabled again.*/
  if (sio_lld_is_tx_full(siop)) {
    gd32_sio_enable_tx_irq(siop);
  }

  /* The transmit complete interrupt is always re-enabled on write.*/
  gd32_sio_enable_tx_end_irq(siop);
}

/**
 * @brief   Control operation on a serial port.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @param[in] operation control operation code
 * @param[in,out] arg   operation argument
 *
 * @return              The control operation status.
 * @retval MSG_OK       in case of success.
 * @retval MSG_TIMEOUT  in case of operation timeout.
 * @retval MSG_RESET    in case of operation reset.
 *
 * @notapi
 */
msg_t sio_lld_control(SIODriver *siop, unsigned int operation, void *arg) {

  (void)siop;
  (void)operation;
  (void)arg;

  return MSG_OK;
}

/**
 * @brief   Serves an USART interrupt.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 *
 * @notapi
 */
void sio_lld_serve_interrupt(SIODriver *siop) {
  uint32_t u = siop->usart;
  uint32_t ctl0, ctl1, ctl2, stat, statmask;

  osalDbgAssert(siop->state == SIO_READY, "invalid state");

  /* Read on control registers.*/
  ctl0 = USART_CTL0(u);
  ctl1 = USART_CTL1(u);
  ctl2 = USART_CTL2(u);

  /* Calculating the mask of status bits that should be processed according
     to the state of the various CTLx registers.*/
  statmask = __sio_reloc_field(ctl0, USART_CTL0_TBEIE,  GD32_USART_CTL0_TBEIE_POS,  GD32_USART_STAT_TBE_POS)   |
             __sio_reloc_field(ctl0, USART_CTL0_RBNEIE, GD32_USART_CTL0_RBNEIE_POS, GD32_USART_STAT_RBNE_POS)  |
              /* NOTE: ORERR interrupt also enabled by USART_CTL0_RBNEIE, not just USART_CTL2_ERRIE.*/
             __sio_reloc_field(ctl0, USART_CTL0_RBNEIE, GD32_USART_CTL0_RBNEIE_POS, GD32_USART_STAT_ORERR_POS) |
             __sio_reloc_field(ctl0, USART_CTL0_IDLEIE, GD32_USART_CTL0_IDLEIE_POS, GD32_USART_STAT_IDLEF_POS) |
             __sio_reloc_field(ctl0, USART_CTL0_TCIE,   GD32_USART_CTL0_TCIE_POS,   GD32_USART_STAT_TC_POS)    |
             __sio_reloc_field(ctl0, USART_CTL0_PERRIE, GD32_USART_CTL0_PERRIE_POS, GD32_USART_STAT_PERR_POS)  |
             __sio_reloc_field(ctl1, USART_CTL1_LBDIE,  GD32_USART_CTL1_LBDIE_POS,  GD32_USART_STAT_LBDF_POS);
  if ((ctl2 & USART_CTL2_ERRIE) != 0U) {
    statmask |= USART_STAT_NERR | USART_STAT_FERR | USART_STAT_ORERR;
  }

  /* Status flags to be processed.*/
  stat = USART_STAT(u) & statmask;
  if (stat != 0U) {

    /* Error flags handled as a group.
       NOTE: LBDF is mapped here too, so RX paths stay quiescent until the
             application clears errors.*/
    if ((stat & SIO_LLD_STAT_RX_ERRORS) != 0U) {
      /* NOTE: We could get here because various causes: PERRIE, ERRIE, LBDIE
               and RBNEIE. Because RBNEIE we can get here even when the
               application has not enabled errors notifications.*/
#if SIO_USE_SYNCHRONIZATION
      /* The idle flag is forcibly cleared when an RX error event is
         detected.*/
      USART_INTC(u) = USART_INTC_IDLEC;
#endif

      /* All RX-related interrupt sources disabled.*/
      ctl0 &= ~(USART_CTL0_PERRIE | USART_CTL0_RBNEIE | USART_CTL0_IDLEIE);
      ctl1 &= ~(USART_CTL1_LBDIE);
      ctl2 &= ~(USART_CTL2_ERRIE);

      /* Notifying the application only if it is interested in errors.*/
      if ((siop->enabled & SIO_EV_ALL_ERRORS) != (sioevents_t)0) {
        __sio_wakeup_errors(siop);
      }
    }
    /* If there are no errors then we check for the other RX-related
       status flags.*/
    else {
      /* Idle RX flag. Note: At start the USART will produce an IDLE
         interrupt.*/
      if ((stat & USART_STAT_IDLEF) != 0U) {

        /* Interrupt source disabled, the flag is left set for
           sioSynchronizeRXIdle().*/
        ctl0 &= ~USART_CTL0_IDLEIE;

        /* Re-enabling reception on the first character after idle, if the
           application is interested in data events.*/
        if ((siop->enabled & SIO_EV_RXNOTEMPY) != 0U) {
          ctl0 |= USART_CTL0_RBNEIE;
        }

        /* Waiting thread woken, if any.*/
        __sio_wakeup_rxidle(siop);
      }

      /* RX buffer/FIFO is non-empty, the whole FIFO is drained by the
         application in order to prevent further interrupts.*/
      if ((stat & USART_STAT_RBNE) != 0U) {

#if SIO_USE_SYNCHRONIZATION
        /* The idle flag is forcibly cleared when an RX data event is
           detected.*/
        USART_INTC(u) = USART_INTC_IDLEC;
#endif

        /* Interrupt source disabled.*/
        ctl0 &= ~USART_CTL0_RBNEIE;

        /* Waiting thread woken, if any.*/
        __sio_wakeup_rx(siop);
      }
    }

    /* TX buffer/FIFO is non-full.*/
    if ((stat & USART_STAT_TBE) != 0U) {

      /* Interrupt source disabled.*/
      ctl0 &= ~USART_CTL0_TBEIE;

      /* Waiting thread woken, if any.*/
      __sio_wakeup_tx(siop);
    }

    /* Physical transmission end.*/
    if ((stat & USART_STAT_TC) != 0U) {

      /* Interrupt source disabled, the flag is left set for
         sioSynchronizeTXEnd().*/
      ctl0 &= ~USART_CTL0_TCIE;

      /* Waiting thread woken, if any.*/
      __sio_wakeup_txend(siop);
    }

    /* Updating control registers, some sources could have been disabled.*/
    USART_CTL0(u) = ctl0;
    USART_CTL1(u) = ctl1;
    USART_CTL2(u) = ctl2;

    /* The callback is invoked.*/
    __sio_callback(siop);
  }
  else {
    osalDbgAssert(false, "spurious interrupt");
  }
}

#endif /* HAL_USE_SIO == TRUE */

/** @} */
