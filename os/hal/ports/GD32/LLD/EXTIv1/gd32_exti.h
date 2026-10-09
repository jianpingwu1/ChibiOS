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
 * @file    EXTIv1/gd32_exti.h
 * @brief   EXTI helper driver header.
 *
 * @addtogroup GD32_EXTI
 * @{
 */

#ifndef GD32_EXTI_H
#define GD32_EXTI_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    EXTI channel modes
 * @{
 */
#define EXTI_MODE_MASK              7U  /**< @brief Mode parameter mask.    */
#define EXTI_MODE_EDGES_MASK        3U  /**< @brief Edges field mask.       */
#define EXTI_MODE_DISABLED          0U  /**< @brief Channel disabled.       */
#define EXTI_MODE_RISING_EDGE       1U  /**< @brief Rising edge callback.   */
#define EXTI_MODE_FALLING_EDGE      2U  /**< @brief Falling edge callback.  */
#define EXTI_MODE_BOTH_EDGES        3U  /**< @brief Both edges callback.    */
#define EXTI_MODE_ACTION_MASK       4U  /**< @brief Action field mask.      */
#define EXTI_MODE_ACTION_INTERRUPT  0U  /**< @brief Interrupt mode.         */
#define EXTI_MODE_ACTION_EVENT      4U  /**< @brief Event mode.             */
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(GD32_EXTI_NUM_LINES)
#error "GD32_EXTI_NUM_LINES not defined in registry"
#endif

/* Checking for presence of bank 2 registers. If the definition is not
   present in registry then it is inferred by the number of channels (which
   is not an always-good method, see G0.*/
#if !defined(GD32_EXTI_HAS_GROUP2)
#if GD32_EXTI_NUM_LINES <= 32
#define GD32_EXTI_HAS_GROUP2       FALSE
#else
#define GD32_EXTI_HAS_GROUP2       TRUE
#endif
#endif /* !defined(GD32_EXTI_HAS_GROUP2) */

#if (GD32_EXTI_NUM_LINES < 0) || (GD32_EXTI_NUM_LINES > 63)
#error "invalid GD32_EXTI_NUM_LINES value"
#endif

#if !defined(GD32_EXTI_INTEN0_MASK)
#error "GD32_EXTI_INTEN0_MASK not defined in registry"
#endif

#if GD32_EXTI_NUM_LINES > 32
#if !defined(GD32_EXTI_INTEN1_MASK)
#error "GD32_EXTI_INTEN1_MASK not defined in registry"
#endif
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of an EXTI line identifier.
 */
typedef uint32_t extiline_t;

/**
 * @brief   Type of an EXTI line mode.
 */
typedef uint32_t extimode_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   From group 1 line number to mask.
 *
 * @param[in] line      line number in range 0..31
 */
#define EXTI_MASK1(line) (uint32_t)(1U << (line))

/**
 * @brief   From group 2 line number to mask.
 *
 * @param[in] line      line number in range 32..63
 */
#define EXTI_MASK2(line) (uint32_t)(1U << ((line) - 32U))

/**
 * @brief   GD32 EXTI group 1 IRQ status clearing.
 *
 * @param[in] mask      mask of group 1 lines to be initialized
 *
 * @special
 */
#define extiClearGroup1(mask) do {                                          \
  osalDbgAssert(((mask) & GD32_EXTI_INTEN0_MASK) == 0U, "fixed lines");    \
  EXTI_PD0 = (uint32_t)(mask);                                              \
} while (false)

#if (GD32_EXTI_HAS_GROUP2 == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   GD32 EXTI group 2 IRQ status clearing.
 *
 * @param[in] mask      mask of group 2 lines to be initialized
 *
 * @special
 */
#define extiClearGroup2(mask) do {                                          \
  osalDbgAssert(((mask) & GD32_EXTI_INTEN1_MASK) == 0U, "fixed lines");    \
  EXTI_PD1 = (uint32_t)(mask);                                              \
} while (false)
#endif /* GD32_EXTI_HAS_GROUP2 == TRUE */

/**
 * @brief   Serves an EXTI interrupt in group 1.
 *
 * @param[in] mask      mask of lines to be cleared
 * @param[out] out      mask of lines needing processing
 *
 * @special
 */
#define extiGetAndClearGroup1(mask, out) do {                               \
  uint32_t pd0;                                                             \
                                                                            \
  pd0 = EXTI_PD0 & (mask);                                                  \
  (out) = pd0;                                                              \
  EXTI_PD0 = pd0;                                                          \
} while (false)

#if (GD32_EXTI_HAS_GROUP2 == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Serves an EXTI interrupt in group 2.
 *
 * @param[in] mask      mask of lines to be cleared
 * @param[out] out      mask of lines needing processing
 *
 * @special
 */
#define extiGetAndClearGroup2(mask, out) do {                               \
  uint32_t pd1;                                                             \
                                                                            \
  pd1 = EXTI_PD1 & (mask);                                                  \
  (out) = pd1;                                                              \
  EXTI_PD1 = pd1;                                                          \
} while (false)
#endif /* GD32_EXTI_HAS_GROUP2 == TRUE */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void extiEnableGroup1(uint32_t mask, extimode_t mode);
#if (GD32_EXTI_HAS_GROUP2 == TRUE) || defined(__DOXYGEN__)
  void extiEnableGroup2(uint32_t mask, extimode_t mode);
#endif /* GD32_EXTI_HAS_GROUP2 == TRUE */
  void extiEnableLine(extiline_t line, extimode_t mode);
  void extiClearLine(extiline_t line);
#ifdef __cplusplus
}
#endif

#endif /* GD32_EXTI_H */

/** @} */