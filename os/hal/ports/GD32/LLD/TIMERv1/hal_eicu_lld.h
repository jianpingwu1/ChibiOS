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
   Rewritten by Emil Fresk (1/5 - 2014) for extended input capture
   functionality. And fix for spurious callbacks in the interrupt handler.
*/
/*
   Improved by Uladzimir Pylinsky aka barthess (1/3 - 2015) for support of
   32-bit timers and timers with single capture/compare channels.
*/

#ifndef HAL_EICU_LLD_H
#define HAL_EICU_LLD_H

#include "gd32_timer.h"

#if (HAL_USE_EICU == TRUE) || defined(__DOXYGEN__)

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
 * @brief   EICUD0 driver enable switch.
 * @details If set to @p TRUE the support for EICUD0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER0) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER0                 FALSE
#endif

/**
 * @brief   EICUD1 driver enable switch.
 * @details If set to @p TRUE the support for EICUD1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER1) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER1                 FALSE
#endif

/**
 * @brief   EICUD2 driver enable switch.
 * @details If set to @p TRUE the support for EICUD2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER2) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER2                 FALSE
#endif

/**
 * @brief   EICUD3 driver enable switch.
 * @details If set to @p TRUE the support for EICUD3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER3) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER3                 FALSE
#endif

/**
 * @brief   EICUD4 driver enable switch.
 * @details If set to @p TRUE the support for EICUD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER4) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER4                 FALSE
#endif

/**
 * @brief   EICUD7 driver enable switch.
 * @details If set to @p TRUE the support for EICUD7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER7) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER7                 FALSE
#endif

/**
 * @brief   EICUD14 driver enable switch.
 * @details If set to @p TRUE the support for EICUD14 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER14) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER14                FALSE
#endif

/**
 * @brief   EICUD15 driver enable switch.
 * @details If set to @p TRUE the support for EICUD15 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER15) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER15                FALSE
#endif

/**
 * @brief   EICUD16 driver enable switch.
 * @details If set to @p TRUE the support for EICUD16 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER16) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER16                FALSE
#endif

/**
 * @brief   EICUD22 driver enable switch.
 * @details If set to @p TRUE the support for EICUD22 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER22) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER22                FALSE
#endif

/**
 * @brief   EICUD23 driver enable switch.
 * @details If set to @p TRUE the support for EICUD23 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER23) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER23                FALSE
#endif

/**
 * @brief   EICUD30 driver enable switch.
 * @details If set to @p TRUE the support for EICUD30 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER30) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER30                FALSE
#endif

/**
 * @brief   EICUD31 driver enable switch.
 * @details If set to @p TRUE the support for EICUD31 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER31) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER31                FALSE
#endif

/**
 * @brief   EICUD40 driver enable switch.
 * @details If set to @p TRUE the support for EICUD40 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER40) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER40                FALSE
#endif

/**
 * @brief   EICUD41 driver enable switch.
 * @details If set to @p TRUE the support for EICUD41 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER41) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER41                FALSE
#endif

/**
 * @brief   EICUD42 driver enable switch.
 * @details If set to @p TRUE the support for EICUD42 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER42) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER42                FALSE
#endif

/**
 * @brief   EICUD43 driver enable switch.
 * @details If set to @p TRUE the support for EICUD43 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER43) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER43                FALSE
#endif

/**
 * @brief   EICUD44 driver enable switch.
 * @details If set to @p TRUE the support for EICUD44 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_EICU_USE_TIMER44) || defined(__DOXYGEN__)
#define GD32_EICU_USE_TIMER44                FALSE
#endif

/**
 * @brief   EICUD0 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER0_IRQ_PRIORITY        7
#endif

/**
 * @brief   EICUD1 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER1_IRQ_PRIORITY        7
#endif

/**
 * @brief   EICUD2 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER2_IRQ_PRIORITY        7
#endif

/**
 * @brief   EICUD3 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER3_IRQ_PRIORITY        7
#endif

/**
 * @brief   EICUD4 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER4_IRQ_PRIORITY        7
#endif

/**
 * @brief   EICUD7 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER7_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER7_IRQ_PRIORITY        7
#endif

/**
 * @brief   EICUD14 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER14_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER14_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD15 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER15_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER15_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD16 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER16_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER16_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD22 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER22_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER22_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD23 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER23_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER23_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD30 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER30_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER30_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD31 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER31_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER31_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD40 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER40_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER40_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD41 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER41_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER41_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD42 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER42_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER42_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD43 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER43_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER43_IRQ_PRIORITY       7
#endif

/**
 * @brief   EICUD44 interrupt priority level setting.
 */
#if !defined(GD32_EICU_TIMER44_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_EICU_TIMER44_IRQ_PRIORITY       7
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if GD32_EICU_USE_TIMER0 && !GD32_HAS_TIMER0
#error "TIMER0 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER1 && !GD32_HAS_TIMER1
#error "TIMER1 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER2 && !GD32_HAS_TIMER2
#error "TIMER2 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER3 && !GD32_HAS_TIMER3
#error "TIMER3 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER4 && !GD32_HAS_TIMER4
#error "TIMER4 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER7 && !GD32_HAS_TIMER7
#error "TIMER7 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER14 && !GD32_HAS_TIMER14
#error "TIMER14 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER15 && !GD32_HAS_TIMER15
#error "TIMER15 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER16 && !GD32_HAS_TIMER16
#error "TIMER16 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER22 && !GD32_HAS_TIMER22
#error "TIMER22 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER23 && !GD32_HAS_TIMER23
#error "TIMER23 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER30 && !GD32_HAS_TIMER30
#error "TIMER30 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER31 && !GD32_HAS_TIMER31
#error "TIMER31 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER40 && !GD32_HAS_TIMER40
#error "TIMER40 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER41 && !GD32_HAS_TIMER41
#error "TIMER41 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER42 && !GD32_HAS_TIMER42
#error "TIMER42 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER43 && !GD32_HAS_TIMER43
#error "TIMER43 not present in the selected device"
#endif

#if GD32_EICU_USE_TIMER44 && !GD32_HAS_TIMER44
#error "TIMER44 not present in the selected device"
#endif

#if !GD32_EICU_USE_TIMER0  && !GD32_EICU_USE_TIMER1  &&                       \
    !GD32_EICU_USE_TIMER2  && !GD32_EICU_USE_TIMER3  &&                       \
    !GD32_EICU_USE_TIMER4  && !GD32_EICU_USE_TIMER7  &&                       \
    !GD32_EICU_USE_TIMER14 && !GD32_EICU_USE_TIMER15 &&                       \
    !GD32_EICU_USE_TIMER16 && !GD32_EICU_USE_TIMER22 &&                       \
    !GD32_EICU_USE_TIMER23 && !GD32_EICU_USE_TIMER30 &&                       \
    !GD32_EICU_USE_TIMER31 && !GD32_EICU_USE_TIMER40 &&                       \
    !GD32_EICU_USE_TIMER41 && !GD32_EICU_USE_TIMER42 &&                       \
    !GD32_EICU_USE_TIMER43 && !GD32_EICU_USE_TIMER44
#error "EICU driver activated but no TIMER peripheral assigned"
#endif

/* Checks on allocation of TIMERx units.*/
#if GD32_EICU_USE_TIMER0
#if defined(GD32_TIMER0_IS_USED)
#error "EICUD0 requires TIMER0 but the timer is already used"
#else
#define GD32_TIMER0_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER1
#if defined(GD32_TIMER1_IS_USED)
#error "EICUD1 requires TIMER1 but the timer is already used"
#else
#define GD32_TIMER1_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER2
#if defined(GD32_TIMER2_IS_USED)
#error "EICUD2 requires TIMER2 but the timer is already used"
#else
#define GD32_TIMER2_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER3
#if defined(GD32_TIMER3_IS_USED)
#error "EICUD3 requires TIMER3 but the timer is already used"
#else
#define GD32_TIMER3_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER4
#if defined(GD32_TIMER4_IS_USED)
#error "EICUD4 requires TIMER4 but the timer is already used"
#else
#define GD32_TIMER4_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER7
#if defined(GD32_TIMER7_IS_USED)
#error "EICUD7 requires TIMER7 but the timer is already used"
#else
#define GD32_TIMER7_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER14
#if defined(GD32_TIMER14_IS_USED)
#error "EICUD14 requires TIMER14 but the timer is already used"
#else
#define GD32_TIMER14_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER15
#if defined(GD32_TIMER15_IS_USED)
#error "EICUD15 requires TIMER15 but the timer is already used"
#else
#define GD32_TIMER15_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER16
#if defined(GD32_TIMER16_IS_USED)
#error "EICUD16 requires TIMER16 but the timer is already used"
#else
#define GD32_TIMER16_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER22
#if defined(GD32_TIMER22_IS_USED)
#error "EICUD22 requires TIMER22 but the timer is already used"
#else
#define GD32_TIMER22_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER23
#if defined(GD32_TIMER23_IS_USED)
#error "EICUD23 requires TIMER23 but the timer is already used"
#else
#define GD32_TIMER23_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER30
#if defined(GD32_TIMER30_IS_USED)
#error "EICUD30 requires TIMER30 but the timer is already used"
#else
#define GD32_TIMER30_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER31
#if defined(GD32_TIMER31_IS_USED)
#error "EICUD31 requires TIMER31 but the timer is already used"
#else
#define GD32_TIMER31_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER40
#if defined(GD32_TIMER40_IS_USED)
#error "EICUD40 requires TIMER40 but the timer is already used"
#else
#define GD32_TIMER40_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER41
#if defined(GD32_TIMER41_IS_USED)
#error "EICUD41 requires TIMER41 but the timer is already used"
#else
#define GD32_TIMER41_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER42
#if defined(GD32_TIMER42_IS_USED)
#error "EICUD42 requires TIMER42 but the timer is already used"
#else
#define GD32_TIMER42_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER43
#if defined(GD32_TIMER43_IS_USED)
#error "EICUD43 requires TIMER43 but the timer is already used"
#else
#define GD32_TIMER43_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER44
#if defined(GD32_TIMER44_IS_USED)
#error "EICUD44 requires TIMER44 but the timer is already used"
#else
#define GD32_TIMER44_IS_USED
#endif
#endif

#if GD32_EICU_USE_TIMER0 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER0_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER0"
#endif

#if GD32_EICU_USE_TIMER1 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER1"
#endif

#if GD32_EICU_USE_TIMER2 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER2_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER2"
#endif

#if GD32_EICU_USE_TIMER3 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER3_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER3"
#endif

#if GD32_EICU_USE_TIMER4 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER4_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER4"
#endif

#if GD32_EICU_USE_TIMER7 &&                                                   \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER7_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER7"
#endif

#if GD32_EICU_USE_TIMER14 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER14_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER14"
#endif

#if GD32_EICU_USE_TIMER15 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER15_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER15"
#endif

#if GD32_EICU_USE_TIMER16 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER16_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER16"
#endif

#if GD32_EICU_USE_TIMER22 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER22_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER22"
#endif

#if GD32_EICU_USE_TIMER23 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER23_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER23"
#endif

#if GD32_EICU_USE_TIMER30 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER30_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER30"
#endif

#if GD32_EICU_USE_TIMER31 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER31_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER31"
#endif

#if GD32_EICU_USE_TIMER40 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER40_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER40"
#endif

#if GD32_EICU_USE_TIMER41 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER41_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER41"
#endif

#if GD32_EICU_USE_TIMER42 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER42_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER42"
#endif

#if GD32_EICU_USE_TIMER43 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER43_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER43"
#endif

#if GD32_EICU_USE_TIMER44 &&                                                  \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_EICU_TIMER44_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER44"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/
/**
 * @brief   Active level selector.
 */
typedef enum {
  EICU_INPUT_ACTIVE_HIGH,           /**< Trigger on rising edge.            */
  EICU_INPUT_ACTIVE_LOW,            /**< Trigger on falling edge.           */
} eicuactivelevel_t;

/**
 * @brief   Timer registers width in bits.
 */
typedef enum {
  EICU_WIDTH_16,
  EICU_WIDTH_32
} eicutimerwidth_t;

/**
 * @brief   EICU frequency type.
 */
typedef uint32_t eicufreq_t;

/**
 * @brief   EICU counter type.
 */
typedef uint32_t eicucnt_t;

/**
 * @brief EICU captured width and (or) period.
 */
typedef struct {
  /**
   * @brief   Pulse width.
   */
  eicucnt_t               width;
  /**
   * @brief   Pulse period.
   */
  eicucnt_t               period;
} eicuresult_t;

/**
 * @brief EICU Capture Channel Config structure definition.
 */
typedef struct {
  /**
   * @brief   Specifies the active level of the input signal.
   */
  eicuactivelevel_t       alvl;
  /**
   * @brief   Capture event callback. Used for PWM width, pulse width and
   *          pulse period capture event.
   */
  eicucallback_t          capture_cb;
} EICUChannelConfig;

/**
 * @brief EICU Capture Channel structure definition.
 */
typedef struct {
  /**
   * @brief   Channel state for the internal state machine.
   */
  eicuchannelstate_t      state;
  /**
   * @brief   Cached value for pulse width calculation.
   */
  eicucnt_t               last_active;
  /**
   * @brief   Cached value for period calculation.
   */
  eicucnt_t               last_idle;
  /**
   * @brief   Pointer to Input Capture channel configuration.
   */
  const EICUChannelConfig *config;
  /**
   * @brief   CHxCV register pointer for faster access.
   */
  volatile uint32_t       *ccrp;
} EICUChannel;

/**
 * @brief EICU Config structure definition.
 */
typedef struct {
  /**
   * @brief   Specifies the Timer clock in Hz.
   */
  eicufreq_t              frequency;
  /**
   * @brief   Pointer to each Input Capture channel configuration.
   * @note    A NULL parameter indicates the channel as unused.
   * @note    In PWM mode, only Channel 1 OR Channel 2 may be used.
   */
  const EICUChannelConfig *iccfgp[EICU_CHANNEL_ENUM_END];
  /**
   * @brief   TIMER DMAINTEN register initialization data.
   */
  uint32_t                dmainten;
} EICUConfig;

/**
 * @brief EICU Driver structure definition
 */
struct EICUDriver {
  /**
   * @brief   TIMERx registers block reference.
   */
  uint32_t                tim;
  /**
   * @brief   Driver state for the internal state machine.
   */
  eicustate_t             state;
  /**
   * @brief   Channels' data structures.
   */
  EICUChannel             channel[EICU_CHANNEL_ENUM_END];
  /**
   * @brief   Timer base clock.
   */
  uint32_t                clock;
  /**
   * @brief   Number of available capture compare channels in timer.
   */
  size_t                  channels;
  /**
   * @brief   Timer registers width in bits.
   */
  eicutimerwidth_t        width;
    /**
     * @brief   SYSCFG TIMER index used to clear slave mode configuration,
     *          @p GD32_TIMER_SYSCFG_NONE on instances without a slave mode
     *          controller.
     */
  uint8_t                 syscfg;
  /**
   * @brief   Pointer to configuration for the driver.
   */
  const EICUConfig        *config;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/
#if GD32_EICU_USE_TIMER0 && !defined(__DOXYGEN__)
extern EICUDriver EICUD0;
#endif

#if GD32_EICU_USE_TIMER1 && !defined(__DOXYGEN__)
extern EICUDriver EICUD1;
#endif

#if GD32_EICU_USE_TIMER2 && !defined(__DOXYGEN__)
extern EICUDriver EICUD2;
#endif

#if GD32_EICU_USE_TIMER3 && !defined(__DOXYGEN__)
extern EICUDriver EICUD3;
#endif

#if GD32_EICU_USE_TIMER4 && !defined(__DOXYGEN__)
extern EICUDriver EICUD4;
#endif

#if GD32_EICU_USE_TIMER7 && !defined(__DOXYGEN__)
extern EICUDriver EICUD7;
#endif

#if GD32_EICU_USE_TIMER14 && !defined(__DOXYGEN__)
extern EICUDriver EICUD14;
#endif

#if GD32_EICU_USE_TIMER15 && !defined(__DOXYGEN__)
extern EICUDriver EICUD15;
#endif

#if GD32_EICU_USE_TIMER16 && !defined(__DOXYGEN__)
extern EICUDriver EICUD16;
#endif

#if GD32_EICU_USE_TIMER22 && !defined(__DOXYGEN__)
extern EICUDriver EICUD22;
#endif

#if GD32_EICU_USE_TIMER23 && !defined(__DOXYGEN__)
extern EICUDriver EICUD23;
#endif

#if GD32_EICU_USE_TIMER30 && !defined(__DOXYGEN__)
extern EICUDriver EICUD30;
#endif

#if GD32_EICU_USE_TIMER31 && !defined(__DOXYGEN__)
extern EICUDriver EICUD31;
#endif

#if GD32_EICU_USE_TIMER40 && !defined(__DOXYGEN__)
extern EICUDriver EICUD40;
#endif

#if GD32_EICU_USE_TIMER41 && !defined(__DOXYGEN__)
extern EICUDriver EICUD41;
#endif

#if GD32_EICU_USE_TIMER42 && !defined(__DOXYGEN__)
extern EICUDriver EICUD42;
#endif

#if GD32_EICU_USE_TIMER43 && !defined(__DOXYGEN__)
extern EICUDriver EICUD43;
#endif

#if GD32_EICU_USE_TIMER44 && !defined(__DOXYGEN__)
extern EICUDriver EICUD44;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void eicu_lld_init(void);
  void eicu_lld_start(EICUDriver *eicup);
  void eicu_lld_stop(EICUDriver *eicup);
  void eicu_lld_enable(EICUDriver *eicup);
  void eicu_lld_disable(EICUDriver *eicup);
  void eicu_lld_serve_interrupt(EICUDriver *eicup);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_EICU */

#endif /* HAL_EICU_LLD_H */

/** @} */
