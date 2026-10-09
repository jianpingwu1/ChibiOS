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
 * @file    SYSTICKv1/hal_st_lld.c
 * @brief   ST Driver subsystem low level driver code.
 * @note    Supports both @p OSAL_ST_MODE_PERIODIC (Cortex-M core SysTick)
 *          and @p OSAL_ST_MODE_FREERUNNING (GD32 TIMER4).
 *
 * @addtogroup ST
 * @{
 */

#include "hal.h"

#if (OSAL_ST_MODE != OSAL_ST_MODE_NONE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING

#if (OSAL_ST_RESOLUTION != 16) && (OSAL_ST_RESOLUTION != 32)
#error "unsupported ST resolution"
#endif

#if (OSAL_ST_RESOLUTION == 32)
#define ST_ARR_INIT                         0xFFFFFFFFU
#else
#define ST_ARR_INIT                         0x0000FFFFU
#endif

#define ST_HANDLER                          GD32_ST_TIMER_IRQ_HANDLER
#define ST_NUMBER                           GD32_ST_TIMER_IRQ_NUMBER
#define ST_CLOCK_SRC                        GD32_ST_TIMER_CLOCK
#define ST_ENABLE_CLOCK()                   GD32_ST_TIMER_ENABLE_CLOCK()
#define ST_ENABLE_STOP()                    GD32_ST_TIMER_ENABLE_STOP()

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC

#define ST_HANDLER                          SysTick_Handler

/*
 * GD32H73x/75x Cortex-M7 core is clocked directly by CK_SYS,
 * so its reload comes from GD32_SYSCLK instead of GD32_HCLK.
 */
#define SYSTICK_CK                          GD32_SYSCLK

#if SYSTICK_CK % OSAL_ST_FREQUENCY != 0
#error "the selected ST frequency is not obtainable because integer rounding"
#endif

#if (SYSTICK_CK / OSAL_ST_FREQUENCY) - 1 > 0xFFFFFF
#error "the selected ST frequency is not obtainable because SysTick timer counter limits"
#endif

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if !defined(GD32_ST_SUPPRESS_ISR)
/**
 * @brief   Interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(ST_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  st_lld_serve_interrupt();

  OSAL_IRQ_EPILOGUE();
}
#endif

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ST driver initialization.
 *
 * @notapi
 */
void st_lld_init(void) {

#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING
  /* Free running counter mode.*/
  osalDbgAssert((ST_CLOCK_SRC % OSAL_ST_FREQUENCY) == 0U,
                "clock rounding error");
  osalDbgAssert(((ST_CLOCK_SRC / OSAL_ST_FREQUENCY) - 1U) < 0x10000,
                "clock prescaler overflow");

  /* Enabling timer clock.*/
  ST_ENABLE_CLOCK();

  /* Keep the counter running when the core is halted by the debugger.*/
  ST_ENABLE_STOP();

  /* Initializing the counter in free running mode.*/
  TIMER_PSC(GD32_ST_TIMER)   = (ST_CLOCK_SRC / OSAL_ST_FREQUENCY) - 1;
  TIMER_CAR(GD32_ST_TIMER)   = ST_ARR_INIT;
  TIMER_CHCTL0(GD32_ST_TIMER) = 0;
  TIMER_CH0CV(GD32_ST_TIMER) = 0;
#if ST_LLD_NUM_ALARMS > 1
  TIMER_CH1CV(GD32_ST_TIMER) = 0;
#endif
#if ST_LLD_NUM_ALARMS > 2
  TIMER_CH2CV(GD32_ST_TIMER) = 0;
#endif
#if ST_LLD_NUM_ALARMS > 3
  TIMER_CH3CV(GD32_ST_TIMER) = 0;
#endif
  TIMER_DMAINTEN(GD32_ST_TIMER) = 0;
  TIMER_CTL1(GD32_ST_TIMER)  = 0;
  TIMER_SWEVG(GD32_ST_TIMER) = TIMER_SWEVG_UPG;
  TIMER_CTL0(GD32_ST_TIMER)  = TIMER_CTL0_CEN;

#if !defined(GD32_ST_SUPPRESS_ISR)
  /* IRQ enabled.*/
  nvicEnableVector(ST_NUMBER, GD32_ST_IRQ_PRIORITY);
#endif
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#if OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC
  /* Periodic systick mode, the Cortex-Mx internal systick timer is used
     in this mode.*/
  SysTick->LOAD = (SYSTICK_CK / OSAL_ST_FREQUENCY) - 1;
  SysTick->VAL = 0;
  SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
                  SysTick_CTRL_ENABLE_Msk |
                  SysTick_CTRL_TICKINT_Msk;

  /* IRQ enabled.*/
  nvicSetSystemHandlerPriority(HANDLER_SYSTICK, GD32_ST_IRQ_PRIORITY);
#endif /* OSAL_ST_MODE == OSAL_ST_MODE_PERIODIC */
}

/**
 * @brief   IRQ handling code.
 */
void st_lld_serve_interrupt(void) {
#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING
  uint32_t flags;

  flags  = TIMER_INTF(GD32_ST_TIMER);
  flags &= TIMER_DMAINTEN(GD32_ST_TIMER) & GD32_TIMER_DMAINTEN_IRQ_MASK;
  TIMER_INTF(GD32_ST_TIMER) = ~flags;

  if ((flags & TIMER_INTF_CH0IF) != 0U)
#endif
  {
    osalSysLockFromISR();
    osalOsTimerHandlerI();
    osalSysUnlockFromISR();
  }
#if OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING
#if ST_LLD_NUM_ALARMS > 1
  if ((flags & TIMER_INTF_CH1IF) != 0U) {
    if (st_callbacks[1] != NULL) {
      st_callbacks[1](1U);
    }
  }
#endif
#if ST_LLD_NUM_ALARMS > 2
  if ((flags & TIMER_INTF_CH2IF) != 0U) {
    if (st_callbacks[2] != NULL) {
      st_callbacks[2](2U);
    }
  }
#endif
#if ST_LLD_NUM_ALARMS > 3
  if ((flags & TIMER_INTF_CH3IF) != 0U) {
    if (st_callbacks[3] != NULL) {
      st_callbacks[3](3U);
    }
  }
#endif
#endif
}

#endif /* OSAL_ST_MODE != OSAL_ST_MODE_NONE */

/** @} */
