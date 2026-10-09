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
 * @file    USARTv1/hal_uart_lld.c
 * @brief   GD32 low level UART driver code.
 *
 * @addtogroup UART
 * @{
 */

#include "hal.h"

#if (HAL_USE_UART == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Clearable flags, STAT and INTC share the same bit positions for all of
   them but not for the remaining STAT flags.*/
#define GD32_USART_INTC_ALL_FLAGS   (USART_INTC_PEC   | USART_INTC_FEC  |   \
                                     USART_INTC_NEC   | USART_INTC_OREC |   \
                                     USART_INTC_IDLEC | USART_INTC_TCC  |   \
                                     USART_INTC_LBDC  | USART_INTC_CTSC |   \
                                     USART_INTC_RTC   | USART_INTC_EBC)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief USART0 UART driver identifier.*/
#if (GD32_UART_USE_USART0 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD0;
#endif

/** @brief USART1 UART driver identifier.*/
#if (GD32_UART_USE_USART1 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD1;
#endif

/** @brief USART2 UART driver identifier.*/
#if (GD32_UART_USE_USART2 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD2;
#endif

/** @brief UART3 UART driver identifier.*/
#if (GD32_UART_USE_UART3 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD3;
#endif

/** @brief UART4 UART driver identifier.*/
#if (GD32_UART_USE_UART4 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD4;
#endif

/** @brief USART5 UART driver identifier.*/
#if (GD32_UART_USE_USART5 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD5;
#endif

/** @brief UART6 UART driver identifier.*/
#if (GD32_UART_USE_UART6 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD6;
#endif

/** @brief UART7 UART driver identifier.*/
#if (GD32_UART_USE_UART7 == TRUE) || defined(__DOXYGEN__)
UARTDriver UARTD7;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Status flags translation to the event flags.
 *
 * @param[in] stat      USART STAT register value
 * @return              The error flags mask.
 */
static uartflags_t translate_errors(uint32_t stat) {
  uartflags_t sts = 0;

  if ((stat & USART_STAT_PERR) != 0U)
    sts |= UART_PARITY_ERROR;
  if ((stat & USART_STAT_FERR) != 0U)
    sts |= UART_FRAMING_ERROR;
  if ((stat & USART_STAT_NERR) != 0U)
    sts |= UART_NOISE_ERROR;
  if ((stat & USART_STAT_ORERR) != 0U)
    sts |= UART_OVERRUN_ERROR;
  if ((stat & USART_STAT_LBDF) != 0U)
    sts |= UART_BREAK_DETECTED;
  return sts;
}

/**
 * @brief   Puts the receiver in the UART_RX_IDLE state.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
static void uart_enter_rx_idle_loop(UARTDriver *uartp) {
  uint32_t mode;

  /* RX DMA channel preparation, if the char callback is defined then
     the TCIE interrupt is enabled too.*/
  if (uartp->config->rxchar_cb == NULL)
    mode = GD32_DMA_CTL_DIR_P2M | GD32_DMA_CTL_CMEN;
  else
    mode = GD32_DMA_CTL_DIR_P2M | GD32_DMA_CTL_CMEN | GD32_DMA_CTL_FTFIE;

  dmaStreamSetMemory0(uartp->dmarx, (void *)&uartp->rxbuf);
  dmaStreamSetTransactionSize(uartp->dmarx, 1);
  dmaStreamSetMode(uartp->dmarx, uartp->dmarxmode | mode);
  dmaStreamEnable(uartp->dmarx);
}

/**
 * @brief   RX DMA common service routine.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] flags     pre-shifted content of the INTF register
 */
static void uart_lld_serve_rx_end_irq(UARTDriver *uartp, uint32_t flags) {

  /* DMA errors handling.*/
#if defined(GD32_UART_DMA_ERROR_HOOK)
  if ((flags & (GD32_DMA_ISR_TAEIF | GD32_DMA_ISR_FEIF)) != 0U) {
    GD32_UART_DMA_ERROR_HOOK(uartp);
  }
#else
  (void)flags;
#endif

  if (uartp->rxstate == UART_RX_IDLE) {
    /* Receiver in idle state, a callback is generated, if enabled, for
       each received character and then the driver stays in the same
       state.*/
    _uart_rx_idle_code(uartp);
  }
  else {
    /* Receiver in active state, a callback is generated, if enabled,
       after a completed transfer.*/
    dmaStreamDisable(uartp->dmarx);
    _uart_rx_complete_isr_code(uartp);
  }
}

/**
 * @brief   TX DMA common service routine.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] flags     pre-shifted content of the INTF register
 */
static void uart_lld_serve_tx_end_irq(UARTDriver *uartp, uint32_t flags) {

  /* DMA errors handling.*/
#if defined(GD32_UART_DMA_ERROR_HOOK)
  if ((flags & (GD32_DMA_ISR_TAEIF | GD32_DMA_ISR_FEIF)) != 0U) {
    GD32_UART_DMA_ERROR_HOOK(uartp);
  }
#else
  (void)flags;
#endif

  dmaStreamDisable(uartp->dmatx);

  /* A callback is generated, if enabled, after a completed transfer.*/
  _uart_tx1_isr_code(uartp);
}

/**
 * @brief   USART initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
static void usart_start(UARTDriver *uartp) {
  uint32_t fck;
  uint32_t u = uartp->usart;
  const uint32_t tmo = uartp->config->timeout;

  /* Defensive programming, starting from a clean state.*/
  USART_CTL0(u) = 0;
  USART_CTL1(u) = 0;
  USART_CTL2(u) = 0;

  /* Baud rate setting.*/
  fck = (uint32_t)((uartp->clock + uartp->config->speed / 2) /
                   uartp->config->speed);

  /* Correcting the BAUD value when oversampling by 8 instead of 16.
     Fraction is still 4 bits wide, but only the lower 3 bits are used.
     Mantissa is doubled, but Fraction is left the same.*/
  if (uartp->config->ctl0 & USART_CTL0_OVSMOD) {
    fck = ((fck & ~7U) * 2U) | (fck & 7U);
  }

  osalDbgAssert(fck < 0x10000U, "invalid BAUD value");

  USART_BAUD(u) = fck;

  /* Clearing any pending clearable status flags and flushing the
     RX/TX data paths.*/
  USART_INTC(u) = GD32_USART_INTC_ALL_FLAGS;
  (void)USART_RDATA(u);

  /* Note that some bits are enforced because required for correct
     driver operations.*/
  USART_CTL1(u) = uartp->config->ctl1 | USART_CTL1_LBDIE;
  USART_CTL2(u) = uartp->config->ctl2 | USART_CTL2_ERRIE |
                                      USART_CTL2_DENT | USART_CTL2_DENR;
  USART_CTL0(u) = uartp->config->ctl0 | USART_CTL0_UEN | USART_CTL0_PERRIE |
                                      USART_CTL0_TEN | USART_CTL0_REN;

  /* Set receive timeout and checks if it is really applied.*/
  if (tmo > 0U) {
    osalDbgAssert(tmo <= 0x00FFFFFFU, "Timeout overflow");
    USART_RT(u) = tmo;
    osalDbgAssert(tmo == USART_RT(u), "Timeout feature unsupported in this UART");
  }

  /* Starting the receiver idle loop.*/
  uart_enter_rx_idle_loop(uartp);
}

/**
 * @brief   USART de-initialization.
 * @details This function must be invoked with interrupts disabled.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
static void usart_stop(UARTDriver *uartp) {
  uint32_t u = uartp->usart;

  /* Stops RX and TX DMA channels.*/
  dmaStreamDisable(uartp->dmarx);
  dmaStreamDisable(uartp->dmatx);

  /* Stops USART operations.*/
  USART_CTL0(u) = 0;
  USART_CTL1(u) = 0;
  USART_CTL2(u) = 0;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if (GD32_UART_USE_USART0 == TRUE) || defined(__DOXYGEN__)
#if !defined(GD32_USART0_SUPPRESS_ISR)
#if !defined(GD32_USART0_HANDLER)
#error "GD32_USART0_HANDLER not defined"
#endif
/**
 * @brief   USART0 IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_USART0_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  uart_lld_serve_interrupt(&UARTD0);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if (GD32_UART_USE_USART1 == TRUE) || defined(__DOXYGEN__)
#if !defined(GD32_USART1_SUPPRESS_ISR)
#if !defined(GD32_USART1_HANDLER)
#error "GD32_USART1_HANDLER not defined"
#endif
/**
 * @brief   USART1 IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_USART1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  uart_lld_serve_interrupt(&UARTD1);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if (GD32_UART_USE_USART2 == TRUE) || defined(__DOXYGEN__)
#if !defined(GD32_USART2_SUPPRESS_ISR)
#if !defined(GD32_USART2_HANDLER)
#error "GD32_USART2_HANDLER not defined"
#endif
/**
 * @brief   USART2 IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_USART2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  uart_lld_serve_interrupt(&UARTD2);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if (GD32_UART_USE_UART3 == TRUE) || defined(__DOXYGEN__)
#if !defined(GD32_UART3_SUPPRESS_ISR)
#if !defined(GD32_UART3_HANDLER)
#error "GD32_UART3_HANDLER not defined"
#endif
/**
 * @brief   UART3 IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_UART3_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  uart_lld_serve_interrupt(&UARTD3);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if (GD32_UART_USE_UART4 == TRUE) || defined(__DOXYGEN__)
#if !defined(GD32_UART4_SUPPRESS_ISR)
#if !defined(GD32_UART4_HANDLER)
#error "GD32_UART4_HANDLER not defined"
#endif
/**
 * @brief   UART4 IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_UART4_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  uart_lld_serve_interrupt(&UARTD4);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if (GD32_UART_USE_USART5 == TRUE) || defined(__DOXYGEN__)
#if !defined(GD32_USART5_SUPPRESS_ISR)
#if !defined(GD32_USART5_HANDLER)
#error "GD32_USART5_HANDLER not defined"
#endif
/**
 * @brief   USART5 IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_USART5_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  uart_lld_serve_interrupt(&UARTD5);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if (GD32_UART_USE_UART6 == TRUE) || defined(__DOXYGEN__)
#if !defined(GD32_UART6_SUPPRESS_ISR)
#if !defined(GD32_UART6_HANDLER)
#error "GD32_UART6_HANDLER not defined"
#endif
/**
 * @brief   UART6 IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_UART6_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  uart_lld_serve_interrupt(&UARTD6);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

#if (GD32_UART_USE_UART7 == TRUE) || defined(__DOXYGEN__)
#if !defined(GD32_UART7_SUPPRESS_ISR)
#if !defined(GD32_UART7_HANDLER)
#error "GD32_UART7_HANDLER not defined"
#endif
/**
 * @brief   UART7 IRQ handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_UART7_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  uart_lld_serve_interrupt(&UARTD7);

  OSAL_IRQ_EPILOGUE();
}
#endif
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level UART driver initialization.
 *
 * @notapi
 */
void uart_lld_init(void) {

#if GD32_UART_USE_USART0 == TRUE
  uartObjectInit(&UARTD0);
  UARTD0.usart   = USART0;
  UARTD0.clock   = GD32_USART0CLK;
  UARTD0.dmarxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD0.dmatxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD0.dmarx   = NULL;
  UARTD0.dmatx   = NULL;
#if !defined(GD32_USART0_SUPPRESS_ISR)
  nvicEnableVector(GD32_USART0_NUMBER, GD32_UART_USART0_IRQ_PRIORITY);
#endif
#endif

#if GD32_UART_USE_USART1 == TRUE
  uartObjectInit(&UARTD1);
  UARTD1.usart   = USART1;
  UARTD1.clock   = GD32_USART1CLK;
  UARTD1.dmarxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD1.dmatxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD1.dmarx   = NULL;
  UARTD1.dmatx   = NULL;
#if !defined(GD32_USART1_SUPPRESS_ISR)
  nvicEnableVector(GD32_USART1_NUMBER, GD32_UART_USART1_IRQ_PRIORITY);
#endif
#endif

#if GD32_UART_USE_USART2 == TRUE
  uartObjectInit(&UARTD2);
  UARTD2.usart   = USART2;
  UARTD2.clock   = GD32_USART2CLK;
  UARTD2.dmarxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD2.dmatxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD2.dmarx   = NULL;
  UARTD2.dmatx   = NULL;
#if !defined(GD32_USART2_SUPPRESS_ISR)
  nvicEnableVector(GD32_USART2_NUMBER, GD32_UART_USART2_IRQ_PRIORITY);
#endif
#endif

#if GD32_UART_USE_UART3 == TRUE
  uartObjectInit(&UARTD3);
  UARTD3.usart   = UART3;
  UARTD3.clock   = GD32_UART3CLK;
  UARTD3.dmarxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD3.dmatxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD3.dmarx   = NULL;
  UARTD3.dmatx   = NULL;
#if !defined(GD32_UART3_SUPPRESS_ISR)
  nvicEnableVector(GD32_UART3_NUMBER, GD32_UART_UART3_IRQ_PRIORITY);
#endif
#endif

#if GD32_UART_USE_UART4 == TRUE
  uartObjectInit(&UARTD4);
  UARTD4.usart   = UART4;
  UARTD4.clock   = GD32_UART4CLK;
  UARTD4.dmarxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD4.dmatxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD4.dmarx   = NULL;
  UARTD4.dmatx   = NULL;
#if !defined(GD32_UART4_SUPPRESS_ISR)
  nvicEnableVector(GD32_UART4_NUMBER, GD32_UART_UART4_IRQ_PRIORITY);
#endif
#endif

#if GD32_UART_USE_USART5 == TRUE
  uartObjectInit(&UARTD5);
  UARTD5.usart   = USART5;
  UARTD5.clock   = GD32_USART5CLK;
  UARTD5.dmarxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD5.dmatxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD5.dmarx   = NULL;
  UARTD5.dmatx   = NULL;
#if !defined(GD32_USART5_SUPPRESS_ISR)
  nvicEnableVector(GD32_USART5_NUMBER, GD32_UART_USART5_IRQ_PRIORITY);
#endif
#endif

#if GD32_UART_USE_UART6 == TRUE
  uartObjectInit(&UARTD6);
  UARTD6.usart   = UART6;
  UARTD6.clock   = GD32_UART6CLK;
  UARTD6.dmarxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD6.dmatxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD6.dmarx   = NULL;
  UARTD6.dmatx   = NULL;
#if !defined(GD32_UART6_SUPPRESS_ISR)
  nvicEnableVector(GD32_UART6_NUMBER, GD32_UART_UART6_IRQ_PRIORITY);
#endif
#endif

#if GD32_UART_USE_UART7 == TRUE
  uartObjectInit(&UARTD7);
  UARTD7.usart   = UART7;
  UARTD7.clock   = GD32_UART7CLK;
  UARTD7.dmarxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD7.dmatxmode = GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_TAEIE;
  UARTD7.dmarx   = NULL;
  UARTD7.dmatx   = NULL;
#if !defined(GD32_UART7_SUPPRESS_ISR)
  nvicEnableVector(GD32_UART7_NUMBER, GD32_UART_UART7_IRQ_PRIORITY);
#endif
#endif
}

/**
 * @brief   Configures and activates the UART peripheral.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @notapi
 */
void uart_lld_start(UARTDriver *uartp) {

  if (uartp->state == UART_STOP) {

    if (false) {
    }
#if GD32_UART_USE_USART0 == TRUE
    else if (&UARTD0 == uartp) {
      uartp->dmarx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_USART0_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_rx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmarx != NULL, "unable to allocate stream");
      uartp->dmatx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_USART0_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_tx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmatx != NULL, "unable to allocate stream");

      rcuEnableUSART0(true);
      uartp->dmarxmode |= GD32_DMA_CTL_PRIO(GD32_UART_USART0_DMA_PRIORITY);
      uartp->dmatxmode |= GD32_DMA_CTL_PRIO(GD32_UART_USART0_DMA_PRIORITY);
      dmaSetRequestSource(uartp->dmarx, GD32_DMAMUX1_USART0_RX);
      dmaSetRequestSource(uartp->dmatx, GD32_DMAMUX1_USART0_TX);
    }
#endif

#if GD32_UART_USE_USART1 == TRUE
    else if (&UARTD1 == uartp) {
      uartp->dmarx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_USART1_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_rx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmarx != NULL, "unable to allocate stream");
      uartp->dmatx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_USART1_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_tx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmatx != NULL, "unable to allocate stream");

      rcuEnableUSART1(true);
      uartp->dmarxmode |= GD32_DMA_CTL_PRIO(GD32_UART_USART1_DMA_PRIORITY);
      uartp->dmatxmode |= GD32_DMA_CTL_PRIO(GD32_UART_USART1_DMA_PRIORITY);
      dmaSetRequestSource(uartp->dmarx, GD32_DMAMUX1_USART1_RX);
      dmaSetRequestSource(uartp->dmatx, GD32_DMAMUX1_USART1_TX);
    }
#endif

#if GD32_UART_USE_USART2 == TRUE
    else if (&UARTD2 == uartp) {
      uartp->dmarx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_USART2_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_rx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmarx != NULL, "unable to allocate stream");
      uartp->dmatx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_USART2_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_tx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmatx != NULL, "unable to allocate stream");

      rcuEnableUSART2(true);
      uartp->dmarxmode |= GD32_DMA_CTL_PRIO(GD32_UART_USART2_DMA_PRIORITY);
      uartp->dmatxmode |= GD32_DMA_CTL_PRIO(GD32_UART_USART2_DMA_PRIORITY);
      dmaSetRequestSource(uartp->dmarx, GD32_DMAMUX1_USART2_RX);
      dmaSetRequestSource(uartp->dmatx, GD32_DMAMUX1_USART2_TX);
    }
#endif

#if GD32_UART_USE_UART3 == TRUE
    else if (&UARTD3 == uartp) {
      uartp->dmarx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_UART3_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_rx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmarx != NULL, "unable to allocate stream");
      uartp->dmatx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_UART3_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_tx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmatx != NULL, "unable to allocate stream");

      rcuEnableUART3(true);
      uartp->dmarxmode |= GD32_DMA_CTL_PRIO(GD32_UART_UART3_DMA_PRIORITY);
      uartp->dmatxmode |= GD32_DMA_CTL_PRIO(GD32_UART_UART3_DMA_PRIORITY);
      dmaSetRequestSource(uartp->dmarx, GD32_DMAMUX1_UART3_RX);
      dmaSetRequestSource(uartp->dmatx, GD32_DMAMUX1_UART3_TX);
    }
#endif

#if GD32_UART_USE_UART4 == TRUE
    else if (&UARTD4 == uartp) {
      uartp->dmarx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_UART4_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_rx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmarx != NULL, "unable to allocate stream");
      uartp->dmatx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_UART4_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_tx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmatx != NULL, "unable to allocate stream");

      rcuEnableUART4(true);
      uartp->dmarxmode |= GD32_DMA_CTL_PRIO(GD32_UART_UART4_DMA_PRIORITY);
      uartp->dmatxmode |= GD32_DMA_CTL_PRIO(GD32_UART_UART4_DMA_PRIORITY);
      dmaSetRequestSource(uartp->dmarx, GD32_DMAMUX1_UART4_RX);
      dmaSetRequestSource(uartp->dmatx, GD32_DMAMUX1_UART4_TX);
    }
#endif

#if GD32_UART_USE_USART5 == TRUE
    else if (&UARTD5 == uartp) {
      uartp->dmarx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_USART5_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_rx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmarx != NULL, "unable to allocate stream");
      uartp->dmatx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_USART5_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_tx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmatx != NULL, "unable to allocate stream");

      rcuEnableUSART5(true);
      uartp->dmarxmode |= GD32_DMA_CTL_PRIO(GD32_UART_USART5_DMA_PRIORITY);
      uartp->dmatxmode |= GD32_DMA_CTL_PRIO(GD32_UART_USART5_DMA_PRIORITY);
      dmaSetRequestSource(uartp->dmarx, GD32_DMAMUX1_USART5_RX);
      dmaSetRequestSource(uartp->dmatx, GD32_DMAMUX1_USART5_TX);
    }
#endif

#if GD32_UART_USE_UART6 == TRUE
    else if (&UARTD6 == uartp) {
      uartp->dmarx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_UART6_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_rx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmarx != NULL, "unable to allocate stream");
      uartp->dmatx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_UART6_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_tx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmatx != NULL, "unable to allocate stream");

      rcuEnableUART6(true);
      uartp->dmarxmode |= GD32_DMA_CTL_PRIO(GD32_UART_UART6_DMA_PRIORITY);
      uartp->dmatxmode |= GD32_DMA_CTL_PRIO(GD32_UART_UART6_DMA_PRIORITY);
      dmaSetRequestSource(uartp->dmarx, GD32_DMAMUX1_UART6_RX);
      dmaSetRequestSource(uartp->dmatx, GD32_DMAMUX1_UART6_TX);
    }
#endif

#if GD32_UART_USE_UART7 == TRUE
    else if (&UARTD7 == uartp) {
      uartp->dmarx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_UART7_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_rx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmarx != NULL, "unable to allocate stream");
      uartp->dmatx = dmaStreamAllocI(GD32_DMA_STREAM_ID_ANY,
                                     GD32_UART_UART7_IRQ_PRIORITY,
                                     (gd32_dmaisr_t)uart_lld_serve_tx_end_irq,
                                     (void *)uartp);
      osalDbgAssert(uartp->dmatx != NULL, "unable to allocate stream");

      rcuEnableUART7(true);
      uartp->dmarxmode |= GD32_DMA_CTL_PRIO(GD32_UART_UART7_DMA_PRIORITY);
      uartp->dmatxmode |= GD32_DMA_CTL_PRIO(GD32_UART_UART7_DMA_PRIORITY);
      dmaSetRequestSource(uartp->dmarx, GD32_DMAMUX1_UART7_RX);
      dmaSetRequestSource(uartp->dmatx, GD32_DMAMUX1_UART7_TX);
    }
#endif

    else {
      osalDbgAssert(false, "invalid USART instance");
    }

    /* Static DMA setup, the transfer size depends on the USART settings,
       it is 16 bits if WL0=1 and PCEN=0 else it is 8 bits.*/
    if ((uartp->config->ctl0 & (USART_CTL0_WL0 | USART_CTL0_PCEN)) == USART_CTL0_WL0) {
      uartp->dmarxmode |= GD32_DMA_CTL_PWIDTH_HWORD | GD32_DMA_CTL_MWIDTH_HWORD;
      uartp->dmatxmode |= GD32_DMA_CTL_PWIDTH_HWORD | GD32_DMA_CTL_MWIDTH_HWORD;
    }
    dmaStreamSetPeripheral(uartp->dmarx, (void *)&USART_RDATA(uartp->usart));
    dmaStreamSetPeripheral(uartp->dmatx, (void *)&USART_TDATA(uartp->usart));
    uartp->rxbuf = 0;
  }

  uartp->rxstate = UART_RX_IDLE;
  uartp->txstate = UART_TX_IDLE;
  usart_start(uartp);
}

/**
 * @brief   Deactivates the UART peripheral.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @notapi
 */
void uart_lld_stop(UARTDriver *uartp) {

  if (uartp->state == UART_READY) {
    usart_stop(uartp);
    dmaStreamFreeI(uartp->dmarx);
    dmaStreamFreeI(uartp->dmatx);
    uartp->dmarx = NULL;
    uartp->dmatx = NULL;

#if GD32_UART_USE_USART0 == TRUE
    if (&UARTD0 == uartp) {
      rcuDisableUSART0();
      return;
    }
#endif

#if GD32_UART_USE_USART1 == TRUE
    if (&UARTD1 == uartp) {
      rcuDisableUSART1();
      return;
    }
#endif

#if GD32_UART_USE_USART2 == TRUE
    if (&UARTD2 == uartp) {
      rcuDisableUSART2();
      return;
    }
#endif

#if GD32_UART_USE_UART3 == TRUE
    if (&UARTD3 == uartp) {
      rcuDisableUART3();
      return;
    }
#endif

#if GD32_UART_USE_UART4 == TRUE
    if (&UARTD4 == uartp) {
      rcuDisableUART4();
      return;
    }
#endif

#if GD32_UART_USE_USART5 == TRUE
    if (&UARTD5 == uartp) {
      rcuDisableUSART5();
      return;
    }
#endif

#if GD32_UART_USE_UART6 == TRUE
    if (&UARTD6 == uartp) {
      rcuDisableUART6();
      return;
    }
#endif

#if GD32_UART_USE_UART7 == TRUE
    if (&UARTD7 == uartp) {
      rcuDisableUART7();
      return;
    }
#endif

  }
}

/**
 * @brief   Starts a transmission on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void uart_lld_start_send(UARTDriver *uartp, size_t n, const void *txbuf) {

  /* TX DMA channel preparation.*/
  dmaStreamSetMemory0(uartp->dmatx, txbuf);
  dmaStreamSetTransactionSize(uartp->dmatx, n);
  dmaStreamSetMode(uartp->dmatx, uartp->dmatxmode | GD32_DMA_CTL_DIR_M2P |
                                 GD32_DMA_CTL_MNAGA | GD32_DMA_CTL_FTFIE);

  /* Only enable TC interrupt if there's a callback attached to it or
     if called from uartSendFullTimeout(). Also we need to clear TC flag
     which could be set before.*/
#if UART_USE_WAIT == TRUE
  if ((uartp->config->txend2_cb != NULL) || (uartp->early == false)) {
#else
  if (uartp->config->txend2_cb != NULL) {
#endif
    USART_INTC(uartp->usart) = USART_INTC_TCC;
    USART_CTL0(uartp->usart) |= USART_CTL0_TCIE;
  }

  /* Starting transfer.*/
  dmaStreamEnable(uartp->dmatx);
}

/**
 * @brief   Stops any ongoing transmission.
 * @note    Stopping a transmission also suppresses the transmission callbacks.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @return              The number of data frames not transmitted by the
 *                      stopped transmit operation.
 *
 * @notapi
 */
size_t uart_lld_stop_send(UARTDriver *uartp) {

  dmaStreamDisable(uartp->dmatx);

  return dmaStreamGetTransactionSize(uartp->dmatx);
}

/**
 * @brief   Starts a receive operation on the UART peripheral.
 * @note    The buffers are organized as uint8_t arrays for data sizes below
 *          or equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] n         number of data frames to send
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void uart_lld_start_receive(UARTDriver *uartp, size_t n, void *rxbuf) {

  /* Stopping previous activity (idle state).*/
  dmaStreamDisable(uartp->dmarx);

  /* RX DMA channel preparation.*/
  dmaStreamSetMemory0(uartp->dmarx, rxbuf);
  dmaStreamSetTransactionSize(uartp->dmarx, n);
  dmaStreamSetMode(uartp->dmarx, uartp->dmarxmode | GD32_DMA_CTL_DIR_P2M |
                                 GD32_DMA_CTL_MNAGA | GD32_DMA_CTL_FTFIE);

  /* Starting transfer.*/
  dmaStreamEnable(uartp->dmarx);
}

/**
 * @brief   Stops any ongoing receive operation.
 * @note    Stopping a receive operation also suppresses the receive callbacks.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 *
 * @return              The number of data frames not received by the
 *                      stopped receive operation.
 *
 * @notapi
 */
size_t uart_lld_stop_receive(UARTDriver *uartp) {
  size_t n;

  dmaStreamDisable(uartp->dmarx);
  n = dmaStreamGetTransactionSize(uartp->dmarx);

  /* Restarting the receiver idle loop.*/
  uart_enter_rx_idle_loop(uartp);

  return n;
}

/**
 * @brief   USART common service routine.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
void uart_lld_serve_interrupt(UARTDriver *uartp) {
  uint32_t stat;
  uint32_t ctl0;
  uint32_t u = uartp->usart;

  /* Reading and clearing status.*/
  stat = USART_STAT(u);
  ctl0 = USART_CTL0(u);
  USART_INTC(u) = stat;

  if (stat & (USART_STAT_PERR | USART_STAT_FERR | USART_STAT_NERR |
              USART_STAT_ORERR | USART_STAT_LBDF)) {
    _uart_rx_error_isr_code(uartp, translate_errors(stat));
  }

  if ((stat & USART_STAT_TC) && (ctl0 & USART_CTL0_TCIE)) {
    /* TC interrupt disabled.*/
    USART_CTL0(u) = ctl0 & ~USART_CTL0_TCIE;

    /* End of transmission, a callback is generated.*/
    _uart_tx2_isr_code(uartp);
  }

  /* Timeout interrupt sources are only checked if enabled in CTL0.*/
  if (((ctl0 & USART_CTL0_IDLEIE) && (stat & USART_STAT_IDLEF)) ||
      ((ctl0 & USART_CTL0_RTIE) && (stat & USART_STAT_RTF))) {
    _uart_timeout_isr_code(uartp);
  }
}

#endif /* HAL_USE_UART == TRUE */

/** @} */
