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
 * @file    TIMERv1/hal_gpt_lld.h
 * @brief   GD32 GPT subsystem low level driver header. 
 *
 * @addtogroup GPT
 * @{
 */

#ifndef HAL_GPT_LLD_H
#define HAL_GPT_LLD_H

#include "gd32_timer.h"

#if HAL_USE_GPT || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   GPTD0 driver enable switch.
 * @details If set to @p TRUE the support for GPTD0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER0) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER0                 FALSE
#endif

/**
 * @brief   GPTD1 driver enable switch.
 * @details If set to @p TRUE the support for GPTD1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER1) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER1                 FALSE
#endif

/**
 * @brief   GPTD2 driver enable switch.
 * @details If set to @p TRUE the support for GPTD2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER2) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER2                 FALSE
#endif

/**
 * @brief   GPTD3 driver enable switch.
 * @details If set to @p TRUE the support for GPTD3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER3) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER3                 FALSE
#endif

/**
 * @brief   GPTD4 driver enable switch.
 * @details If set to @p TRUE the support for GPTD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER4) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER4                 FALSE
#endif

/**
 * @brief   GPTD5 driver enable switch.
 * @details If set to @p TRUE the support for GPTD5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER5) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER5                 FALSE
#endif

/**
 * @brief   GPTD6 driver enable switch.
 * @details If set to @p TRUE the support for GPTD6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER6) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER6                 FALSE
#endif

/**
 * @brief   GPTD7 driver enable switch.
 * @details If set to @p TRUE the support for GPTD7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER7) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER7                 FALSE
#endif

/**
 * @brief   GPTD14 driver enable switch.
 * @details If set to @p TRUE the support for GPTD14 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER14) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER14                FALSE
#endif

/**
 * @brief   GPTD15 driver enable switch.
 * @details If set to @p TRUE the support for GPTD15 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER15) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER15                FALSE
#endif

/**
 * @brief   GPTD16 driver enable switch.
 * @details If set to @p TRUE the support for GPTD16 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER16) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER16                FALSE
#endif

/**
 * @brief   GPTD22 driver enable switch.
 * @details If set to @p TRUE the support for GPTD22 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER22) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER22                FALSE
#endif

/**
 * @brief   GPTD23 driver enable switch.
 * @details If set to @p TRUE the support for GPTD23 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER23) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER23                FALSE
#endif

/**
 * @brief   GPTD30 driver enable switch.
 * @details If set to @p TRUE the support for GPTD30 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER30) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER30                FALSE
#endif

/**
 * @brief   GPTD31 driver enable switch.
 * @details If set to @p TRUE the support for GPTD31 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER31) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER31                FALSE
#endif

/**
 * @brief   GPTD40 driver enable switch.
 * @details If set to @p TRUE the support for GPTD40 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER40) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER40                FALSE
#endif

/**
 * @brief   GPTD41 driver enable switch.
 * @details If set to @p TRUE the support for GPTD41 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER41) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER41                FALSE
#endif

/**
 * @brief   GPTD42 driver enable switch.
 * @details If set to @p TRUE the support for GPTD42 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER42) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER42                FALSE
#endif

/**
 * @brief   GPTD43 driver enable switch.
 * @details If set to @p TRUE the support for GPTD43 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER43) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER43                FALSE
#endif

/**
 * @brief   GPTD44 driver enable switch.
 * @details If set to @p TRUE the support for GPTD44 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER44) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER44                FALSE
#endif

/**
 * @brief   GPTD50 driver enable switch.
 * @details If set to @p TRUE the support for GPTD50 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER50) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER50                FALSE
#endif

/**
 * @brief   GPTD51 driver enable switch.
 * @details If set to @p TRUE the support for GPTD51 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_GPT_USE_TIMER51) || defined(__DOXYGEN__)
#define GD32_GPT_USE_TIMER51                FALSE
#endif

/**
 * @brief   GPTD0 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER0_IRQ_PRIORITY        7
#endif

/**
 * @brief   GPTD1 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER1_IRQ_PRIORITY        7
#endif

/**
 * @brief   GPTD2 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER2_IRQ_PRIORITY        7
#endif

/**
 * @brief   GPTD3 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER3_IRQ_PRIORITY        7
#endif

/**
 * @brief   GPTD4 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER4_IRQ_PRIORITY        7
#endif

/**
 * @brief   GPTD5 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER5_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER5_IRQ_PRIORITY        7
#endif

/**
 * @brief   GPTD6 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER6_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER6_IRQ_PRIORITY        7
#endif

/**
 * @brief   GPTD7 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER7_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER7_IRQ_PRIORITY        7
#endif

/**
 * @brief   GPTD14 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER14_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER14_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD15 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER15_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER15_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD16 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER16_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER16_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD22 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER22_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER22_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD23 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER23_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER23_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD30 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER30_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER30_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD31 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER31_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER31_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD40 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER40_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER40_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD41 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER41_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER41_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD42 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER42_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER42_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD43 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER43_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER43_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD44 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER44_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER44_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD50 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER50_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER50_IRQ_PRIORITY       7
#endif

/**
 * @brief   GPTD51 interrupt priority level setting.
 */
#if !defined(GD32_GPT_TIMER51_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_GPT_TIMER51_IRQ_PRIORITY       7
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

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

#if GD32_GPT_USE_TIMER0 && !GD32_HAS_TIMER0
#error "TIMER0 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER1 && !GD32_HAS_TIMER1
#error "TIMER1 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER2 && !GD32_HAS_TIMER2
#error "TIMER2 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER3 && !GD32_HAS_TIMER3
#error "TIMER3 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER4 && !GD32_HAS_TIMER4
#error "TIMER4 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER5 && !GD32_HAS_TIMER5
#error "TIMER5 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER6 && !GD32_HAS_TIMER6
#error "TIMER6 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER7 && !GD32_HAS_TIMER7
#error "TIMER7 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER14 && !GD32_HAS_TIMER14
#error "TIMER14 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER15 && !GD32_HAS_TIMER15
#error "TIMER15 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER16 && !GD32_HAS_TIMER16
#error "TIMER16 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER22 && !GD32_HAS_TIMER22
#error "TIMER22 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER23 && !GD32_HAS_TIMER23
#error "TIMER23 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER30 && !GD32_HAS_TIMER30
#error "TIMER30 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER31 && !GD32_HAS_TIMER31
#error "TIMER31 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER40 && !GD32_HAS_TIMER40
#error "TIMER40 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER41 && !GD32_HAS_TIMER41
#error "TIMER41 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER42 && !GD32_HAS_TIMER42
#error "TIMER42 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER43 && !GD32_HAS_TIMER43
#error "TIMER43 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER44 && !GD32_HAS_TIMER44
#error "TIMER44 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER50 && !GD32_HAS_TIMER50
#error "TIMER50 not present in the selected device"
#endif

#if GD32_GPT_USE_TIMER51 && !GD32_HAS_TIMER51
#error "TIMER51 not present in the selected device"
#endif

#if !GD32_GPT_USE_TIMER0  && !GD32_GPT_USE_TIMER1  &&                       \
    !GD32_GPT_USE_TIMER2  && !GD32_GPT_USE_TIMER3  &&                       \
    !GD32_GPT_USE_TIMER4  && !GD32_GPT_USE_TIMER5  &&                       \
    !GD32_GPT_USE_TIMER6  && !GD32_GPT_USE_TIMER7  &&                       \
    !GD32_GPT_USE_TIMER14 && !GD32_GPT_USE_TIMER15 &&                       \
    !GD32_GPT_USE_TIMER16 && !GD32_GPT_USE_TIMER22 &&                       \
    !GD32_GPT_USE_TIMER23 && !GD32_GPT_USE_TIMER30 &&                       \
    !GD32_GPT_USE_TIMER31 && !GD32_GPT_USE_TIMER40 &&                       \
    !GD32_GPT_USE_TIMER41 && !GD32_GPT_USE_TIMER42 &&                       \
    !GD32_GPT_USE_TIMER43 && !GD32_GPT_USE_TIMER44 &&                       \
    !GD32_GPT_USE_TIMER50 && !GD32_GPT_USE_TIMER51
#error "GPT driver activated but no TIMER peripheral assigned"
#endif

/* Checks on allocation of TIMERx units.*/
#if GD32_GPT_USE_TIMER0
#if defined(GD32_TIMER0_IS_USED)
#error "GPTD0 requires TIMER0 but the timer is already used"
#else
#define GD32_TIMER0_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER1
#if defined(GD32_TIMER1_IS_USED)
#error "GPTD1 requires TIMER1 but the timer is already used"
#else
#define GD32_TIMER1_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER2
#if defined(GD32_TIMER2_IS_USED)
#error "GPTD2 requires TIMER2 but the timer is already used"
#else
#define GD32_TIMER2_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER3
#if defined(GD32_TIMER3_IS_USED)
#error "GPTD3 requires TIMER3 but the timer is already used"
#else
#define GD32_TIMER3_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER4
#if defined(GD32_TIMER4_IS_USED)
#error "GPTD4 requires TIMER4 but the timer is already used"
#else
#define GD32_TIMER4_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER5
#if defined(GD32_TIMER5_IS_USED)
#error "GPTD5 requires TIMER5 but the timer is already used"
#else
#define GD32_TIMER5_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER6
#if defined(GD32_TIMER6_IS_USED)
#error "GPTD6 requires TIMER6 but the timer is already used"
#else
#define GD32_TIMER6_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER7
#if defined(GD32_TIMER7_IS_USED)
#error "GPTD7 requires TIMER7 but the timer is already used"
#else
#define GD32_TIMER7_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER14
#if defined(GD32_TIMER14_IS_USED)
#error "GPTD14 requires TIMER14 but the timer is already used"
#else
#define GD32_TIMER14_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER15
#if defined(GD32_TIMER15_IS_USED)
#error "GPTD15 requires TIMER15 but the timer is already used"
#else
#define GD32_TIMER15_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER16
#if defined(GD32_TIMER16_IS_USED)
#error "GPTD16 requires TIMER16 but the timer is already used"
#else
#define GD32_TIMER16_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER22
#if defined(GD32_TIMER22_IS_USED)
#error "GPTD22 requires TIMER22 but the timer is already used"
#else
#define GD32_TIMER22_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER23
#if defined(GD32_TIMER23_IS_USED)
#error "GPTD23 requires TIMER23 but the timer is already used"
#else
#define GD32_TIMER23_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER30
#if defined(GD32_TIMER30_IS_USED)
#error "GPTD30 requires TIMER30 but the timer is already used"
#else
#define GD32_TIMER30_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER31
#if defined(GD32_TIMER31_IS_USED)
#error "GPTD31 requires TIMER31 but the timer is already used"
#else
#define GD32_TIMER31_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER40
#if defined(GD32_TIMER40_IS_USED)
#error "GPTD40 requires TIMER40 but the timer is already used"
#else
#define GD32_TIMER40_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER41
#if defined(GD32_TIMER41_IS_USED)
#error "GPTD41 requires TIMER41 but the timer is already used"
#else
#define GD32_TIMER41_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER42
#if defined(GD32_TIMER42_IS_USED)
#error "GPTD42 requires TIMER42 but the timer is already used"
#else
#define GD32_TIMER42_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER43
#if defined(GD32_TIMER43_IS_USED)
#error "GPTD43 requires TIMER43 but the timer is already used"
#else
#define GD32_TIMER43_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER44
#if defined(GD32_TIMER44_IS_USED)
#error "GPTD44 requires TIMER44 but the timer is already used"
#else
#define GD32_TIMER44_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER50
#if defined(GD32_TIMER50_IS_USED)
#error "GPTD50 requires TIMER50 but the timer is already used"
#else
#define GD32_TIMER50_IS_USED
#endif
#endif

#if GD32_GPT_USE_TIMER51
#if defined(GD32_TIMER51_IS_USED)
#error "GPTD51 requires TIMER51 but the timer is already used"
#else
#define GD32_TIMER51_IS_USED
#endif
#endif

/* IRQ priority checks.*/
#if GD32_GPT_USE_TIMER0 && !defined(GD32_TIMER0_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER0_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER0"
#endif

#if GD32_GPT_USE_TIMER1 && !defined(GD32_TIMER1_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER1"
#endif

#if GD32_GPT_USE_TIMER2 && !defined(GD32_TIMER2_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER2_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER2"
#endif

#if GD32_GPT_USE_TIMER3 && !defined(GD32_TIMER3_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER3_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER3"
#endif

#if GD32_GPT_USE_TIMER4 && !defined(GD32_TIMER4_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER4_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER4"
#endif

#if GD32_GPT_USE_TIMER5 && !defined(GD32_TIMER5_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER5_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER5"
#endif

#if GD32_GPT_USE_TIMER6 && !defined(GD32_TIMER6_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER6_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER6"
#endif

#if GD32_GPT_USE_TIMER7 && !defined(GD32_TIMER7_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER7_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER7"
#endif

#if GD32_GPT_USE_TIMER14 && !defined(GD32_TIMER14_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER14_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER14"
#endif

#if GD32_GPT_USE_TIMER15 && !defined(GD32_TIMER15_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER15_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER15"
#endif

#if GD32_GPT_USE_TIMER16 && !defined(GD32_TIMER16_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER16_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER16"
#endif

#if GD32_GPT_USE_TIMER22 && !defined(GD32_TIMER22_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER22_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER22"
#endif

#if GD32_GPT_USE_TIMER23 && !defined(GD32_TIMER23_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER23_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER23"
#endif

#if GD32_GPT_USE_TIMER30 && !defined(GD32_TIMER30_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER30_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER30"
#endif

#if GD32_GPT_USE_TIMER31 && !defined(GD32_TIMER31_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER31_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER31"
#endif

#if GD32_GPT_USE_TIMER40 && !defined(GD32_TIMER40_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER40_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER40"
#endif

#if GD32_GPT_USE_TIMER41 && !defined(GD32_TIMER41_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER41_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER41"
#endif

#if GD32_GPT_USE_TIMER42 && !defined(GD32_TIMER42_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER42_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER42"
#endif

#if GD32_GPT_USE_TIMER43 && !defined(GD32_TIMER43_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER43_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER43"
#endif

#if GD32_GPT_USE_TIMER44 && !defined(GD32_TIMER44_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER44_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER44"
#endif

#if GD32_GPT_USE_TIMER50 && !defined(GD32_TIMER50_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER50_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER50"
#endif

#if GD32_GPT_USE_TIMER51 && !defined(GD32_TIMER51_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_GPT_TIMER51_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER51"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   GPT frequency type.
 */
typedef uint32_t gptfreq_t;

/**
 * @brief   GPT counter type.
 */
typedef uint32_t gptcnt_t;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Timer clock in Hz.
   * @note    The low level can use assertions in order to catch invalid
   *          frequency specifications.
   */
  gptfreq_t                 frequency;
  /**
   * @brief   Timer callback pointer.
   * @note    This callback is invoked on GPT counter events.
   * @note    This callback can be set to @p NULL but in that case the
   *          one-shot mode cannot be used.
   */
  gptcallback_t             callback;
  /* End of the mandatory fields.*/
  /**
   * @brief TIMER CTL1 register initialization data.
   * @note  The value of this field should normally be equal to zero.
   */
  uint32_t                  ctl1;
  /**
   * @brief TIMER DMAINTEN register initialization data.
   * @note  The value of this field should normally be equal to zero.
   * @note  Only the DMA-related bits can be specified in this field.
   */
  uint32_t                  dmainten;
} GPTConfig;

/**
 * @brief   Structure representing a GPT driver.
 */
struct GPTDriver {
  /**
   * @brief Driver state.
   */
  gptstate_t                state;
  /**
   * @brief Current configuration data.
   */
  const GPTConfig           *config;
#if defined(GPT_DRIVER_EXT_FIELDS)
  GPT_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Timer base clock.
   */
  uint32_t                  clock;
  /**
     * @brief TIMERx registers block reference.
   */
  uint32_t                  tim;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Changes the interval of GPT peripheral.
 * @details This function changes the interval of a running GPT unit.
 * @pre     The GPT unit must be running in continuous mode.
 * @post    The GPT unit interval is changed to the new value.
 * @note    The function has effect at the next cycle start.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 * @param[in] interval  new cycle time in timer ticks
 *
 * @notapi
 */
#define gpt_lld_change_interval(gptp, interval)                             \
  (TIMER_CAR((gptp)->tim) = (uint32_t)((interval) - 1U))

/**
 * @brief   Returns the interval of GPT peripheral.
 * @pre     The GPT unit must be running in continuous mode.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 * @return              The current interval.
 *
 * @notapi
 */
#define gpt_lld_get_interval(gptp) ((gptcnt_t)(TIMER_CAR((gptp)->tim) + 1U))

/**
 * @brief   Returns the counter value of GPT peripheral.
 * @pre     The GPT unit must be running in continuous mode.
 * @note    The nature of the counter is not defined, it may count upward
 *          or downward, it could be continuously running or not.
 *
 * @param[in] gptp      pointer to a @p GPTDriver object
 * @return              The current counter value.
 *
 * @notapi
 */
#define gpt_lld_get_counter(gptp) ((gptcnt_t)TIMER_CNT((gptp)->tim))

/**
 * @brief   Returns the TIMER associated with a GPT.
 *
 * @param[in] gptp      pointer to the @p GPTDriver object
 * @return              The TIMER reference.
 *
 * @notapi
 */
#define gpt_lld_get_timer(gptp) ((gptp)->tim)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if GD32_GPT_USE_TIMER0 && !defined(__DOXYGEN__)
extern GPTDriver GPTD0;
#endif

#if GD32_GPT_USE_TIMER1 && !defined(__DOXYGEN__)
extern GPTDriver GPTD1;
#endif

#if GD32_GPT_USE_TIMER2 && !defined(__DOXYGEN__)
extern GPTDriver GPTD2;
#endif

#if GD32_GPT_USE_TIMER3 && !defined(__DOXYGEN__)
extern GPTDriver GPTD3;
#endif

#if GD32_GPT_USE_TIMER4 && !defined(__DOXYGEN__)
extern GPTDriver GPTD4;
#endif

#if GD32_GPT_USE_TIMER5 && !defined(__DOXYGEN__)
extern GPTDriver GPTD5;
#endif

#if GD32_GPT_USE_TIMER6 && !defined(__DOXYGEN__)
extern GPTDriver GPTD6;
#endif

#if GD32_GPT_USE_TIMER7 && !defined(__DOXYGEN__)
extern GPTDriver GPTD7;
#endif

#if GD32_GPT_USE_TIMER14 && !defined(__DOXYGEN__)
extern GPTDriver GPTD14;
#endif

#if GD32_GPT_USE_TIMER15 && !defined(__DOXYGEN__)
extern GPTDriver GPTD15;
#endif

#if GD32_GPT_USE_TIMER16 && !defined(__DOXYGEN__)
extern GPTDriver GPTD16;
#endif

#if GD32_GPT_USE_TIMER22 && !defined(__DOXYGEN__)
extern GPTDriver GPTD22;
#endif

#if GD32_GPT_USE_TIMER23 && !defined(__DOXYGEN__)
extern GPTDriver GPTD23;
#endif

#if GD32_GPT_USE_TIMER30 && !defined(__DOXYGEN__)
extern GPTDriver GPTD30;
#endif

#if GD32_GPT_USE_TIMER31 && !defined(__DOXYGEN__)
extern GPTDriver GPTD31;
#endif

#if GD32_GPT_USE_TIMER40 && !defined(__DOXYGEN__)
extern GPTDriver GPTD40;
#endif

#if GD32_GPT_USE_TIMER41 && !defined(__DOXYGEN__)
extern GPTDriver GPTD41;
#endif

#if GD32_GPT_USE_TIMER42 && !defined(__DOXYGEN__)
extern GPTDriver GPTD42;
#endif

#if GD32_GPT_USE_TIMER43 && !defined(__DOXYGEN__)
extern GPTDriver GPTD43;
#endif

#if GD32_GPT_USE_TIMER44 && !defined(__DOXYGEN__)
extern GPTDriver GPTD44;
#endif

#if GD32_GPT_USE_TIMER50 && !defined(__DOXYGEN__)
extern GPTDriver GPTD50;
#endif

#if GD32_GPT_USE_TIMER51 && !defined(__DOXYGEN__)
extern GPTDriver GPTD51;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void gpt_lld_init(void);
  void gpt_lld_start(GPTDriver *gptp);
  void gpt_lld_stop(GPTDriver *gptp);
  void gpt_lld_start_timer(GPTDriver *gptp, gptcnt_t period);
  void gpt_lld_stop_timer(GPTDriver *gptp);
  void gpt_lld_polled_delay(GPTDriver *gptp, gptcnt_t interval);
  void gpt_lld_serve_interrupt(GPTDriver *gptp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_GPT */

#endif /* HAL_GPT_LLD_H */

/** @} */
