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
 * @file    TIMERv1/hal_icu_lld.h
 * @brief   GD32 ICU subsystem low level driver header.
 *
 * @addtogroup ICU
 * @{
 */

#ifndef HAL_ICU_LLD_H
#define HAL_ICU_LLD_H

#include "gd32_timer.h"

#if HAL_USE_ICU || defined(__DOXYGEN__)

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
 * @brief   ICUD0 driver enable switch.
 * @details If set to @p TRUE the support for ICUD0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER0) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER0                 FALSE
#endif

/**
 * @brief   ICUD1 driver enable switch.
 * @details If set to @p TRUE the support for ICUD1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER1) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER1                 FALSE
#endif

/**
 * @brief   ICUD2 driver enable switch.
 * @details If set to @p TRUE the support for ICUD2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER2) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER2                 FALSE
#endif

/**
 * @brief   ICUD3 driver enable switch.
 * @details If set to @p TRUE the support for ICUD3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER3) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER3                 FALSE
#endif

/**
 * @brief   ICUD4 driver enable switch.
 * @details If set to @p TRUE the support for ICUD4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER4) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER4                 FALSE
#endif

/**
 * @brief   ICUD7 driver enable switch.
 * @details If set to @p TRUE the support for ICUD7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER7) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER7                 FALSE
#endif

/**
 * @brief   ICUD14 driver enable switch.
 * @details If set to @p TRUE the support for ICUD14 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER14) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER14                FALSE
#endif

/**
 * @brief   ICUD22 driver enable switch.
 * @details If set to @p TRUE the support for ICUD22 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER22) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER22                FALSE
#endif

/**
 * @brief   ICUD23 driver enable switch.
 * @details If set to @p TRUE the support for ICUD23 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER23) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER23                FALSE
#endif

/**
 * @brief   ICUD30 driver enable switch.
 * @details If set to @p TRUE the support for ICUD30 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER30) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER30                FALSE
#endif

/**
 * @brief   ICUD31 driver enable switch.
 * @details If set to @p TRUE the support for ICUD31 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER31) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER31                FALSE
#endif

/**
 * @brief   ICUD40 driver enable switch.
 * @details If set to @p TRUE the support for ICUD40 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER40) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER40                FALSE
#endif

/**
 * @brief   ICUD41 driver enable switch.
 * @details If set to @p TRUE the support for ICUD41 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER41) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER41                FALSE
#endif

/**
 * @brief   ICUD42 driver enable switch.
 * @details If set to @p TRUE the support for ICUD42 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER42) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER42                FALSE
#endif

/**
 * @brief   ICUD43 driver enable switch.
 * @details If set to @p TRUE the support for ICUD43 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER43) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER43                FALSE
#endif

/**
 * @brief   ICUD44 driver enable switch.
 * @details If set to @p TRUE the support for ICUD44 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_ICU_USE_TIMER44) || defined(__DOXYGEN__)
#define GD32_ICU_USE_TIMER44                FALSE
#endif

/**
 * @brief   ICUD0 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER0_IRQ_PRIORITY        7
#endif

/**
 * @brief   ICUD1 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER1_IRQ_PRIORITY        7
#endif

/**
 * @brief   ICUD2 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER2_IRQ_PRIORITY        7
#endif

/**
 * @brief   ICUD3 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER3_IRQ_PRIORITY        7
#endif

/**
 * @brief   ICUD4 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER4_IRQ_PRIORITY        7
#endif

/**
 * @brief   ICUD7 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER7_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER7_IRQ_PRIORITY        7
#endif

/**
 * @brief   ICUD14 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER14_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER14_IRQ_PRIORITY       7
#endif

/**
 * @brief   ICUD22 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER22_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER22_IRQ_PRIORITY       7
#endif

/**
 * @brief   ICUD23 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER23_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER23_IRQ_PRIORITY       7
#endif

/**
 * @brief   ICUD30 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER30_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER30_IRQ_PRIORITY       7
#endif

/**
 * @brief   ICUD31 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER31_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER31_IRQ_PRIORITY       7
#endif

/**
 * @brief   ICUD40 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER40_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER40_IRQ_PRIORITY       7
#endif

/**
 * @brief   ICUD41 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER41_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER41_IRQ_PRIORITY       7
#endif

/**
 * @brief   ICUD42 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER42_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER42_IRQ_PRIORITY       7
#endif

/**
 * @brief   ICUD43 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER43_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER43_IRQ_PRIORITY       7
#endif

/**
 * @brief   ICUD44 interrupt priority level setting.
 */
#if !defined(GD32_ICU_TIMER44_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ICU_TIMER44_IRQ_PRIORITY       7
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

#if GD32_ICU_USE_TIMER0 && !GD32_HAS_TIMER0
#error "TIMER0 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER1 && !GD32_HAS_TIMER1
#error "TIMER1 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER2 && !GD32_HAS_TIMER2
#error "TIMER2 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER3 && !GD32_HAS_TIMER3
#error "TIMER3 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER4 && !GD32_HAS_TIMER4
#error "TIMER4 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER7 && !GD32_HAS_TIMER7
#error "TIMER7 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER14 && !GD32_HAS_TIMER14
#error "TIMER14 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER22 && !GD32_HAS_TIMER22
#error "TIMER22 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER23 && !GD32_HAS_TIMER23
#error "TIMER23 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER30 && !GD32_HAS_TIMER30
#error "TIMER30 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER31 && !GD32_HAS_TIMER31
#error "TIMER31 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER40 && !GD32_HAS_TIMER40
#error "TIMER40 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER41 && !GD32_HAS_TIMER41
#error "TIMER41 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER42 && !GD32_HAS_TIMER42
#error "TIMER42 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER43 && !GD32_HAS_TIMER43
#error "TIMER43 not present in the selected device"
#endif

#if GD32_ICU_USE_TIMER44 && !GD32_HAS_TIMER44
#error "TIMER44 not present in the selected device"
#endif

#if !GD32_ICU_USE_TIMER0  && !GD32_ICU_USE_TIMER1  &&                       \
    !GD32_ICU_USE_TIMER2  && !GD32_ICU_USE_TIMER3  &&                       \
    !GD32_ICU_USE_TIMER4  && !GD32_ICU_USE_TIMER7  &&                       \
    !GD32_ICU_USE_TIMER14 && !GD32_ICU_USE_TIMER22 &&                       \
    !GD32_ICU_USE_TIMER23 && !GD32_ICU_USE_TIMER30 &&                       \
    !GD32_ICU_USE_TIMER31 && !GD32_ICU_USE_TIMER40 &&                       \
    !GD32_ICU_USE_TIMER41 && !GD32_ICU_USE_TIMER42 &&                       \
    !GD32_ICU_USE_TIMER43 && !GD32_ICU_USE_TIMER44
#error "ICU driver activated but no TIMER peripheral assigned"
#endif

/* Checks on allocation of TIMERx units.*/
#if GD32_ICU_USE_TIMER0
#if defined(GD32_TIMER0_IS_USED)
#error "ICUD0 requires TIMER0 but the timer is already used"
#else
#define GD32_TIMER0_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER1
#if defined(GD32_TIMER1_IS_USED)
#error "ICUD1 requires TIMER1 but the timer is already used"
#else
#define GD32_TIMER1_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER2
#if defined(GD32_TIMER2_IS_USED)
#error "ICUD2 requires TIMER2 but the timer is already used"
#else
#define GD32_TIMER2_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER3
#if defined(GD32_TIMER3_IS_USED)
#error "ICUD3 requires TIMER3 but the timer is already used"
#else
#define GD32_TIMER3_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER4
#if defined(GD32_TIMER4_IS_USED)
#error "ICUD4 requires TIMER4 but the timer is already used"
#else
#define GD32_TIMER4_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER7
#if defined(GD32_TIMER7_IS_USED)
#error "ICUD7 requires TIMER7 but the timer is already used"
#else
#define GD32_TIMER7_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER14
#if defined(GD32_TIMER14_IS_USED)
#error "ICUD14 requires TIMER14 but the timer is already used"
#else
#define GD32_TIMER14_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER22
#if defined(GD32_TIMER22_IS_USED)
#error "ICUD22 requires TIMER22 but the timer is already used"
#else
#define GD32_TIMER22_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER23
#if defined(GD32_TIMER23_IS_USED)
#error "ICUD23 requires TIMER23 but the timer is already used"
#else
#define GD32_TIMER23_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER30
#if defined(GD32_TIMER30_IS_USED)
#error "ICUD30 requires TIMER30 but the timer is already used"
#else
#define GD32_TIMER30_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER31
#if defined(GD32_TIMER31_IS_USED)
#error "ICUD31 requires TIMER31 but the timer is already used"
#else
#define GD32_TIMER31_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER40
#if defined(GD32_TIMER40_IS_USED)
#error "ICUD40 requires TIMER40 but the timer is already used"
#else
#define GD32_TIMER40_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER41
#if defined(GD32_TIMER41_IS_USED)
#error "ICUD41 requires TIMER41 but the timer is already used"
#else
#define GD32_TIMER41_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER42
#if defined(GD32_TIMER42_IS_USED)
#error "ICUD42 requires TIMER42 but the timer is already used"
#else
#define GD32_TIMER42_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER43
#if defined(GD32_TIMER43_IS_USED)
#error "ICUD43 requires TIMER43 but the timer is already used"
#else
#define GD32_TIMER43_IS_USED
#endif
#endif

#if GD32_ICU_USE_TIMER44
#if defined(GD32_TIMER44_IS_USED)
#error "ICUD44 requires TIMER44 but the timer is already used"
#else
#define GD32_TIMER44_IS_USED
#endif
#endif

/* IRQ priority checks.*/
#if GD32_ICU_USE_TIMER0 && !defined(GD32_TIMER0_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER0_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER0"
#endif

#if GD32_ICU_USE_TIMER1 && !defined(GD32_TIMER1_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER1"
#endif

#if GD32_ICU_USE_TIMER2 && !defined(GD32_TIMER2_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER2_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER2"
#endif

#if GD32_ICU_USE_TIMER3 && !defined(GD32_TIMER3_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER3_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER3"
#endif

#if GD32_ICU_USE_TIMER4 && !defined(GD32_TIMER4_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER4_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER4"
#endif

#if GD32_ICU_USE_TIMER7 && !defined(GD32_TIMER7_SUPPRESS_ISR) &&            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER7_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER7"
#endif

#if GD32_ICU_USE_TIMER14 && !defined(GD32_TIMER14_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER14_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER14"
#endif

#if GD32_ICU_USE_TIMER22 && !defined(GD32_TIMER22_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER22_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER22"
#endif

#if GD32_ICU_USE_TIMER23 && !defined(GD32_TIMER23_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER23_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER23"
#endif

#if GD32_ICU_USE_TIMER30 && !defined(GD32_TIMER30_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER30_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER30"
#endif

#if GD32_ICU_USE_TIMER31 && !defined(GD32_TIMER31_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER31_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER31"
#endif

#if GD32_ICU_USE_TIMER40 && !defined(GD32_TIMER40_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER40_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER40"
#endif

#if GD32_ICU_USE_TIMER41 && !defined(GD32_TIMER41_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER41_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER41"
#endif

#if GD32_ICU_USE_TIMER42 && !defined(GD32_TIMER42_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER42_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER42"
#endif

#if GD32_ICU_USE_TIMER43 && !defined(GD32_TIMER43_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER43_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER43"
#endif

#if GD32_ICU_USE_TIMER44 && !defined(GD32_TIMER44_SUPPRESS_ISR) &&          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_ICU_TIMER44_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to TIMER44"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   ICU driver mode.
 */
typedef enum {
  ICU_INPUT_ACTIVE_HIGH = 0,        /**< Trigger on rising edge.            */
  ICU_INPUT_ACTIVE_LOW = 1,         /**< Trigger on falling edge.           */
} icumode_t;

/**
 * @brief   ICU frequency type.
 */
typedef uint32_t icufreq_t;

/**
 * @brief   ICU channel type.
 */
typedef enum {
  ICU_CHANNEL_1 = 0,              /**< Use TIMERxCH0.      */
  ICU_CHANNEL_2 = 1,              /**< Use TIMERxCH1.      */
} icuchannel_t;

/**
 * @brief   ICU counter type.
 */
typedef uint32_t icucnt_t;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Driver mode.
   */
  icumode_t                 mode;
  /**
   * @brief   Timer clock in Hz.
   * @note    The low level can use assertions in order to catch invalid
   *          frequency specifications.
   */
  icufreq_t                 frequency;
  /**
   * @brief   Callback for pulse width measurement.
   */
  icucallback_t             width_cb;
  /**
   * @brief   Callback for cycle period measurement.
   */
  icucallback_t             period_cb;
  /**
   * @brief   Callback for timer overflow.
   */
  icucallback_t             overflow_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief   Timer input channel to be used.
   * @note    Only inputs TIMERx 0 and 1 are supported.
   */
  icuchannel_t              channel;
  /**
   * @brief TIMER DMAINTEN register initialization data.
   * @note  The value of this field should normally be equal to zero.
   * @note  Only the DMA-related bits can be specified in this field.
   */
  uint32_t                  dmainten;
  /**
   * @brief TIMER CAR register initialization data.
   * @note  The value of this field should normally be equal to 0xFFFFFFFFU.
   */
  uint32_t                  car;
} ICUConfig;

/**
 * @brief   Structure representing an ICU driver.
 */
struct ICUDriver {
  /**
   * @brief Driver state.
   */
  icustate_t                state;
  /**
   * @brief Current configuration data.
   */
  const ICUConfig           *config;
#if defined(ICU_DRIVER_EXT_FIELDS)
  ICU_DRIVER_EXT_FIELDS
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
  /**
   * @brief SYSCFG TIMER index used to configure the restart (slave) mode.
   */
  uint8_t                   syscfg;
  /**
   * @brief CHxCV register used for width capture.
   */
  volatile uint32_t         *wccrp;
  /**
   * @brief CHxCV register used for period capture.
   */
  volatile uint32_t         *pccrp;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the width of the latest pulse.
 * @details The pulse width is defined as number of ticks between the start
 *          edge and the stop edge.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The number of ticks.
 *
 * @notapi
 */
#define icu_lld_get_width(icup) (*((icup)->wccrp) + 1)

/**
 * @brief   Returns the width of the latest cycle.
 * @details The cycle width is defined as number of ticks between a start
 *          edge and the next start edge.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The number of ticks.
 *
 * @notapi
 */
#define icu_lld_get_period(icup) (*((icup)->pccrp) + 1)

/**
 * @brief   Check on notifications status.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The notifications status.
 * @retval false        if notifications are not enabled.
 * @retval true         if notifications are enabled.
 *
 * @notapi
 */
#define icu_lld_are_notifications_enabled(icup)                             \
  (bool)((TIMER_DMAINTEN((icup)->tim) & GD32_TIMER_DMAINTEN_IRQ_MASK) != 0)

/**
 * @brief   Returns the TIMER associated with an ICU.
 *
 * @param[in] icup      pointer to the @p ICUDriver object
 * @return              The TIMER reference.
 *
 * @notapi
 */
#define icu_lld_get_timer(icup) ((icup)->tim)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if GD32_ICU_USE_TIMER0 && !defined(__DOXYGEN__)
extern ICUDriver ICUD0;
#endif

#if GD32_ICU_USE_TIMER1 && !defined(__DOXYGEN__)
extern ICUDriver ICUD1;
#endif

#if GD32_ICU_USE_TIMER2 && !defined(__DOXYGEN__)
extern ICUDriver ICUD2;
#endif

#if GD32_ICU_USE_TIMER3 && !defined(__DOXYGEN__)
extern ICUDriver ICUD3;
#endif

#if GD32_ICU_USE_TIMER4 && !defined(__DOXYGEN__)
extern ICUDriver ICUD4;
#endif

#if GD32_ICU_USE_TIMER7 && !defined(__DOXYGEN__)
extern ICUDriver ICUD7;
#endif

#if GD32_ICU_USE_TIMER14 && !defined(__DOXYGEN__)
extern ICUDriver ICUD14;
#endif

#if GD32_ICU_USE_TIMER22 && !defined(__DOXYGEN__)
extern ICUDriver ICUD22;
#endif

#if GD32_ICU_USE_TIMER23 && !defined(__DOXYGEN__)
extern ICUDriver ICUD23;
#endif

#if GD32_ICU_USE_TIMER30 && !defined(__DOXYGEN__)
extern ICUDriver ICUD30;
#endif

#if GD32_ICU_USE_TIMER31 && !defined(__DOXYGEN__)
extern ICUDriver ICUD31;
#endif

#if GD32_ICU_USE_TIMER40 && !defined(__DOXYGEN__)
extern ICUDriver ICUD40;
#endif

#if GD32_ICU_USE_TIMER41 && !defined(__DOXYGEN__)
extern ICUDriver ICUD41;
#endif

#if GD32_ICU_USE_TIMER42 && !defined(__DOXYGEN__)
extern ICUDriver ICUD42;
#endif

#if GD32_ICU_USE_TIMER43 && !defined(__DOXYGEN__)
extern ICUDriver ICUD43;
#endif

#if GD32_ICU_USE_TIMER44 && !defined(__DOXYGEN__)
extern ICUDriver ICUD44;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void icu_lld_init(void);
  void icu_lld_start(ICUDriver *icup);
  void icu_lld_stop(ICUDriver *icup);
  void icu_lld_start_capture(ICUDriver *icup);
  bool icu_lld_wait_capture(ICUDriver *icup);
  void icu_lld_stop_capture(ICUDriver *icup);
  void icu_lld_enable_notifications(ICUDriver *icup);
  void icu_lld_disable_notifications(ICUDriver *icup);
  void icu_lld_serve_interrupt(ICUDriver *icup);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_ICU */

#endif /* HAL_ICU_LLD_H */

/** @} */
