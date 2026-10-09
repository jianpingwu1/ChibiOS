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
 * @file    GD32/LLD/TIMERv1/gd32_timer.h
 * @brief   GD32 TIMER driver macros and structures.
 * @details This header defines the GD32 TIMER peripheral register access
 *          helper macros shared by the ST, GPT, PWM, ICU and EICU drivers.
 *
 * @addtogroup GD32_TIMER
 * @{
 */

#ifndef GD32_TIMER_H
#define GD32_TIMER_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    TIMER_DMAINTEN register interrupt enable mask
 * @note    Only the interrupt enable bits, DMA request enable bits are
 *          intentionally excluded.
 * @{
 */
#define GD32_TIMER_DMAINTEN_IRQ_MASK        (TIMER_DMAINTEN_UPIE   |         \
                                             TIMER_DMAINTEN_CH0IE  |         \
                                             TIMER_DMAINTEN_CH1IE  |         \
                                             TIMER_DMAINTEN_CH2IE  |         \
                                             TIMER_DMAINTEN_CH3IE  |         \
                                             TIMER_DMAINTEN_CMTIE  |         \
                                             TIMER_DMAINTEN_TRGIE  |         \
                                             TIMER_DMAINTEN_BRKIE)
/** @} */

/**
 * @name    TIMER channel capture/compare value register access
 * @note    The CH0CV...CH3CV registers are four bytes apart, the base address
 *          is derived from the vendor CH0CV macro.
 * @{
 */
#define GD32_TIMER_CHCV(timerx, ch)                                          \
  REG32((uint32_t)&TIMER_CH0CV(timerx) + ((uint32_t)(ch) * 4U))
/** @} */

/**
 * @name    TIMER output compare control field placement
 * @note    The vendor header only provides the mode values, these macros
 *          place a mode value into the field of the specified channel.
 * @{
 */
#define GD32_TIMER_CHCTL0_CH0COMCTL(n)  ((((n) & 7U) << 4)  | (((n) >> 3U) << 16))
#define GD32_TIMER_CHCTL0_CH1COMCTL(n)  ((((n) & 7U) << 12) | (((n) >> 3U) << 24))
#define GD32_TIMER_CHCTL1_CH2COMCTL(n)  ((((n) & 7U) << 4)  | (((n) >> 3U) << 16))
#define GD32_TIMER_CHCTL1_CH3COMCTL(n)  ((((n) & 7U) << 12) | (((n) >> 3U) << 24))
/** @} */

/**
 * @name    TIMER channel input selection field placement
 * @note    The vendor header only provides the mode values, these macros
 *          place a mode value into the field of the specified channel.
 * @{
 */
#define GD32_TIMER_CHCTL0_CH0MS(n)  ((((n) & 3U) << 0) | (((n) >> 2U) << 30))
#define GD32_TIMER_CHCTL0_CH1MS(n)  ((((n) & 3U) << 8) | (((n) >> 2U) << 31))
#define GD32_TIMER_CHCTL1_CH2MS(n)  ((((n) & 3U) << 0) | (((n) >> 2U) << 30))
#define GD32_TIMER_CHCTL1_CH3MS(n)  ((((n) & 3U) << 8) | (((n) >> 2U) << 31))
/** @} */

/**
 * @name    SYSCFG TIMERxCFG0 restart mode trigger source placement
 * @note    Restart mode is selected by writing the trigger source into the
 *          TSCFG3 field, the slave mode itself is implied by the field
 *          position (GD32H7 user manual SYSCFG_TIMERxCFG0 register).
 * @{
 */
#define GD32_TIMER_TSCFG3(n)  (((uint32_t)(n) << 16) & SYSCFG_TIMERCFG_TSCFG3)
/** @} */

/**
 * @name    SYSCFG TIMER index sentinel
 * @note    Marks TIMER instances that have no SYSCFG_TIMERxCFG group (no
 *          slave mode controller), e.g. the general level4 timers.
 * @{
 */
#define GD32_TIMER_SYSCFG_NONE      0xFFU
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Driver inline functions.                                                  */
/*===========================================================================*/

#endif /* GD32_TIMER_H */

/** @} */