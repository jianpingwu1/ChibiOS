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
 * @file    TIMERv1/hal_gpt_lld.c
 * @brief   GD32 GPT subsystem low level driver source.
 *
 * @addtogroup GPT
 * @{
 */

#include "hal.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   GPTD0 driver identifier.
 * @note    The driver GPTD0 allocates the complex timer TIMER0 when enabled.
 */
#if GD32_GPT_USE_TIMER0 || defined(__DOXYGEN__)
GPTDriver GPTD0;
#endif

/**
 * @brief   GPTD1 driver identifier.
 * @note    The driver GPTD1 allocates the timer TIMER1 when enabled.
 */
#if GD32_GPT_USE_TIMER1 || defined(__DOXYGEN__)
GPTDriver GPTD1;
#endif

/**
 * @brief   GPTD2 driver identifier.
 * @note    The driver GPTD2 allocates the timer TIMER2 when enabled.
 */
#if GD32_GPT_USE_TIMER2 || defined(__DOXYGEN__)
GPTDriver GPTD2;
#endif

/**
 * @brief   GPTD3 driver identifier.
 * @note    The driver GPTD3 allocates the timer TIMER3 when enabled.
 */
#if GD32_GPT_USE_TIMER3 || defined(__DOXYGEN__)
GPTDriver GPTD3;
#endif

/**
 * @brief   GPTD4 driver identifier.
 * @note    The driver GPTD4 allocates the timer TIMER4 when enabled.
 */
#if GD32_GPT_USE_TIMER4 || defined(__DOXYGEN__)
GPTDriver GPTD4;
#endif

/**
 * @brief   GPTD5 driver identifier.
 * @note    The driver GPTD5 allocates the timer TIMER5 when enabled.
 */
#if GD32_GPT_USE_TIMER5 || defined(__DOXYGEN__)
GPTDriver GPTD5;
#endif

/**
 * @brief   GPTD6 driver identifier.
 * @note    The driver GPTD6 allocates the timer TIMER6 when enabled.
 */
#if GD32_GPT_USE_TIMER6 || defined(__DOXYGEN__)
GPTDriver GPTD6;
#endif

/**
 * @brief   GPTD7 driver identifier.
 * @note    The driver GPTD7 allocates the complex timer TIMER7 when enabled.
 */
#if GD32_GPT_USE_TIMER7 || defined(__DOXYGEN__)
GPTDriver GPTD7;
#endif

/**
 * @brief   GPTD14 driver identifier.
 * @note    The driver GPTD14 allocates the timer TIMER14 when enabled.
 */
#if GD32_GPT_USE_TIMER14 || defined(__DOXYGEN__)
GPTDriver GPTD14;
#endif

/**
 * @brief   GPTD15 driver identifier.
 * @note    The driver GPTD15 allocates the timer TIMER15 when enabled.
 */
#if GD32_GPT_USE_TIMER15 || defined(__DOXYGEN__)
GPTDriver GPTD15;
#endif

/**
 * @brief   GPTD16 driver identifier.
 * @note    The driver GPTD16 allocates the timer TIMER16 when enabled.
 */
#if GD32_GPT_USE_TIMER16 || defined(__DOXYGEN__)
GPTDriver GPTD16;
#endif

/**
 * @brief   GPTD22 driver identifier.
 * @note    The driver GPTD22 allocates the timer TIMER22 when enabled.
 */
#if GD32_GPT_USE_TIMER22 || defined(__DOXYGEN__)
GPTDriver GPTD22;
#endif

/**
 * @brief   GPTD23 driver identifier.
 * @note    The driver GPTD23 allocates the timer TIMER23 when enabled.
 */
#if GD32_GPT_USE_TIMER23 || defined(__DOXYGEN__)
GPTDriver GPTD23;
#endif

/**
 * @brief   GPTD30 driver identifier.
 * @note    The driver GPTD30 allocates the timer TIMER30 when enabled.
 */
#if GD32_GPT_USE_TIMER30 || defined(__DOXYGEN__)
GPTDriver GPTD30;
#endif

/**
 * @brief   GPTD31 driver identifier.
 * @note    The driver GPTD31 allocates the timer TIMER31 when enabled.
 */
#if GD32_GPT_USE_TIMER31 || defined(__DOXYGEN__)
GPTDriver GPTD31;
#endif

/**
 * @brief   GPTD40 driver identifier.
 * @note    The driver GPTD40 allocates the timer TIMER40 when enabled.
 */
#if GD32_GPT_USE_TIMER40 || defined(__DOXYGEN__)
GPTDriver GPTD40;
#endif

/**
 * @brief   GPTD41 driver identifier.
 * @note    The driver GPTD41 allocates the timer TIMER41 when enabled.
 */
#if GD32_GPT_USE_TIMER41 || defined(__DOXYGEN__)
GPTDriver GPTD41;
#endif

/**
 * @brief   GPTD42 driver identifier.
 * @note    The driver GPTD42 allocates the timer TIMER42 when enabled.
 */
#if GD32_GPT_USE_TIMER42 || defined(__DOXYGEN__)
GPTDriver GPTD42;
#endif

/**
 * @brief   GPTD43 driver identifier.
 * @note    The driver GPTD43 allocates the timer TIMER43 when enabled.
 */
#if GD32_GPT_USE_TIMER43 || defined(__DOXYGEN__)
GPTDriver GPTD43;
#endif

/**
 * @brief   GPTD44 driver identifier.
 * @note    The driver GPTD44 allocates the timer TIMER44 when enabled.
 */
#if GD32_GPT_USE_TIMER44 || defined(__DOXYGEN__)
GPTDriver GPTD44;
#endif

/**
 * @brief   GPTD50 driver identifier.
 * @note    The driver GPTD50 allocates the timer TIMER50 when enabled.
 */
#if GD32_GPT_USE_TIMER50 || defined(__DOXYGEN__)
GPTDriver GPTD50;
#endif

/**
 * @brief   GPTD51 driver identifier.
 * @note    The driver GPTD51 allocates the timer TIMER51 when enabled.
 */
#if GD32_GPT_USE_TIMER51 || defined(__DOXYGEN__)
GPTDriver GPTD51;
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

#if GD32_GPT_USE_TIMER0 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
#if !defined(GD32_TIMER0_UP_HANDLER)
#error "GD32_TIMER0_UP_HANDLER not defined"
#endif
/**
 * @brief   TIMER0 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER0_UP_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD0);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER0_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER0 */

#if GD32_GPT_USE_TIMER1 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD1);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER1_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER1 */

#if GD32_GPT_USE_TIMER2 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD2);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER2_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER2 */

#if GD32_GPT_USE_TIMER3 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD3);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER3_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER3 */

#if GD32_GPT_USE_TIMER4 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD4);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER4_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER4 */

#if GD32_GPT_USE_TIMER5 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER5_SUPPRESS_ISR)
#if !defined(GD32_TIMER5_DAC_UDR_HANDLER)
#error "GD32_TIMER5_DAC_UDR_HANDLER not defined"
#endif
/**
 * @brief   TIMER5/DAC underrun interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER5_DAC_UDR_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD5);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER5_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER5 */

#if GD32_GPT_USE_TIMER6 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER6_SUPPRESS_ISR)
#if !defined(GD32_TIMER6_HANDLER)
#error "GD32_TIMER6_HANDLER not defined"
#endif
/**
 * @brief   TIMER6 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER6_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD6);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER6_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER6 */

#if GD32_GPT_USE_TIMER7 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
#if !defined(GD32_TIMER7_UP_HANDLER)
#error "GD32_TIMER7_UP_HANDLER not defined"
#endif
/**
 * @brief   TIMER7 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER7_UP_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD7);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER7_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER7 */

#if GD32_GPT_USE_TIMER14 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD14);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER14_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER14 */

#if GD32_GPT_USE_TIMER15 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD15);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER15_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER15 */

#if GD32_GPT_USE_TIMER16 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD16);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER16_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER16 */

#if GD32_GPT_USE_TIMER22 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD22);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER22_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER22 */

#if GD32_GPT_USE_TIMER23 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD23);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER23_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER23 */

#if GD32_GPT_USE_TIMER30 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD30);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER30_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER30 */

#if GD32_GPT_USE_TIMER31 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD31);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER31_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER31 */

#if GD32_GPT_USE_TIMER40 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD40);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER40_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER40 */

#if GD32_GPT_USE_TIMER41 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD41);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER41_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER41 */

#if GD32_GPT_USE_TIMER42 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD42);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER42_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER42 */

#if GD32_GPT_USE_TIMER43 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD43);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER43_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER43 */

#if GD32_GPT_USE_TIMER44 || defined(__DOXYGEN__)
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

  gpt_lld_serve_interrupt(&GPTD44);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER44_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER44 */

#if GD32_GPT_USE_TIMER50 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER50_SUPPRESS_ISR)
#if !defined(GD32_TIMER50_HANDLER)
#error "GD32_TIMER50_HANDLER not defined"
#endif
/**
 * @brief   TIMER50 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER50_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD50);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER50_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER50 */

#if GD32_GPT_USE_TIMER51 || defined(__DOXYGEN__)
#if !defined(GD32_TIMER51_SUPPRESS_ISR)
#if !defined(GD32_TIMER51_HANDLER)
#error "GD32_TIMER51_HANDLER not defined"
#endif
/**
 * @brief   TIMER51 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_TIMER51_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  gpt_lld_serve_interrupt(&GPTD51);

  OSAL_IRQ_EPILOGUE();
}
#endif /* !defined(GD32_TIMER51_SUPPRESS_ISR) */
#endif /* GD32_GPT_USE_TIMER51 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level GPT driver initialization.
 *
 * @notapi
 */
void gpt_lld_init(void) {

#if GD32_GPT_USE_TIMER0
  /* Driver initialization.*/
  GPTD0.tim = TIMER0;
  gptObjectInit(&GPTD0);
#endif

#if GD32_GPT_USE_TIMER1
  /* Driver initialization.*/
  GPTD1.tim = TIMER1;
  gptObjectInit(&GPTD1);
#endif

#if GD32_GPT_USE_TIMER2
  /* Driver initialization.*/
  GPTD2.tim = TIMER2;
  gptObjectInit(&GPTD2);
#endif

#if GD32_GPT_USE_TIMER3
  /* Driver initialization.*/
  GPTD3.tim = TIMER3;
  gptObjectInit(&GPTD3);
#endif

#if GD32_GPT_USE_TIMER4
  /* Driver initialization.*/
  GPTD4.tim = TIMER4;
  gptObjectInit(&GPTD4);
#endif

#if GD32_GPT_USE_TIMER5
  /* Driver initialization.*/
  GPTD5.tim = TIMER5;
  gptObjectInit(&GPTD5);
#endif

#if GD32_GPT_USE_TIMER6
  /* Driver initialization.*/
  GPTD6.tim = TIMER6;
  gptObjectInit(&GPTD6);
#endif

#if GD32_GPT_USE_TIMER7
  /* Driver initialization.*/
  GPTD7.tim = TIMER7;
  gptObjectInit(&GPTD7);
#endif

#if GD32_GPT_USE_TIMER14
  /* Driver initialization.*/
  GPTD14.tim = TIMER14;
  gptObjectInit(&GPTD14);
#endif

#if GD32_GPT_USE_TIMER15
  /* Driver initialization.*/
  GPTD15.tim = TIMER15;
  gptObjectInit(&GPTD15);
#endif

#if GD32_GPT_USE_TIMER16
  /* Driver initialization.*/
  GPTD16.tim = TIMER16;
  gptObjectInit(&GPTD16);
#endif

#if GD32_GPT_USE_TIMER22
  /* Driver initialization.*/
  GPTD22.tim = TIMER22;
  gptObjectInit(&GPTD22);
#endif

#if GD32_GPT_USE_TIMER23
  /* Driver initialization.*/
  GPTD23.tim = TIMER23;
  gptObjectInit(&GPTD23);
#endif

#if GD32_GPT_USE_TIMER30
  /* Driver initialization.*/
  GPTD30.tim = TIMER30;
  gptObjectInit(&GPTD30);
#endif

#if GD32_GPT_USE_TIMER31
  /* Driver initialization.*/
  GPTD31.tim = TIMER31;
  gptObjectInit(&GPTD31);
#endif

#if GD32_GPT_USE_TIMER40
  /* Driver initialization.*/
  GPTD40.tim = TIMER40;
  gptObjectInit(&GPTD40);
#endif

#if GD32_GPT_USE_TIMER41
  /* Driver initialization.*/
  GPTD41.tim = TIMER41;
  gptObjectInit(&GPTD41);
#endif

#if GD32_GPT_USE_TIMER42
  /* Driver initialization.*/
  GPTD42.tim = TIMER42;
  gptObjectInit(&GPTD42);
#endif

#if GD32_GPT_USE_TIMER43
  /* Driver initialization.*/
  GPTD43.tim = TIMER43;
  gptObjectInit(&GPTD43);
#endif

#if GD32_GPT_USE_TIMER44
  /* Driver initialization.*/
  GPTD44.tim = TIMER44;
  gptObjectInit(&GPTD44);
#endif

#if GD32_GPT_USE_TIMER50
  /* Driver initialization.*/
  GPTD50.tim = TIMER50;
  gptObjectInit(&GPTD50);
#endif

#if GD32_GPT_USE_TIMER51
  /* Driver initialization.*/
  GPTD51.tim = TIMER51;
  gptObjectInit(&GPTD51);
#endif
}

/**
 * @brief   Configures and activates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_start(GPTDriver *gptp) {
  uint16_t psc;

  if (gptp->state == GPT_STOP) {
    /* Clock activation.*/
#if GD32_GPT_USE_TIMER0
    if (&GPTD0 == gptp) {
      rcuEnableTIMER0(true);
      rcuResetTIMER0();
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER0_UP_NUMBER, GD32_GPT_TIMER0_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_GPT_USE_TIMER1
    if (&GPTD1 == gptp) {
      rcuEnableTIMER1(true);
      rcuResetTIMER1();
#if !defined(GD32_TIMER1_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER1_NUMBER, GD32_GPT_TIMER1_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER2
    if (&GPTD2 == gptp) {
      rcuEnableTIMER2(true);
      rcuResetTIMER2();
#if !defined(GD32_TIMER2_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER2_NUMBER, GD32_GPT_TIMER2_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER3
    if (&GPTD3 == gptp) {
      rcuEnableTIMER3(true);
      rcuResetTIMER3();
#if !defined(GD32_TIMER3_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER3_NUMBER, GD32_GPT_TIMER3_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER4
    if (&GPTD4 == gptp) {
      rcuEnableTIMER4(true);
      rcuResetTIMER4();
#if !defined(GD32_TIMER4_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER4_NUMBER, GD32_GPT_TIMER4_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER5
    if (&GPTD5 == gptp) {
      rcuEnableTIMER5(true);
      rcuResetTIMER5();
#if !defined(GD32_TIMER5_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER5_DAC_UDR_NUMBER, GD32_GPT_TIMER5_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER6
    if (&GPTD6 == gptp) {
      rcuEnableTIMER6(true);
      rcuResetTIMER6();
#if !defined(GD32_TIMER6_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER6_NUMBER, GD32_GPT_TIMER6_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER7
    if (&GPTD7 == gptp) {
      rcuEnableTIMER7(true);
      rcuResetTIMER7();
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER7_UP_NUMBER, GD32_GPT_TIMER7_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_GPT_USE_TIMER14
    if (&GPTD14 == gptp) {
      rcuEnableTIMER14(true);
      rcuResetTIMER14();
#if !defined(GD32_TIMER14_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER14_NUMBER, GD32_GPT_TIMER14_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_GPT_USE_TIMER15
    if (&GPTD15 == gptp) {
      rcuEnableTIMER15(true);
      rcuResetTIMER15();
#if !defined(GD32_TIMER15_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER15_NUMBER, GD32_GPT_TIMER15_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_GPT_USE_TIMER16
    if (&GPTD16 == gptp) {
      rcuEnableTIMER16(true);
      rcuResetTIMER16();
#if !defined(GD32_TIMER16_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER16_NUMBER, GD32_GPT_TIMER16_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_GPT_USE_TIMER22
    if (&GPTD22 == gptp) {
      rcuEnableTIMER22(true);
      rcuResetTIMER22();
#if !defined(GD32_TIMER22_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER22_NUMBER, GD32_GPT_TIMER22_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER23
    if (&GPTD23 == gptp) {
      rcuEnableTIMER23(true);
      rcuResetTIMER23();
#if !defined(GD32_TIMER23_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER23_NUMBER, GD32_GPT_TIMER23_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER30
    if (&GPTD30 == gptp) {
      rcuEnableTIMER30(true);
      rcuResetTIMER30();
#if !defined(GD32_TIMER30_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER30_NUMBER, GD32_GPT_TIMER30_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER31
    if (&GPTD31 == gptp) {
      rcuEnableTIMER31(true);
      rcuResetTIMER31();
#if !defined(GD32_TIMER31_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER31_NUMBER, GD32_GPT_TIMER31_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER40
    if (&GPTD40 == gptp) {
      rcuEnableTIMER40(true);
      rcuResetTIMER40();
#if !defined(GD32_TIMER40_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER40_NUMBER, GD32_GPT_TIMER40_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_GPT_USE_TIMER41
    if (&GPTD41 == gptp) {
      rcuEnableTIMER41(true);
      rcuResetTIMER41();
#if !defined(GD32_TIMER41_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER41_NUMBER, GD32_GPT_TIMER41_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_GPT_USE_TIMER42
    if (&GPTD42 == gptp) {
      rcuEnableTIMER42(true);
      rcuResetTIMER42();
#if !defined(GD32_TIMER42_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER42_NUMBER, GD32_GPT_TIMER42_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_GPT_USE_TIMER43
    if (&GPTD43 == gptp) {
      rcuEnableTIMER43(true);
      rcuResetTIMER43();
#if !defined(GD32_TIMER43_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER43_NUMBER, GD32_GPT_TIMER43_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_GPT_USE_TIMER44
    if (&GPTD44 == gptp) {
      rcuEnableTIMER44(true);
      rcuResetTIMER44();
#if !defined(GD32_TIMER44_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER44_NUMBER, GD32_GPT_TIMER44_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK2;
    }
#endif

#if GD32_GPT_USE_TIMER50
    if (&GPTD50 == gptp) {
      rcuEnableTIMER50(true);
      rcuResetTIMER50();
#if !defined(GD32_TIMER50_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER50_NUMBER, GD32_GPT_TIMER50_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif

#if GD32_GPT_USE_TIMER51
    if (&GPTD51 == gptp) {
      rcuEnableTIMER51(true);
      rcuResetTIMER51();
#if !defined(GD32_TIMER51_SUPPRESS_ISR)
      nvicEnableVector(GD32_TIMER51_NUMBER, GD32_GPT_TIMER51_IRQ_PRIORITY);
#endif
      gptp->clock = GD32_TIMERCLK1;
    }
#endif
  }

  /* Prescaler value calculation.*/
  psc = (uint16_t)((gptp->clock / gptp->config->frequency) - 1);
  osalDbgAssert(((uint32_t)(psc + 1) * gptp->config->frequency) == gptp->clock,
                "invalid frequency");

  /* Timer configuration.*/
  TIMER_CTL0(gptp->tim)     = 0U;              /* Initially stopped.       */
  TIMER_CTL1(gptp->tim)     = gptp->config->ctl1;
  TIMER_PSC(gptp->tim)      = psc;             /* Prescaler value.         */
  TIMER_INTF(gptp->tim)     = 0U;              /* Clear pending IRQs.      */
  TIMER_DMAINTEN(gptp->tim) = gptp->config->dmainten & /* DMA-related bits.*/
                              ~GD32_TIMER_DMAINTEN_IRQ_MASK;
}

/**
 * @brief   Deactivates the GPT peripheral.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop(GPTDriver *gptp) {

  if (gptp->state == GPT_READY) {
    TIMER_CTL0(gptp->tim)     = 0U;             /* Timer disabled.          */
    TIMER_DMAINTEN(gptp->tim) = 0U;             /* All IRQs disabled.       */
    TIMER_INTF(gptp->tim)     = 0U;             /* Clear pending IRQs.      */

#if GD32_GPT_USE_TIMER0
    if (&GPTD0 == gptp) {
#if !defined(GD32_TIMER0_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER0_UP_NUMBER);
#endif
      rcuDisableTIMER0();
    }
#endif

#if GD32_GPT_USE_TIMER1
    if (&GPTD1 == gptp) {
#if !defined(GD32_TIMER1_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER1_NUMBER);
#endif
      rcuDisableTIMER1();
    }
#endif

#if GD32_GPT_USE_TIMER2
    if (&GPTD2 == gptp) {
#if !defined(GD32_TIMER2_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER2_NUMBER);
#endif
      rcuDisableTIMER2();
    }
#endif

#if GD32_GPT_USE_TIMER3
    if (&GPTD3 == gptp) {
#if !defined(GD32_TIMER3_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER3_NUMBER);
#endif
      rcuDisableTIMER3();
    }
#endif

#if GD32_GPT_USE_TIMER4
    if (&GPTD4 == gptp) {
#if !defined(GD32_TIMER4_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER4_NUMBER);
#endif
      rcuDisableTIMER4();
    }
#endif

#if GD32_GPT_USE_TIMER5
    if (&GPTD5 == gptp) {
#if !defined(GD32_TIMER5_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER5_DAC_UDR_NUMBER);
#endif
      rcuDisableTIMER5();
    }
#endif

#if GD32_GPT_USE_TIMER6
    if (&GPTD6 == gptp) {
#if !defined(GD32_TIMER6_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER6_NUMBER);
#endif
      rcuDisableTIMER6();
    }
#endif

#if GD32_GPT_USE_TIMER7
    if (&GPTD7 == gptp) {
#if !defined(GD32_TIMER7_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER7_UP_NUMBER);
#endif
      rcuDisableTIMER7();
    }
#endif

#if GD32_GPT_USE_TIMER14
    if (&GPTD14 == gptp) {
#if !defined(GD32_TIMER14_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER14_NUMBER);
#endif
      rcuDisableTIMER14();
    }
#endif

#if GD32_GPT_USE_TIMER15
    if (&GPTD15 == gptp) {
#if !defined(GD32_TIMER15_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER15_NUMBER);
#endif
      rcuDisableTIMER15();
    }
#endif

#if GD32_GPT_USE_TIMER16
    if (&GPTD16 == gptp) {
#if !defined(GD32_TIMER16_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER16_NUMBER);
#endif
      rcuDisableTIMER16();
    }
#endif

#if GD32_GPT_USE_TIMER22
    if (&GPTD22 == gptp) {
#if !defined(GD32_TIMER22_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER22_NUMBER);
#endif
      rcuDisableTIMER22();
    }
#endif

#if GD32_GPT_USE_TIMER23
    if (&GPTD23 == gptp) {
#if !defined(GD32_TIMER23_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER23_NUMBER);
#endif
      rcuDisableTIMER23();
    }
#endif

#if GD32_GPT_USE_TIMER30
    if (&GPTD30 == gptp) {
#if !defined(GD32_TIMER30_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER30_NUMBER);
#endif
      rcuDisableTIMER30();
    }
#endif

#if GD32_GPT_USE_TIMER31
    if (&GPTD31 == gptp) {
#if !defined(GD32_TIMER31_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER31_NUMBER);
#endif
      rcuDisableTIMER31();
    }
#endif

#if GD32_GPT_USE_TIMER40
    if (&GPTD40 == gptp) {
#if !defined(GD32_TIMER40_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER40_NUMBER);
#endif
      rcuDisableTIMER40();
    }
#endif

#if GD32_GPT_USE_TIMER41
    if (&GPTD41 == gptp) {
#if !defined(GD32_TIMER41_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER41_NUMBER);
#endif
      rcuDisableTIMER41();
    }
#endif

#if GD32_GPT_USE_TIMER42
    if (&GPTD42 == gptp) {
#if !defined(GD32_TIMER42_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER42_NUMBER);
#endif
      rcuDisableTIMER42();
    }
#endif

#if GD32_GPT_USE_TIMER43
    if (&GPTD43 == gptp) {
#if !defined(GD32_TIMER43_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER43_NUMBER);
#endif
      rcuDisableTIMER43();
    }
#endif

#if GD32_GPT_USE_TIMER44
    if (&GPTD44 == gptp) {
#if !defined(GD32_TIMER44_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER44_NUMBER);
#endif
      rcuDisableTIMER44();
    }
#endif

#if GD32_GPT_USE_TIMER50
    if (&GPTD50 == gptp) {
#if !defined(GD32_TIMER50_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER50_NUMBER);
#endif
      rcuDisableTIMER50();
    }
#endif

#if GD32_GPT_USE_TIMER51
    if (&GPTD51 == gptp) {
#if !defined(GD32_TIMER51_SUPPRESS_ISR)
      nvicDisableVector(GD32_TIMER51_NUMBER);
#endif
      rcuDisableTIMER51();
    }
#endif
  }
}

/**
 * @brief   Starts the timer in continuous mode.
 * @note    Interval values 0 and 1 are invalid on this architecture.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  period in ticks
 *
 * @notapi
 */
void gpt_lld_start_timer(GPTDriver *gptp, gptcnt_t interval) {

  osalDbgAssert(interval > (gptcnt_t)0, "invalid interval");

  TIMER_CAR(gptp->tim) = (uint32_t)(interval - 1U); /* Time constant.       */
  TIMER_SWEVG(gptp->tim) = TIMER_SWEVG_UPG;         /* Update event.        */
  TIMER_CNT(gptp->tim) = 0U;                        /* Reset counter.       */

  /* NOTE: After generating the UPG event it takes several clock cycles
     before INTF bit 0 goes to 1. This is why the clearing of CNT has been
     inserted before the clearing of INTF, to give it some time.*/
  TIMER_INTF(gptp->tim) = 0U;                       /* Clear pending IRQs.  */
  if (NULL != gptp->config->callback)
    TIMER_DMAINTEN(gptp->tim) |= TIMER_DMAINTEN_UPIE; /* Update IRQ enabled.*/
  TIMER_CTL0(gptp->tim) = TIMER_CTL0_ARSE | TIMER_CTL0_UPS | TIMER_CTL0_CEN;
}

/**
 * @brief   Stops the timer.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_stop_timer(GPTDriver *gptp) {

  TIMER_CTL0(gptp->tim) = 0U;                 /* Initially stopped.       */
  TIMER_INTF(gptp->tim) = 0U;                 /* Clear pending IRQs.      */

  /* All interrupts disabled.*/
  TIMER_DMAINTEN(gptp->tim) &= ~GD32_TIMER_DMAINTEN_IRQ_MASK;
}

/**
 * @brief   Starts the timer in one shot mode and waits for completion.
 * @details This function specifically polls the timer waiting for completion
 *          in order to not have extra delays caused by interrupt servicing,
 *          this function is only recommended for short delays.
 * @note    Interval values 0 and 1 are invalid on this architecture.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @param[in] interval  time interval in ticks
 *
 * @notapi
 */
void gpt_lld_polled_delay(GPTDriver *gptp, gptcnt_t interval) {

  osalDbgAssert(interval > (gptcnt_t)0, "invalid interval");

  TIMER_CTL0(gptp->tim) = TIMER_CTL0_UPDIS;         /* Immediate update.    */
  TIMER_CAR(gptp->tim) = (uint32_t)(interval - 1U); /* Time constant.       */
  TIMER_SWEVG(gptp->tim) = TIMER_SWEVG_UPG;         /* Update event.        */
  TIMER_INTF(gptp->tim) = 0U;                       /* Clear pending IRQs.  */
  TIMER_CTL0(gptp->tim) = TIMER_CTL0_SPM | TIMER_CTL0_UPS | TIMER_CTL0_CEN;
  while (!(TIMER_INTF(gptp->tim) & TIMER_INTF_UPIF))
    ;
  TIMER_INTF(gptp->tim) = 0U;                       /* Clear pending IRQs.  */
}

/**
 * @brief   Shared IRQ handler.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 *
 * @notapi
 */
void gpt_lld_serve_interrupt(GPTDriver *gptp) {
  uint32_t sr;

  sr  = TIMER_INTF(gptp->tim);
  sr &= TIMER_DMAINTEN(gptp->tim) & GD32_TIMER_DMAINTEN_IRQ_MASK;
  TIMER_INTF(gptp->tim) = ~sr;
  if ((sr & TIMER_INTF_UPIF) != 0) {
    _gpt_isr_invoke_cb(gptp);
  }
}

#endif /* HAL_USE_GPT */

/** @} */
