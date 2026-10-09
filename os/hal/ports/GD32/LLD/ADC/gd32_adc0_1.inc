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
 * @file    GD32/LLD/ADC/gd32_adc0_1.inc
 * @brief   Shared ADC0/ADC1 handler.
 * @note    Driver instance numbering follows GD32's own 0-based hardware
 *          peripheral numbering:
 *          ADC0/ADC1 (hardware) <-> ADCD0/ADCD1 (ChibiOS driver objects).
 *
 * @addtogroup GD32_ADC0_1_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(GD32_HAS_ADC0)
#error "GD32_HAS_ADC0 not defined in registry"
#endif

#if !defined(GD32_HAS_ADC1)
#error "GD32_HAS_ADC1 not defined in registry"
#endif

#if defined(GD32_ADC0_IS_USED) || defined(GD32_ADC1_IS_USED)

/* Priority settings checks.*/
#if !defined(GD32_IRQ_ADC0_1_PRIORITY)
#error "GD32_IRQ_ADC0_1_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_ADC0_1_PRIORITY)
#error "Invalid IRQ priority assigned to GD32_IRQ_ADC0_1_PRIORITY"
#endif

#endif /* defined(GD32_ADC0_IS_USED) || defined(GD32_ADC1_IS_USED) */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void gd32_adc0_1_irq_init(void) {
#if defined(GD32_ADC0_IS_USED) || defined(GD32_ADC1_IS_USED)
  nvicEnableVector(GD32_ADC0_1_NUMBER, GD32_IRQ_ADC0_1_PRIORITY);
#endif
}

static inline void gd32_adc0_1_irq_deinit(void) {
#if defined(GD32_ADC0_IS_USED) || defined(GD32_ADC1_IS_USED)
  nvicDisableVector(GD32_ADC0_1_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(GD32_ADC0_IS_USED) || defined(GD32_ADC1_IS_USED) ||             \
    defined(__DOXYGEN__)
/**
 * @brief   ADC0/ADC1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_ADC0_1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_ADC
#if GD32_ADC_USE_ADC0
  adc_lld_serve_interrupt(&ADCD0);
#endif
#if GD32_ADC_USE_ADC1
  adc_lld_serve_interrupt(&ADCD1);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

/** @} */
