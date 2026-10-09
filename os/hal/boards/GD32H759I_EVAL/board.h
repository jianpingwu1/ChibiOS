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
 * This file has been automatically generated from board.chcfg by
 * tools/gd32/boardgen.py, do not edit it manually. Edit the .chcfg
 * description and regenerate instead.
 */

#ifndef BOARD_H
#define BOARD_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * Board identifier.
 */
#define BOARD_GD32_H73X_75X_MINIMAL
#define BOARD_NAME                  "GD32H73x_75x Minimal Template"

/*
 * Board oscillators-related settings.
 * @note    Overridable so that a project can select a different crystal
 *          without editing the generated board description.
 */
#if !defined(GD32_HXTALCLK)
#define GD32_HXTALCLK               25000000U
#endif

/*
 * MCU type as defined in the GD32 vendor header.
 */
#define GD32H73X_75X

/*
 * IO pins assignments.
 */
#define GPIOA_PIN0                  0U
#define GPIOA_PIN1                  1U
#define GPIOA_PIN2                  2U
#define GPIOA_PIN3                  3U
#define GPIOA_PIN4                  4U
#define GPIOA_PIN5                  5U
#define GPIOA_LED2                  6U
#define GPIOA_PIN7                  7U
#define GPIOA_PIN8                  8U
#define GPIOA_PIN9                  9U
#define GPIOA_PIN10                 10U
#define GPIOA_PIN11                 11U
#define GPIOA_PIN12                 12U
#define GPIOA_SWDIO                 13U
#define GPIOA_SWCLK                 14U
#define GPIOA_PIN15                 15U

#define GPIOB_PIN0                  0U
#define GPIOB_PIN1                  1U
#define GPIOB_PIN2                  2U
#define GPIOB_PIN3                  3U
#define GPIOB_PIN4                  4U
#define GPIOB_PIN5                  5U
#define GPIOB_PIN6                  6U
#define GPIOB_PIN7                  7U
#define GPIOB_PIN8                  8U
#define GPIOB_PIN9                  9U
#define GPIOB_PIN10                 10U
#define GPIOB_PIN11                 11U
#define GPIOB_PIN12                 12U
#define GPIOB_PIN13                 13U
#define GPIOB_PIN14                 14U
#define GPIOB_PIN15                 15U

#define GPIOC_PIN0                  0U
#define GPIOC_PIN1                  1U
#define GPIOC_PIN2                  2U
#define GPIOC_PIN3                  3U
#define GPIOC_PIN4                  4U
#define GPIOC_PIN5                  5U
#define GPIOC_PIN6                  6U
#define GPIOC_PIN7                  7U
#define GPIOC_PIN8                  8U
#define GPIOC_PIN9                  9U
#define GPIOC_PIN10                 10U
#define GPIOC_PIN11                 11U
#define GPIOC_PIN12                 12U
#define GPIOC_PIN13                 13U
#define GPIOC_PIN14                 14U
#define GPIOC_PIN15                 15U

#define GPIOD_PIN0                  0U
#define GPIOD_PIN1                  1U
#define GPIOD_PIN2                  2U
#define GPIOD_PIN3                  3U
#define GPIOD_PIN4                  4U
#define GPIOD_PIN5                  5U
#define GPIOD_PIN6                  6U
#define GPIOD_PIN7                  7U
#define GPIOD_PIN8                  8U
#define GPIOD_PIN9                  9U
#define GPIOD_PIN10                 10U
#define GPIOD_PIN11                 11U
#define GPIOD_PIN12                 12U
#define GPIOD_PIN13                 13U
#define GPIOD_PIN14                 14U
#define GPIOD_PIN15                 15U

#define GPIOE_PIN0                  0U
#define GPIOE_PIN1                  1U
#define GPIOE_PIN2                  2U
#define GPIOE_PIN3                  3U
#define GPIOE_PIN4                  4U
#define GPIOE_PIN5                  5U
#define GPIOE_PIN6                  6U
#define GPIOE_PIN7                  7U
#define GPIOE_PIN8                  8U
#define GPIOE_PIN9                  9U
#define GPIOE_PIN10                 10U
#define GPIOE_PIN11                 11U
#define GPIOE_PIN12                 12U
#define GPIOE_PIN13                 13U
#define GPIOE_PIN14                 14U
#define GPIOE_PIN15                 15U

#define GPIOF_PIN0                  0U
#define GPIOF_PIN1                  1U
#define GPIOF_PIN2                  2U
#define GPIOF_PIN3                  3U
#define GPIOF_PIN4                  4U
#define GPIOF_PIN5                  5U
#define GPIOF_PIN6                  6U
#define GPIOF_PIN7                  7U
#define GPIOF_PIN8                  8U
#define GPIOF_PIN9                  9U
#define GPIOF_LED1                  10U
#define GPIOF_PIN11                 11U
#define GPIOF_PIN12                 12U
#define GPIOF_PIN13                 13U
#define GPIOF_PIN14                 14U
#define GPIOF_PIN15                 15U

#define GPIOG_PIN0                  0U
#define GPIOG_PIN1                  1U
#define GPIOG_PIN2                  2U
#define GPIOG_PIN3                  3U
#define GPIOG_PIN4                  4U
#define GPIOG_PIN5                  5U
#define GPIOG_PIN6                  6U
#define GPIOG_PIN7                  7U
#define GPIOG_PIN8                  8U
#define GPIOG_PIN9                  9U
#define GPIOG_PIN10                 10U
#define GPIOG_PIN11                 11U
#define GPIOG_PIN12                 12U
#define GPIOG_PIN13                 13U
#define GPIOG_PIN14                 14U
#define GPIOG_PIN15                 15U

#define GPIOH_PIN0                  0U
#define GPIOH_PIN1                  1U
#define GPIOH_PIN2                  2U
#define GPIOH_PIN3                  3U
#define GPIOH_PIN4                  4U
#define GPIOH_PIN5                  5U
#define GPIOH_PIN6                  6U
#define GPIOH_PIN7                  7U
#define GPIOH_PIN8                  8U
#define GPIOH_PIN9                  9U
#define GPIOH_PIN10                 10U
#define GPIOH_PIN11                 11U
#define GPIOH_PIN12                 12U
#define GPIOH_PIN13                 13U
#define GPIOH_PIN14                 14U
#define GPIOH_PIN15                 15U

#define GPIOJ_PIN0                  0U
#define GPIOJ_PIN1                  1U
#define GPIOJ_PIN2                  2U
#define GPIOJ_PIN3                  3U
#define GPIOJ_PIN4                  4U
#define GPIOJ_PIN5                  5U
#define GPIOJ_PIN6                  6U
#define GPIOJ_PIN7                  7U
#define GPIOJ_PIN8                  8U
#define GPIOJ_PIN9                  9U
#define GPIOJ_PIN10                 10U
#define GPIOJ_PIN11                 11U
#define GPIOJ_PIN12                 12U
#define GPIOJ_PIN13                 13U
#define GPIOJ_PIN14                 14U
#define GPIOJ_PIN15                 15U

#define GPIOK_PIN0                  0U
#define GPIOK_PIN1                  1U
#define GPIOK_PIN2                  2U
#define GPIOK_PIN3                  3U
#define GPIOK_PIN4                  4U
#define GPIOK_PIN5                  5U
#define GPIOK_PIN6                  6U
#define GPIOK_PIN7                  7U
#define GPIOK_PIN8                  8U
#define GPIOK_PIN9                  9U
#define GPIOK_PIN10                 10U
#define GPIOK_PIN11                 11U
#define GPIOK_PIN12                 12U
#define GPIOK_PIN13                 13U
#define GPIOK_PIN14                 14U
#define GPIOK_PIN15                 15U

/*
 * IO lines assignments.
 */
#define LINE_LED2                   PAL_LINE(GPIOA, GPIOA_LED2)
#define LINE_SWDIO                  PAL_LINE(GPIOA, GPIOA_SWDIO)
#define LINE_SWCLK                  PAL_LINE(GPIOA, GPIOA_SWCLK)
#define LINE_LED1                   PAL_LINE(GPIOF, GPIOF_LED1)

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

/*
 * I/O ports initial setup, this configuration is established soon after
 * reset in the initialization code.
 * Please refer to the GD32H73x_75x User Manual for details (GPIO_CTL/
 * GPIO_OMODE/GPIO_OSPD/GPIO_PUD/GPIO_OCTL/GPIO_AFSEL0/GPIO_AFSEL1).
 */
#define PIN_MODE_INPUT(n)           (0U << ((n) * 2U))
#define PIN_MODE_OUTPUT(n)          (1U << ((n) * 2U))
#define PIN_MODE_ALTERNATE(n)       (2U << ((n) * 2U))
#define PIN_MODE_ANALOG(n)          (3U << ((n) * 2U))
#define PIN_ODR_LOW(n)              (0U << (n))
#define PIN_ODR_HIGH(n)             (1U << (n))
#define PIN_OTYPE_PUSHPULL(n)       (0U << (n))
#define PIN_OTYPE_OPENDRAIN(n)      (1U << (n))
#define PIN_OSPEED_LOWEST(n)        (0U << ((n) * 2U))
#define PIN_OSPEED_MID1(n)          (1U << ((n) * 2U))
#define PIN_OSPEED_MID2(n)          (2U << ((n) * 2U))
#define PIN_OSPEED_HIGHEST(n)       (3U << ((n) * 2U))
#define PIN_PUPDR_FLOATING(n)       (0U << ((n) * 2U))
#define PIN_PUPDR_PULLUP(n)         (1U << ((n) * 2U))
#define PIN_PUPDR_PULLDOWN(n)       (2U << ((n) * 2U))
#define PIN_AFIO_AF(n, v)           ((v) << (((n) % 8U) * 4U))

/*
 * GPIOA setup:
 *
 * PA0  - PIN0                      (input floating).
 * PA1  - PIN1                      (input floating).
 * PA2  - PIN2                      (input floating).
 * PA3  - PIN3                      (input floating).
 * PA4  - PIN4                      (input floating).
 * PA5  - PIN5                      (input floating).
 * PA6  - LED2                      (output pushpull minimum).
 * PA7  - PIN7                      (input floating).
 * PA8  - PIN8                      (input floating).
 * PA9  - PIN9                      (input floating).
 * PA10 - PIN10                     (input floating).
 * PA11 - PIN11                     (input floating).
 * PA12 - PIN12                     (input floating).
 * PA13 - SWDIO                     (alternate 0).
 * PA14 - SWCLK                     (alternate 0).
 * PA15 - PIN15                     (input floating).
 */
#define VAL_GPIOA_CTL               (PIN_MODE_INPUT(GPIOA_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN5) |           \
                                     PIN_MODE_OUTPUT(GPIOA_LED2) |          \
                                     PIN_MODE_INPUT(GPIOA_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOA_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOA_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOA_PIN12) |          \
                                     PIN_MODE_ALTERNATE(GPIOA_SWDIO) |      \
                                     PIN_MODE_ALTERNATE(GPIOA_SWCLK) |      \
                                     PIN_MODE_INPUT(GPIOA_PIN15))
#define VAL_GPIOA_OMODE             (PIN_OTYPE_PUSHPULL(GPIOA_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_LED2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWDIO) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_SWCLK) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOA_PIN15))
#define VAL_GPIOA_OSPD              (PIN_OSPEED_LOWEST(GPIOA_PIN0) |        \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN1) |        \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN2) |        \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN3) |        \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN4) |        \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN5) |        \
                                     PIN_OSPEED_LOWEST(GPIOA_LED2) |        \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN7) |        \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN8) |        \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN9) |        \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN10) |       \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN11) |       \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN12) |       \
                                     PIN_OSPEED_HIGHEST(GPIOA_SWDIO) |      \
                                     PIN_OSPEED_HIGHEST(GPIOA_SWCLK) |      \
                                     PIN_OSPEED_LOWEST(GPIOA_PIN15))
#define VAL_GPIOA_PUD               (PIN_PUPDR_FLOATING(GPIOA_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_LED2) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_SWDIO) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_SWCLK) |      \
                                     PIN_PUPDR_FLOATING(GPIOA_PIN15))
#define VAL_GPIOA_OCTL              (PIN_ODR_LOW(GPIOA_PIN0) |              \
                                     PIN_ODR_LOW(GPIOA_PIN1) |              \
                                     PIN_ODR_LOW(GPIOA_PIN2) |              \
                                     PIN_ODR_LOW(GPIOA_PIN3) |              \
                                     PIN_ODR_LOW(GPIOA_PIN4) |              \
                                     PIN_ODR_LOW(GPIOA_PIN5) |              \
                                     PIN_ODR_LOW(GPIOA_LED2) |              \
                                     PIN_ODR_LOW(GPIOA_PIN7) |              \
                                     PIN_ODR_LOW(GPIOA_PIN8) |              \
                                     PIN_ODR_LOW(GPIOA_PIN9) |              \
                                     PIN_ODR_LOW(GPIOA_PIN10) |             \
                                     PIN_ODR_LOW(GPIOA_PIN11) |             \
                                     PIN_ODR_LOW(GPIOA_PIN12) |             \
                                     PIN_ODR_LOW(GPIOA_SWDIO) |             \
                                     PIN_ODR_LOW(GPIOA_SWCLK) |             \
                                     PIN_ODR_LOW(GPIOA_PIN15))
#define VAL_GPIOA_AFSEL0            (PIN_AFIO_AF(GPIOA_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_LED2, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN7, 0U))
#define VAL_GPIOA_AFSEL1            (PIN_AFIO_AF(GPIOA_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOA_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_SWDIO, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_SWCLK, 0U) |         \
                                     PIN_AFIO_AF(GPIOA_PIN15, 0U))

/*
 * GPIOB setup:
 *
 * PB0  - PIN0                      (input floating).
 * PB1  - PIN1                      (input floating).
 * PB2  - PIN2                      (input floating).
 * PB3  - PIN3                      (input floating).
 * PB4  - PIN4                      (input floating).
 * PB5  - PIN5                      (input floating).
 * PB6  - PIN6                      (input floating).
 * PB7  - PIN7                      (input floating).
 * PB8  - PIN8                      (input floating).
 * PB9  - PIN9                      (input floating).
 * PB10 - PIN10                     (input floating).
 * PB11 - PIN11                     (input floating).
 * PB12 - PIN12                     (input floating).
 * PB13 - PIN13                     (input floating).
 * PB14 - PIN14                     (input floating).
 * PB15 - PIN15                     (input floating).
 */
#define VAL_GPIOB_CTL               (PIN_MODE_INPUT(GPIOB_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOB_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOB_PIN15))
#define VAL_GPIOB_OMODE             (PIN_OTYPE_PUSHPULL(GPIOB_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOB_PIN15))
#define VAL_GPIOB_OSPD              (PIN_OSPEED_LOWEST(GPIOB_PIN0) |        \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN1) |        \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN2) |        \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN3) |        \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN4) |        \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN5) |        \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN6) |        \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN7) |        \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN8) |        \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN9) |        \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN10) |       \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN11) |       \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN12) |       \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN13) |       \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN14) |       \
                                     PIN_OSPEED_LOWEST(GPIOB_PIN15))
#define VAL_GPIOB_PUD               (PIN_PUPDR_FLOATING(GPIOB_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOB_PIN15))
#define VAL_GPIOB_OCTL              (PIN_ODR_LOW(GPIOB_PIN0) |              \
                                     PIN_ODR_LOW(GPIOB_PIN1) |              \
                                     PIN_ODR_LOW(GPIOB_PIN2) |              \
                                     PIN_ODR_LOW(GPIOB_PIN3) |              \
                                     PIN_ODR_LOW(GPIOB_PIN4) |              \
                                     PIN_ODR_LOW(GPIOB_PIN5) |              \
                                     PIN_ODR_LOW(GPIOB_PIN6) |              \
                                     PIN_ODR_LOW(GPIOB_PIN7) |              \
                                     PIN_ODR_LOW(GPIOB_PIN8) |              \
                                     PIN_ODR_LOW(GPIOB_PIN9) |              \
                                     PIN_ODR_LOW(GPIOB_PIN10) |             \
                                     PIN_ODR_LOW(GPIOB_PIN11) |             \
                                     PIN_ODR_LOW(GPIOB_PIN12) |             \
                                     PIN_ODR_LOW(GPIOB_PIN13) |             \
                                     PIN_ODR_LOW(GPIOB_PIN14) |             \
                                     PIN_ODR_LOW(GPIOB_PIN15))
#define VAL_GPIOB_AFSEL0            (PIN_AFIO_AF(GPIOB_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN7, 0U))
#define VAL_GPIOB_AFSEL1            (PIN_AFIO_AF(GPIOB_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOB_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOB_PIN15, 0U))

/*
 * GPIOC setup:
 *
 * PC0  - PIN0                      (input floating).
 * PC1  - PIN1                      (input floating).
 * PC2  - PIN2                      (input floating).
 * PC3  - PIN3                      (input floating).
 * PC4  - PIN4                      (input floating).
 * PC5  - PIN5                      (input floating).
 * PC6  - PIN6                      (input floating).
 * PC7  - PIN7                      (input floating).
 * PC8  - PIN8                      (input floating).
 * PC9  - PIN9                      (input floating).
 * PC10 - PIN10                     (input floating).
 * PC11 - PIN11                     (input floating).
 * PC12 - PIN12                     (input floating).
 * PC13 - PIN13                     (input floating).
 * PC14 - PIN14                     (input floating).
 * PC15 - PIN15                     (input floating).
 */
#define VAL_GPIOC_CTL               (PIN_MODE_INPUT(GPIOC_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOC_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOC_PIN15))
#define VAL_GPIOC_OMODE             (PIN_OTYPE_PUSHPULL(GPIOC_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOC_PIN15))
#define VAL_GPIOC_OSPD              (PIN_OSPEED_LOWEST(GPIOC_PIN0) |        \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN1) |        \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN2) |        \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN3) |        \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN4) |        \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN5) |        \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN6) |        \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN7) |        \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN8) |        \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN9) |        \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN10) |       \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN11) |       \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN12) |       \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN13) |       \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN14) |       \
                                     PIN_OSPEED_LOWEST(GPIOC_PIN15))
#define VAL_GPIOC_PUD               (PIN_PUPDR_FLOATING(GPIOC_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOC_PIN15))
#define VAL_GPIOC_OCTL              (PIN_ODR_LOW(GPIOC_PIN0) |              \
                                     PIN_ODR_LOW(GPIOC_PIN1) |              \
                                     PIN_ODR_LOW(GPIOC_PIN2) |              \
                                     PIN_ODR_LOW(GPIOC_PIN3) |              \
                                     PIN_ODR_LOW(GPIOC_PIN4) |              \
                                     PIN_ODR_LOW(GPIOC_PIN5) |              \
                                     PIN_ODR_LOW(GPIOC_PIN6) |              \
                                     PIN_ODR_LOW(GPIOC_PIN7) |              \
                                     PIN_ODR_LOW(GPIOC_PIN8) |              \
                                     PIN_ODR_LOW(GPIOC_PIN9) |              \
                                     PIN_ODR_LOW(GPIOC_PIN10) |             \
                                     PIN_ODR_LOW(GPIOC_PIN11) |             \
                                     PIN_ODR_LOW(GPIOC_PIN12) |             \
                                     PIN_ODR_LOW(GPIOC_PIN13) |             \
                                     PIN_ODR_LOW(GPIOC_PIN14) |             \
                                     PIN_ODR_LOW(GPIOC_PIN15))
#define VAL_GPIOC_AFSEL0            (PIN_AFIO_AF(GPIOC_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN7, 0U))
#define VAL_GPIOC_AFSEL1            (PIN_AFIO_AF(GPIOC_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOC_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOC_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOC_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOC_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOC_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOC_PIN15, 0U))

/*
 * GPIOD setup:
 *
 * PD0  - PIN0                      (input floating).
 * PD1  - PIN1                      (input floating).
 * PD2  - PIN2                      (input floating).
 * PD3  - PIN3                      (input floating).
 * PD4  - PIN4                      (input floating).
 * PD5  - PIN5                      (input floating).
 * PD6  - PIN6                      (input floating).
 * PD7  - PIN7                      (input floating).
 * PD8  - PIN8                      (input floating).
 * PD9  - PIN9                      (input floating).
 * PD10 - PIN10                     (input floating).
 * PD11 - PIN11                     (input floating).
 * PD12 - PIN12                     (input floating).
 * PD13 - PIN13                     (input floating).
 * PD14 - PIN14                     (input floating).
 * PD15 - PIN15                     (input floating).
 */
#define VAL_GPIOD_CTL               (PIN_MODE_INPUT(GPIOD_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOD_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOD_PIN15))
#define VAL_GPIOD_OMODE             (PIN_OTYPE_PUSHPULL(GPIOD_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOD_PIN15))
#define VAL_GPIOD_OSPD              (PIN_OSPEED_LOWEST(GPIOD_PIN0) |        \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN1) |        \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN2) |        \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN3) |        \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN4) |        \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN5) |        \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN6) |        \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN7) |        \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN8) |        \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN9) |        \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN10) |       \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN11) |       \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN12) |       \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN13) |       \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN14) |       \
                                     PIN_OSPEED_LOWEST(GPIOD_PIN15))
#define VAL_GPIOD_PUD               (PIN_PUPDR_FLOATING(GPIOD_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOD_PIN15))
#define VAL_GPIOD_OCTL              (PIN_ODR_LOW(GPIOD_PIN0) |              \
                                     PIN_ODR_LOW(GPIOD_PIN1) |              \
                                     PIN_ODR_LOW(GPIOD_PIN2) |              \
                                     PIN_ODR_LOW(GPIOD_PIN3) |              \
                                     PIN_ODR_LOW(GPIOD_PIN4) |              \
                                     PIN_ODR_LOW(GPIOD_PIN5) |              \
                                     PIN_ODR_LOW(GPIOD_PIN6) |              \
                                     PIN_ODR_LOW(GPIOD_PIN7) |              \
                                     PIN_ODR_LOW(GPIOD_PIN8) |              \
                                     PIN_ODR_LOW(GPIOD_PIN9) |              \
                                     PIN_ODR_LOW(GPIOD_PIN10) |             \
                                     PIN_ODR_LOW(GPIOD_PIN11) |             \
                                     PIN_ODR_LOW(GPIOD_PIN12) |             \
                                     PIN_ODR_LOW(GPIOD_PIN13) |             \
                                     PIN_ODR_LOW(GPIOD_PIN14) |             \
                                     PIN_ODR_LOW(GPIOD_PIN15))
#define VAL_GPIOD_AFSEL0            (PIN_AFIO_AF(GPIOD_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN7, 0U))
#define VAL_GPIOD_AFSEL1            (PIN_AFIO_AF(GPIOD_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOD_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOD_PIN15, 0U))

/*
 * GPIOE setup:
 *
 * PE0  - PIN0                      (input floating).
 * PE1  - PIN1                      (input floating).
 * PE2  - PIN2                      (input floating).
 * PE3  - PIN3                      (input floating).
 * PE4  - PIN4                      (input floating).
 * PE5  - PIN5                      (input floating).
 * PE6  - PIN6                      (input floating).
 * PE7  - PIN7                      (input floating).
 * PE8  - PIN8                      (input floating).
 * PE9  - PIN9                      (input floating).
 * PE10 - PIN10                     (input floating).
 * PE11 - PIN11                     (input floating).
 * PE12 - PIN12                     (input floating).
 * PE13 - PIN13                     (input floating).
 * PE14 - PIN14                     (input floating).
 * PE15 - PIN15                     (input floating).
 */
#define VAL_GPIOE_CTL               (PIN_MODE_INPUT(GPIOE_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOE_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOE_PIN15))
#define VAL_GPIOE_OMODE             (PIN_OTYPE_PUSHPULL(GPIOE_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOE_PIN15))
#define VAL_GPIOE_OSPD              (PIN_OSPEED_LOWEST(GPIOE_PIN0) |        \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN1) |        \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN2) |        \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN3) |        \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN4) |        \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN5) |        \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN6) |        \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN7) |        \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN8) |        \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN9) |        \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN10) |       \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN11) |       \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN12) |       \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN13) |       \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN14) |       \
                                     PIN_OSPEED_LOWEST(GPIOE_PIN15))
#define VAL_GPIOE_PUD               (PIN_PUPDR_FLOATING(GPIOE_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOE_PIN15))
#define VAL_GPIOE_OCTL              (PIN_ODR_LOW(GPIOE_PIN0) |              \
                                     PIN_ODR_LOW(GPIOE_PIN1) |              \
                                     PIN_ODR_LOW(GPIOE_PIN2) |              \
                                     PIN_ODR_LOW(GPIOE_PIN3) |              \
                                     PIN_ODR_LOW(GPIOE_PIN4) |              \
                                     PIN_ODR_LOW(GPIOE_PIN5) |              \
                                     PIN_ODR_LOW(GPIOE_PIN6) |              \
                                     PIN_ODR_LOW(GPIOE_PIN7) |              \
                                     PIN_ODR_LOW(GPIOE_PIN8) |              \
                                     PIN_ODR_LOW(GPIOE_PIN9) |              \
                                     PIN_ODR_LOW(GPIOE_PIN10) |             \
                                     PIN_ODR_LOW(GPIOE_PIN11) |             \
                                     PIN_ODR_LOW(GPIOE_PIN12) |             \
                                     PIN_ODR_LOW(GPIOE_PIN13) |             \
                                     PIN_ODR_LOW(GPIOE_PIN14) |             \
                                     PIN_ODR_LOW(GPIOE_PIN15))
#define VAL_GPIOE_AFSEL0            (PIN_AFIO_AF(GPIOE_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN7, 0U))
#define VAL_GPIOE_AFSEL1            (PIN_AFIO_AF(GPIOE_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOE_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOE_PIN15, 0U))

/*
 * GPIOF setup:
 *
 * PF0  - PIN0                      (input floating).
 * PF1  - PIN1                      (input floating).
 * PF2  - PIN2                      (input floating).
 * PF3  - PIN3                      (input floating).
 * PF4  - PIN4                      (input floating).
 * PF5  - PIN5                      (input floating).
 * PF6  - PIN6                      (input floating).
 * PF7  - PIN7                      (input floating).
 * PF8  - PIN8                      (input floating).
 * PF9  - PIN9                      (input floating).
 * PF10 - LED1                      (output pushpull minimum).
 * PF11 - PIN11                     (input floating).
 * PF12 - PIN12                     (input floating).
 * PF13 - PIN13                     (input floating).
 * PF14 - PIN14                     (input floating).
 * PF15 - PIN15                     (input floating).
 */
#define VAL_GPIOF_CTL               (PIN_MODE_INPUT(GPIOF_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOF_PIN9) |           \
                                     PIN_MODE_OUTPUT(GPIOF_LED1) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOF_PIN15))
#define VAL_GPIOF_OMODE             (PIN_OTYPE_PUSHPULL(GPIOF_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_LED1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOF_PIN15))
#define VAL_GPIOF_OSPD              (PIN_OSPEED_LOWEST(GPIOF_PIN0) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN1) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN2) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN3) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN4) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN5) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN6) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN7) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN8) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN9) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_LED1) |        \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN11) |       \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN12) |       \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN13) |       \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN14) |       \
                                     PIN_OSPEED_LOWEST(GPIOF_PIN15))
#define VAL_GPIOF_PUD               (PIN_PUPDR_FLOATING(GPIOF_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_LED1) |       \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOF_PIN15))
#define VAL_GPIOF_OCTL              (PIN_ODR_LOW(GPIOF_PIN0) |              \
                                     PIN_ODR_LOW(GPIOF_PIN1) |              \
                                     PIN_ODR_LOW(GPIOF_PIN2) |              \
                                     PIN_ODR_LOW(GPIOF_PIN3) |              \
                                     PIN_ODR_LOW(GPIOF_PIN4) |              \
                                     PIN_ODR_LOW(GPIOF_PIN5) |              \
                                     PIN_ODR_LOW(GPIOF_PIN6) |              \
                                     PIN_ODR_LOW(GPIOF_PIN7) |              \
                                     PIN_ODR_LOW(GPIOF_PIN8) |              \
                                     PIN_ODR_LOW(GPIOF_PIN9) |              \
                                     PIN_ODR_LOW(GPIOF_LED1) |              \
                                     PIN_ODR_LOW(GPIOF_PIN11) |             \
                                     PIN_ODR_LOW(GPIOF_PIN12) |             \
                                     PIN_ODR_LOW(GPIOF_PIN13) |             \
                                     PIN_ODR_LOW(GPIOF_PIN14) |             \
                                     PIN_ODR_LOW(GPIOF_PIN15))
#define VAL_GPIOF_AFSEL0            (PIN_AFIO_AF(GPIOF_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN7, 0U))
#define VAL_GPIOF_AFSEL1            (PIN_AFIO_AF(GPIOF_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_LED1, 0U) |          \
                                     PIN_AFIO_AF(GPIOF_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOF_PIN15, 0U))

/*
 * GPIOG setup:
 *
 * PG0  - PIN0                      (input floating).
 * PG1  - PIN1                      (input floating).
 * PG2  - PIN2                      (input floating).
 * PG3  - PIN3                      (input floating).
 * PG4  - PIN4                      (input floating).
 * PG5  - PIN5                      (input floating).
 * PG6  - PIN6                      (input floating).
 * PG7  - PIN7                      (input floating).
 * PG8  - PIN8                      (input floating).
 * PG9  - PIN9                      (input floating).
 * PG10 - PIN10                     (input floating).
 * PG11 - PIN11                     (input floating).
 * PG12 - PIN12                     (input floating).
 * PG13 - PIN13                     (input floating).
 * PG14 - PIN14                     (input floating).
 * PG15 - PIN15                     (input floating).
 */
#define VAL_GPIOG_CTL               (PIN_MODE_INPUT(GPIOG_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOG_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOG_PIN15))
#define VAL_GPIOG_OMODE             (PIN_OTYPE_PUSHPULL(GPIOG_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOG_PIN15))
#define VAL_GPIOG_OSPD              (PIN_OSPEED_LOWEST(GPIOG_PIN0) |        \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN1) |        \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN2) |        \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN3) |        \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN4) |        \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN5) |        \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN6) |        \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN7) |        \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN8) |        \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN9) |        \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN10) |       \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN11) |       \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN12) |       \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN13) |       \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN14) |       \
                                     PIN_OSPEED_LOWEST(GPIOG_PIN15))
#define VAL_GPIOG_PUD               (PIN_PUPDR_FLOATING(GPIOG_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOG_PIN15))
#define VAL_GPIOG_OCTL              (PIN_ODR_LOW(GPIOG_PIN0) |              \
                                     PIN_ODR_LOW(GPIOG_PIN1) |              \
                                     PIN_ODR_LOW(GPIOG_PIN2) |              \
                                     PIN_ODR_LOW(GPIOG_PIN3) |              \
                                     PIN_ODR_LOW(GPIOG_PIN4) |              \
                                     PIN_ODR_LOW(GPIOG_PIN5) |              \
                                     PIN_ODR_LOW(GPIOG_PIN6) |              \
                                     PIN_ODR_LOW(GPIOG_PIN7) |              \
                                     PIN_ODR_LOW(GPIOG_PIN8) |              \
                                     PIN_ODR_LOW(GPIOG_PIN9) |              \
                                     PIN_ODR_LOW(GPIOG_PIN10) |             \
                                     PIN_ODR_LOW(GPIOG_PIN11) |             \
                                     PIN_ODR_LOW(GPIOG_PIN12) |             \
                                     PIN_ODR_LOW(GPIOG_PIN13) |             \
                                     PIN_ODR_LOW(GPIOG_PIN14) |             \
                                     PIN_ODR_LOW(GPIOG_PIN15))
#define VAL_GPIOG_AFSEL0            (PIN_AFIO_AF(GPIOG_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN7, 0U))
#define VAL_GPIOG_AFSEL1            (PIN_AFIO_AF(GPIOG_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOG_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOG_PIN15, 0U))

/*
 * GPIOH setup:
 *
 * PH0  - PIN0                      (input floating).
 * PH1  - PIN1                      (input floating).
 * PH2  - PIN2                      (input floating).
 * PH3  - PIN3                      (input floating).
 * PH4  - PIN4                      (input floating).
 * PH5  - PIN5                      (input floating).
 * PH6  - PIN6                      (input floating).
 * PH7  - PIN7                      (input floating).
 * PH8  - PIN8                      (input floating).
 * PH9  - PIN9                      (input floating).
 * PH10 - PIN10                     (input floating).
 * PH11 - PIN11                     (input floating).
 * PH12 - PIN12                     (input floating).
 * PH13 - PIN13                     (input floating).
 * PH14 - PIN14                     (input floating).
 * PH15 - PIN15                     (input floating).
 */
#define VAL_GPIOH_CTL               (PIN_MODE_INPUT(GPIOH_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOH_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOH_PIN15))
#define VAL_GPIOH_OMODE             (PIN_OTYPE_PUSHPULL(GPIOH_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOH_PIN15))
#define VAL_GPIOH_OSPD              (PIN_OSPEED_LOWEST(GPIOH_PIN0) |        \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN1) |        \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN2) |        \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN3) |        \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN4) |        \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN5) |        \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN6) |        \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN7) |        \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN8) |        \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN9) |        \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN10) |       \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN11) |       \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN12) |       \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN13) |       \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN14) |       \
                                     PIN_OSPEED_LOWEST(GPIOH_PIN15))
#define VAL_GPIOH_PUD               (PIN_PUPDR_FLOATING(GPIOH_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOH_PIN15))
#define VAL_GPIOH_OCTL              (PIN_ODR_LOW(GPIOH_PIN0) |              \
                                     PIN_ODR_LOW(GPIOH_PIN1) |              \
                                     PIN_ODR_LOW(GPIOH_PIN2) |              \
                                     PIN_ODR_LOW(GPIOH_PIN3) |              \
                                     PIN_ODR_LOW(GPIOH_PIN4) |              \
                                     PIN_ODR_LOW(GPIOH_PIN5) |              \
                                     PIN_ODR_LOW(GPIOH_PIN6) |              \
                                     PIN_ODR_LOW(GPIOH_PIN7) |              \
                                     PIN_ODR_LOW(GPIOH_PIN8) |              \
                                     PIN_ODR_LOW(GPIOH_PIN9) |              \
                                     PIN_ODR_LOW(GPIOH_PIN10) |             \
                                     PIN_ODR_LOW(GPIOH_PIN11) |             \
                                     PIN_ODR_LOW(GPIOH_PIN12) |             \
                                     PIN_ODR_LOW(GPIOH_PIN13) |             \
                                     PIN_ODR_LOW(GPIOH_PIN14) |             \
                                     PIN_ODR_LOW(GPIOH_PIN15))
#define VAL_GPIOH_AFSEL0            (PIN_AFIO_AF(GPIOH_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN7, 0U))
#define VAL_GPIOH_AFSEL1            (PIN_AFIO_AF(GPIOH_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOH_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOH_PIN15, 0U))

/*
 * GPIOJ setup:
 *
 * PJ0  - PIN0                      (input floating).
 * PJ1  - PIN1                      (input floating).
 * PJ2  - PIN2                      (input floating).
 * PJ3  - PIN3                      (input floating).
 * PJ4  - PIN4                      (input floating).
 * PJ5  - PIN5                      (input floating).
 * PJ6  - PIN6                      (input floating).
 * PJ7  - PIN7                      (input floating).
 * PJ8  - PIN8                      (input floating).
 * PJ9  - PIN9                      (input floating).
 * PJ10 - PIN10                     (input floating).
 * PJ11 - PIN11                     (input floating).
 * PJ12 - PIN12                     (input floating).
 * PJ13 - PIN13                     (input floating).
 * PJ14 - PIN14                     (input floating).
 * PJ15 - PIN15                     (input floating).
 */
#define VAL_GPIOJ_CTL               (PIN_MODE_INPUT(GPIOJ_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOJ_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOJ_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOJ_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOJ_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOJ_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOJ_PIN15))
#define VAL_GPIOJ_OMODE             (PIN_OTYPE_PUSHPULL(GPIOJ_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOJ_PIN15))
#define VAL_GPIOJ_OSPD              (PIN_OSPEED_LOWEST(GPIOJ_PIN0) |        \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN1) |        \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN2) |        \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN3) |        \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN4) |        \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN5) |        \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN6) |        \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN7) |        \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN8) |        \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN9) |        \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN10) |       \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN11) |       \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN12) |       \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN13) |       \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN14) |       \
                                     PIN_OSPEED_LOWEST(GPIOJ_PIN15))
#define VAL_GPIOJ_PUD               (PIN_PUPDR_FLOATING(GPIOJ_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOJ_PIN15))
#define VAL_GPIOJ_OCTL              (PIN_ODR_LOW(GPIOJ_PIN0) |              \
                                     PIN_ODR_LOW(GPIOJ_PIN1) |              \
                                     PIN_ODR_LOW(GPIOJ_PIN2) |              \
                                     PIN_ODR_LOW(GPIOJ_PIN3) |              \
                                     PIN_ODR_LOW(GPIOJ_PIN4) |              \
                                     PIN_ODR_LOW(GPIOJ_PIN5) |              \
                                     PIN_ODR_LOW(GPIOJ_PIN6) |              \
                                     PIN_ODR_LOW(GPIOJ_PIN7) |              \
                                     PIN_ODR_LOW(GPIOJ_PIN8) |              \
                                     PIN_ODR_LOW(GPIOJ_PIN9) |              \
                                     PIN_ODR_LOW(GPIOJ_PIN10) |             \
                                     PIN_ODR_LOW(GPIOJ_PIN11) |             \
                                     PIN_ODR_LOW(GPIOJ_PIN12) |             \
                                     PIN_ODR_LOW(GPIOJ_PIN13) |             \
                                     PIN_ODR_LOW(GPIOJ_PIN14) |             \
                                     PIN_ODR_LOW(GPIOJ_PIN15))
#define VAL_GPIOJ_AFSEL0            (PIN_AFIO_AF(GPIOJ_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN7, 0U))
#define VAL_GPIOJ_AFSEL1            (PIN_AFIO_AF(GPIOJ_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOJ_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOJ_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOJ_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOJ_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOJ_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOJ_PIN15, 0U))

/*
 * GPIOK setup:
 *
 * PK0  - PIN0                      (input floating).
 * PK1  - PIN1                      (input floating).
 * PK2  - PIN2                      (input floating).
 * PK3  - PIN3                      (input floating).
 * PK4  - PIN4                      (input floating).
 * PK5  - PIN5                      (input floating).
 * PK6  - PIN6                      (input floating).
 * PK7  - PIN7                      (input floating).
 * PK8  - PIN8                      (input floating).
 * PK9  - PIN9                      (input floating).
 * PK10 - PIN10                     (input floating).
 * PK11 - PIN11                     (input floating).
 * PK12 - PIN12                     (input floating).
 * PK13 - PIN13                     (input floating).
 * PK14 - PIN14                     (input floating).
 * PK15 - PIN15                     (input floating).
 */
#define VAL_GPIOK_CTL               (PIN_MODE_INPUT(GPIOK_PIN0) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN1) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN2) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN3) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN4) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN5) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN6) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN7) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN8) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN9) |           \
                                     PIN_MODE_INPUT(GPIOK_PIN10) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN11) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN12) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN13) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN14) |          \
                                     PIN_MODE_INPUT(GPIOK_PIN15))
#define VAL_GPIOK_OMODE             (PIN_OTYPE_PUSHPULL(GPIOK_PIN0) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN1) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN2) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN3) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN4) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN5) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN6) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN7) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN8) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN9) |       \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN10) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN11) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN12) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN13) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN14) |      \
                                     PIN_OTYPE_PUSHPULL(GPIOK_PIN15))
#define VAL_GPIOK_OSPD              (PIN_OSPEED_LOWEST(GPIOK_PIN0) |        \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN1) |        \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN2) |        \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN3) |        \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN4) |        \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN5) |        \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN6) |        \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN7) |        \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN8) |        \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN9) |        \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN10) |       \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN11) |       \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN12) |       \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN13) |       \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN14) |       \
                                     PIN_OSPEED_LOWEST(GPIOK_PIN15))
#define VAL_GPIOK_PUD               (PIN_PUPDR_FLOATING(GPIOK_PIN0) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN1) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN2) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN3) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN4) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN5) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN6) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN7) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN8) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN9) |       \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN10) |      \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN11) |      \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN12) |      \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN13) |      \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN14) |      \
                                     PIN_PUPDR_FLOATING(GPIOK_PIN15))
#define VAL_GPIOK_OCTL              (PIN_ODR_LOW(GPIOK_PIN0) |              \
                                     PIN_ODR_LOW(GPIOK_PIN1) |              \
                                     PIN_ODR_LOW(GPIOK_PIN2) |              \
                                     PIN_ODR_LOW(GPIOK_PIN3) |              \
                                     PIN_ODR_LOW(GPIOK_PIN4) |              \
                                     PIN_ODR_LOW(GPIOK_PIN5) |              \
                                     PIN_ODR_LOW(GPIOK_PIN6) |              \
                                     PIN_ODR_LOW(GPIOK_PIN7) |              \
                                     PIN_ODR_LOW(GPIOK_PIN8) |              \
                                     PIN_ODR_LOW(GPIOK_PIN9) |              \
                                     PIN_ODR_LOW(GPIOK_PIN10) |             \
                                     PIN_ODR_LOW(GPIOK_PIN11) |             \
                                     PIN_ODR_LOW(GPIOK_PIN12) |             \
                                     PIN_ODR_LOW(GPIOK_PIN13) |             \
                                     PIN_ODR_LOW(GPIOK_PIN14) |             \
                                     PIN_ODR_LOW(GPIOK_PIN15))
#define VAL_GPIOK_AFSEL0            (PIN_AFIO_AF(GPIOK_PIN0, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN1, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN2, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN3, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN4, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN5, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN6, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN7, 0U))
#define VAL_GPIOK_AFSEL1            (PIN_AFIO_AF(GPIOK_PIN8, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN9, 0U) |          \
                                     PIN_AFIO_AF(GPIOK_PIN10, 0U) |         \
                                     PIN_AFIO_AF(GPIOK_PIN11, 0U) |         \
                                     PIN_AFIO_AF(GPIOK_PIN12, 0U) |         \
                                     PIN_AFIO_AF(GPIOK_PIN13, 0U) |         \
                                     PIN_AFIO_AF(GPIOK_PIN14, 0U) |         \
                                     PIN_AFIO_AF(GPIOK_PIN15, 0U))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(_FROM_ASM_)
#ifdef __cplusplus
extern "C" {
#endif
  void boardInit(void);
#ifdef __cplusplus
}
#endif
#endif /* _FROM_ASM_ */

#endif /* BOARD_H */
