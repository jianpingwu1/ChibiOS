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
 * @file    GD32H73x_75x/gd32_isr.c
 * @brief   GD32H73x_75x ISR handler code.
 *
 * @addtogroup GD32H73x_75x_ISR
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define exti_serve_irq(pr, channel) {                                       \
                                                                            \
  if ((pr) & (1U << (channel))) {                                           \
    _pal_isr_code(channel);                                                 \
  }                                                                         \
}

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#include "gd32_adc0_1.inc"
#include "gd32_adc2.inc"

#include "gd32_exti0.inc"
#include "gd32_exti1.inc"
#include "gd32_exti2.inc"
#include "gd32_exti3.inc"
#include "gd32_exti4.inc"
#include "gd32_exti5_9.inc"
#include "gd32_exti10_15.inc"

#include "gd32_usart0.inc"
#include "gd32_usart1.inc"
#include "gd32_usart2.inc"
#include "gd32_uart3.inc"
#include "gd32_uart4.inc"
#include "gd32_usart5.inc"
#include "gd32_uart6.inc"
#include "gd32_uart7.inc"

#include "gd32_i2c0.inc"
#include "gd32_i2c1.inc"
#include "gd32_i2c2.inc"
#include "gd32_i2c3.inc"

#include "gd32_timer0.inc"
#include "gd32_timer1.inc"
#include "gd32_timer2.inc"
#include "gd32_timer3.inc"
#include "gd32_timer4.inc"
#include "gd32_timer5.inc"
#include "gd32_timer6.inc"
#include "gd32_timer7.inc"
#include "gd32_timer14.inc"
#include "gd32_timer15.inc"
#include "gd32_timer16.inc"
#include "gd32_timer22.inc"
#include "gd32_timer23.inc"
#include "gd32_timer30.inc"
#include "gd32_timer31.inc"
#include "gd32_timer40.inc"
#include "gd32_timer41.inc"
#include "gd32_timer42.inc"
#include "gd32_timer43.inc"
#include "gd32_timer44.inc"
#include "gd32_timer50.inc"
#include "gd32_timer51.inc"

#include "gd32_sdio0.inc"
#include "gd32_sdio1.inc"

#include "gd32_spi0.inc"
#include "gd32_spi1.inc"
#include "gd32_spi2.inc"
#include "gd32_spi3.inc"
#include "gd32_spi4.inc"
#include "gd32_spi5.inc"

#include "gd32_can0.inc"
#include "gd32_can1.inc"
#include "gd32_can2.inc"

#include "gd32_usbhs0.inc"
#include "gd32_usbhs1.inc"

/*
 * TODO: OSPI0/1, MDMA, TLI, IPA, DCI
 * are not wired up yet. Each of these needs its own GD32 LLD driver port
 * (verified against the GD32H7 reference manual) before an .inc file can
 * be added here, following the same pattern as the EXTI/USART ones above.
 * DMA0/1 ISRs are handled inside gd32_dma.c (inlined, not via .inc files)
 * because the DMA driver is an independent LLD module like in STM32.
 * TIMER0/TIMER7 have split BRK/UP/TRG_CMT/CC vectors; only their CC
 * vector is wired up here for the ST driver, the others need additional
 * .inc files when a PWM/ICU driver is ported for them.
 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Enables IRQ sources.
 *
 * @notapi
 */
void irqInit(void) {

  gd32_adc0_1_irq_init();
  gd32_adc2_irq_init();

  exti0_irq_init();
  exti1_irq_init();
  exti2_irq_init();
  exti3_irq_init();
  exti4_irq_init();
  exti5_9_irq_init();
  exti10_15_irq_init();

  usart0_irq_init();
  usart1_irq_init();
  usart2_irq_init();
  uart3_irq_init();
  uart4_irq_init();
  usart5_irq_init();
  uart6_irq_init();
  uart7_irq_init();

  i2c0_irq_init();
  i2c1_irq_init();
  i2c2_irq_init();
  i2c3_irq_init();

  gd32_timer0_irq_init();
  gd32_timer1_irq_init();
  gd32_timer2_irq_init();
  gd32_timer3_irq_init();
  gd32_timer4_irq_init();
  gd32_timer5_irq_init();
  gd32_timer6_irq_init();
  gd32_timer7_irq_init();
  gd32_timer14_irq_init();
  gd32_timer15_irq_init();
  gd32_timer16_irq_init();
  gd32_timer22_irq_init();
  gd32_timer23_irq_init();
  gd32_timer30_irq_init();
  gd32_timer31_irq_init();
  gd32_timer40_irq_init();
  gd32_timer41_irq_init();
  gd32_timer42_irq_init();
  gd32_timer43_irq_init();
  gd32_timer44_irq_init();
  gd32_timer50_irq_init();
  gd32_timer51_irq_init();

  sdio0_irq_init();
  sdio1_irq_init();

  spi0_irq_init();
  spi1_irq_init();
  spi2_irq_init();
  spi3_irq_init();
  spi4_irq_init();
  spi5_irq_init();

  gd32_can0_irq_init();
  gd32_can1_irq_init();
  gd32_can2_irq_init();

  usbhs0_irq_init();
  usbhs1_irq_init();
}

/**
 * @brief   Disables IRQ sources.
 *
 * @notapi
 */
void irqDeinit(void) {
  gd32_adc0_1_irq_deinit();
  gd32_adc2_irq_deinit();

  exti0_irq_deinit();
  exti1_irq_deinit();
  exti2_irq_deinit();
  exti3_irq_deinit();
  exti4_irq_deinit();
  exti5_9_irq_deinit();
  exti10_15_irq_deinit();

  usart0_irq_deinit();
  usart1_irq_deinit();
  usart2_irq_deinit();
  uart3_irq_deinit();
  uart4_irq_deinit();
  usart5_irq_deinit();
  uart6_irq_deinit();
  uart7_irq_deinit();

  i2c0_irq_deinit();
  i2c1_irq_deinit();
  i2c2_irq_deinit();
  i2c3_irq_deinit();

  gd32_timer0_irq_deinit();
  gd32_timer1_irq_deinit();
  gd32_timer2_irq_deinit();
  gd32_timer3_irq_deinit();
  gd32_timer4_irq_deinit();
  gd32_timer5_irq_deinit();
  gd32_timer6_irq_deinit();
  gd32_timer7_irq_deinit();
  gd32_timer14_irq_deinit();
  gd32_timer15_irq_deinit();
  gd32_timer16_irq_deinit();
  gd32_timer22_irq_deinit();
  gd32_timer23_irq_deinit();
  gd32_timer30_irq_deinit();
  gd32_timer31_irq_deinit();
  gd32_timer40_irq_deinit();
  gd32_timer41_irq_deinit();
  gd32_timer42_irq_deinit();
  gd32_timer43_irq_deinit();
  gd32_timer44_irq_deinit();
  gd32_timer50_irq_deinit();
  gd32_timer51_irq_deinit();

  sdio0_irq_deinit();
  sdio1_irq_deinit();

  spi0_irq_deinit();
  spi1_irq_deinit();
  spi2_irq_deinit();
  spi3_irq_deinit();
  spi4_irq_deinit();
  spi5_irq_deinit();

  gd32_can0_irq_deinit();
  gd32_can1_irq_deinit();
  gd32_can2_irq_deinit();

  usbhs0_irq_deinit();
  usbhs1_irq_deinit();
}

/** @} */
