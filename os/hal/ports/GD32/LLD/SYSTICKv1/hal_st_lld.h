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
 * @file    SYSTICKv1/hal_st_lld.h
 * @brief   ST Driver subsystem low level driver header.
 * @details This header is designed to be include-able without having to
 *          include other files from the HAL.
 *
 * @addtogroup ST
 * @{
 */

#ifndef HAL_ST_LLD_H
#define HAL_ST_LLD_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief   SysTick timer IRQ priority.
 */
#if !defined(GD32_ST_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ST_IRQ_PRIORITY                8
#endif

/**
 * @brief   TIMERx unit (by number) to be used for free running operations.
 * @note    GD32 TIMER4 is the 32-bit general purpose timer used as the
 *          free running ST source.
 */
#if !defined(GD32_ST_USE_TIMER) || defined(__DOXYGEN__)
#define GD32_ST_USE_TIMER                   4
#endif

/**
 * @brief   Overrides the number of supported alarms.
 * @note    The default number of alarms is equal to the number of
 *          comparators in the timer, overriding it to one makes
 *          the driver a little faster and smaller. The kernel itself
 *          only needs one alarm, additional features could need more.
 * @note    Zero means do not override.
 * @note    This setting is only meaningful in free running mode, in
 *          tick mode there are no alarms.
 */
#if !defined(GD32_ST_OVERRIDE_ALARMS) || defined(__DOXYGEN__)
#define GD32_ST_OVERRIDE_ALARMS             1
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING)

/* This has to go after transition to shared handlers is complete for all
   platforms.*/
#if !defined(GD32_HAS_TIMER0)
#define GD32_HAS_TIMER0                     FALSE
#endif
#if !defined(GD32_HAS_TIMER1)
#define GD32_HAS_TIMER1                     FALSE
#endif
#if !defined(GD32_HAS_TIMER2)
#define GD32_HAS_TIMER2                     FALSE
#endif
#if !defined(GD32_HAS_TIMER3)
#define GD32_HAS_TIMER3                     FALSE
#endif
#if !defined(GD32_HAS_TIMER4)
#define GD32_HAS_TIMER4                     FALSE
#endif
#if !defined(GD32_HAS_TIMER5)
#define GD32_HAS_TIMER5                     FALSE
#endif
#if !defined(GD32_HAS_TIMER6)
#define GD32_HAS_TIMER6                     FALSE
#endif
#if !defined(GD32_HAS_TIMER7)
#define GD32_HAS_TIMER7                     FALSE
#endif
#if !defined(GD32_HAS_TIMER14)
#define GD32_HAS_TIMER14                    FALSE
#endif
#if !defined(GD32_HAS_TIMER15)
#define GD32_HAS_TIMER15                    FALSE
#endif
#if !defined(GD32_HAS_TIMER16)
#define GD32_HAS_TIMER16                    FALSE
#endif
#if !defined(GD32_HAS_TIMER22)
#define GD32_HAS_TIMER22                    FALSE
#endif
#if !defined(GD32_HAS_TIMER23)
#define GD32_HAS_TIMER23                    FALSE
#endif
#if !defined(GD32_HAS_TIMER30)
#define GD32_HAS_TIMER30                    FALSE
#endif
#if !defined(GD32_HAS_TIMER31)
#define GD32_HAS_TIMER31                    FALSE
#endif
#if !defined(GD32_HAS_TIMER40)
#define GD32_HAS_TIMER40                    FALSE
#endif
#if !defined(GD32_HAS_TIMER41)
#define GD32_HAS_TIMER41                    FALSE
#endif
#if !defined(GD32_HAS_TIMER42)
#define GD32_HAS_TIMER42                    FALSE
#endif
#if !defined(GD32_HAS_TIMER43)
#define GD32_HAS_TIMER43                    FALSE
#endif
#if !defined(GD32_HAS_TIMER44)
#define GD32_HAS_TIMER44                    FALSE
#endif
#if !defined(GD32_HAS_TIMER50)
#define GD32_HAS_TIMER50                    FALSE
#endif
#if !defined(GD32_HAS_TIMER51)
#define GD32_HAS_TIMER51                    FALSE
#endif

#if GD32_ST_USE_TIMER == 0

#if !GD32_HAS_TIMER0
#error "TIMER0 not present in the selected device"
#endif

#if defined(GD32_TIMER0_IS_USED)
#error "ST requires TIMER0 but the timer is already used"
#else
#define GD32_TIMER0_IS_USED
#endif

#if defined(GD32_TIMER0_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER0
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER0_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER0_CC_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER0_CC_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK2
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER0(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL3 |= DBG_CTL3_TIMER0_HOLD

#elif GD32_ST_USE_TIMER == 1

#if !GD32_HAS_TIMER1
#error "TIMER1 not present in the selected device"
#endif

#if defined(GD32_TIMER1_IS_USED)
#error "ST requires TIMER1 but the timer is already used"
#else
#define GD32_TIMER1_IS_USED
#endif

#if defined(GD32_TIMER1_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER1
#define GD32_ST_TIMER_IS_32BITS             TRUE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER1_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER1_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER1_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER1(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER1_HOLD

#elif GD32_ST_USE_TIMER == 2

#if !GD32_HAS_TIMER2
#error "TIMER2 not present in the selected device"
#endif

#if defined(GD32_TIMER2_IS_USED)
#error "ST requires TIMER2 but the timer is already used"
#else
#define GD32_TIMER2_IS_USED
#endif

#if defined(GD32_TIMER2_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER2
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER2_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER2_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER2_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER2(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER2_HOLD

#elif GD32_ST_USE_TIMER == 3

#if !GD32_HAS_TIMER3
#error "TIMER3 not present in the selected device"
#endif

#if defined(GD32_TIMER3_IS_USED)
#error "ST requires TIMER3 but the timer is already used"
#else
#define GD32_TIMER3_IS_USED
#endif

#if defined(GD32_TIMER3_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER3
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER3_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER3_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER3_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER3(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER3_HOLD

#elif GD32_ST_USE_TIMER == 4

#if !GD32_HAS_TIMER4
#error "TIMER4 not present in the selected device"
#endif

#if defined(GD32_TIMER4_IS_USED)
#error "ST requires TIMER4 but the timer is already used"
#else
#define GD32_TIMER4_IS_USED
#endif

#if defined(GD32_TIMER4_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER4
#define GD32_ST_TIMER_IS_32BITS             TRUE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER4_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER4_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER4_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER4(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER4_HOLD

#elif GD32_ST_USE_TIMER == 5

#if !GD32_HAS_TIMER5
#error "TIMER5 not present in the selected device"
#endif

#if defined(GD32_TIMER5_IS_USED)
#error "ST requires TIMER5 but the timer is already used"
#else
#define GD32_TIMER5_IS_USED
#endif

#if defined(GD32_TIMER5_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER5
#define GD32_ST_TIMER_IS_32BITS             TRUE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER5_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER5_DAC_UDR_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER5_DAC_UDR_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER5(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER5_HOLD

#elif GD32_ST_USE_TIMER == 6

#if !GD32_HAS_TIMER6
#error "TIMER6 not present in the selected device"
#endif

#if defined(GD32_TIMER6_IS_USED)
#error "ST requires TIMER6 but the timer is already used"
#else
#define GD32_TIMER6_IS_USED
#endif

#if defined(GD32_TIMER6_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER6
#define GD32_ST_TIMER_IS_32BITS             TRUE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER6_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER6_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER6_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER6(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER6_HOLD

#elif GD32_ST_USE_TIMER == 7

#if !GD32_HAS_TIMER7
#error "TIMER7 not present in the selected device"
#endif

#if defined(GD32_TIMER7_IS_USED)
#error "ST requires TIMER7 but the timer is already used"
#else
#define GD32_TIMER7_IS_USED
#endif

#if defined(GD32_TIMER7_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER7
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER7_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER7_CC_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER7_CC_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK2
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER7(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL3 |= DBG_CTL3_TIMER7_HOLD

#elif GD32_ST_USE_TIMER == 14

#if !GD32_HAS_TIMER14
#error "TIMER14 not present in the selected device"
#endif

#if defined(GD32_TIMER14_IS_USED)
#error "ST requires TIMER14 but the timer is already used"
#else
#define GD32_TIMER14_IS_USED
#endif

#if defined(GD32_TIMER14_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER14
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER14_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER14_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER14_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK2
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER14(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL3 |= DBG_CTL3_TIMER14_HOLD

#elif GD32_ST_USE_TIMER == 15

#if !GD32_HAS_TIMER15
#error "TIMER15 not present in the selected device"
#endif

#if defined(GD32_TIMER15_IS_USED)
#error "ST requires TIMER15 but the timer is already used"
#else
#define GD32_TIMER15_IS_USED
#endif

#if defined(GD32_TIMER15_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER15
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER15_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER15_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER15_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK2
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER15(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL3 |= DBG_CTL3_TIMER15_HOLD

#elif GD32_ST_USE_TIMER == 16

#if !GD32_HAS_TIMER16
#error "TIMER16 not present in the selected device"
#endif

#if defined(GD32_TIMER16_IS_USED)
#error "ST requires TIMER16 but the timer is already used"
#else
#define GD32_TIMER16_IS_USED
#endif

#if defined(GD32_TIMER16_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER16
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER16_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER16_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER16_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK2
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER16(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL3 |= DBG_CTL3_TIMER16_HOLD

#elif GD32_ST_USE_TIMER == 22

#if !GD32_HAS_TIMER22
#error "TIMER22 not present in the selected device"
#endif

#if defined(GD32_TIMER22_IS_USED)
#error "ST requires TIMER22 but the timer is already used"
#else
#define GD32_TIMER22_IS_USED
#endif

#if defined(GD32_TIMER22_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER22
#define GD32_ST_TIMER_IS_32BITS             TRUE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER22_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER22_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER22_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER22(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER22_HOLD

#elif GD32_ST_USE_TIMER == 23

#if !GD32_HAS_TIMER23
#error "TIMER23 not present in the selected device"
#endif

#if defined(GD32_TIMER23_IS_USED)
#error "ST requires TIMER23 but the timer is already used"
#else
#define GD32_TIMER23_IS_USED
#endif

#if defined(GD32_TIMER23_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER23
#define GD32_ST_TIMER_IS_32BITS             TRUE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER23_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER23_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER23_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER23(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER23_HOLD

#elif GD32_ST_USE_TIMER == 30

#if !GD32_HAS_TIMER30
#error "TIMER30 not present in the selected device"
#endif

#if defined(GD32_TIMER30_IS_USED)
#error "ST requires TIMER30 but the timer is already used"
#else
#define GD32_TIMER30_IS_USED
#endif

#if defined(GD32_TIMER30_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER30
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER30_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER30_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER30_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER30(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER30_HOLD

#elif GD32_ST_USE_TIMER == 31

#if !GD32_HAS_TIMER31
#error "TIMER31 not present in the selected device"
#endif

#if defined(GD32_TIMER31_IS_USED)
#error "ST requires TIMER31 but the timer is already used"
#else
#define GD32_TIMER31_IS_USED
#endif

#if defined(GD32_TIMER31_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER31
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER31_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER31_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER31_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER31(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER31_HOLD

#elif GD32_ST_USE_TIMER == 40

#if !GD32_HAS_TIMER40
#error "TIMER40 not present in the selected device"
#endif

#if defined(GD32_TIMER40_IS_USED)
#error "ST requires TIMER40 but the timer is already used"
#else
#define GD32_TIMER40_IS_USED
#endif

#if defined(GD32_TIMER40_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER40
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER40_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER40_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER40_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK2
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER40(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL3 |= DBG_CTL3_TIMER40_HOLD

#elif GD32_ST_USE_TIMER == 41

#if !GD32_HAS_TIMER41
#error "TIMER41 not present in the selected device"
#endif

#if defined(GD32_TIMER41_IS_USED)
#error "ST requires TIMER41 but the timer is already used"
#else
#define GD32_TIMER41_IS_USED
#endif

#if defined(GD32_TIMER41_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER41
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER41_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER41_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER41_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK2
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER41(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL3 |= DBG_CTL3_TIMER41_HOLD

#elif GD32_ST_USE_TIMER == 42

#if !GD32_HAS_TIMER42
#error "TIMER42 not present in the selected device"
#endif

#if defined(GD32_TIMER42_IS_USED)
#error "ST requires TIMER42 but the timer is already used"
#else
#define GD32_TIMER42_IS_USED
#endif

#if defined(GD32_TIMER42_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER42
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER42_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER42_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER42_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK2
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER42(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL3 |= DBG_CTL3_TIMER42_HOLD

#elif GD32_ST_USE_TIMER == 43

#if !GD32_HAS_TIMER43
#error "TIMER43 not present in the selected device"
#endif

#if defined(GD32_TIMER43_IS_USED)
#error "ST requires TIMER43 but the timer is already used"
#else
#define GD32_TIMER43_IS_USED
#endif

#if defined(GD32_TIMER43_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER43
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER43_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER43_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER43_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK2
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER43(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL3 |= DBG_CTL3_TIMER43_HOLD

#elif GD32_ST_USE_TIMER == 44

#if !GD32_HAS_TIMER44
#error "TIMER44 not present in the selected device"
#endif

#if defined(GD32_TIMER44_IS_USED)
#error "ST requires TIMER44 but the timer is already used"
#else
#define GD32_TIMER44_IS_USED
#endif

#if defined(GD32_TIMER44_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER44
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER44_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER44_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER44_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK2
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER44(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL3 |= DBG_CTL3_TIMER44_HOLD

#elif GD32_ST_USE_TIMER == 50

#if !GD32_HAS_TIMER50
#error "TIMER50 not present in the selected device"
#endif

#if defined(GD32_TIMER50_IS_USED)
#error "ST requires TIMER50 but the timer is already used"
#else
#define GD32_TIMER50_IS_USED
#endif

#if defined(GD32_TIMER50_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER50
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER50_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER50_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER50_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER50(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER50_HOLD

#elif GD32_ST_USE_TIMER == 51

#if !GD32_HAS_TIMER51
#error "TIMER51 not present in the selected device"
#endif

#if defined(GD32_TIMER51_IS_USED)
#error "ST requires TIMER51 but the timer is already used"
#else
#define GD32_TIMER51_IS_USED
#endif

#if defined(GD32_TIMER51_SUPPRESS_ISR)
#define GD32_ST_SUPPRESS_ISR
#endif

#define GD32_ST_TIMER                       TIMER51
#define GD32_ST_TIMER_IS_32BITS             FALSE
#define ST_LLD_NUM_ALARMS                   GD32_TIMER51_CHANNELS
#define GD32_ST_TIMER_IRQ_HANDLER           GD32_TIMER51_HANDLER
#define GD32_ST_TIMER_IRQ_NUMBER            GD32_TIMER51_NUMBER
#define GD32_ST_TIMER_CLOCK                 GD32_TIMERCLK1
#define GD32_ST_TIMER_ENABLE_CLOCK()        rcuEnableTIMER51(true)
#define GD32_ST_TIMER_ENABLE_STOP()         DBG_CTL2 |= DBG_CTL2_TIMER51_HOLD

#else
#error "GD32_ST_USE_TIMER specifies an unsupported timer"
#endif

#if (OSAL_ST_RESOLUTION == 32) && !GD32_ST_TIMER_IS_32BITS
#error "the selected timer is not 32 bits but OSAL_ST_RESOLUTION is 32"
#endif

#if ST_LLD_NUM_ALARMS < 1
#error "the selected timer must have at least one channel"
#endif

#if (GD32_ST_OVERRIDE_ALARMS < 0) ||                                       \
    (GD32_ST_OVERRIDE_ALARMS > ST_LLD_NUM_ALARMS)
#error "invalid GD32_ST_OVERRIDE_ALARMS value"
#endif

#if GD32_ST_OVERRIDE_ALARMS > 0
#undef ST_LLD_NUM_ALARMS
#define ST_LLD_NUM_ALARMS                   GD32_ST_OVERRIDE_ALARMS
#endif

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */
/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void st_lld_init(void);
  void st_lld_serve_interrupt(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Driver inline functions.                                                  */
/*===========================================================================*/

#if (OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING) || defined(__DOXYGEN__)

/**
 * @brief   Returns the time counter value.
 *
 * @return              The counter value.
 *
 * @notapi
 */
static inline systime_t st_lld_get_counter(void) {

  return (systime_t)TIMER_CNT(GD32_ST_TIMER);
}

/**
 * @brief   Starts the alarm.
 * @note    Makes sure that no spurious alarms are triggered after
 *          this call.
 *
 * @param[in] abstime   the time to be set for the first alarm
 *
 * @notapi
 */
static inline void st_lld_start_alarm(systime_t abstime) {

  TIMER_CH0CV(GD32_ST_TIMER) = (uint32_t)abstime;
  TIMER_INTF(GD32_ST_TIMER)  = 0;
#if ST_LLD_NUM_ALARMS == 1
  TIMER_DMAINTEN(GD32_ST_TIMER) = TIMER_DMAINTEN_CH0IE;
#else
  TIMER_DMAINTEN(GD32_ST_TIMER) |= TIMER_DMAINTEN_CH0IE;
#endif
}

/**
 * @brief   Stops the alarm interrupt.
 *
 * @notapi
 */
static inline void st_lld_stop_alarm(void) {

#if ST_LLD_NUM_ALARMS == 1
  TIMER_DMAINTEN(GD32_ST_TIMER) = 0U;
#else
  TIMER_DMAINTEN(GD32_ST_TIMER) &= ~TIMER_DMAINTEN_CH0IE;
#endif
}

/**
 * @brief   Sets the alarm time.
 *
 * @param[in] abstime   the time to be set for the next alarm
 *
 * @notapi
 */
static inline void st_lld_set_alarm(systime_t abstime) {

  TIMER_CH0CV(GD32_ST_TIMER) = (uint32_t)abstime;
}

/**
 * @brief   Returns the current alarm time.
 *
 * @return              The currently set alarm time.
 *
 * @notapi
 */
static inline systime_t st_lld_get_alarm(void) {

  return (systime_t)TIMER_CH0CV(GD32_ST_TIMER);
}

/**
 * @brief   Determines if the alarm is active.
 *
 * @return              The alarm status.
 * @retval false        if the alarm is not active.
 * @retval true         is the alarm is active
 *
 * @notapi
 */
static inline bool st_lld_is_alarm_active(void) {

  return (bool)((TIMER_DMAINTEN(GD32_ST_TIMER) & TIMER_DMAINTEN_CH0IE) != 0);
}

#if (ST_LLD_NUM_ALARMS > 1) || defined(__DOXYGEN__)
/**
 * @brief   Starts an alarm.
 * @note    Makes sure that no spurious alarms are triggered after
 *          this call.
 * @note    This functionality is only available in free running mode, the
 *          behavior in periodic mode is undefined.
 *
 * @param[in] abstime   the time to be set for the first alarm
 * @param[in] alarm     alarm channel number
 *
 * @notapi
 */
static inline void st_lld_start_alarm_n(unsigned alarm, systime_t abstime) {

  switch (alarm) {
  case 0U:
    TIMER_CH0CV(GD32_ST_TIMER) = (uint32_t)abstime;
    break;
#if ST_LLD_NUM_ALARMS > 1
  case 1U:
    TIMER_CH1CV(GD32_ST_TIMER) = (uint32_t)abstime;
    break;
#endif
#if ST_LLD_NUM_ALARMS > 2
  case 2U:
    TIMER_CH2CV(GD32_ST_TIMER) = (uint32_t)abstime;
    break;
#endif
#if ST_LLD_NUM_ALARMS > 3
  case 3U:
    TIMER_CH3CV(GD32_ST_TIMER) = (uint32_t)abstime;
    break;
#endif
  default:
    break;
  }

  TIMER_INTF(GD32_ST_TIMER) = 0;
  TIMER_DMAINTEN(GD32_ST_TIMER) |= (TIMER_DMAINTEN_CH0IE << alarm);
}

/**
 * @brief   Stops an alarm interrupt.
 * @note    This functionality is only available in free running mode, the
 *          behavior in periodic mode is undefined.
 *
 * @param[in] alarm     alarm channel number
 *
 * @notapi
 */
static inline void st_lld_stop_alarm_n(unsigned alarm) {

  TIMER_DMAINTEN(GD32_ST_TIMER) &= ~(TIMER_DMAINTEN_CH0IE << alarm);
}

/**
 * @brief   Sets an alarm time.
 * @note    This functionality is only available in free running mode, the
 *          behavior in periodic mode is undefined.
 *
 * @param[in] alarm     alarm channel number
 * @param[in] abstime   the time to be set for the next alarm
 *
 * @notapi
 */
static inline void st_lld_set_alarm_n(unsigned alarm, systime_t abstime) {

  switch (alarm) {
  case 0U:
    TIMER_CH0CV(GD32_ST_TIMER) = (uint32_t)abstime;
    break;
#if ST_LLD_NUM_ALARMS > 1
  case 1U:
    TIMER_CH1CV(GD32_ST_TIMER) = (uint32_t)abstime;
    break;
#endif
#if ST_LLD_NUM_ALARMS > 2
  case 2U:
    TIMER_CH2CV(GD32_ST_TIMER) = (uint32_t)abstime;
    break;
#endif
#if ST_LLD_NUM_ALARMS > 3
  case 3U:
    TIMER_CH3CV(GD32_ST_TIMER) = (uint32_t)abstime;
    break;
#endif
  default:
    break;
  }
}

/**
 * @brief   Returns an alarm current time.
 * @note    This functionality is only available in free running mode, the
 *          behavior in periodic mode is undefined.
 *
 * @param[in] alarm     alarm channel number
 * @return              The currently set alarm time.
 *
 * @notapi
 */
static inline systime_t st_lld_get_alarm_n(unsigned alarm) {

  switch (alarm) {
  case 0U:
    return (systime_t)TIMER_CH0CV(GD32_ST_TIMER);
#if ST_LLD_NUM_ALARMS > 1
  case 1U:
    return (systime_t)TIMER_CH1CV(GD32_ST_TIMER);
#endif
#if ST_LLD_NUM_ALARMS > 2
  case 2U:
    return (systime_t)TIMER_CH2CV(GD32_ST_TIMER);
#endif
#if ST_LLD_NUM_ALARMS > 3
  case 3U:
    return (systime_t)TIMER_CH3CV(GD32_ST_TIMER);
#endif
  default:
    return 0;
  }
}

/**
 * @brief   Determines if an alarm is active.
 *
 * @param[in] alarm     alarm channel number
 * @return              The alarm status.
 * @retval false        if the alarm is not active.
 * @retval true         is the alarm is active
 *
 * @notapi
 */
static inline bool st_lld_is_alarm_active_n(unsigned alarm) {

  return (bool)((TIMER_DMAINTEN(GD32_ST_TIMER) & (TIMER_DMAINTEN_CH0IE << alarm)) != 0);
}
#endif /* ST_LLD_NUM_ALARMS > 1 */

#endif /* OSAL_ST_MODE == OSAL_ST_MODE_FREERUNNING */

#endif /* HAL_ST_LLD_H */

/** @} */
