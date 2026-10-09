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
 * @file    SPI/gd32_spi3.inc
 * @brief   Shared SPI3 handler.
 *
 * @addtogroup GD32_SPI3_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(GD32_HAS_SPI3)
#error "GD32_HAS_SPI3 not defined in registry"
#endif

/* Other checks.*/
#if (HAL_USE_SPI && GD32_SPI_USE_SPI3)
#define GD32_SPI3_IS_USED           TRUE
#else
#define GD32_SPI3_IS_USED           FALSE
#endif

#if GD32_SPI3_IS_USED

/* Priority settings checks.*/
#if !defined(GD32_IRQ_SPI3_PRIORITY)
#error "GD32_IRQ_SPI3_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_SPI3_PRIORITY)
#error "Invalid IRQ priority assigned to GD32_IRQ_SPI3_PRIORITY"
#endif

#endif /* GD32_SPI3_IS_USED */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void spi3_irq_init(void) {
#if GD32_SPI3_IS_USED
  nvicEnableVector(GD32_SPI3_NUMBER, GD32_IRQ_SPI3_PRIORITY);
#endif
}

static inline void spi3_irq_deinit(void) {
#if GD32_SPI3_IS_USED
  nvicDisableVector(GD32_SPI3_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if GD32_SPI3_IS_USED || defined(__DOXYGEN__)
/**
 * @brief   SPI3 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_SPI3_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  spi_lld_serve_interrupt(&SPID3);

  OSAL_IRQ_EPILOGUE();
}
#endif

/** @} */
