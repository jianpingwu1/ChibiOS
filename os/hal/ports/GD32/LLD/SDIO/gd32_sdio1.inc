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
 * @file    SDIO/gd32_sdio1.inc
 * @brief   Shared SDIO1 handler.
 *
 * @addtogroup GD32_SDIO1_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(GD32_HAS_SDIO1)
#error "GD32_HAS_SDIO1 not defined in registry"
#endif

/* Other checks.*/
#if (HAL_USE_SDC && GD32_SDC_USE_SDIO1)
#define GD32_SDIO1_IS_USED          TRUE
#else
#define GD32_SDIO1_IS_USED          FALSE
#endif

#if GD32_SDIO1_IS_USED

/* Priority settings checks.*/
#if !defined(GD32_IRQ_SDIO1_PRIORITY)
#error "GD32_IRQ_SDIO1_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_SDIO1_PRIORITY)
#error "Invalid IRQ priority assigned to GD32_IRQ_SDIO1_PRIORITY"
#endif

#endif /* GD32_SDIO1_IS_USED */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void sdio1_irq_init(void) {
#if GD32_SDIO1_IS_USED
  nvicEnableVector(GD32_SDIO1_NUMBER, GD32_IRQ_SDIO1_PRIORITY);
#endif
}

static inline void sdio1_irq_deinit(void) {
#if GD32_SDIO1_IS_USED
  nvicDisableVector(GD32_SDIO1_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if GD32_SDIO1_IS_USED || defined(__DOXYGEN__)
/**
 * @brief   SDIO1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_SDIO1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_SDC
#if GD32_SDC_USE_SDIO1
  sdc_lld_serve_interrupt(&SDCD1);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/** @} */
