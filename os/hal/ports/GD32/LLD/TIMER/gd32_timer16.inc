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
 * @file    GD32/LLD/TIMER/gd32_timer16.inc
 * @brief   Shared TIMER16 handler.
 *
 * @addtogroup GD32_TIMER16_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(GD32_HAS_TIMER16)
#error "GD32_HAS_TIMER16 not defined in registry"
#endif

/* Driver checks for robustness, undefined USE macros are defaulted to
   FALSE. This makes this module independent from drivers implementation.*/
#if !defined(GD32_GPT_USE_TIMER16)
#define GD32_GPT_USE_TIMER16            FALSE
#endif
#if !defined(GD32_ICU_USE_TIMER16)
#define GD32_ICU_USE_TIMER16            FALSE
#endif
#if !defined(GD32_EICU_USE_TIMER16)
#define GD32_EICU_USE_TIMER16           FALSE
#endif
#if !defined(GD32_PWM_USE_TIMER16)
#define GD32_PWM_USE_TIMER16            FALSE
#endif

#if defined(GD32_TIMER16_IS_USED)

/* Priority settings checks.*/
#if !defined(GD32_IRQ_TIMER16_PRIORITY)
#error "GD32_IRQ_TIMER16_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_TIMER16_PRIORITY)
#error "Invalid IRQ priority assigned to GD32_IRQ_TIMER16_PRIORITY"
#endif

#endif /* defined(GD32_TIMER16_IS_USED) */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void gd32_timer16_irq_init(void) {
#if defined(GD32_TIMER16_IS_USED)
  nvicEnableVector(GD32_TIMER16_NUMBER, GD32_IRQ_TIMER16_PRIORITY);
#endif
}

static inline void gd32_timer16_irq_deinit(void) {
#if defined(GD32_TIMER16_IS_USED)
  nvicDisableVector(GD32_TIMER16_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(GD32_TIMER16_IS_USED) || defined(__DOXYGEN__)
/**
 * @brief   TIMER16 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER16_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_GPT
#if GD32_GPT_USE_TIMER16
  gpt_lld_serve_interrupt(&GPTD16);
#endif
#endif
#if HAL_USE_ICU
#if GD32_ICU_USE_TIMER16
  icu_lld_serve_interrupt(&ICUD16);
#endif
#endif
#if HAL_USE_EICU
#if GD32_EICU_USE_TIMER16
  eicu_lld_serve_interrupt(&EICUD16);
#endif
#endif
#if HAL_USE_PWM
#if GD32_PWM_USE_TIMER16
  pwm_lld_serve_interrupt(&PWMD16);
#endif
#endif
#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) && (GD32_ST_USE_TIMER == 16)
  st_lld_serve_interrupt();
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

/** @} */