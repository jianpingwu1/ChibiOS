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
 * @file    TIMERv1/hal_pwm_lld.h
 * @brief   GD32 PWM subsystem low level driver header.
 *
 * @addtogroup PWM
 * @{
 */

#ifndef HAL_PWM_LLD_H
#define HAL_PWM_LLD_H

#if HAL_USE_PWM || defined(__DOXYGEN__)

#include "gd32_timer.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Number of PWM channels per PWM driver.
 */
#define PWM_CHANNELS                            GD32_TIMER_MAX_CHANNELS

/**
 * @name    GD32-specific PWM complementary output mode macros
 * @{
 */
/**
 * @brief   Complementary output modes mask.
 * @note    This is a GD32-specific setting.
 */
#define PWM_COMPLEMENTARY_OUTPUT_MASK           0xF0

/**
 * @brief   Complementary output not driven.
 * @note    This is a GD32-specific setting.
 */
#define PWM_COMPLEMENTARY_OUTPUT_DISABLED       0x00

/**
 * @brief   Complementary output, active is logic level one.
 * @note    This is a GD32-specific setting.
 * @note    This setting is only available if the timer supports the
 *          CCHP register.
 */
#define PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH    0x10

/**
 * @brief   Complementary output, active is logic level zero.
 * @note    This is a GD32-specific setting.
 * @note    This setting is only available if the timer supports the
 *          CCHP register.
 */
#define PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW     0x20
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   PWMD0 driver enable switch.
 * @details If set to @p TRUE the support for PWMD0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER0) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER0                 FALSE
#endif

/**
 * @brief   PWMD1 driver enable switch.
 * @details If set to @p TRUE the support for PWMD1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER1) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER1                 FALSE
#endif

/**
 * @brief   PWMD2 driver enable switch.
 * @details If set to @p TRUE the support for PWMD2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER2) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER2                 FALSE
#endif

/**
 * @brief   PWMD3 driver enable switch.
 * @details If set to @p TRUE the support for PWMD3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER3) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER3                 FALSE
#endif

/**
 * @brief   PWMD4 driver enable switch.
 * @details If set to @p TRUE the support for PWMD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER4) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER4                 FALSE
#endif

/**
 * @brief   PWMD7 driver enable switch.
 * @details If set to @p TRUE the support for PWMD7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER7) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER7                 FALSE
#endif

/**
 * @brief   PWMD14 driver enable switch.
 * @details If set to @p TRUE the support for PWMD14 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER14) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER14                FALSE
#endif

/**
 * @brief   PWMD15 driver enable switch.
 * @details If set to @p TRUE the support for PWMD15 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER15) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER15                FALSE
#endif

/**
 * @brief   PWMD16 driver enable switch.
 * @details If set to @p TRUE the support for PWMD16 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER16) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER16                FALSE
#endif

/**
 * @brief   PWMD22 driver enable switch.
 * @details If set to @p TRUE the support for PWMD22 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER22) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER22                FALSE
#endif

/**
 * @brief   PWMD23 driver enable switch.
 * @details If set to @p TRUE the support for PWMD23 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER23) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER23                FALSE
#endif

/**
 * @brief   PWMD30 driver enable switch.
 * @details If set to @p TRUE the support for PWMD30 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER30) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER30                FALSE
#endif

/**
 * @brief   PWMD31 driver enable switch.
 * @details If set to @p TRUE the support for PWMD31 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER31) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER31                FALSE
#endif

/**
 * @brief   PWMD40 driver enable switch.
 * @details If set to @p TRUE the support for PWMD40 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER40) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER40                FALSE
#endif

/**
 * @brief   PWMD41 driver enable switch.
 * @details If set to @p TRUE the support for PWMD41 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER41) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER41                FALSE
#endif

/**
 * @brief   PWMD42 driver enable switch.
 * @details If set to @p TRUE the support for PWMD42 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER42) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER42                FALSE
#endif

/**
 * @brief   PWMD43 driver enable switch.
 * @details If set to @p TRUE the support for PWMD43 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER43) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER43                FALSE
#endif

/**
 * @brief   PWMD44 driver enable switch.
 * @details If set to @p TRUE the support for PWMD44 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_PWM_USE_TIMER44) || defined(__DOXYGEN__)
#define GD32_PWM_USE_TIMER44                FALSE
#endif

/**
 * @brief   PWMD0 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER0_IRQ_PRIORITY        7
#endif

/**
 * @brief   PWMD1 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER1_IRQ_PRIORITY        7
#endif

/**
 * @brief   PWMD2 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER2_IRQ_PRIORITY        7
#endif

/**
 * @brief   PWMD3 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER3_IRQ_PRIORITY        7
#endif

/**
 * @brief   PWMD4 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER4_IRQ_PRIORITY        7
#endif

/**
 * @brief   PWMD7 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER7_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER7_IRQ_PRIORITY        7
#endif

/**
 * @brief   PWMD14 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER14_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER14_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD15 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER15_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER15_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD16 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER16_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER16_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD22 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER22_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER22_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD23 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER23_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER23_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD30 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER30_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER30_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD31 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER31_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER31_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD40 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER40_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER40_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD41 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER41_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER41_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD42 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER42_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER42_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD43 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER43_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER43_IRQ_PRIORITY       7
#endif

/**
 * @brief   PWMD44 interrupt priority level setting.
 */
#if !defined(GD32_PWM_TIMER44_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_PWM_TIMER44_IRQ_PRIORITY       7
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

#if GD32_PWM_USE_TIMER0 && !GD32_HAS_TIMER0
#error "TIMER0 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER1 && !GD32_HAS_TIMER1
#error "TIMER1 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER2 && !GD32_HAS_TIMER2
#error "TIMER2 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER3 && !GD32_HAS_TIMER3
#error "TIMER3 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER4 && !GD32_HAS_TIMER4
#error "TIMER4 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER7 && !GD32_HAS_TIMER7
#error "TIMER7 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER14 && !GD32_HAS_TIMER14
#error "TIMER14 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER15 && !GD32_HAS_TIMER15
#error "TIMER15 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER16 && !GD32_HAS_TIMER16
#error "TIMER16 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER22 && !GD32_HAS_TIMER22
#error "TIMER22 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER23 && !GD32_HAS_TIMER23
#error "TIMER23 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER30 && !GD32_HAS_TIMER30
#error "TIMER30 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER31 && !GD32_HAS_TIMER31
#error "TIMER31 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER40 && !GD32_HAS_TIMER40
#error "TIMER40 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER41 && !GD32_HAS_TIMER41
#error "TIMER41 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER42 && !GD32_HAS_TIMER42
#error "TIMER42 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER43 && !GD32_HAS_TIMER43
#error "TIMER43 not present in the selected device"
#endif

#if GD32_PWM_USE_TIMER44 && !GD32_HAS_TIMER44
#error "TIMER44 not present in the selected device"
#endif

#if !GD32_PWM_USE_TIMER0  && !GD32_PWM_USE_TIMER1  &&                       \
    !GD32_PWM_USE_TIMER2  && !GD32_PWM_USE_TIMER3  &&                       \
    !GD32_PWM_USE_TIMER4  && !GD32_PWM_USE_TIMER7  &&                       \
    !GD32_PWM_USE_TIMER14 && !GD32_PWM_USE_TIMER15 &&                       \
    !GD32_PWM_USE_TIMER16 && !GD32_PWM_USE_TIMER22 &&                       \
    !GD32_PWM_USE_TIMER23 && !GD32_PWM_USE_TIMER30 &&                       \
    !GD32_PWM_USE_TIMER31 && !GD32_PWM_USE_TIMER40 &&                       \
    !GD32_PWM_USE_TIMER41 && !GD32_PWM_USE_TIMER42 &&                       \
    !GD32_PWM_USE_TIMER43 && !GD32_PWM_USE_TIMER44
#error "PWM driver activated but no TIMER peripheral assigned"
#endif

/* Checks on allocation of TIMERx units.*/
#if GD32_PWM_USE_TIMER0
#if defined(GD32_TIMER0_IS_USED)
#error "PWMD0 requires TIMER0 but the timer is already used"
#else
#define GD32_TIMER0_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER1
#if defined(GD32_TIMER1_IS_USED)
#error "PWMD1 requires TIMER1 but the timer is already used"
#else
#define GD32_TIMER1_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER2
#if defined(GD32_TIMER2_IS_USED)
#error "PWMD2 requires TIMER2 but the timer is already used"
#else
#define GD32_TIMER2_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER3
#if defined(GD32_TIMER3_IS_USED)
#error "PWMD3 requires TIMER3 but the timer is already used"
#else
#define GD32_TIMER3_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER4
#if defined(GD32_TIMER4_IS_USED)
#error "PWMD4 requires TIMER4 but the timer is already used"
#else
#define GD32_TIMER4_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER7
#if defined(GD32_TIMER7_IS_USED)
#error "PWMD7 requires TIMER7 but the timer is already used"
#else
#define GD32_TIMER7_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER14
#if defined(GD32_TIMER14_IS_USED)
#error "PWMD14 requires TIMER14 but the timer is already used"
#else
#define GD32_TIMER14_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER15
#if defined(GD32_TIMER15_IS_USED)
#error "PWMD15 requires TIMER15 but the timer is already used"
#else
#define GD32_TIMER15_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER16
#if defined(GD32_TIMER16_IS_USED)
#error "PWMD16 requires TIMER16 but the timer is already used"
#else
#define GD32_TIMER16_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER22
#if defined(GD32_TIMER22_IS_USED)
#error "PWMD22 requires TIMER22 but the timer is already used"
#else
#define GD32_TIMER22_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER23
#if defined(GD32_TIMER23_IS_USED)
#error "PWMD23 requires TIMER23 but the timer is already used"
#else
#define GD32_TIMER23_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER30
#if defined(GD32_TIMER30_IS_USED)
#error "PWMD30 requires TIMER30 but the timer is already used"
#else
#define GD32_TIMER30_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER31
#if defined(GD32_TIMER31_IS_USED)
#error "PWMD31 requires TIMER31 but the timer is already used"
#else
#define GD32_TIMER31_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER40
#if defined(GD32_TIMER40_IS_USED)
#error "PWMD40 requires TIMER40 but the timer is already used"
#else
#define GD32_TIMER40_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER41
#if defined(GD32_TIMER41_IS_USED)
#error "PWMD41 requires TIMER41 but the timer is already used"
#else
#define GD32_TIMER41_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER42
#if defined(GD32_TIMER42_IS_USED)
#error "PWMD42 requires TIMER42 but the timer is already used"
#else
#define GD32_TIMER42_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER43
#if defined(GD32_TIMER43_IS_USED)
#error "PWMD43 requires TIMER43 but the timer is already used"
#else
#define GD32_TIMER43_IS_USED
#endif
#endif

#if GD32_PWM_USE_TIMER44
#if defined(GD32_TIMER44_IS_USED)
#error "PWMD44 requires TIMER44 but the timer is already used"
#else
#define GD32_TIMER44_IS_USED
#endif
#endif

/* IRQ priority checks.*/
#if GD32_PWM_USE_TIMER0 && !defined(GD32_TIMER0_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER0_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER0"
#endif

#if GD32_PWM_USE_TIMER1 && !defined(GD32_TIMER1_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER1"
#endif

#if GD32_PWM_USE_TIMER2 && !defined(GD32_TIMER2_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER2_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER2"
#endif

#if GD32_PWM_USE_TIMER3 && !defined(GD32_TIMER3_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER3_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER3"
#endif

#if GD32_PWM_USE_TIMER4 && !defined(GD32_TIMER4_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER4_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER4"
#endif

#if GD32_PWM_USE_TIMER7 && !defined(GD32_TIMER7_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER7_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER7"
#endif

#if GD32_PWM_USE_TIMER14 && !defined(GD32_TIMER14_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER14_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER14"
#endif

#if GD32_PWM_USE_TIMER15 && !defined(GD32_TIMER15_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER15_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER15"
#endif

#if GD32_PWM_USE_TIMER16 && !defined(GD32_TIMER16_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER16_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER16"
#endif

#if GD32_PWM_USE_TIMER22 && !defined(GD32_TIMER22_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER22_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER22"
#endif

#if GD32_PWM_USE_TIMER23 && !defined(GD32_TIMER23_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER23_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER23"
#endif

#if GD32_PWM_USE_TIMER30 && !defined(GD32_TIMER30_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER30_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER30"
#endif

#if GD32_PWM_USE_TIMER31 && !defined(GD32_TIMER31_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER31_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER31"
#endif

#if GD32_PWM_USE_TIMER40 && !defined(GD32_TIMER40_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER40_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER40"
#endif

#if GD32_PWM_USE_TIMER41 && !defined(GD32_TIMER41_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER41_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER41"
#endif

#if GD32_PWM_USE_TIMER42 && !defined(GD32_TIMER42_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER42_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER42"
#endif

#if GD32_PWM_USE_TIMER43 && !defined(GD32_TIMER43_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER43_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER43"
#endif

#if GD32_PWM_USE_TIMER44 && !defined(GD32_TIMER44_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_PWM_TIMER44_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER44"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a PWM mode.
 */
typedef uint32_t pwmmode_t;

/**
 * @brief   Type of a PWM channel.
 */
typedef uint8_t pwmchannel_t;

/**
 * @brief   Type of a channels mask.
 */
typedef uint32_t pwmchnmsk_t;

/**
 * @brief   Type of a PWM counter.
 */
typedef uint32_t pwmcnt_t;

/**
 * @brief   Type of a PWM driver channel configuration structure.
 */
typedef struct {
  /**
   * @brief Channel active logic level.
   */
  pwmmode_t                 mode;
  /**
   * @brief Channel callback pointer.
   * @note  This callback is invoked on the channel compare event. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             callback;
  /* End of the mandatory fields.*/
} PWMChannelConfig;

/**
 * @brief   Type of a PWM driver configuration structure.
 */
typedef struct {
  /**
   * @brief   Timer clock in Hz.
   * @note    The low level can use assertions in order to catch invalid
   *          frequency specifications.
   */
  uint32_t                  frequency;
  /**
   * @brief   PWM period in ticks.
   * @note    The low level can use assertions in order to catch invalid
   *          period specifications.
   */
  pwmcnt_t                  period;
  /**
   * @brief Periodic callback pointer.
   * @note  This callback is invoked on PWM counter reset. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             callback;
  /**
   * @brief Channels configurations.
   */
  PWMChannelConfig          channels[PWM_CHANNELS];
  /* End of the mandatory fields.*/
  /**
   * @brief TIMER CTL1 register initialization data.
   * @note  The value of this field should normally be equal to zero.
   */
  uint32_t                  ctl1;
  /**
   * @brief TIMER CCHP (complementary channel protection) register
   *        initialization data.
   * @note  The value of this field should normally be equal to zero.
   */
  uint32_t                  cchp;
  /**
   * @brief TIMER DMAINTEN register initialization data.
   * @note  The value of this field should normally be equal to zero.
   * @note  Only the DMA-related bits can be specified in this field.
   */
  uint32_t                  dmainten;
} PWMConfig;

/**
 * @brief   Structure representing a PWM driver.
 */
struct PWMDriver {
  /**
   * @brief Driver state.
   */
  pwmstate_t                state;
  /**
   * @brief Current driver configuration data.
   */
  const PWMConfig           *config;
  /**
   * @brief   Current PWM period in ticks.
   */
  pwmcnt_t                  period;
  /**
   * @brief   Mask of the enabled channels.
   */
  pwmchnmsk_t               enabled;
  /**
   * @brief   Number of channels in this instance.
   */
  pwmchannel_t              channels;
#if defined(PWM_DRIVER_EXT_FIELDS)
  PWM_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief Timer base clock.
   */
  uint32_t                  clock;
  /**
   * @brief Presence of CCHP register.
   */
  bool                      has_cchp;
  /**
   * @brief TIMERx registers block reference.
   */
  uint32_t                  tim;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Changes the period the PWM peripheral.
 * @details This function changes the period of a PWM unit that has already
 *          been activated using @p pwmStart().
 * @pre     The PWM unit must have been activated using @p pwmStart().
 * @post    The PWM unit period is changed to the new value.
 * @note    The function has effect at the next cycle start.
 * @note    If a period is specified that is shorter than the pulse width
 *          programmed in one of the channels then the behavior is not
 *          guaranteed.
 *
 * @param[in] pwmp      pointer to a @p PWMDriver object
 * @param[in] period    new cycle time in ticks
 *
 * @notapi
 */
#define pwm_lld_change_period(pwmp, period)                                 \
  (TIMER_CAR((pwmp)->tim) = ((period) - 1))

/**
 * @brief   Returns the TIMER associated with a PWM.
 *
 * @param[in] pwmp      pointer to the @p PWMDriver object
 * @return              The TIMER reference.
 *
 * @notapi
 */
#define pwm_lld_get_timer(pwmp) ((pwmp)->tim)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if GD32_PWM_USE_TIMER0 && !defined(__DOXYGEN__)
extern PWMDriver PWMD0;
#endif

#if GD32_PWM_USE_TIMER1 && !defined(__DOXYGEN__)
extern PWMDriver PWMD1;
#endif

#if GD32_PWM_USE_TIMER2 && !defined(__DOXYGEN__)
extern PWMDriver PWMD2;
#endif

#if GD32_PWM_USE_TIMER3 && !defined(__DOXYGEN__)
extern PWMDriver PWMD3;
#endif

#if GD32_PWM_USE_TIMER4 && !defined(__DOXYGEN__)
extern PWMDriver PWMD4;
#endif

#if GD32_PWM_USE_TIMER7 && !defined(__DOXYGEN__)
extern PWMDriver PWMD7;
#endif

#if GD32_PWM_USE_TIMER14 && !defined(__DOXYGEN__)
extern PWMDriver PWMD14;
#endif

#if GD32_PWM_USE_TIMER15 && !defined(__DOXYGEN__)
extern PWMDriver PWMD15;
#endif

#if GD32_PWM_USE_TIMER16 && !defined(__DOXYGEN__)
extern PWMDriver PWMD16;
#endif

#if GD32_PWM_USE_TIMER22 && !defined(__DOXYGEN__)
extern PWMDriver PWMD22;
#endif

#if GD32_PWM_USE_TIMER23 && !defined(__DOXYGEN__)
extern PWMDriver PWMD23;
#endif

#if GD32_PWM_USE_TIMER30 && !defined(__DOXYGEN__)
extern PWMDriver PWMD30;
#endif

#if GD32_PWM_USE_TIMER31 && !defined(__DOXYGEN__)
extern PWMDriver PWMD31;
#endif

#if GD32_PWM_USE_TIMER40 && !defined(__DOXYGEN__)
extern PWMDriver PWMD40;
#endif

#if GD32_PWM_USE_TIMER41 && !defined(__DOXYGEN__)
extern PWMDriver PWMD41;
#endif

#if GD32_PWM_USE_TIMER42 && !defined(__DOXYGEN__)
extern PWMDriver PWMD42;
#endif

#if GD32_PWM_USE_TIMER43 && !defined(__DOXYGEN__)
extern PWMDriver PWMD43;
#endif

#if GD32_PWM_USE_TIMER44 && !defined(__DOXYGEN__)
extern PWMDriver PWMD44;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void pwm_lld_init(void);
  void pwm_lld_start(PWMDriver *pwmp);
  void pwm_lld_stop(PWMDriver *pwmp);
  void pwm_lld_enable_channel(PWMDriver *pwmp,
                              pwmchannel_t channel,
                              pwmcnt_t width);
  void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel);
  void pwm_lld_enable_periodic_notification(PWMDriver *pwmp);
  void pwm_lld_disable_periodic_notification(PWMDriver *pwmp);
  void pwm_lld_enable_channel_notification(PWMDriver *pwmp,
                                           pwmchannel_t channel);
  void pwm_lld_disable_channel_notification(PWMDriver *pwmp,
                                            pwmchannel_t channel);
  void pwm_lld_serve_interrupt(PWMDriver *pwmp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PWM */

#endif /* HAL_PWM_LLD_H */

/** @} */
