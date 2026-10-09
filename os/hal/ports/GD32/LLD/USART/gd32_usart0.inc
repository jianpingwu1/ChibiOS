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
 * @file    USART/gd32_usart0.inc
 * @brief   Shared USART0 handler.
 * @note    Driver instance numbering follows GD32's own 0-based hardware
 *          peripheral numbering:
 *          USART0 (hardware) <-> SD0/SIOD0/UARTD0 (ChibiOS driver objects).
 *          This mapping must stay consistent once the GD32 serial/uart LLD
 *          drivers are written.
 *
 * @addtogroup GD32_USART0_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(GD32_HAS_USART0)
#error "GD32_HAS_USART0 not defined in registry"
#endif

#if defined(GD32_USART0_IS_USED)

/* Priority settings checks.*/
#if !defined(GD32_IRQ_USART0_PRIORITY)
#error "GD32_IRQ_USART0_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_USART0_PRIORITY)
#error "Invalid IRQ priority assigned to GD32_IRQ_USART0_PRIORITY"
#endif

#endif /* defined(GD32_USART0_IS_USED) */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void usart0_irq_init(void) {
#if defined(GD32_USART0_IS_USED)
  nvicEnableVector(GD32_USART0_NUMBER, GD32_IRQ_USART0_PRIORITY);
#endif
}

static inline void usart0_irq_deinit(void) {
#if defined(GD32_USART0_IS_USED)
  nvicDisableVector(GD32_USART0_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(GD32_USART0_IS_USED) || defined(__DOXYGEN__)
/**
 * @brief   USART0 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_USART0_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_SERIAL
#if GD32_SERIAL_USE_USART0
  sd_lld_serve_interrupt(&SD0);
#endif
#endif

#if HAL_USE_SIO
#if GD32_SIO_USE_USART0
  sio_lld_serve_interrupt(&SIOD0);
#endif
#endif

#if HAL_USE_UART
#if GD32_UART_USE_USART0
  uart_lld_serve_interrupt(&UARTD0);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

/** @} */
