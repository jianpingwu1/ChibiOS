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
 * @file    TIMERv1/hal_pwm_lld.c
 * @brief   GD32 PWM subsystem low level driver source.
 *
 * @addtogroup PWM
 * @{
 */

#include "hal.h"

#if HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PWMD0 driver identifier.
 * @note    The driver PWMD0 allocates the complex timer TIMER0 when enabled.
 */
#if GD32_PWM_USE_TIMER0 || defined(__DOXYGEN__)
PWMDriver PWMD0;
#endif

/**
 * @brief   PWMD1 driver identifier.
 * @note    The driver PWMD1 allocates the timer TIMER1 when enabled.
 */
#if GD32_PWM_USE_TIMER1 || defined(__DOXYGEN__)
PWMDriver PWMD1;
#endif

/**
 * @brief   PWMD2 driver identifier.
 * @note    The driver PWMD2 allocates the timer TIMER2 when enabled.
 */
#if GD32_PWM_USE_TIMER2 || defined(__DOXYGEN__)
PWMDriver PWMD2;
#endif

/**
 * @brief   PWMD3 driver identifier.
 * @note    The driver PWMD3 allocates the timer TIMER3 when enabled.
 */
#if GD32_PWM_USE_TIMER3 || defined(__DOXYGEN__)
PWMDriver PWMD3;
#endif

/**
 * @brief   PWMD4 driver identifier.
 * @note    The driver PWMD4 allocates the timer TIMER4 when enabled.
 */
#if GD32_PWM_USE_TIMER4 || defined(__DOXYGEN__)
PWMDriver PWMD4;
#endif

/**
 * @brief   PWMD7 driver identifier.
 * @note    The driver PWMD7 allocates the complex timer TIMER7 when enabled.
 */
#if GD32_PWM_USE_TIMER7 || defined(__DOXYGEN__)
PWMDriver PWMD7;
#endif

/**
 * @brief   PWMD14 driver identifier.
 * @note    The driver PWMD14 allocates the timer TIMER14 when enabled.
 */
#if GD32_PWM_USE_TIMER14 || defined(__DOXYGEN__)
PWMDriver PWMD14;
#endif

/**
 * @brief   PWMD15 driver identifier.
 * @note    The driver PWMD15 allocates the timer TIMER15 when enabled.
 */
#if GD32_PWM_USE_TIMER15 || defined(__DOXYGEN__)
PWMDriver PWMD15;
#endif

/**
 * @brief   PWMD16 driver identifier.
 * @note    The driver PWMD16 allocates the timer TIMER16 when enabled.
 */
#if GD32_PWM_USE_TIMER16 || defined(__DOXYGEN__)
PWMDriver PWMD16;
#endif

/**
 * @brief   PWMD22 driver identifier.
 * @note    The driver PWMD22 allocates the timer TIMER22 when enabled.
 */
#if GD32_PWM_USE_TIMER22 || defined(__DOXYGEN__)
PWMDriver PWMD22;
#endif

/**
 * @brief   PWMD23 driver identifier.
 * @note    The driver PWMD23 allocates the timer TIMER23 when enabled.
 */
#if GD32_PWM_USE_TIMER23 || defined(__DOXYGEN__)
PWMDriver PWMD23;
#endif

/**
 * @brief   PWMD30 driver identifier.
 * @note    The driver PWMD30 allocates the timer TIMER30 when enabled.
 */
#if GD32_PWM_USE_TIMER30 || defined(__DOXYGEN__)
PWMDriver PWMD30;
#endif

/**
 * @brief   PWMD31 driver identifier.
 * @note    The driver PWMD31 allocates the timer TIMER31 when enabled.
 */
#if GD32_PWM_USE_TIMER31 || defined(__DOXYGEN__)
PWMDriver PWMD31;
#endif

/**
 * @brief   PWMD40 driver identifier.
 * @note    The driver PWMD40 allocates the timer TIMER40 when enabled.
 */
#if GD32_PWM_USE_TIMER40 || defined(__DOXYGEN__)
PWMDriver PWMD40;
#endif

/**
 * @brief   PWMD41 driver identifier.
 * @note    The driver PWMD41 allocates the timer TIMER41 when enabled.
 */
#if GD32_PWM_USE_TIMER41 || defined(__DOXYGEN__)
PWMDriver PWMD41;
#endif

/**
 * @brief   PWMD42 driver identifier.
 * @note    The driver PWMD42 allocates the timer TIMER42 when enabled.
 */
#if GD32_PWM_USE_TIMER42 || defined(__DOXYGEN__)
PWMDriver PWMD42;
#endif

/**
 * @brief   PWMD43 driver identifier.
 * @note    The driver PWMD43 allocates the timer TIMER43 when enabled.
 */
#if GD32_PWM_USE_TIMER43 || defined(__DOXYGEN__)
PWMDriver PWMD43;
#endif

/**
 * @brief   PWMD44 driver identifier.
 * @note    The driver PWMD44 allocates the timer TIMER44 when enabled.
 */
#if GD32_PWM_USE_TIMER44 || defined(__DOXYGEN__)
PWMDriver PWMD44;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if GD32_PWM_USE_TIMER0 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
#if !defined(GD32_TIMER0_UP_HANDLER)
#error "GD32_TIMER0_UP_HANDLER not defined"
#endif
/**
 * @brief   TIMER0 update interrupt handler.
 * @note    It is assumed that this interrupt is only activated if the callback
 *          pointer is not equal to @p NULL in order to not perform an extra
 *          check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER0_UP_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD0);

  OSAL_IRQ_EPILOGUE();
}

#if !defined(GD32_TIMER0_CC_HANDLER)
#error "GD32_TIMER0_CC_HANDLER not defined"
#endif
/**
 * @brief   TIMER0 compare interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER0_CC_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD0);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER0_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER0 */

#if GD32_PWM_USE_TIMER1 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER1_SUPPRESS_ISR)
#if !defined(GD32_TIMER1_HANDLER)
#error "GD32_TIMER1_HANDLER not defined"
#endif
/**
 * @brief   TIMER1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD1);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER1_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER1 */

#if GD32_PWM_USE_TIMER2 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER2_SUPPRESS_ISR)
#if !defined(GD32_TIMER2_HANDLER)
#error "GD32_TIMER2_HANDLER not defined"
#endif
/**
 * @brief   TIMER2 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD2);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER2_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER2 */

#if GD32_PWM_USE_TIMER3 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER3_SUPPRESS_ISR)
#if !defined(GD32_TIMER3_HANDLER)
#error "GD32_TIMER3_HANDLER not defined"
#endif
/**
 * @brief   TIMER3 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER3_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD3);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER3_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER3 */

#if GD32_PWM_USE_TIMER4 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER4_SUPPRESS_ISR)
#if !defined(GD32_TIMER4_HANDLER)
#error "GD32_TIMER4_HANDLER not defined"
#endif
/**
 * @brief   TIMER4 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER4_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD4);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER4_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER4 */

#if GD32_PWM_USE_TIMER7 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
#if !defined(GD32_TIMER7_UP_HANDLER)
#error "GD32_TIMER7_UP_HANDLER not defined"
#endif
/**
 * @brief   TIMER7 update interrupt handler.
 * @note    It is assumed that this interrupt is only activated if the callback
 *          pointer is not equal to @p NULL in order to not perform an extra
 *          check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER7_UP_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD7);

  OSAL_IRQ_EPILOGUE();
}

#if !defined(GD32_TIMER7_CC_HANDLER)
#error "GD32_TIMER7_CC_HANDLER not defined"
#endif
/**
 * @brief   TIMER7 compare interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER7_CC_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD7);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER7_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER7 */

#if GD32_PWM_USE_TIMER14 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER14_SUPPRESS_ISR)
#if !defined(GD32_TIMER14_HANDLER)
#error "GD32_TIMER14_HANDLER not defined"
#endif
/**
 * @brief   TIMER14 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER14_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD14);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER14_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER14 */

#if GD32_PWM_USE_TIMER15 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER15_SUPPRESS_ISR)
#if !defined(GD32_TIMER15_HANDLER)
#error "GD32_TIMER15_HANDLER not defined"
#endif
/**
 * @brief   TIMER15 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER15_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD15);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER15_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER15 */

#if GD32_PWM_USE_TIMER16 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER16_SUPPRESS_ISR)
#if !defined(GD32_TIMER16_HANDLER)
#error "GD32_TIMER16_HANDLER not defined"
#endif
/**
 * @brief   TIMER16 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER16_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD16);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER16_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER16 */

#if GD32_PWM_USE_TIMER22 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER22_SUPPRESS_ISR)
#if !defined(GD32_TIMER22_HANDLER)
#error "GD32_TIMER22_HANDLER not defined"
#endif
/**
 * @brief   TIMER22 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER22_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD22);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER22_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER22 */

#if GD32_PWM_USE_TIMER23 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER23_SUPPRESS_ISR)
#if !defined(GD32_TIMER23_HANDLER)
#error "GD32_TIMER23_HANDLER not defined"
#endif
/**
 * @brief   TIMER23 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER23_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD23);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER23_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER23 */

#if GD32_PWM_USE_TIMER30 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER30_SUPPRESS_ISR)
#if !defined(GD32_TIMER30_HANDLER)
#error "GD32_TIMER30_HANDLER not defined"
#endif
/**
 * @brief   TIMER30 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER30_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD30);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER30_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER30 */

#if GD32_PWM_USE_TIMER31 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER31_SUPPRESS_ISR)
#if !defined(GD32_TIMER31_HANDLER)
#error "GD32_TIMER31_HANDLER not defined"
#endif
/**
 * @brief   TIMER31 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER31_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD31);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER31_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER31 */

#if GD32_PWM_USE_TIMER40 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER40_SUPPRESS_ISR)
#if !defined(GD32_TIMER40_HANDLER)
#error "GD32_TIMER40_HANDLER not defined"
#endif
/**
 * @brief   TIMER40 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER40_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD40);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER40_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER40 */

#if GD32_PWM_USE_TIMER41 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER41_SUPPRESS_ISR)
#if !defined(GD32_TIMER41_HANDLER)
#error "GD32_TIMER41_HANDLER not defined"
#endif
/**
 * @brief   TIMER41 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER41_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD41);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER41_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER41 */

#if GD32_PWM_USE_TIMER42 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER42_SUPPRESS_ISR)
#if !defined(GD32_TIMER42_HANDLER)
#error "GD32_TIMER42_HANDLER not defined"
#endif
/**
 * @brief   TIMER42 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER42_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD42);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER42_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER42 */

#if GD32_PWM_USE_TIMER43 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER43_SUPPRESS_ISR)
#if !defined(GD32_TIMER43_HANDLER)
#error "GD32_TIMER43_HANDLER not defined"
#endif
/**
 * @brief   TIMER43 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER43_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD43);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER43_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER43 */

#if GD32_PWM_USE_TIMER44 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER44_SUPPRESS_ISR)
#if !defined(GD32_TIMER44_HANDLER)
#error "GD32_TIMER44_HANDLER not defined"
#endif
/**
 * @brief   TIMER44 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER44_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  pwm_lld_serve_interrupt(&PWMD44);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER44_SUPPRESS_ISR) */
#endif /* GD32_PWM_USE_TIMER44 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level PWM driver initialization.
 *
 * @notapi
 */
void pwm_lld_init(void) {

#if GD32_PWM_USE_TIMER0
  /* Driver initialization.*/
  pwmObjectInit(&PWMD0);
  PWMD0.channels = GD32_TIMER0_CHANNELS;
  PWMD0.tim = TIMER0;
  PWMD0.has_cchp = GD32_TIMER0_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER1
  /* Driver initialization.*/
  pwmObjectInit(&PWMD1);
  PWMD1.channels = GD32_TIMER1_CHANNELS;
  PWMD1.tim = TIMER1;
  PWMD1.has_cchp = GD32_TIMER1_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER2
  /* Driver initialization.*/
  pwmObjectInit(&PWMD2);
  PWMD2.channels = GD32_TIMER2_CHANNELS;
  PWMD2.tim = TIMER2;
  PWMD2.has_cchp = GD32_TIMER2_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER3
  /* Driver initialization.*/
  pwmObjectInit(&PWMD3);
  PWMD3.channels = GD32_TIMER3_CHANNELS;
  PWMD3.tim = TIMER3;
  PWMD3.has_cchp = GD32_TIMER3_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER4
  /* Driver initialization.*/
  pwmObjectInit(&PWMD4);
  PWMD4.channels = GD32_TIMER4_CHANNELS;
  PWMD4.tim = TIMER4;
  PWMD4.has_cchp = GD32_TIMER4_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER7
  /* Driver initialization.*/
  pwmObjectInit(&PWMD7);
  PWMD7.channels = GD32_TIMER7_CHANNELS;
  PWMD7.tim = TIMER7;
  PWMD7.has_cchp = GD32_TIMER7_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER14
  /* Driver initialization.*/
  pwmObjectInit(&PWMD14);
  PWMD14.channels = GD32_TIMER14_CHANNELS;
  PWMD14.tim = TIMER14;
  PWMD14.has_cchp = GD32_TIMER14_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER15
  /* Driver initialization.*/
  pwmObjectInit(&PWMD15);
  PWMD15.channels = GD32_TIMER15_CHANNELS;
  PWMD15.tim = TIMER15;
  PWMD15.has_cchp = GD32_TIMER15_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER16
  /* Driver initialization.*/
  pwmObjectInit(&PWMD16);
  PWMD16.channels = GD32_TIMER16_CHANNELS;
  PWMD16.tim = TIMER16;
  PWMD16.has_cchp = GD32_TIMER16_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER22
  /* Driver initialization.*/
  pwmObjectInit(&PWMD22);
  PWMD22.channels = GD32_TIMER22_CHANNELS;
  PWMD22.tim = TIMER22;
  PWMD22.has_cchp = GD32_TIMER22_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER23
  /* Driver initialization.*/
  pwmObjectInit(&PWMD23);
  PWMD23.channels = GD32_TIMER23_CHANNELS;
  PWMD23.tim = TIMER23;
  PWMD23.has_cchp = GD32_TIMER23_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER30
  /* Driver initialization.*/
  pwmObjectInit(&PWMD30);
  PWMD30.channels = GD32_TIMER30_CHANNELS;
  PWMD30.tim = TIMER30;
  PWMD30.has_cchp = GD32_TIMER30_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER31
  /* Driver initialization.*/
  pwmObjectInit(&PWMD31);
  PWMD31.channels = GD32_TIMER31_CHANNELS;
  PWMD31.tim = TIMER31;
  PWMD31.has_cchp = GD32_TIMER31_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER40
  /* Driver initialization.*/
  pwmObjectInit(&PWMD40);
  PWMD40.channels = GD32_TIMER40_CHANNELS;
  PWMD40.tim = TIMER40;
  PWMD40.has_cchp = GD32_TIMER40_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER41
  /* Driver initialization.*/
  pwmObjectInit(&PWMD41);
  PWMD41.channels = GD32_TIMER41_CHANNELS;
  PWMD41.tim = TIMER41;
  PWMD41.has_cchp = GD32_TIMER41_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER42
  /* Driver initialization.*/
  pwmObjectInit(&PWMD42);
  PWMD42.channels = GD32_TIMER42_CHANNELS;
  PWMD42.tim = TIMER42;
  PWMD42.has_cchp = GD32_TIMER42_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER43
  /* Driver initialization.*/
  pwmObjectInit(&PWMD43);
  PWMD43.channels = GD32_TIMER43_CHANNELS;
  PWMD43.tim = TIMER43;
  PWMD43.has_cchp = GD32_TIMER43_IS_ADVANCED;
#endif

#if GD32_PWM_USE_TIMER44
  /* Driver initialization.*/
  pwmObjectInit(&PWMD44);
  PWMD44.channels = GD32_TIMER44_CHANNELS;
  PWMD44.tim = TIMER44;
  PWMD44.has_cchp = GD32_TIMER44_IS_ADVANCED;
#endif
}

/**
 * @brief   Configures and activates the PWM peripheral.
 * @note    Starting a driver that is already in the @p PWM_READY state
 *          disables all the active channels.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_start(PWMDriver *pwmp) {
  uint32_t psc;
  uint32_t chctl2;

  if (pwmp->state == PWM_STOP) {
    /* Clock activation and timer reset.*/
#if GD32_PWM_USE_TIMER0
    if (&PWMD0 == pwmp) {
      rcuEnableTIMER0(true);
      rcuResetTIMER0();
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER0_UP_NUMBER, GD32_PWM_TIMER0_IRQ_PRIORITY);
      nvicEnableVector(GD32_TIMER0_CC_NUMBER, GD32_PWM_TIMER0_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_PWM_USE_TIMER1
    if (&PWMD1 == pwmp) {
      rcuEnableTIMER1(true);
      rcuResetTIMER1();
#if !defined(GD32_TIMER1_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER1_NUMBER, GD32_PWM_TIMER1_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_PWM_USE_TIMER2
    if (&PWMD2 == pwmp) {
      rcuEnableTIMER2(true);
      rcuResetTIMER2();
#if !defined(GD32_TIMER2_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER2_NUMBER, GD32_PWM_TIMER2_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_PWM_USE_TIMER3
    if (&PWMD3 == pwmp) {
      rcuEnableTIMER3(true);
      rcuResetTIMER3();
#if !defined(GD32_TIMER3_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER3_NUMBER, GD32_PWM_TIMER3_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_PWM_USE_TIMER4
    if (&PWMD4 == pwmp) {
      rcuEnableTIMER4(true);
      rcuResetTIMER4();
#if !defined(GD32_TIMER4_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER4_NUMBER, GD32_PWM_TIMER4_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_PWM_USE_TIMER7
    if (&PWMD7 == pwmp) {
      rcuEnableTIMER7(true);
      rcuResetTIMER7();
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER7_UP_NUMBER, GD32_PWM_TIMER7_IRQ_PRIORITY);
      nvicEnableVector(GD32_TIMER7_CC_NUMBER, GD32_PWM_TIMER7_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_PWM_USE_TIMER14
    if (&PWMD14 == pwmp) {
      rcuEnableTIMER14(true);
      rcuResetTIMER14();
#if !defined(GD32_TIMER14_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER14_NUMBER, GD32_PWM_TIMER14_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_PWM_USE_TIMER15
    if (&PWMD15 == pwmp) {
      rcuEnableTIMER15(true);
      rcuResetTIMER15();
#if !defined(GD32_TIMER15_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER15_NUMBER, GD32_PWM_TIMER15_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_PWM_USE_TIMER16
    if (&PWMD16 == pwmp) {
      rcuEnableTIMER16(true);
      rcuResetTIMER16();
#if !defined(GD32_TIMER16_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER16_NUMBER, GD32_PWM_TIMER16_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_PWM_USE_TIMER22
    if (&PWMD22 == pwmp) {
      rcuEnableTIMER22(true);
      rcuResetTIMER22();
#if !defined(GD32_TIMER22_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER22_NUMBER, GD32_PWM_TIMER22_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_PWM_USE_TIMER23
    if (&PWMD23 == pwmp) {
      rcuEnableTIMER23(true);
      rcuResetTIMER23();
#if !defined(GD32_TIMER23_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER23_NUMBER, GD32_PWM_TIMER23_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_PWM_USE_TIMER30
    if (&PWMD30 == pwmp) {
      rcuEnableTIMER30(true);
      rcuResetTIMER30();
#if !defined(GD32_TIMER30_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER30_NUMBER, GD32_PWM_TIMER30_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_PWM_USE_TIMER31
    if (&PWMD31 == pwmp) {
      rcuEnableTIMER31(true);
      rcuResetTIMER31();
#if !defined(GD32_TIMER31_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER31_NUMBER, GD32_PWM_TIMER31_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_PWM_USE_TIMER40
    if (&PWMD40 == pwmp) {
      rcuEnableTIMER40(true);
      rcuResetTIMER40();
#if !defined(GD32_TIMER40_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER40_NUMBER, GD32_PWM_TIMER40_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_PWM_USE_TIMER41
    if (&PWMD41 == pwmp) {
      rcuEnableTIMER41(true);
      rcuResetTIMER41();
#if !defined(GD32_TIMER41_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER41_NUMBER, GD32_PWM_TIMER41_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_PWM_USE_TIMER42
    if (&PWMD42 == pwmp) {
      rcuEnableTIMER42(true);
      rcuResetTIMER42();
#if !defined(GD32_TIMER42_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER42_NUMBER, GD32_PWM_TIMER42_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_PWM_USE_TIMER43
    if (&PWMD43 == pwmp) {
      rcuEnableTIMER43(true);
      rcuResetTIMER43();
#if !defined(GD32_TIMER43_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER43_NUMBER, GD32_PWM_TIMER43_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_PWM_USE_TIMER44
    if (&PWMD44 == pwmp) {
      rcuEnableTIMER44(true);
      rcuResetTIMER44();
#if !defined(GD32_TIMER44_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER44_NUMBER, GD32_PWM_TIMER44_IRQ_PRIORITY);
#endif
      pwmp->clock = GD32_TIMERCLK2;
    }
#endif

     /* Output compare mode 6 selects PWM mode 0 with preload enabled. */
    TIMER_CHCTL0(pwmp->tim) = GD32_TIMER_CHCTL0_CH0COMCTL(6) |
                              TIMER_CHCTL0_CH0COMSEN |
                              GD32_TIMER_CHCTL0_CH1COMCTL(6) |
                              TIMER_CHCTL0_CH1COMSEN;
    TIMER_CHCTL1(pwmp->tim) = GD32_TIMER_CHCTL1_CH2COMCTL(6) |
                              TIMER_CHCTL1_CH2COMSEN |
                              GD32_TIMER_CHCTL1_CH3COMCTL(6) |
                              TIMER_CHCTL1_CH3COMSEN;
#if GD32_TIMER_MAX_CHANNELS > 4
    /* Extended channel support is pending. */
#endif
  }
  else {
    /* Driver re-configuration scenario, it must be stopped first.*/
    TIMER_CTL0(pwmp->tim) = 0;                    /* Timer disabled.        */
    GD32_TIMER_CHCV(pwmp->tim, 0) = 0;            /* Comparator 0 disabled. */
    GD32_TIMER_CHCV(pwmp->tim, 1) = 0;            /* Comparator 1 disabled. */
    GD32_TIMER_CHCV(pwmp->tim, 2) = 0;            /* Comparator 2 disabled. */
    GD32_TIMER_CHCV(pwmp->tim, 3) = 0;            /* Comparator 3 disabled. */
#if GD32_TIMER_MAX_CHANNELS > 4
    /* Extended channel support is pending. */
#endif
    TIMER_CNT(pwmp->tim) = 0;                     /* Counter reset to zero. */
  }

  /* Timer configuration.*/
  psc = (pwmp->clock / pwmp->config->frequency) - 1;
  osalDbgAssert((psc <= 0xFFFF),
                "invalid frequency");
  TIMER_PSC(pwmp->tim)  = psc;
  TIMER_CAR(pwmp->tim)  = pwmp->period - 1;
  TIMER_CTL1(pwmp->tim) = pwmp->config->ctl1;

  /* Output enables and polarities setup.*/
  chctl2 = 0;
  switch (pwmp->config->channels[0].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    chctl2 |= TIMER_CHCTL2_CH0P;
    /* Falls through.*/
  case PWM_OUTPUT_ACTIVE_HIGH:
    chctl2 |= TIMER_CHCTL2_CH0EN;
    /* Falls through.*/
  default:
    ;
  }
  switch (pwmp->config->channels[1].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    chctl2 |= TIMER_CHCTL2_CH1P;
    /* Falls through.*/
  case PWM_OUTPUT_ACTIVE_HIGH:
    chctl2 |= TIMER_CHCTL2_CH1EN;
    /* Falls through.*/
  default:
    ;
  }
  switch (pwmp->config->channels[2].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    chctl2 |= TIMER_CHCTL2_CH2P;
    /* Falls through.*/
  case PWM_OUTPUT_ACTIVE_HIGH:
    chctl2 |= TIMER_CHCTL2_CH2EN;
    /* Falls through.*/
  default:
    ;
  }
  switch (pwmp->config->channels[3].mode & PWM_OUTPUT_MASK) {
  case PWM_OUTPUT_ACTIVE_LOW:
    chctl2 |= TIMER_CHCTL2_CH3P;
    /* Falls through.*/
  case PWM_OUTPUT_ACTIVE_HIGH:
    chctl2 |= TIMER_CHCTL2_CH3EN;
    /* Falls through.*/
  default:
    ;
  }
  if (pwmp->has_cchp) {
    switch (pwmp->config->channels[0].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
      chctl2 |= TIMER_CHCTL2_CH0NP;
      /* Falls through.*/
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
      chctl2 |= TIMER_CHCTL2_CH0NEN;
      /* Falls through.*/
    default:
      ;
    }
    switch (pwmp->config->channels[1].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
      chctl2 |= TIMER_CHCTL2_CH1NP;
      /* Falls through.*/
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
      chctl2 |= TIMER_CHCTL2_CH1NEN;
      /* Falls through.*/
    default:
      ;
    }
    switch (pwmp->config->channels[2].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
      chctl2 |= TIMER_CHCTL2_CH2NP;
      /* Falls through.*/
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
      chctl2 |= TIMER_CHCTL2_CH2NEN;
      /* Falls through.*/
    default:
      ;
    }
    switch (pwmp->config->channels[3].mode & PWM_COMPLEMENTARY_OUTPUT_MASK) {
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW:
      chctl2 |= TIMER_CHCTL2_CH3NP;
      /* Falls through.*/
    case PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH:
      chctl2 |= TIMER_CHCTL2_CH3NEN;
      /* Falls through.*/
    default:
      ;
    }

    /* MCHxMSEL selects between independent and complementary mode for the
       physical bit shared with CHxNEN/CHxNP, forced to complementary.*/
    {
      uint32_t ctl2 = TIMER_CTL2(pwmp->tim);

      if (pwmp->channels > 0U) {
        ctl2 |= TIMER_CTL2_MCH0MSEL;
      }
      if (pwmp->channels > 1U) {
        ctl2 |= TIMER_CTL2_MCH1MSEL;
      }
      if (pwmp->channels > 2U) {
        ctl2 |= TIMER_CTL2_MCH2MSEL;
      }
      if (pwmp->channels > 3U) {
        ctl2 |= TIMER_CTL2_MCH3MSEL;
      }
      TIMER_CTL2(pwmp->tim) = ctl2;
    }
  }

  TIMER_CHCTL2(pwmp->tim)   = chctl2;
  TIMER_SWEVG(pwmp->tim)    = TIMER_SWEVG_UPG;      /* Update event.        */
  TIMER_INTF(pwmp->tim)     = 0;                    /* Clear pending IRQs.  */
  TIMER_DMAINTEN(pwmp->tim) = pwmp->config->dmainten & /* DMA-related bits. */
                              ~GD32_TIMER_DMAINTEN_IRQ_MASK;
  if (pwmp->has_cchp) {
    TIMER_CCHP(pwmp->tim) = pwmp->config->cchp | TIMER_CCHP_POEN;
  }
  /* Timer configured and started.*/
  TIMER_CTL0(pwmp->tim) = TIMER_CTL0_ARSE | TIMER_CTL0_UPS | TIMER_CTL0_CEN;
}

/**
 * @brief   Deactivates the PWM peripheral.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_stop(PWMDriver *pwmp) {

  /* If in ready state then disables the PWM clock.*/
  if (pwmp->state == PWM_READY) {
    TIMER_CTL0(pwmp->tim)     = 0;                  /* Timer disabled.     */
    TIMER_DMAINTEN(pwmp->tim) = 0;                  /* All IRQs disabled.  */
    TIMER_INTF(pwmp->tim)     = 0;                  /* Clear pending IRQs. */
    if (pwmp->has_cchp) {
      TIMER_CCHP(pwmp->tim) = 0;
    }

#if GD32_PWM_USE_TIMER0
    if (&PWMD0 == pwmp) {
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER0_UP_NUMBER);
      nvicDisableVector(GD32_TIMER0_CC_NUMBER);
#endif
      rcuDisableTIMER0();
    }
#endif

#if GD32_PWM_USE_TIMER1
    if (&PWMD1 == pwmp) {
#if !defined(GD32_TIMER1_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER1_NUMBER);
#endif
      rcuDisableTIMER1();
    }
#endif

#if GD32_PWM_USE_TIMER2
    if (&PWMD2 == pwmp) {
#if !defined(GD32_TIMER2_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER2_NUMBER);
#endif
      rcuDisableTIMER2();
    }
#endif

#if GD32_PWM_USE_TIMER3
    if (&PWMD3 == pwmp) {
#if !defined(GD32_TIMER3_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER3_NUMBER);
#endif
      rcuDisableTIMER3();
    }
#endif

#if GD32_PWM_USE_TIMER4
    if (&PWMD4 == pwmp) {
#if !defined(GD32_TIMER4_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER4_NUMBER);
#endif
      rcuDisableTIMER4();
    }
#endif

#if GD32_PWM_USE_TIMER7
    if (&PWMD7 == pwmp) {
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER7_UP_NUMBER);
      nvicDisableVector(GD32_TIMER7_CC_NUMBER);
#endif
      rcuDisableTIMER7();
    }
#endif

#if GD32_PWM_USE_TIMER14
    if (&PWMD14 == pwmp) {
#if !defined(GD32_TIMER14_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER14_NUMBER);
#endif
      rcuDisableTIMER14();
    }
#endif

#if GD32_PWM_USE_TIMER15
    if (&PWMD15 == pwmp) {
#if !defined(GD32_TIMER15_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER15_NUMBER);
#endif
      rcuDisableTIMER15();
    }
#endif

#if GD32_PWM_USE_TIMER16
    if (&PWMD16 == pwmp) {
#if !defined(GD32_TIMER16_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER16_NUMBER);
#endif
      rcuDisableTIMER16();
    }
#endif

#if GD32_PWM_USE_TIMER22
    if (&PWMD22 == pwmp) {
#if !defined(GD32_TIMER22_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER22_NUMBER);
#endif
      rcuDisableTIMER22();
    }
#endif

#if GD32_PWM_USE_TIMER23
    if (&PWMD23 == pwmp) {
#if !defined(GD32_TIMER23_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER23_NUMBER);
#endif
      rcuDisableTIMER23();
    }
#endif

#if GD32_PWM_USE_TIMER30
    if (&PWMD30 == pwmp) {
#if !defined(GD32_TIMER30_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER30_NUMBER);
#endif
      rcuDisableTIMER30();
    }
#endif

#if GD32_PWM_USE_TIMER31
    if (&PWMD31 == pwmp) {
#if !defined(GD32_TIMER31_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER31_NUMBER);
#endif
      rcuDisableTIMER31();
    }
#endif

#if GD32_PWM_USE_TIMER40
    if (&PWMD40 == pwmp) {
#if !defined(GD32_TIMER40_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER40_NUMBER);
#endif
      rcuDisableTIMER40();
    }
#endif

#if GD32_PWM_USE_TIMER41
    if (&PWMD41 == pwmp) {
#if !defined(GD32_TIMER41_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER41_NUMBER);
#endif
      rcuDisableTIMER41();
    }
#endif

#if GD32_PWM_USE_TIMER42
    if (&PWMD42 == pwmp) {
#if !defined(GD32_TIMER42_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER42_NUMBER);
#endif
      rcuDisableTIMER42();
    }
#endif

#if GD32_PWM_USE_TIMER43
    if (&PWMD43 == pwmp) {
#if !defined(GD32_TIMER43_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER43_NUMBER);
#endif
      rcuDisableTIMER43();
    }
#endif

#if GD32_PWM_USE_TIMER44
    if (&PWMD44 == pwmp) {
#if !defined(GD32_TIMER44_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER44_NUMBER);
#endif
      rcuDisableTIMER44();
    }
#endif
  }
}

/**
 * @brief   Enables a PWM channel.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is active using the specified configuration.
 * @note    The function has effect at the next cycle start.
 * @note    Channel notification is not enabled.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 * @param[in] width     PWM pulse width as clock pulses number
 *
 * @notapi
 */
void pwm_lld_enable_channel(PWMDriver *pwmp,
                            pwmchannel_t channel,
                            pwmcnt_t width) {

  /* Changing channel duty cycle on the fly.*/
#if GD32_TIMER_MAX_CHANNELS <= 4
  GD32_TIMER_CHCV(pwmp->tim, channel) = width;
#else
  if (channel < 4) {
    GD32_TIMER_CHCV(pwmp->tim, channel) = width;
  }
  else {
    /* Extended channel support is pending. */
  }
#endif
}

/**
 * @brief   Disables a PWM channel and its notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The channel is disabled and its output line returned to the
 *          idle state.
 * @note    The function has effect at the next cycle start.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 *
 * @notapi
 */
void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel) {

#if GD32_TIMER_MAX_CHANNELS <= 4
  GD32_TIMER_CHCV(pwmp->tim, channel) = 0;
  TIMER_DMAINTEN(pwmp->tim) &= ~(2 << channel);
#else
  if (channel < 4) {
    GD32_TIMER_CHCV(pwmp->tim, channel) = 0;
    TIMER_DMAINTEN(pwmp->tim) &= ~(2 << channel);
  }
  else {
    /* Extended channel support is pending. */
  }
#endif
}

/**
 * @brief   Enables the periodic activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @note    If the notification is already enabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_enable_periodic_notification(PWMDriver *pwmp) {
  uint32_t dmainten = TIMER_DMAINTEN(pwmp->tim);

  /* If the IRQ is not already enabled care must be taken to clear it,
     it is probably already pending because the timer is running.*/
  if ((dmainten & TIMER_DMAINTEN_UPIE) == 0) {
    TIMER_INTF(pwmp->tim)     = ~TIMER_INTF_UPIF;
    TIMER_DMAINTEN(pwmp->tim) = dmainten | TIMER_DMAINTEN_UPIE;
  }
}

/**
 * @brief   Disables the periodic activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @note    If the notification is already disabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_disable_periodic_notification(PWMDriver *pwmp) {

  TIMER_DMAINTEN(pwmp->tim) &= ~TIMER_DMAINTEN_UPIE;
}

/**
 * @brief   Enables a channel de-activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @pre     The channel must have been activated using @p pwmEnableChannel().
 * @note    If the notification is already enabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 *
 * @notapi
 */
void pwm_lld_enable_channel_notification(PWMDriver *pwmp,
                                         pwmchannel_t channel) {
  uint32_t dmainten = TIMER_DMAINTEN(pwmp->tim);

#if GD32_TIMER_MAX_CHANNELS > 4
  /* Extended channel support is pending. */
  osalDbgAssert(channel < 4U, "callback not supported");
#endif

  /* If the IRQ is not already enabled care must be taken to clear it,
     it is probably already pending because the timer is running.*/
  if ((dmainten & (2 << channel)) == 0) {
    TIMER_INTF(pwmp->tim)     = ~(2 << channel);
    TIMER_DMAINTEN(pwmp->tim) = dmainten | (2 << channel);
  }
}

/**
 * @brief   Disables a channel de-activation edge notification.
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @pre     The channel must have been activated using @p pwmEnableChannel().
 * @note    If the notification is already disabled then the call has no effect.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] channel   PWM channel identifier (0...channels-1)
 *
 * @notapi
 */
void pwm_lld_disable_channel_notification(PWMDriver *pwmp,
                                          pwmchannel_t channel) {

  TIMER_DMAINTEN(pwmp->tim) &= ~(2 << channel);
}

/**
 * @brief   Shared IRQ handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 *
 * @notapi
 */
void pwm_lld_serve_interrupt(PWMDriver *pwmp) {
  uint32_t sr;

  sr  = TIMER_INTF(pwmp->tim);
  sr &= TIMER_DMAINTEN(pwmp->tim) & GD32_TIMER_DMAINTEN_IRQ_MASK;
  TIMER_INTF(pwmp->tim) = ~sr;
  if (((sr & TIMER_INTF_CH0IF) != 0) &&
      (pwmp->config->channels[0].callback != NULL))
    pwmp->config->channels[0].callback(pwmp);
  if (((sr & TIMER_INTF_CH1IF) != 0) &&
      (pwmp->config->channels[1].callback != NULL))
    pwmp->config->channels[1].callback(pwmp);
  if (((sr & TIMER_INTF_CH2IF) != 0) &&
      (pwmp->config->channels[2].callback != NULL))
    pwmp->config->channels[2].callback(pwmp);
  if (((sr & TIMER_INTF_CH3IF) != 0) &&
      (pwmp->config->channels[3].callback != NULL))
    pwmp->config->channels[3].callback(pwmp);
  if (((sr & TIMER_INTF_UPIF) != 0) && (pwmp->config->callback != NULL))
    pwmp->config->callback(pwmp);
}

#endif /* HAL_USE_PWM */

/** @} */
