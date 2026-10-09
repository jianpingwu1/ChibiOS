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
 * @file    TIMERv1/hal_icu_lld.c
 * @brief   GD32 ICU subsystem low level driver source.
 *
 * @addtogroup ICU
 * @{
 */

#include "hal.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   ICUD0 driver identifier.
 * @note    The driver ICUD0 allocates the complex timer TIMER0 when enabled.
 */
#if GD32_ICU_USE_TIMER0 || defined(__DOXYGEN__)
ICUDriver ICUD0;
#endif

/**
 * @brief   ICUD1 driver identifier.
 * @note    The driver ICUD1 allocates the timer TIMER1 when enabled.
 */
#if GD32_ICU_USE_TIMER1 || defined(__DOXYGEN__)
ICUDriver ICUD1;
#endif

/**
 * @brief   ICUD2 driver identifier.
 * @note    The driver ICUD2 allocates the timer TIMER2 when enabled.
 */
#if GD32_ICU_USE_TIMER2 || defined(__DOXYGEN__)
ICUDriver ICUD2;
#endif

/**
 * @brief   ICUD3 driver identifier.
 * @note    The driver ICUD3 allocates the timer TIMER3 when enabled.
 */
#if GD32_ICU_USE_TIMER3 || defined(__DOXYGEN__)
ICUDriver ICUD3;
#endif

/**
 * @brief   ICUD4 driver identifier.
 * @note    The driver ICUD4 allocates the timer TIMER4 when enabled.
 */
#if GD32_ICU_USE_TIMER4 || defined(__DOXYGEN__)
ICUDriver ICUD4;
#endif

/**
 * @brief   ICUD7 driver identifier.
 * @note    The driver ICUD7 allocates the complex timer TIMER7 when enabled.
 */
#if GD32_ICU_USE_TIMER7 || defined(__DOXYGEN__)
ICUDriver ICUD7;
#endif

/**
 * @brief   ICUD14 driver identifier.
 * @note    The driver ICUD14 allocates the timer TIMER14 when enabled.
 */
#if GD32_ICU_USE_TIMER14 || defined(__DOXYGEN__)
ICUDriver ICUD14;
#endif

/**
 * @brief   ICUD22 driver identifier.
 * @note    The driver ICUD22 allocates the timer TIMER22 when enabled.
 */
#if GD32_ICU_USE_TIMER22 || defined(__DOXYGEN__)
ICUDriver ICUD22;
#endif

/**
 * @brief   ICUD23 driver identifier.
 * @note    The driver ICUD23 allocates the timer TIMER23 when enabled.
 */
#if GD32_ICU_USE_TIMER23 || defined(__DOXYGEN__)
ICUDriver ICUD23;
#endif

/**
 * @brief   ICUD30 driver identifier.
 * @note    The driver ICUD30 allocates the timer TIMER30 when enabled.
 */
#if GD32_ICU_USE_TIMER30 || defined(__DOXYGEN__)
ICUDriver ICUD30;
#endif

/**
 * @brief   ICUD31 driver identifier.
 * @note    The driver ICUD31 allocates the timer TIMER31 when enabled.
 */
#if GD32_ICU_USE_TIMER31 || defined(__DOXYGEN__)
ICUDriver ICUD31;
#endif

/**
 * @brief   ICUD40 driver identifier.
 * @note    The driver ICUD40 allocates the timer TIMER40 when enabled.
 */
#if GD32_ICU_USE_TIMER40 || defined(__DOXYGEN__)
ICUDriver ICUD40;
#endif

/**
 * @brief   ICUD41 driver identifier.
 * @note    The driver ICUD41 allocates the timer TIMER41 when enabled.
 */
#if GD32_ICU_USE_TIMER41 || defined(__DOXYGEN__)
ICUDriver ICUD41;
#endif

/**
 * @brief   ICUD42 driver identifier.
 * @note    The driver ICUD42 allocates the timer TIMER42 when enabled.
 */
#if GD32_ICU_USE_TIMER42 || defined(__DOXYGEN__)
ICUDriver ICUD42;
#endif

/**
 * @brief   ICUD43 driver identifier.
 * @note    The driver ICUD43 allocates the timer TIMER43 when enabled.
 */
#if GD32_ICU_USE_TIMER43 || defined(__DOXYGEN__)
ICUDriver ICUD43;
#endif

/**
 * @brief   ICUD44 driver identifier.
 * @note    The driver ICUD44 allocates the timer TIMER44 when enabled.
 */
#if GD32_ICU_USE_TIMER44 || defined(__DOXYGEN__)
ICUDriver ICUD44;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Waits for a capture or a timer overflow.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The result.
 * @retval false        if an edge has been captured.
 * @retval true         if a timer overflow occurred.
 */
static bool icu_lld_wait_edge(ICUDriver *icup) {
  uint32_t sr;
  bool result;

  /* Polled mode so re-enabling the interrupts while the operation is
     performed.*/
  osalSysUnlock();

  /* Polling the right bit depending on the input channel.*/
  if (icup->config->channel == ICU_CHANNEL_1) {
    /* Waiting for an edge.*/
    while (((sr = TIMER_INTF(icup->tim)) &
            (TIMER_INTF_CH0IF | TIMER_INTF_UPIF)) == 0)
      ;
  }
  else {
    /* Waiting for an edge.*/
    while (((sr = TIMER_INTF(icup->tim)) &
            (TIMER_INTF_CH1IF | TIMER_INTF_UPIF)) == 0)
      ;
  }

  /* Edge or overflow?*/
  result = (sr & TIMER_INTF_UPIF) != 0 ? true : false;

  /* Done, disabling interrupts again.*/
  osalSysLock();

  /* Resetting all flags.*/
  TIMER_INTF(icup->tim) &= ~(TIMER_INTF_CH0IF |
                             TIMER_INTF_CH1IF |
                             TIMER_INTF_UPIF);

  return result;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if GD32_ICU_USE_TIMER0 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
#if !defined(GD32_TIMER0_UP_HANDLER)
#error "GD32_TIMER0_UP_HANDLER not defined"
#endif
/**
 * @brief   TIMER0 update interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER0_UP_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD0);

  OSAL_IRQ_EPILOGUE();
}

#if !defined(GD32_TIMER0_CC_HANDLER)
#error "GD32_TIMER0_CC_HANDLER not defined"
#endif
/**
 * @brief   TIMER0 compare interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER0_CC_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD0);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER0_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER0 */

#if GD32_ICU_USE_TIMER1 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD1);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER1_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER1 */

#if GD32_ICU_USE_TIMER2 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD2);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER2_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER2 */

#if GD32_ICU_USE_TIMER3 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD3);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER3_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER3 */

#if GD32_ICU_USE_TIMER4 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD4);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER4_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER4 */

#if GD32_ICU_USE_TIMER7 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
#if !defined(GD32_TIMER7_UP_HANDLER)
#error "GD32_TIMER7_UP_HANDLER not defined"
#endif
/**
 * @brief   TIMER7 update interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER7_UP_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD7);

  OSAL_IRQ_EPILOGUE();
}

#if !defined(GD32_TIMER7_CC_HANDLER)
#error "GD32_TIMER7_CC_HANDLER not defined"
#endif
/**
 * @brief   TIMER7 compare interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER7_CC_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  icu_lld_serve_interrupt(&ICUD7);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER7_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER7 */

#if GD32_ICU_USE_TIMER14 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD14);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER14_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER14 */

#if GD32_ICU_USE_TIMER22 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD22);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER22_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER22 */

#if GD32_ICU_USE_TIMER23 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD23);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER23_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER23 */

#if GD32_ICU_USE_TIMER30 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD30);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER30_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER30 */

#if GD32_ICU_USE_TIMER31 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD31);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER31_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER31 */

#if GD32_ICU_USE_TIMER40 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD40);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER40_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER40 */

#if GD32_ICU_USE_TIMER41 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD41);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER41_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER41 */

#if GD32_ICU_USE_TIMER42 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD42);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER42_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER42 */

#if GD32_ICU_USE_TIMER43 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD43);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER43_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER43 */

#if GD32_ICU_USE_TIMER44 || defined(__DOXYGEN__)
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

  icu_lld_serve_interrupt(&ICUD44);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER44_SUPPRESS_ISR) */
#endif /* GD32_ICU_USE_TIMER44 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ICU driver initialization.
 *
 * @notapi
 */
void icu_lld_init(void) {

#if GD32_ICU_USE_TIMER0
  /* Driver initialization.*/
  icuObjectInit(&ICUD0);
  ICUD0.tim    = TIMER0;
  ICUD0.syscfg = SYSCFG_TIMER0;
#endif

#if GD32_ICU_USE_TIMER1
  /* Driver initialization.*/
  icuObjectInit(&ICUD1);
  ICUD1.tim    = TIMER1;
  ICUD1.syscfg = SYSCFG_TIMER1;
#endif

#if GD32_ICU_USE_TIMER2
  /* Driver initialization.*/
  icuObjectInit(&ICUD2);
  ICUD2.tim    = TIMER2;
  ICUD2.syscfg = SYSCFG_TIMER2;
#endif

#if GD32_ICU_USE_TIMER3
  /* Driver initialization.*/
  icuObjectInit(&ICUD3);
  ICUD3.tim    = TIMER3;
  ICUD3.syscfg = SYSCFG_TIMER3;
#endif

#if GD32_ICU_USE_TIMER4
  /* Driver initialization.*/
  icuObjectInit(&ICUD4);
  ICUD4.tim    = TIMER4;
  ICUD4.syscfg = SYSCFG_TIMER4;
#endif

#if GD32_ICU_USE_TIMER7
  /* Driver initialization.*/
  icuObjectInit(&ICUD7);
  ICUD7.tim    = TIMER7;
  ICUD7.syscfg = SYSCFG_TIMER7;
#endif

#if GD32_ICU_USE_TIMER14
  /* Driver initialization.*/
  icuObjectInit(&ICUD14);
  ICUD14.tim    = TIMER14;
  ICUD14.syscfg = SYSCFG_TIMER14;
#endif

#if GD32_ICU_USE_TIMER22
  /* Driver initialization.*/
  icuObjectInit(&ICUD22);
  ICUD22.tim    = TIMER22;
  ICUD22.syscfg = SYSCFG_TIMER22;
#endif

#if GD32_ICU_USE_TIMER23
  /* Driver initialization.*/
  icuObjectInit(&ICUD23);
  ICUD23.tim    = TIMER23;
  ICUD23.syscfg = SYSCFG_TIMER23;
#endif

#if GD32_ICU_USE_TIMER30
  /* Driver initialization.*/
  icuObjectInit(&ICUD30);
  ICUD30.tim    = TIMER30;
  ICUD30.syscfg = SYSCFG_TIMER30;
#endif

#if GD32_ICU_USE_TIMER31
  /* Driver initialization.*/
  icuObjectInit(&ICUD31);
  ICUD31.tim    = TIMER31;
  ICUD31.syscfg = SYSCFG_TIMER31;
#endif

#if GD32_ICU_USE_TIMER40
  /* Driver initialization.*/
  icuObjectInit(&ICUD40);
  ICUD40.tim    = TIMER40;
  ICUD40.syscfg = SYSCFG_TIMER40;
#endif

#if GD32_ICU_USE_TIMER41
  /* Driver initialization.*/
  icuObjectInit(&ICUD41);
  ICUD41.tim    = TIMER41;
  ICUD41.syscfg = SYSCFG_TIMER41;
#endif

#if GD32_ICU_USE_TIMER42
  /* Driver initialization.*/
  icuObjectInit(&ICUD42);
  ICUD42.tim    = TIMER42;
  ICUD42.syscfg = SYSCFG_TIMER42;
#endif

#if GD32_ICU_USE_TIMER43
  /* Driver initialization.*/
  icuObjectInit(&ICUD43);
  ICUD43.tim    = TIMER43;
  ICUD43.syscfg = SYSCFG_TIMER43;
#endif

#if GD32_ICU_USE_TIMER44
  /* Driver initialization.*/
  icuObjectInit(&ICUD44);
  ICUD44.tim    = TIMER44;
  ICUD44.syscfg = SYSCFG_TIMER44;
#endif
}

/**
 * @brief   Configures and activates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_start(ICUDriver *icup) {
  uint32_t psc;

  osalDbgAssert((icup->config->channel == ICU_CHANNEL_1) ||
                (icup->config->channel == ICU_CHANNEL_2),
                "invalid input");

  if (icup->state == ICU_STOP) {
    /* Clock activation and timer reset.*/
#if GD32_ICU_USE_TIMER0
    if (&ICUD0 == icup) {
      rcuEnableTIMER0(true);
      rcuResetTIMER0();
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER0_UP_NUMBER, GD32_ICU_TIMER0_IRQ_PRIORITY);
      nvicEnableVector(GD32_TIMER0_CC_NUMBER, GD32_ICU_TIMER0_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_ICU_USE_TIMER1
    if (&ICUD1 == icup) {
      rcuEnableTIMER1(true);
      rcuResetTIMER1();
#if !defined(GD32_TIMER1_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER1_NUMBER, GD32_ICU_TIMER1_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_ICU_USE_TIMER2
    if (&ICUD2 == icup) {
      rcuEnableTIMER2(true);
      rcuResetTIMER2();
#if !defined(GD32_TIMER2_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER2_NUMBER, GD32_ICU_TIMER2_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_ICU_USE_TIMER3
    if (&ICUD3 == icup) {
      rcuEnableTIMER3(true);
      rcuResetTIMER3();
#if !defined(GD32_TIMER3_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER3_NUMBER, GD32_ICU_TIMER3_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_ICU_USE_TIMER4
    if (&ICUD4 == icup) {
      rcuEnableTIMER4(true);
      rcuResetTIMER4();
#if !defined(GD32_TIMER4_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER4_NUMBER, GD32_ICU_TIMER4_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_ICU_USE_TIMER7
    if (&ICUD7 == icup) {
      rcuEnableTIMER7(true);
      rcuResetTIMER7();
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER7_UP_NUMBER, GD32_ICU_TIMER7_IRQ_PRIORITY);
      nvicEnableVector(GD32_TIMER7_CC_NUMBER, GD32_ICU_TIMER7_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_ICU_USE_TIMER14
    if (&ICUD14 == icup) {
      rcuEnableTIMER14(true);
      rcuResetTIMER14();
#if !defined(GD32_TIMER14_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER14_NUMBER, GD32_ICU_TIMER14_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_ICU_USE_TIMER22
    if (&ICUD22 == icup) {
      rcuEnableTIMER22(true);
      rcuResetTIMER22();
#if !defined(GD32_TIMER22_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER22_NUMBER, GD32_ICU_TIMER22_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_ICU_USE_TIMER23
    if (&ICUD23 == icup) {
      rcuEnableTIMER23(true);
      rcuResetTIMER23();
#if !defined(GD32_TIMER23_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER23_NUMBER, GD32_ICU_TIMER23_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_ICU_USE_TIMER30
    if (&ICUD30 == icup) {
      rcuEnableTIMER30(true);
      rcuResetTIMER30();
#if !defined(GD32_TIMER30_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER30_NUMBER, GD32_ICU_TIMER30_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_ICU_USE_TIMER31
    if (&ICUD31 == icup) {
      rcuEnableTIMER31(true);
      rcuResetTIMER31();
#if !defined(GD32_TIMER31_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER31_NUMBER, GD32_ICU_TIMER31_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_ICU_USE_TIMER40
    if (&ICUD40 == icup) {
      rcuEnableTIMER40(true);
      rcuResetTIMER40();
#if !defined(GD32_TIMER40_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER40_NUMBER, GD32_ICU_TIMER40_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_ICU_USE_TIMER41
    if (&ICUD41 == icup) {
      rcuEnableTIMER41(true);
      rcuResetTIMER41();
#if !defined(GD32_TIMER41_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER41_NUMBER, GD32_ICU_TIMER41_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_ICU_USE_TIMER42
    if (&ICUD42 == icup) {
      rcuEnableTIMER42(true);
      rcuResetTIMER42();
#if !defined(GD32_TIMER42_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER42_NUMBER, GD32_ICU_TIMER42_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_ICU_USE_TIMER43
    if (&ICUD43 == icup) {
      rcuEnableTIMER43(true);
      rcuResetTIMER43();
#if !defined(GD32_TIMER43_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER43_NUMBER, GD32_ICU_TIMER43_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_ICU_USE_TIMER44
    if (&ICUD44 == icup) {
      rcuEnableTIMER44(true);
      rcuResetTIMER44();
#if !defined(GD32_TIMER44_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER44_NUMBER, GD32_ICU_TIMER44_IRQ_PRIORITY);
#endif
      icup->clock = GD32_TIMERCLK2;
    }
#endif
  }
  else {
    /* Driver re-configuration scenario, it must be stopped first.*/
    TIMER_CTL0(icup->tim)         = 0U;  /* Timer disabled.               */
    GD32_TIMER_CHCV(icup->tim, 0U) = 0U; /* Comparator 0 disabled.        */
    GD32_TIMER_CHCV(icup->tim, 1U) = 0U; /* Comparator 1 disabled.        */
    TIMER_CNT(icup->tim)          = 0U;  /* Counter reset to zero.        */
  }

  /* Timer configuration.*/
  TIMER_INTF(icup->tim)     = 0U;        /* Clear eventual pending IRQs.  */
  TIMER_DMAINTEN(icup->tim) = icup->config->dmainten &
                              ~GD32_TIMER_DMAINTEN_IRQ_MASK;
  psc = (icup->clock / icup->config->frequency) - 1U;
  osalDbgAssert((psc <= 0xFFFFU) &&
                ((psc + 1U) * icup->config->frequency) == icup->clock,
                "invalid frequency");
  TIMER_PSC(icup->tim) = psc;
  if (icup->config->car == 0U) {
    /* Zero is an invalid value and is turned in maximum value, also for
       legacy configurations compatibility.*/
    TIMER_CAR(icup->tim) = 0xFFFFFFFFU;
  }
  else {
    TIMER_CAR(icup->tim) = icup->config->car;
  }

  if (icup->config->channel == ICU_CHANNEL_1) {
    /* Selected input 0.
       CH0MS = DIRECTTI = CH0 input on CI0.
       CH1MS = INDIRECTTI = CH1 input on CI0.*/
    TIMER_CHCTL0(icup->tim) =
      GD32_TIMER_CHCTL0_CH0MS(TIMER_IC_SELECTION_DIRECTTI) |
      GD32_TIMER_CHCTL0_CH1MS(TIMER_IC_SELECTION_INDIRECTTI);

    /* Restart mode, trigger source is the filtered CI0 input (CI0FE0).
       The other slave-mode configuration registers must be cleared first,
       the TSCFGy fields are mutually exclusive.*/
    SYSCFG_TIMERCFG1(icup->syscfg) = 0U;
    SYSCFG_TIMERCFG2(icup->syscfg) = 0U;
    SYSCFG_TIMERCFG0(icup->syscfg) =
      GD32_TIMER_TSCFG3(TIMER_SMCFG_TRGSEL_CI0FE0);

    /* The CHCTL2 settings depend on the selected trigger mode.
       ICU_INPUT_ACTIVE_HIGH: Active on rising edge, idle on falling edge.
       ICU_INPUT_ACTIVE_LOW:  Active on falling edge, idle on rising edge.*/
    if (icup->config->mode == ICU_INPUT_ACTIVE_HIGH)
      TIMER_CHCTL2(icup->tim) = TIMER_CHCTL2_CH0EN |
                                TIMER_CHCTL2_CH1EN | TIMER_CHCTL2_CH1P;
    else
      TIMER_CHCTL2(icup->tim) = TIMER_CHCTL2_CH0EN | TIMER_CHCTL2_CH0P |
                                TIMER_CHCTL2_CH1EN;

    /* Direct pointers to the capture registers in order to make reading
       data faster from within callbacks.*/
    icup->wccrp = &GD32_TIMER_CHCV(icup->tim, 1U);
    icup->pccrp = &GD32_TIMER_CHCV(icup->tim, 0U);
  }
  else {
    /* Selected input 1.
       CH0MS = INDIRECTTI = CH0 input on CI1.
       CH1MS = DIRECTTI = CH1 input on CI1.*/
    TIMER_CHCTL0(icup->tim) =
      GD32_TIMER_CHCTL0_CH0MS(TIMER_IC_SELECTION_INDIRECTTI) |
      GD32_TIMER_CHCTL0_CH1MS(TIMER_IC_SELECTION_DIRECTTI);

    /* Restart mode, trigger source is the filtered CI1 input (CI1FE1).*/
    SYSCFG_TIMERCFG1(icup->syscfg) = 0U;
    SYSCFG_TIMERCFG2(icup->syscfg) = 0U;
    SYSCFG_TIMERCFG0(icup->syscfg) =
      GD32_TIMER_TSCFG3(TIMER_SMCFG_TRGSEL_CI1FE1);

    /* The CHCTL2 settings depend on the selected trigger mode.
       ICU_INPUT_ACTIVE_HIGH: Active on rising edge, idle on falling edge.
       ICU_INPUT_ACTIVE_LOW:  Active on falling edge, idle on rising edge.*/
    if (icup->config->mode == ICU_INPUT_ACTIVE_HIGH)
      TIMER_CHCTL2(icup->tim) = TIMER_CHCTL2_CH0EN | TIMER_CHCTL2_CH0P |
                                TIMER_CHCTL2_CH1EN;
    else
      TIMER_CHCTL2(icup->tim) = TIMER_CHCTL2_CH0EN |
                                TIMER_CHCTL2_CH1EN | TIMER_CHCTL2_CH1P;

    /* Direct pointers to the capture registers in order to make reading
       data faster from within callbacks.*/
    icup->wccrp = &GD32_TIMER_CHCV(icup->tim, 0U);
    icup->pccrp = &GD32_TIMER_CHCV(icup->tim, 1U);
  }
}

/**
 * @brief   Deactivates the ICU peripheral.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_stop(ICUDriver *icup) {

  if (icup->state == ICU_READY) {
    /* Clock deactivation.*/
    TIMER_CTL0(icup->tim)     = 0U; /* Timer disabled.                   */
    TIMER_DMAINTEN(icup->tim) = 0U; /* All IRQs disabled.                */
    TIMER_INTF(icup->tim)     = 0U; /* Clear eventual pending IRQs.      */

    /* Restart mode configuration is outside the TIMER reset domain. */
    SYSCFG_TIMERCFG0(icup->syscfg) = 0U;
    SYSCFG_TIMERCFG1(icup->syscfg) = 0U;
    SYSCFG_TIMERCFG2(icup->syscfg) = 0U;

#if GD32_ICU_USE_TIMER0
    if (&ICUD0 == icup) {
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER0_UP_NUMBER);
      nvicDisableVector(GD32_TIMER0_CC_NUMBER);
#endif
      rcuDisableTIMER0();
    }
#endif

#if GD32_ICU_USE_TIMER1
    if (&ICUD1 == icup) {
#if !defined(GD32_TIMER1_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER1_NUMBER);
#endif
      rcuDisableTIMER1();
    }
#endif

#if GD32_ICU_USE_TIMER2
    if (&ICUD2 == icup) {
#if !defined(GD32_TIMER2_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER2_NUMBER);
#endif
      rcuDisableTIMER2();
    }
#endif

#if GD32_ICU_USE_TIMER3
    if (&ICUD3 == icup) {
#if !defined(GD32_TIMER3_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER3_NUMBER);
#endif
      rcuDisableTIMER3();
    }
#endif

#if GD32_ICU_USE_TIMER4
    if (&ICUD4 == icup) {
#if !defined(GD32_TIMER4_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER4_NUMBER);
#endif
      rcuDisableTIMER4();
    }
#endif

#if GD32_ICU_USE_TIMER7
    if (&ICUD7 == icup) {
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER7_UP_NUMBER);
      nvicDisableVector(GD32_TIMER7_CC_NUMBER);
#endif
      rcuDisableTIMER7();
    }
#endif

#if GD32_ICU_USE_TIMER14
    if (&ICUD14 == icup) {
#if !defined(GD32_TIMER14_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER14_NUMBER);
#endif
      rcuDisableTIMER14();
    }
#endif

#if GD32_ICU_USE_TIMER22
    if (&ICUD22 == icup) {
#if !defined(GD32_TIMER22_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER22_NUMBER);
#endif
      rcuDisableTIMER22();
    }
#endif

#if GD32_ICU_USE_TIMER23
    if (&ICUD23 == icup) {
#if !defined(GD32_TIMER23_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER23_NUMBER);
#endif
      rcuDisableTIMER23();
    }
#endif

#if GD32_ICU_USE_TIMER30
    if (&ICUD30 == icup) {
#if !defined(GD32_TIMER30_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER30_NUMBER);
#endif
      rcuDisableTIMER30();
    }
#endif

#if GD32_ICU_USE_TIMER31
    if (&ICUD31 == icup) {
#if !defined(GD32_TIMER31_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER31_NUMBER);
#endif
      rcuDisableTIMER31();
    }
#endif

#if GD32_ICU_USE_TIMER40
    if (&ICUD40 == icup) {
#if !defined(GD32_TIMER40_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER40_NUMBER);
#endif
      rcuDisableTIMER40();
    }
#endif

#if GD32_ICU_USE_TIMER41
    if (&ICUD41 == icup) {
#if !defined(GD32_TIMER41_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER41_NUMBER);
#endif
      rcuDisableTIMER41();
    }
#endif

#if GD32_ICU_USE_TIMER42
    if (&ICUD42 == icup) {
#if !defined(GD32_TIMER42_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER42_NUMBER);
#endif
      rcuDisableTIMER42();
    }
#endif

#if GD32_ICU_USE_TIMER43
    if (&ICUD43 == icup) {
#if !defined(GD32_TIMER43_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER43_NUMBER);
#endif
      rcuDisableTIMER43();
    }
#endif

#if GD32_ICU_USE_TIMER44
    if (&ICUD44 == icup) {
#if !defined(GD32_TIMER44_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER44_NUMBER);
#endif
      rcuDisableTIMER44();
    }
#endif
  }
}

/**
 * @brief   Starts the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_start_capture(ICUDriver *icup) {

  /* Triggering an UPG and clearing the IRQ status.*/
  TIMER_SWEVG(icup->tim) |= TIMER_SWEVG_UPG;
  TIMER_INTF(icup->tim) = 0U;

  /* Timer is started.*/
  TIMER_CTL0(icup->tim) = TIMER_CTL0_UPS | TIMER_CTL0_CEN;
}

/**
 * @brief   Waits for a completed capture.
 * @note    The operation is performed in polled mode.
 * @note    In order to use this function notifications must be disabled.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The capture status.
 * @retval false        if the capture is successful.
 * @retval true         if a timer overflow occurred.
 *
 * @notapi
 */
bool icu_lld_wait_capture(ICUDriver *icup) {

  /* If the driver is still in the ICU_WAITING state then we need to wait
     for the first activation edge.*/
  if (icup->state == ICU_WAITING)
    if (icu_lld_wait_edge(icup))
      return true;

  /* This edge marks the availability of a capture result.*/
  return icu_lld_wait_edge(icup);
}

/**
 * @brief   Stops the input capture.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_stop_capture(ICUDriver *icup) {

  /* Timer stopped.*/
  TIMER_CTL0(icup->tim) = 0U;

  /* All interrupts disabled.*/
  TIMER_DMAINTEN(icup->tim) &= ~GD32_TIMER_DMAINTEN_IRQ_MASK;
}

/**
 * @brief   Enables notifications.
 * @pre     The ICU unit must have been activated using @p icuStart() and the
 *          capture started using @p icuStartCapture().
 * @note    If the notification is already enabled then the call has no effect.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_enable_notifications(ICUDriver *icup) {
  uint32_t dmainten = TIMER_DMAINTEN(icup->tim);

  /* If interrupts were already enabled then the operation is skipped.
     This is done in order to avoid clearing the INTF and risk losing
     pending interrupts.*/
  if ((dmainten & GD32_TIMER_DMAINTEN_IRQ_MASK) == 0) {
    /* Previously triggered IRQs are ignored, status cleared.*/
    TIMER_INTF(icup->tim) = 0U;

    if (icup->config->channel == ICU_CHANNEL_1) {
      /* Enabling periodic callback on CH0.*/
      dmainten |= TIMER_DMAINTEN_CH0IE;

      /* Optionally enabling width callback on CH1.*/
      if (icup->config->width_cb != NULL)
        dmainten |= TIMER_DMAINTEN_CH1IE;
    }
    else {
      /* Enabling periodic callback on CH1.*/
      dmainten |= TIMER_DMAINTEN_CH1IE;

      /* Optionally enabling width callback on CH0.*/
      if (icup->config->width_cb != NULL)
        dmainten |= TIMER_DMAINTEN_CH0IE;
    }

    /* If an overflow callback is defined then also the overflow callback
       is enabled.*/
    if (icup->config->overflow_cb != NULL)
      dmainten |= TIMER_DMAINTEN_UPIE;

    /* One single atomic write.*/
    TIMER_DMAINTEN(icup->tim) = dmainten;
  }
}

/**
 * @brief   Disables notifications.
 * @pre     The ICU unit must have been activated using @p icuStart() and the
 *          capture started using @p icuStartCapture().
 * @note    If the notification is already disabled then the call has no effect.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_disable_notifications(ICUDriver *icup) {

  /* All interrupts disabled.*/
  TIMER_DMAINTEN(icup->tim) &= ~GD32_TIMER_DMAINTEN_IRQ_MASK;
}

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 *
 * @notapi
 */
void icu_lld_serve_interrupt(ICUDriver *icup) {
  uint32_t sr;

  sr  = TIMER_INTF(icup->tim);
  sr &= TIMER_DMAINTEN(icup->tim) & GD32_TIMER_DMAINTEN_IRQ_MASK;
  TIMER_INTF(icup->tim) = ~sr;
  if (icup->config->channel == ICU_CHANNEL_1) {
    if ((sr & TIMER_INTF_CH1IF) != 0)
      _icu_isr_invoke_width_cb(icup);
    if ((sr & TIMER_INTF_CH0IF) != 0)
      _icu_isr_invoke_period_cb(icup);
  }
  else {
    if ((sr & TIMER_INTF_CH0IF) != 0)
      _icu_isr_invoke_width_cb(icup);
    if ((sr & TIMER_INTF_CH1IF) != 0)
      _icu_isr_invoke_period_cb(icup);
  }
  if ((sr & TIMER_INTF_UPIF) != 0)
    _icu_isr_invoke_overflow_cb(icup);
}

#endif /* HAL_USE_ICU */

/** @} */
