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
/*
   Concepts and parts of this file have been contributed by Fabio Utzig and
   Xo Wang.
*/
/*
   Rewritten by Emil Fresk (1/5 - 2014) for extended input capture
   functionality. And fix for spurious callbacks in the interrupt handler.
*/
/*
   Improved by Uladzimir Pylinsky aka barthess (1/3 - 2015) for support of
   32-bit timers and timers with single capture/compare channels.
*/

/*
 * Hardware Abstraction Layer for Extended Input Capture Unit
 */
#include "hal.h"

#if (HAL_USE_EICU == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/
/**
 * @brief   Inverts the polarity for the given channel.
 *
 * @param[in] eicup     Pointer to the EICUDriver object.
 * @param[in] channel   The timer channel to invert.
 *
 * @notapi
 */
#define eicu_lld_invert_polarity(eicup, channel)                              \
  TIMER_CHCTL2((eicup)->tim) ^=                                               \
    ((uint16_t)(TIMER_CHCTL2_CH0P << ((channel) * 4)))

/**
 * @brief   Returns the compare value of the latest cycle.
 *
 * @param[in] chp       Pointer to channel structure that fired the interrupt.
 * @return              The number of ticks.
 *
 * @notapi
 */
#define eicu_lld_get_compare(chp)     (*((chp)->ccrp) + 1)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EICUD0 driver identifier.
 * @note    The driver EICUD0 allocates the complex timer TIMER0 when enabled.
 */
#if GD32_EICU_USE_TIMER0 && !defined(__DOXYGEN__)
EICUDriver EICUD0;
#endif

/**
 * @brief   EICUD1 driver identifier.
 * @note    The driver EICUD1 allocates the timer TIMER1 when enabled.
 */
#if GD32_EICU_USE_TIMER1 && !defined(__DOXYGEN__)
EICUDriver EICUD1;
#endif

/**
 * @brief   EICUD2 driver identifier.
 * @note    The driver EICUD2 allocates the timer TIMER2 when enabled.
 */
#if GD32_EICU_USE_TIMER2 && !defined(__DOXYGEN__)
EICUDriver EICUD2;
#endif

/**
 * @brief   EICUD3 driver identifier.
 * @note    The driver EICUD3 allocates the timer TIMER3 when enabled.
 */
#if GD32_EICU_USE_TIMER3 && !defined(__DOXYGEN__)
EICUDriver EICUD3;
#endif

/**
 * @brief   EICUD4 driver identifier.
 * @note    The driver EICUD4 allocates the timer TIMER4 when enabled.
 */
#if GD32_EICU_USE_TIMER4 && !defined(__DOXYGEN__)
EICUDriver EICUD4;
#endif

/**
 * @brief   EICUD7 driver identifier.
 * @note    The driver EICUD7 allocates the complex timer TIMER7 when enabled.
 */
#if GD32_EICU_USE_TIMER7 && !defined(__DOXYGEN__)
EICUDriver EICUD7;
#endif

/**
 * @brief   EICUD14 driver identifier.
 * @note    The driver EICUD14 allocates the timer TIMER14 when enabled.
 */
#if GD32_EICU_USE_TIMER14 && !defined(__DOXYGEN__)
EICUDriver EICUD14;
#endif

/**
 * @brief   EICUD15 driver identifier.
 * @note    The driver EICUD15 allocates the timer TIMER15 when enabled.
 */
#if GD32_EICU_USE_TIMER15 && !defined(__DOXYGEN__)
EICUDriver EICUD15;
#endif

/**
 * @brief   EICUD16 driver identifier.
 * @note    The driver EICUD16 allocates the timer TIMER16 when enabled.
 */
#if GD32_EICU_USE_TIMER16 && !defined(__DOXYGEN__)
EICUDriver EICUD16;
#endif

/**
 * @brief   EICUD22 driver identifier.
 * @note    The driver EICUD22 allocates the timer TIMER22 when enabled.
 */
#if GD32_EICU_USE_TIMER22 && !defined(__DOXYGEN__)
EICUDriver EICUD22;
#endif

/**
 * @brief   EICUD23 driver identifier.
 * @note    The driver EICUD23 allocates the timer TIMER23 when enabled.
 */
#if GD32_EICU_USE_TIMER23 && !defined(__DOXYGEN__)
EICUDriver EICUD23;
#endif

/**
 * @brief   EICUD30 driver identifier.
 * @note    The driver EICUD30 allocates the timer TIMER30 when enabled.
 */
#if GD32_EICU_USE_TIMER30 && !defined(__DOXYGEN__)
EICUDriver EICUD30;
#endif

/**
 * @brief   EICUD31 driver identifier.
 * @note    The driver EICUD31 allocates the timer TIMER31 when enabled.
 */
#if GD32_EICU_USE_TIMER31 && !defined(__DOXYGEN__)
EICUDriver EICUD31;
#endif

/**
 * @brief   EICUD40 driver identifier.
 * @note    The driver EICUD40 allocates the timer TIMER40 when enabled.
 */
#if GD32_EICU_USE_TIMER40 && !defined(__DOXYGEN__)
EICUDriver EICUD40;
#endif

/**
 * @brief   EICUD41 driver identifier.
 * @note    The driver EICUD41 allocates the timer TIMER41 when enabled.
 */
#if GD32_EICU_USE_TIMER41 && !defined(__DOXYGEN__)
EICUDriver EICUD41;
#endif

/**
 * @brief   EICUD42 driver identifier.
 * @note    The driver EICUD42 allocates the timer TIMER42 when enabled.
 */
#if GD32_EICU_USE_TIMER42 && !defined(__DOXYGEN__)
EICUDriver EICUD42;
#endif

/**
 * @brief   EICUD43 driver identifier.
 * @note    The driver EICUD43 allocates the timer TIMER43 when enabled.
 */
#if GD32_EICU_USE_TIMER43 && !defined(__DOXYGEN__)
EICUDriver EICUD43;
#endif

/**
 * @brief   EICUD44 driver identifier.
 * @note    The driver EICUD44 allocates the timer TIMER44 when enabled.
 */
#if GD32_EICU_USE_TIMER44 && !defined(__DOXYGEN__)
EICUDriver EICUD44;
#endif

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] eicup     Pointer to the @p EICUDriver object
 */
void eicu_lld_serve_interrupt(EICUDriver *eicup) {
  uint16_t sr;
  sr = TIMER_INTF(eicup->tim);

  /* Pick out the interrupts we are interested in by using
     the interrupt enable bits as mask */
  sr &= (TIMER_DMAINTEN(eicup->tim) & GD32_TIMER_DMAINTEN_IRQ_MASK);

  /* Clear interrupts */
  TIMER_INTF(eicup->tim) = ~sr;

  if (((sr & TIMER_INTF_CH0IF) != 0) &&
      (eicup->channel[EICU_CHANNEL_1].config != NULL)) {
    if (eicup->channel[EICU_CHANNEL_1].config->capture_cb != NULL)
      eicup->channel[EICU_CHANNEL_1].config->capture_cb(eicup, EICU_CHANNEL_1);
  }
  if (((sr & TIMER_INTF_CH1IF) != 0) &&
      (eicup->channel[EICU_CHANNEL_2].config != NULL)) {
    if (eicup->channel[EICU_CHANNEL_2].config->capture_cb != NULL)
      eicup->channel[EICU_CHANNEL_2].config->capture_cb(eicup, EICU_CHANNEL_2);
  }
  if (((sr & TIMER_INTF_CH2IF) != 0) &&
      (eicup->channel[EICU_CHANNEL_3].config != NULL)) {
    if (eicup->channel[EICU_CHANNEL_3].config->capture_cb != NULL)
      eicup->channel[EICU_CHANNEL_3].config->capture_cb(eicup, EICU_CHANNEL_3);
  }
  if (((sr & TIMER_INTF_CH3IF) != 0) &&
      (eicup->channel[EICU_CHANNEL_4].config != NULL)) {
    if (eicup->channel[EICU_CHANNEL_4].config->capture_cb != NULL)
      eicup->channel[EICU_CHANNEL_4].config->capture_cb(eicup, EICU_CHANNEL_4);
  }
}

/**
 * @brief   Starts every channel.
 *
 * @param[in] eicup     Pointer to the @p EICUDriver object
 */
static void start_channels(EICUDriver *eicup) {

  /* Set each input channel that is used as: a normal input capture channel,
     link the corresponding CHxCV register and set polarity. */

  /* Input capture channel 1 */
  if (eicup->config->iccfgp[0] != NULL) {
    /* Normal capture input input */
    TIMER_CHCTL0(eicup->tim) |=
      GD32_TIMER_CHCTL0_CH0MS(TIMER_IC_SELECTION_DIRECTTI);

    /* Link CHxCV register */
    eicup->channel[0].ccrp = &GD32_TIMER_CHCV(eicup->tim, 0U);

    /* Set input polarity */
    if (eicup->config->iccfgp[0]->alvl == EICU_INPUT_ACTIVE_HIGH)
      TIMER_CHCTL2(eicup->tim) |= TIMER_CHCTL2_CH0EN;
    else
      TIMER_CHCTL2(eicup->tim) |= TIMER_CHCTL2_CH0EN | TIMER_CHCTL2_CH0P;
  }

  /* Input capture channel 2 */
  if (eicup->config->iccfgp[1] != NULL) {
    /* Normal capture input input */
    TIMER_CHCTL0(eicup->tim) |=
      GD32_TIMER_CHCTL0_CH1MS(TIMER_IC_SELECTION_DIRECTTI);

    /* Link CHxCV register */
    eicup->channel[1].ccrp = &GD32_TIMER_CHCV(eicup->tim, 1U);

    /* Set input polarity */
    if (eicup->config->iccfgp[1]->alvl == EICU_INPUT_ACTIVE_HIGH)
      TIMER_CHCTL2(eicup->tim) |= TIMER_CHCTL2_CH1EN;
    else
      TIMER_CHCTL2(eicup->tim) |= TIMER_CHCTL2_CH1EN | TIMER_CHCTL2_CH1P;
  }

  /* Input capture channel 3 (not for 1/2-channel instances) */
  if (eicup->config->iccfgp[2] != NULL) {
    /* Normal capture input input */
    TIMER_CHCTL1(eicup->tim) |=
      GD32_TIMER_CHCTL1_CH2MS(TIMER_IC_SELECTION_DIRECTTI);

    /* Link CHxCV register */
    eicup->channel[2].ccrp = &GD32_TIMER_CHCV(eicup->tim, 2U);
    /* Set input polarity */
    if (eicup->config->iccfgp[2]->alvl == EICU_INPUT_ACTIVE_HIGH)
      TIMER_CHCTL2(eicup->tim) |= TIMER_CHCTL2_CH2EN;
    else
      TIMER_CHCTL2(eicup->tim) |= TIMER_CHCTL2_CH2EN | TIMER_CHCTL2_CH2P;
  }

  /* Input capture channel 4 (not for 1/2-channel instances) */
  if (eicup->config->iccfgp[3] != NULL) {
    /* Normal capture input input */
    TIMER_CHCTL1(eicup->tim) |=
      GD32_TIMER_CHCTL1_CH3MS(TIMER_IC_SELECTION_DIRECTTI);

    /* Link CHxCV register */
    eicup->channel[3].ccrp = &GD32_TIMER_CHCV(eicup->tim, 3U);

    /* Set input polarity */
    if (eicup->config->iccfgp[3]->alvl == EICU_INPUT_ACTIVE_HIGH)
      TIMER_CHCTL2(eicup->tim) |= TIMER_CHCTL2_CH3EN;
    else
      TIMER_CHCTL2(eicup->tim) |= TIMER_CHCTL2_CH3EN | TIMER_CHCTL2_CH3P;
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if GD32_EICU_USE_TIMER0
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
#if !defined(GD32_TIMER0_UP_HANDLER)
#error "GD32_TIMER0_UP_HANDLER not defined"
#endif
/**
 * @brief   TIMER0 update interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER0_UP_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD0);

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

  eicu_lld_serve_interrupt(&EICUD0);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER0_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER0 */

#if GD32_EICU_USE_TIMER1
#if !defined(GD32_TIMER1_SUPPRESS_ISR)
#if !defined(GD32_TIMER1_HANDLER)
#error "GD32_TIMER1_HANDLER not defined"
#endif
/**
 * @brief   TIMER1 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD1);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER1_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER1 */

#if GD32_EICU_USE_TIMER2
#if !defined(GD32_TIMER2_SUPPRESS_ISR)
#if !defined(GD32_TIMER2_HANDLER)
#error "GD32_TIMER2_HANDLER not defined"
#endif
/**
 * @brief   TIMER2 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER2_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD2);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER2_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER2 */

#if GD32_EICU_USE_TIMER3
#if !defined(GD32_TIMER3_SUPPRESS_ISR)
#if !defined(GD32_TIMER3_HANDLER)
#error "GD32_TIMER3_HANDLER not defined"
#endif
/**
 * @brief   TIMER3 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER3_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD3);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER3_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER3 */

#if GD32_EICU_USE_TIMER4
#if !defined(GD32_TIMER4_SUPPRESS_ISR)
#if !defined(GD32_TIMER4_HANDLER)
#error "GD32_TIMER4_HANDLER not defined"
#endif
/**
 * @brief   TIMER4 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER4_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD4);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER4_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER4 */

#if GD32_EICU_USE_TIMER7
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
#if !defined(GD32_TIMER7_UP_HANDLER)
#error "GD32_TIMER7_UP_HANDLER not defined"
#endif
/**
 * @brief   TIMER7 update interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER7_UP_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD7);

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

  eicu_lld_serve_interrupt(&EICUD7);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER7_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER7 */

#if GD32_EICU_USE_TIMER14
#if !defined(GD32_TIMER14_SUPPRESS_ISR)
#if !defined(GD32_TIMER14_HANDLER)
#error "GD32_TIMER14_HANDLER not defined"
#endif
/**
 * @brief   TIMER14 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER14_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD14);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER14_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER14 */

#if GD32_EICU_USE_TIMER15
#if !defined(GD32_TIMER15_SUPPRESS_ISR)
#if !defined(GD32_TIMER15_HANDLER)
#error "GD32_TIMER15_HANDLER not defined"
#endif
/**
 * @brief   TIMER15 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER15_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD15);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER15_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER15 */

#if GD32_EICU_USE_TIMER16
#if !defined(GD32_TIMER16_SUPPRESS_ISR)
#if !defined(GD32_TIMER16_HANDLER)
#error "GD32_TIMER16_HANDLER not defined"
#endif
/**
 * @brief   TIMER16 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER16_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD16);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER16_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER16 */

#if GD32_EICU_USE_TIMER22
#if !defined(GD32_TIMER22_SUPPRESS_ISR)
#if !defined(GD32_TIMER22_HANDLER)
#error "GD32_TIMER22_HANDLER not defined"
#endif
/**
 * @brief   TIMER22 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER22_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD22);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER22_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER22 */

#if GD32_EICU_USE_TIMER23
#if !defined(GD32_TIMER23_SUPPRESS_ISR)
#if !defined(GD32_TIMER23_HANDLER)
#error "GD32_TIMER23_HANDLER not defined"
#endif
/**
 * @brief   TIMER23 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER23_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD23);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER23_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER23 */

#if GD32_EICU_USE_TIMER30
#if !defined(GD32_TIMER30_SUPPRESS_ISR)
#if !defined(GD32_TIMER30_HANDLER)
#error "GD32_TIMER30_HANDLER not defined"
#endif
/**
 * @brief   TIMER30 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER30_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD30);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER30_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER30 */

#if GD32_EICU_USE_TIMER31
#if !defined(GD32_TIMER31_SUPPRESS_ISR)
#if !defined(GD32_TIMER31_HANDLER)
#error "GD32_TIMER31_HANDLER not defined"
#endif
/**
 * @brief   TIMER31 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER31_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD31);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER31_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER31 */

#if GD32_EICU_USE_TIMER40
#if !defined(GD32_TIMER40_SUPPRESS_ISR)
#if !defined(GD32_TIMER40_HANDLER)
#error "GD32_TIMER40_HANDLER not defined"
#endif
/**
 * @brief   TIMER40 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER40_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD40);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER40_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER40 */

#if GD32_EICU_USE_TIMER41
#if !defined(GD32_TIMER41_SUPPRESS_ISR)
#if !defined(GD32_TIMER41_HANDLER)
#error "GD32_TIMER41_HANDLER not defined"
#endif
/**
 * @brief   TIMER41 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER41_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD41);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER41_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER41 */

#if GD32_EICU_USE_TIMER42
#if !defined(GD32_TIMER42_SUPPRESS_ISR)
#if !defined(GD32_TIMER42_HANDLER)
#error "GD32_TIMER42_HANDLER not defined"
#endif
/**
 * @brief   TIMER42 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER42_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD42);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER42_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER42 */

#if GD32_EICU_USE_TIMER43
#if !defined(GD32_TIMER43_SUPPRESS_ISR)
#if !defined(GD32_TIMER43_HANDLER)
#error "GD32_TIMER43_HANDLER not defined"
#endif
/**
 * @brief   TIMER43 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER43_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD43);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER43_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER43 */

#if GD32_EICU_USE_TIMER44
#if !defined(GD32_TIMER44_SUPPRESS_ISR)
#if !defined(GD32_TIMER44_HANDLER)
#error "GD32_TIMER44_HANDLER not defined"
#endif
/**
 * @brief   TIMER44 interrupt handler.
 * @note    It is assumed that the various sources are only activated if the
 *          associated callback pointer is not equal to @p NULL in order to not
 *          perform an extra check in a potentially critical interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER44_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  eicu_lld_serve_interrupt(&EICUD44);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER44_SUPPRESS_ISR) */
#endif /* GD32_EICU_USE_TIMER44 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level EICU driver initialization.
 *
 * @notapi
 */
void eicu_lld_init(void) {
#if GD32_EICU_USE_TIMER0
  /* Driver initialization.*/
  eicuObjectInit(&EICUD0);
  EICUD0.tim    = TIMER0;
  EICUD0.syscfg = SYSCFG_TIMER0;
#endif

#if GD32_EICU_USE_TIMER1
  /* Driver initialization.*/
  eicuObjectInit(&EICUD1);
  EICUD1.tim    = TIMER1;
  EICUD1.syscfg = SYSCFG_TIMER1;
#endif

#if GD32_EICU_USE_TIMER2
  /* Driver initialization.*/
  eicuObjectInit(&EICUD2);
  EICUD2.tim    = TIMER2;
  EICUD2.syscfg = SYSCFG_TIMER2;
#endif

#if GD32_EICU_USE_TIMER3
  /* Driver initialization.*/
  eicuObjectInit(&EICUD3);
  EICUD3.tim    = TIMER3;
  EICUD3.syscfg = SYSCFG_TIMER3;
#endif

#if GD32_EICU_USE_TIMER4
  /* Driver initialization.*/
  eicuObjectInit(&EICUD4);
  EICUD4.tim    = TIMER4;
  EICUD4.syscfg = SYSCFG_TIMER4;
#endif

#if GD32_EICU_USE_TIMER7
  /* Driver initialization.*/
  eicuObjectInit(&EICUD7);
  EICUD7.tim    = TIMER7;
  EICUD7.syscfg = SYSCFG_TIMER7;
#endif

#if GD32_EICU_USE_TIMER14
  /* Driver initialization.*/
  eicuObjectInit(&EICUD14);
  EICUD14.tim    = TIMER14;
  EICUD14.syscfg = SYSCFG_TIMER14;
#endif

#if GD32_EICU_USE_TIMER15
  /* Driver initialization.*/
  eicuObjectInit(&EICUD15);
  EICUD15.tim    = TIMER15;
  EICUD15.syscfg = GD32_TIMER_SYSCFG_NONE;
#endif

#if GD32_EICU_USE_TIMER16
  /* Driver initialization.*/
  eicuObjectInit(&EICUD16);
  EICUD16.tim    = TIMER16;
  EICUD16.syscfg = GD32_TIMER_SYSCFG_NONE;
#endif

#if GD32_EICU_USE_TIMER22
  /* Driver initialization.*/
  eicuObjectInit(&EICUD22);
  EICUD22.tim    = TIMER22;
  EICUD22.syscfg = SYSCFG_TIMER22;
#endif

#if GD32_EICU_USE_TIMER23
  /* Driver initialization.*/
  eicuObjectInit(&EICUD23);
  EICUD23.tim    = TIMER23;
  EICUD23.syscfg = SYSCFG_TIMER23;
#endif

#if GD32_EICU_USE_TIMER30
  /* Driver initialization.*/
  eicuObjectInit(&EICUD30);
  EICUD30.tim    = TIMER30;
  EICUD30.syscfg = SYSCFG_TIMER30;
#endif

#if GD32_EICU_USE_TIMER31
  /* Driver initialization.*/
  eicuObjectInit(&EICUD31);
  EICUD31.tim    = TIMER31;
  EICUD31.syscfg = SYSCFG_TIMER31;
#endif

#if GD32_EICU_USE_TIMER40
  /* Driver initialization.*/
  eicuObjectInit(&EICUD40);
  EICUD40.tim    = TIMER40;
  EICUD40.syscfg = SYSCFG_TIMER40;
#endif

#if GD32_EICU_USE_TIMER41
  /* Driver initialization.*/
  eicuObjectInit(&EICUD41);
  EICUD41.tim    = TIMER41;
  EICUD41.syscfg = SYSCFG_TIMER41;
#endif

#if GD32_EICU_USE_TIMER42
  /* Driver initialization.*/
  eicuObjectInit(&EICUD42);
  EICUD42.tim    = TIMER42;
  EICUD42.syscfg = SYSCFG_TIMER42;
#endif

#if GD32_EICU_USE_TIMER43
  /* Driver initialization.*/
  eicuObjectInit(&EICUD43);
  EICUD43.tim    = TIMER43;
  EICUD43.syscfg = SYSCFG_TIMER43;
#endif

#if GD32_EICU_USE_TIMER44
  /* Driver initialization.*/
  eicuObjectInit(&EICUD44);
  EICUD44.tim    = TIMER44;
  EICUD44.syscfg = SYSCFG_TIMER44;
#endif
}

/**
 * @brief   Configures and activates the EICU peripheral.
 *
 * @param[in] eicup     Pointer to the @p EICUDriver object
 *
 * @notapi
 */
void eicu_lld_start(EICUDriver *eicup) {
  uint32_t psc;
  size_t ch;

  osalDbgAssert((eicup->config->iccfgp[0] != NULL) ||
                (eicup->config->iccfgp[1] != NULL) ||
                (eicup->config->iccfgp[2] != NULL) ||
                (eicup->config->iccfgp[3] != NULL),
                 "invalid input configuration");

  if (eicup->state == EICU_STOP) {
    /* Clock activation and timer reset.*/
#if GD32_EICU_USE_TIMER0
    if (&EICUD0 == eicup) {
      rcuEnableTIMER0(false);
      rcuResetTIMER0();
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER0_UP_NUMBER, GD32_EICU_TIMER0_IRQ_PRIORITY);
      nvicEnableVector(GD32_TIMER0_CC_NUMBER, GD32_EICU_TIMER0_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER0_CHANNELS;
      eicup->clock = GD32_TIMERCLK2;
    }
#endif
#if GD32_EICU_USE_TIMER1
    if (&EICUD1 == eicup) {
      rcuEnableTIMER1(false);
      rcuResetTIMER1();
#if !defined(GD32_TIMER1_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER1_NUMBER, GD32_EICU_TIMER1_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER1_CHANNELS;
      eicup->clock = GD32_TIMERCLK1;
    }
#endif
#if GD32_EICU_USE_TIMER2
    if (&EICUD2 == eicup) {
      rcuEnableTIMER2(false);
      rcuResetTIMER2();
#if !defined(GD32_TIMER2_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER2_NUMBER, GD32_EICU_TIMER2_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER2_CHANNELS;
      eicup->clock = GD32_TIMERCLK1;
    }
#endif
#if GD32_EICU_USE_TIMER3
    if (&EICUD3 == eicup) {
      rcuEnableTIMER3(false);
      rcuResetTIMER3();
#if !defined(GD32_TIMER3_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER3_NUMBER, GD32_EICU_TIMER3_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER3_CHANNELS;
      eicup->clock = GD32_TIMERCLK1;
    }
#endif
#if GD32_EICU_USE_TIMER4
    if (&EICUD4 == eicup) {
      rcuEnableTIMER4(false);
      rcuResetTIMER4();
#if !defined(GD32_TIMER4_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER4_NUMBER, GD32_EICU_TIMER4_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER4_CHANNELS;
      eicup->clock = GD32_TIMERCLK1;
    }
#endif
#if GD32_EICU_USE_TIMER7
    if (&EICUD7 == eicup) {
      rcuEnableTIMER7(false);
      rcuResetTIMER7();
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER7_UP_NUMBER, GD32_EICU_TIMER7_IRQ_PRIORITY);
      nvicEnableVector(GD32_TIMER7_CC_NUMBER, GD32_EICU_TIMER7_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER7_CHANNELS;
      eicup->clock = GD32_TIMERCLK2;
    }
#endif
#if GD32_EICU_USE_TIMER14
    if (&EICUD14 == eicup) {
      rcuEnableTIMER14(false);
      rcuResetTIMER14();
#if !defined(GD32_TIMER14_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER14_NUMBER, GD32_EICU_TIMER14_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER14_CHANNELS;
      eicup->clock = GD32_TIMERCLK2;
    }
#endif
#if GD32_EICU_USE_TIMER15
    if (&EICUD15 == eicup) {
      rcuEnableTIMER15(false);
      rcuResetTIMER15();
#if !defined(GD32_TIMER15_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER15_NUMBER, GD32_EICU_TIMER15_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER15_CHANNELS;
      eicup->clock = GD32_TIMERCLK2;
    }
#endif
#if GD32_EICU_USE_TIMER16
    if (&EICUD16 == eicup) {
      rcuEnableTIMER16(false);
      rcuResetTIMER16();
#if !defined(GD32_TIMER16_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER16_NUMBER, GD32_EICU_TIMER16_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER16_CHANNELS;
      eicup->clock = GD32_TIMERCLK2;
    }
#endif
#if GD32_EICU_USE_TIMER22
    if (&EICUD22 == eicup) {
      rcuEnableTIMER22(false);
      rcuResetTIMER22();
#if !defined(GD32_TIMER22_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER22_NUMBER, GD32_EICU_TIMER22_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER22_CHANNELS;
      eicup->clock = GD32_TIMERCLK1;
    }
#endif
#if GD32_EICU_USE_TIMER23
    if (&EICUD23 == eicup) {
      rcuEnableTIMER23(false);
      rcuResetTIMER23();
#if !defined(GD32_TIMER23_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER23_NUMBER, GD32_EICU_TIMER23_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER23_CHANNELS;
      eicup->clock = GD32_TIMERCLK1;
    }
#endif
#if GD32_EICU_USE_TIMER30
    if (&EICUD30 == eicup) {
      rcuEnableTIMER30(false);
      rcuResetTIMER30();
#if !defined(GD32_TIMER30_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER30_NUMBER, GD32_EICU_TIMER30_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER30_CHANNELS;
      eicup->clock = GD32_TIMERCLK1;
    }
#endif
#if GD32_EICU_USE_TIMER31
    if (&EICUD31 == eicup) {
      rcuEnableTIMER31(false);
      rcuResetTIMER31();
#if !defined(GD32_TIMER31_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER31_NUMBER, GD32_EICU_TIMER31_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER31_CHANNELS;
      eicup->clock = GD32_TIMERCLK1;
    }
#endif
#if GD32_EICU_USE_TIMER40
    if (&EICUD40 == eicup) {
      rcuEnableTIMER40(false);
      rcuResetTIMER40();
#if !defined(GD32_TIMER40_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER40_NUMBER, GD32_EICU_TIMER40_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER40_CHANNELS;
      eicup->clock = GD32_TIMERCLK2;
    }
#endif
#if GD32_EICU_USE_TIMER41
    if (&EICUD41 == eicup) {
      rcuEnableTIMER41(false);
      rcuResetTIMER41();
#if !defined(GD32_TIMER41_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER41_NUMBER, GD32_EICU_TIMER41_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER41_CHANNELS;
      eicup->clock = GD32_TIMERCLK2;
    }
#endif
#if GD32_EICU_USE_TIMER42
    if (&EICUD42 == eicup) {
      rcuEnableTIMER42(false);
      rcuResetTIMER42();
#if !defined(GD32_TIMER42_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER42_NUMBER, GD32_EICU_TIMER42_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER42_CHANNELS;
      eicup->clock = GD32_TIMERCLK2;
    }
#endif
#if GD32_EICU_USE_TIMER43
    if (&EICUD43 == eicup) {
      rcuEnableTIMER43(false);
      rcuResetTIMER43();
#if !defined(GD32_TIMER43_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER43_NUMBER, GD32_EICU_TIMER43_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER43_CHANNELS;
      eicup->clock = GD32_TIMERCLK2;
    }
#endif
#if GD32_EICU_USE_TIMER44
    if (&EICUD44 == eicup) {
      rcuEnableTIMER44(false);
      rcuResetTIMER44();
#if !defined(GD32_TIMER44_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER44_NUMBER, GD32_EICU_TIMER44_IRQ_PRIORITY);
#endif
      eicup->channels = GD32_TIMER44_CHANNELS;
      eicup->clock = GD32_TIMERCLK2;
    }
#endif
  }
  else {
    /* Driver re-configuration scenario, it must be stopped first.*/
    TIMER_CTL0(eicup->tim)     = 0U;      /* Timer disabled.               */
    TIMER_CTL1(eicup->tim)     = 0U;
    TIMER_DMAINTEN(eicup->tim) = eicup->config->dmainten &
                                 /* DMA-related DMAINTEN settings.        */
                                 ~GD32_TIMER_DMAINTEN_IRQ_MASK;
    TIMER_INTF(eicup->tim)     = 0U;      /* Clear eventual pending IRQs. */
    GD32_TIMER_CHCV(eicup->tim, 0U) = 0U; /* Comparator 1 disabled.       */
    GD32_TIMER_CHCV(eicup->tim, 1U) = 0U; /* Comparator 2 disabled.       */
    GD32_TIMER_CHCV(eicup->tim, 2U) = 0U; /* Comparator 3 disabled.       */
    GD32_TIMER_CHCV(eicup->tim, 3U) = 0U; /* Comparator 4 disabled.       */
    TIMER_CNT(eicup->tim)      = 0U;      /* Counter reset to zero.       */
  }

  /* Timer configuration.*/
  psc = (eicup->clock / eicup->config->frequency) - 1U;
  chDbgAssert((psc <= 0xFFFFU) &&
             ((psc + 1U) * eicup->config->frequency) == eicup->clock,
               "invalid frequency");
  TIMER_PSC(eicup->tim) = psc;
  TIMER_CAR(eicup->tim) = (eicucnt_t)-1;

  /* Detect width.*/
  if (0xFFFFFFFFU == TIMER_CAR(eicup->tim))
    eicup->width = EICU_WIDTH_32;
  else if (0xFFFFU == TIMER_CAR(eicup->tim))
    eicup->width = EICU_WIDTH_16;
  else
    osalSysHalt("Unsupported width");

  /* Reset registers.*/
  TIMER_SMCFG(eicup->tim) = 0U;
  if (eicup->syscfg != GD32_TIMER_SYSCFG_NONE) {
    /* Slave mode configuration is outside the TIMER reset domain.*/
    SYSCFG_TIMERCFG0(eicup->syscfg) = 0U;
    SYSCFG_TIMERCFG1(eicup->syscfg) = 0U;
    SYSCFG_TIMERCFG2(eicup->syscfg) = 0U;
  }
  TIMER_CHCTL0(eicup->tim) = 0U;
  if (eicup->channels > 2U)
    TIMER_CHCTL1(eicup->tim) = 0U;

  /* clean channel structures and set pointers to channel configs */
  for (ch=0; ch<EICU_CHANNEL_ENUM_END; ch++) {
    eicup->channel[ch].last_active = 0;
    eicup->channel[ch].last_idle = 0;
    eicup->channel[ch].config = eicup->config->iccfgp[ch];
    eicup->channel[ch].state = EICU_CH_IDLE;
  }

  /* 2-channel instances have only 2 channels.*/
  if (eicup->channels == 2) {
    osalDbgCheck((eicup->config->iccfgp[2] == NULL) &&
                 (eicup->config->iccfgp[3] == NULL));
  }

  /* 1-channel instances have only 1 channel.*/
  if (eicup->channels == 1) {
    osalDbgCheck((eicup->config->iccfgp[1] == NULL) &&
                 (eicup->config->iccfgp[2] == NULL) &&
                 (eicup->config->iccfgp[3] == NULL));
  }

  start_channels(eicup);
}

/**
 * @brief   Deactivates the EICU peripheral.
 *
 * @param[in] eicup     Pointer to the @p EICUDriver object
 *
 * @notapi
 */
void eicu_lld_stop(EICUDriver *eicup) {

  if (eicup->state == EICU_READY) {

    /* Clock deactivation.*/
    TIMER_CTL0(eicup->tim)     = 0U;   /* Timer disabled.              */
    TIMER_DMAINTEN(eicup->tim) = 0U;   /* All IRQs disabled.           */
    TIMER_INTF(eicup->tim)     = 0U;   /* Clear eventual pending IRQs. */

#if GD32_EICU_USE_TIMER0
    if (&EICUD0 == eicup) {
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER0_UP_NUMBER);
#endif
      nvicDisableVector(GD32_TIMER0_CC_NUMBER);
      rcuDisableTIMER0();
    }
#endif
#if GD32_EICU_USE_TIMER1
    if (&EICUD1 == eicup) {
#if !defined(GD32_TIMER1_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER1_NUMBER);
#endif
      rcuDisableTIMER1();
    }
#endif
#if GD32_EICU_USE_TIMER2
    if (&EICUD2 == eicup) {
#if !defined(GD32_TIMER2_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER2_NUMBER);
#endif
      rcuDisableTIMER2();
    }
#endif
#if GD32_EICU_USE_TIMER3
    if (&EICUD3 == eicup) {
#if !defined(GD32_TIMER3_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER3_NUMBER);
#endif
      rcuDisableTIMER3();
    }
#endif
#if GD32_EICU_USE_TIMER4
    if (&EICUD4 == eicup) {
#if !defined(GD32_TIMER4_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER4_NUMBER);
#endif
      rcuDisableTIMER4();
    }
#endif
#if GD32_EICU_USE_TIMER7
    if (&EICUD7 == eicup) {
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER7_UP_NUMBER);
      nvicDisableVector(GD32_TIMER7_CC_NUMBER);
#endif
      rcuDisableTIMER7();
    }
#endif
#if GD32_EICU_USE_TIMER14
    if (&EICUD14 == eicup) {
#if !defined(GD32_TIMER14_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER14_NUMBER);
#endif
      rcuDisableTIMER14();
    }
#endif
#if GD32_EICU_USE_TIMER22
    if (&EICUD22 == eicup) {
#if !defined(GD32_TIMER22_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER22_NUMBER);
#endif
      rcuDisableTIMER22();
    }
#endif
#if GD32_EICU_USE_TIMER23
    if (&EICUD23 == eicup) {
#if !defined(GD32_TIMER23_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER23_NUMBER);
#endif
      rcuDisableTIMER23();
    }
#endif
#if GD32_EICU_USE_TIMER30
    if (&EICUD30 == eicup) {
#if !defined(GD32_TIMER30_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER30_NUMBER);
#endif
      rcuDisableTIMER30();
    }
#endif
#if GD32_EICU_USE_TIMER31
    if (&EICUD31 == eicup) {
#if !defined(GD32_TIMER31_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER31_NUMBER);
#endif
      rcuDisableTIMER31();
    }
#endif
#if GD32_EICU_USE_TIMER40
    if (&EICUD40 == eicup) {
#if !defined(GD32_TIMER40_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER40_NUMBER);
#endif
      rcuDisableTIMER40();
    }
#endif
#if GD32_EICU_USE_TIMER41
    if (&EICUD41 == eicup) {
#if !defined(GD32_TIMER41_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER41_NUMBER);
#endif
      rcuDisableTIMER41();
    }
#endif
#if GD32_EICU_USE_TIMER42
    if (&EICUD42 == eicup) {
#if !defined(GD32_TIMER42_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER42_NUMBER);
#endif
      rcuDisableTIMER42();
    }
#endif
#if GD32_EICU_USE_TIMER43
    if (&EICUD43 == eicup) {
#if !defined(GD32_TIMER43_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER43_NUMBER);
#endif
      rcuDisableTIMER43();
    }
#endif
#if GD32_EICU_USE_TIMER44
    if (&EICUD44 == eicup) {
#if !defined(GD32_TIMER44_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER44_NUMBER);
#endif
      rcuDisableTIMER44();
    }
#endif
  }
#if GD32_EICU_USE_TIMER15
    if (&EICUD15 == eicup) {
#if !defined(GD32_TIMER15_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER15_NUMBER);
#endif
      rcuDisableTIMER15();
    }
#endif
#if GD32_EICU_USE_TIMER16
    if (&EICUD16 == eicup) {
#if !defined(GD32_TIMER16_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER16_NUMBER);
#endif
      rcuDisableTIMER16();
    }
#endif
}

/**
 * @brief   Enables the EICU.
 *
 * @param[in] eicup     Pointer to the @p EICUDriver object
 *
 * @notapi
 */
void eicu_lld_enable(EICUDriver *eicup) {

  TIMER_SWEVG(eicup->tim) = TIMER_SWEVG_UPG;
  TIMER_INTF(eicup->tim) = 0U;                /* Clear pending IRQs (if any). */

  if ((eicup->config->iccfgp[EICU_CHANNEL_1] != NULL) &&
      (eicup->config->iccfgp[EICU_CHANNEL_1]->capture_cb != NULL))
    TIMER_DMAINTEN(eicup->tim) |= TIMER_DMAINTEN_CH0IE;
  if ((eicup->config->iccfgp[EICU_CHANNEL_2] != NULL) &&
      (eicup->config->iccfgp[EICU_CHANNEL_2]->capture_cb != NULL))
    TIMER_DMAINTEN(eicup->tim) |= TIMER_DMAINTEN_CH1IE;
  if ((eicup->config->iccfgp[EICU_CHANNEL_3] != NULL) &&
      (eicup->config->iccfgp[EICU_CHANNEL_3]->capture_cb != NULL))
    TIMER_DMAINTEN(eicup->tim) |= TIMER_DMAINTEN_CH2IE;
  if ((eicup->config->iccfgp[EICU_CHANNEL_4] != NULL) &&
      (eicup->config->iccfgp[EICU_CHANNEL_4]->capture_cb != NULL))
    TIMER_DMAINTEN(eicup->tim) |= TIMER_DMAINTEN_CH3IE;

  TIMER_CTL0(eicup->tim) = TIMER_CTL0_CEN;
}

/**
 * @brief   Disables the EICU.
 *
 * @param[in] eicup     Pointer to the @p EICUDriver object
 *
 * @notapi
 */
void eicu_lld_disable(EICUDriver *eicup) {
  TIMER_CTL0(eicup->tim) = 0U;                /* Initially stopped.           */
  TIMER_INTF(eicup->tim) = 0U;                /* Clear pending IRQs (if any). */

  /* All interrupts disabled.*/
  TIMER_DMAINTEN(eicup->tim) &= ~GD32_TIMER_DMAINTEN_IRQ_MASK;
}

#endif /* HAL_USE_EICU */

/** @} */
