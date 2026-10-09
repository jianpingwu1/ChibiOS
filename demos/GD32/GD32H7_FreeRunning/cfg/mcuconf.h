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

#ifndef MCUCONF_H
#define MCUCONF_H

/*
 * GD32H73x_75x drivers configuration.
 * The following settings override the default settings present in the
 * various device driver implementation headers (GD32H73x_75x/hal_lld.h and
 * the LLD drivers). Note that the settings for each driver only have effect
 * if the whole driver is enabled in halconf.h.
 *
 * This is the complete reference template: every pre-compile setting the
 * port understands is listed here with its default value, so a board
 * configuration can be derived by editing rather than by hunting through
 * the driver headers. Sections for drivers that are not ported yet are
 * present but commented out, they only document the intended macro names.
 *
 * IRQ priorities:
 * 15...0       Lowest...Highest.
 *
 * DMA priorities:
 * 0...3        Lowest...Highest.
 */

#define GD32H73x_75x_MCUCONF

/*
 * General settings.
 */
#define GD32_NO_INIT                        FALSE

/*
 * Memory attributes settings (MPU no-cache region).
 * Defaults match the GD32H759xM.ld NOCACHE_RAM region (SRAM0, 16K at
 * 0x30000000). Changing the base or the size here without updating the
 * linker script breaks D-Cache coherency for DMA buffers.
 */
#define GD32_NOCACHE_ENABLE                 FALSE
#define GD32_NOCACHE_MPU_REGION             MPU_REGION_6
#define GD32_NOCACHE_RBAR                   0x30000000U
#define GD32_NOCACHE_RASR                   MPU_RASR_SIZE_16K

/*
 * Oscillators settings.
 * GD32_HXTALCLK must match the crystal actually fitted on the board.
 */
#define GD32_HXTALCLK                       25000000U
#define GD32_LXTAL_ENABLED                  FALSE
#define GD32_IRC32K_ENABLED                 FALSE

/*
 * Clock tree static settings.
 * Reading the GD32H73x_75x User Manual is required. Defaults reproduce
 * the vendor system_clock_600m_hxtal() reference configuration (600MHz
 * SYSCLK from a 25MHz HXTAL crystal).
 * GD32_SYSCLK_SRC: GD32_SYSCLKSRC_IRC64MDIV, GD32_SYSCLKSRC_HXTAL,
 *                  GD32_SYSCLKSRC_LPIRC4M, GD32_SYSCLKSRC_PLL0P.
 * GD32_IRC64MDIV:  GD32_IRC64MDIV_DIV1/DIV2/DIV4/DIV8.
 * GD32_PLLSRC:     GD32_PLLSRC_IRC64MDIV, GD32_PLLSRC_LPIRC4M,
 *                  GD32_PLLSRC_HXTAL. Shared by PLL0, PLL1 and PLL2.
 */
#define GD32_SYSCLK_SRC                     GD32_SYSCLKSRC_PLL0P
#define GD32_IRC64MDIV                      GD32_IRC64MDIV_DIV1
#define GD32_PLLSRC                         GD32_PLLSRC_HXTAL

/*
 * PLL0 settings.
 * PLL factors are logical values like the rcu_pll0_config() arguments:
 * N is a multiplier and PSC/Q/R are divisors, all counted from 1. The
 * port applies the register bias internally.
 * VCOin = 25MHz / 5 = 5MHz, VCO = 5MHz * 120 = 600MHz, P = 600MHz SYSCLK.
 */
#define GD32_PLL0_ENABLED                   TRUE
#define GD32_PLL0_PSC                       5U
#define GD32_PLL0_N                         120U
#define GD32_PLL0_P                         1U
#define GD32_PLL0_Q                         2U
#define GD32_PLL0_R                         2U
#define GD32_PLL0VCOSEL                     RCU_PLL0VCO_192M_836M
#define GD32_PLL0_P_ENABLED                 TRUE
#define GD32_PLL0_Q_ENABLED                 FALSE
#define GD32_PLL0_R_ENABLED                 FALSE

/*
 * PLL1 settings.
 * PLL1P is the default ADC kernel clock source, it has to be enabled
 * before GD32_ADCSRC_PLL1P can be selected.
 */
#define GD32_PLL1_ENABLED                   FALSE
#define GD32_PLL1_PSC                       5U
#define GD32_PLL1_N                         120U
#define GD32_PLL1_P                         2U
#define GD32_PLL1_Q                         2U
#define GD32_PLL1_R                         2U
#define GD32_PLL1VCOSEL                     RCU_PLL1VCO_192M_836M
#define GD32_PLL1_P_ENABLED                 FALSE
#define GD32_PLL1_Q_ENABLED                 FALSE
#define GD32_PLL1_R_ENABLED                 FALSE

/*
 * PLL2 settings.
 */
#define GD32_PLL2_ENABLED                   FALSE
#define GD32_PLL2_PSC                       5U
#define GD32_PLL2_N                         120U
#define GD32_PLL2_P                         2U
#define GD32_PLL2_Q                         2U
#define GD32_PLL2_R                         2U
#define GD32_PLL2VCOSEL                     RCU_PLL2VCO_192M_836M
#define GD32_PLL2_P_ENABLED                 FALSE
#define GD32_PLL2_Q_ENABLED                 FALSE
#define GD32_PLL2_R_ENABLED                 FALSE

/*
 * Bus prescalers.
 * AHB = SYSCLK/2 = 300MHz, APB1 = 150MHz, APB2 = 300MHz, APB3 and APB4 =
 * 150MHz. The Cortex-M7 core is clocked by SYSCLK directly, the AHB
 * prescaler only affects the bus matrix.
 */
#define GD32_AHBPSC                         GD32_AHBPSC_DIV2
#define GD32_APB1PSC                        GD32_APBPSC_DIV2
#define GD32_APB2PSC                        GD32_APBPSC_DIV1
#define GD32_APB3PSC                        GD32_APBPSC_DIV2
#define GD32_APB4PSC                        GD32_APBPSC_DIV2

/*
 * RTC settings.
 * GD32_RTCSEL: GD32_RTCSEL_NONE, GD32_RTCSEL_LXTAL, GD32_RTCSEL_IRC32K,
 *              GD32_RTCSEL_HXTAL_DIV_RTCDIV.
 * GD32_RTCDIV only applies to the last one and must bring HXTAL to 1MHz.
 */
#define GD32_RTC_ENABLED                    FALSE
#define GD32_RTCSEL                         GD32_RTCSEL_NONE
#define GD32_RTCDIV                         25U

/*
 * USART kernel clock sources.
 * GD32_USARTSRC_APB, GD32_USARTSRC_AHB, GD32_USARTSRC_LXTAL,
 * GD32_USARTSRC_IRC64MDIV.
 * Only USART0/1/2/5 have a selectable kernel clock, UART3/4/6/7 are
 * permanently clocked from CK_APB1.
 */
#define GD32_USART0SEL                      GD32_USARTSRC_APB
#define GD32_USART1SEL                      GD32_USARTSRC_APB
#define GD32_USART2SEL                      GD32_USARTSRC_APB
#define GD32_USART5SEL                      GD32_USARTSRC_APB

/*
 * I2C kernel clock sources.
 * GD32_I2CSRC_APB1, GD32_I2CSRC_PLL2R, GD32_I2CSRC_IRC64MDIV.
 */
#define GD32_I2C0SEL                        GD32_I2CSRC_APB1
#define GD32_I2C1SEL                        GD32_I2CSRC_APB1
#define GD32_I2C2SEL                        GD32_I2CSRC_APB1
#define GD32_I2C3SEL                        GD32_I2CSRC_APB1

/*
 * SPI kernel clock sources.
 * SPI0/1/2/5: GD32_SPISRC_PLL0Q, GD32_SPISRC_PLL1P, GD32_SPISRC_PLL2P,
 *             GD32_SPISRC_I2S_CKIN.
 * SPI3/4:     GD32_SPISRC_APB2, GD32_SPISRC_PLL1Q, GD32_SPISRC_PLL2Q,
 *             GD32_SPISRC_IRC64MDIV, GD32_SPISRC_LPIRC4M,
 *             GD32_SPISRC_HXTAL.
 */
#define GD32_SPI0SEL                        GD32_SPISRC_PLL0Q
#define GD32_SPI1SEL                        GD32_SPISRC_PLL0Q
#define GD32_SPI2SEL                        GD32_SPISRC_PLL0Q
#define GD32_SPI3SEL                        GD32_SPISRC_APB2
#define GD32_SPI4SEL                        GD32_SPISRC_APB2
#define GD32_SPI5SEL                        GD32_SPISRC_PLL0Q

/*
 * External I2S clock input frequency, in Hz.
 * Only required when a GD32_SPISRC_I2S_CKIN selector is used, it is board
 * dependent and there is no sensible default for it.
 */
#define GD32_I2S_CKIN                       0U

/*
 * ADC kernel clock sources.
 * GD32_ADCSRC_PLL1P, GD32_ADCSRC_PLL2R, GD32_ADCSRC_PER.
 */
#define GD32_ADC01SEL                       GD32_ADCSRC_PLL1P
#define GD32_ADC2SEL                        GD32_ADCSRC_PLL1P

/*
 * SDIO kernel clock sources.
 * GD32_SDIOSRC_PLL0Q, GD32_SDIOSRC_PLL1R.
 */
#define GD32_SDIO0SEL                       GD32_SDIOSRC_PLL0Q
#define GD32_SDIO1SEL                       GD32_SDIOSRC_PLL0Q

/*
 * CAN kernel clock sources.
 * GD32_CANSRC_HXTAL, GD32_CANSRC_APB2, GD32_CANSRC_APB2_DIV2.
 */
#define GD32_CAN0SEL                        GD32_CANSRC_HXTAL
#define GD32_CAN1SEL                        GD32_CANSRC_HXTAL
#define GD32_CAN2SEL                        GD32_CANSRC_HXTAL

/*
 * Miscellaneous kernel clock sources.
 * GD32_EXMCSEL:  GD32_EXMCSRC_AHB, GD32_EXMCSRC_PLL0Q, GD32_EXMCSRC_PLL1R,
 *                GD32_EXMCSRC_PER.
 * GD32_PERSEL:   GD32_PERSRC_IRC64MDIV, GD32_PERSRC_LPIRC4M,
 *                GD32_PERSRC_HXTAL.
 * GD32_TIMERSEL: GD32_TIMERSRC_MUL2, GD32_TIMERSRC_MUL4.
 */
#define GD32_EXMCSEL                        GD32_EXMCSRC_AHB
#define GD32_PERSEL                         GD32_PERSRC_IRC64MDIV
#define GD32_TIMERSEL                       GD32_TIMERSRC_MUL2

/*
 * EXTI settings (required by PAL pad-event support).
 */
#define GD32_IRQ_EXTI0_PRIORITY             6
#define GD32_IRQ_EXTI1_PRIORITY             6
#define GD32_IRQ_EXTI2_PRIORITY             6
#define GD32_IRQ_EXTI3_PRIORITY             6
#define GD32_IRQ_EXTI4_PRIORITY             6
#define GD32_IRQ_EXTI5_9_PRIORITY           6
#define GD32_IRQ_EXTI10_15_PRIORITY         6

/*
 * ST driver system settings.
 * Select TIMER4 as the free running ST source (OSAL_ST_MODE_FREERUNNING).
 * Requires CH_CFG_ST_TIMEDELTA!=0 in chconf.h. TIMER4 is a 32-bit general
 * purpose timer, mirroring STM32H7's use of TIM5, so the 1MHz system tick
 * (CH_CFG_ST_FREQUENCY 1000000) overflows 32 bits only after ~4295s.
 */
#define GD32_ST_IRQ_PRIORITY                8
#define GD32_ST_USE_TIMER                   4
#define GD32_IRQ_TIMER0_CC_PRIORITY         7
#define GD32_IRQ_TIMER0_UP_PRIORITY         7
#define GD32_IRQ_TIMER1_PRIORITY            7
#define GD32_IRQ_TIMER2_PRIORITY            7
#define GD32_IRQ_TIMER3_PRIORITY            7
#define GD32_IRQ_TIMER4_PRIORITY            7
#define GD32_IRQ_TIMER5_DAC_UDR_PRIORITY    7
#define GD32_IRQ_TIMER6_PRIORITY            7
#define GD32_IRQ_TIMER7_CC_PRIORITY         7
#define GD32_IRQ_TIMER7_UP_PRIORITY         7
#define GD32_IRQ_TIMER14_PRIORITY           7
#define GD32_IRQ_TIMER15_PRIORITY           7
#define GD32_IRQ_TIMER16_PRIORITY           7
#define GD32_IRQ_TIMER22_PRIORITY           7
#define GD32_IRQ_TIMER23_PRIORITY           7
#define GD32_IRQ_TIMER30_PRIORITY           7
#define GD32_IRQ_TIMER31_PRIORITY           7
#define GD32_IRQ_TIMER40_PRIORITY           7
#define GD32_IRQ_TIMER41_PRIORITY           7
#define GD32_IRQ_TIMER42_PRIORITY           7
#define GD32_IRQ_TIMER43_PRIORITY           7
#define GD32_IRQ_TIMER44_PRIORITY           7
#define GD32_IRQ_TIMER50_PRIORITY           7
#define GD32_IRQ_TIMER51_PRIORITY           7

/*
 * USART/UART vector priorities.
 * Required unconditionally for all eight instances because gd32_isr.c
 * always compiles the dispatch layer, regardless of which driver
 * (SERIAL or UART) is actually selected.
 */
#define GD32_IRQ_USART0_PRIORITY            12
#define GD32_IRQ_USART1_PRIORITY            12
#define GD32_IRQ_USART2_PRIORITY            12
#define GD32_IRQ_UART3_PRIORITY             12
#define GD32_IRQ_UART4_PRIORITY             12
#define GD32_IRQ_USART5_PRIORITY            12
#define GD32_IRQ_UART6_PRIORITY             12
#define GD32_IRQ_UART7_PRIORITY             12

/*
 * DMA driver system settings.
 * Total number of streams shared by DMA0 and DMA1, do not change unless
 * targeting a device with a different DMA layout.
 */
#define GD32_DMA_STREAMS                    16U

/*
 * SERIAL driver system settings.
 * USART0 is the console used by the demo, on PA9/PA10 AF7.
 */
#define GD32_SERIAL_USE_USART0              TRUE
#define GD32_SERIAL_USE_USART1              FALSE
#define GD32_SERIAL_USE_USART2              FALSE
#define GD32_SERIAL_USE_UART3               FALSE
#define GD32_SERIAL_USE_UART4               FALSE
#define GD32_SERIAL_USE_USART5              FALSE
#define GD32_SERIAL_USE_UART6               FALSE
#define GD32_SERIAL_USE_UART7               FALSE

/*
 * UART driver system settings.
 * SERIAL and UART cannot share an instance, keep these off while the
 * matching GD32_SERIAL_USE_* switch is on.
 */
#define GD32_UART_USE_USART0                FALSE
#define GD32_UART_USE_USART1                FALSE
#define GD32_UART_USE_USART2                FALSE
#define GD32_UART_USE_UART3                 FALSE
#define GD32_UART_USE_UART4                 FALSE
#define GD32_UART_USE_USART5                FALSE
#define GD32_UART_USE_UART6                 FALSE
#define GD32_UART_USE_UART7                 FALSE
#define GD32_UART_USART0_IRQ_PRIORITY       12
#define GD32_UART_USART1_IRQ_PRIORITY       12
#define GD32_UART_USART2_IRQ_PRIORITY       12
#define GD32_UART_UART3_IRQ_PRIORITY        12
#define GD32_UART_UART4_IRQ_PRIORITY        12
#define GD32_UART_USART5_IRQ_PRIORITY       12
#define GD32_UART_UART6_IRQ_PRIORITY        12
#define GD32_UART_UART7_IRQ_PRIORITY        12
#define GD32_UART_USART0_DMA_PRIORITY       0
#define GD32_UART_USART1_DMA_PRIORITY       0
#define GD32_UART_USART2_DMA_PRIORITY       0
#define GD32_UART_UART3_DMA_PRIORITY        0
#define GD32_UART_UART4_DMA_PRIORITY        0
#define GD32_UART_USART5_DMA_PRIORITY       0
#define GD32_UART_UART6_DMA_PRIORITY        0
#define GD32_UART_UART7_DMA_PRIORITY        0

/*
 * GPT driver system settings.
 */
#define GD32_GPT_USE_TIMER0                 FALSE
#define GD32_GPT_USE_TIMER1                 FALSE
#define GD32_GPT_USE_TIMER2                 FALSE
#define GD32_GPT_USE_TIMER3                 FALSE
#define GD32_GPT_USE_TIMER4                 FALSE
#define GD32_GPT_USE_TIMER5                 FALSE
#define GD32_GPT_USE_TIMER6                 FALSE
#define GD32_GPT_USE_TIMER7                 FALSE
#define GD32_GPT_USE_TIMER14                FALSE
#define GD32_GPT_USE_TIMER15                FALSE
#define GD32_GPT_USE_TIMER16                FALSE
#define GD32_GPT_USE_TIMER22                FALSE
#define GD32_GPT_USE_TIMER23                FALSE
#define GD32_GPT_USE_TIMER30                FALSE
#define GD32_GPT_USE_TIMER31                FALSE
#define GD32_GPT_USE_TIMER40                FALSE
#define GD32_GPT_USE_TIMER41                FALSE
#define GD32_GPT_USE_TIMER42                FALSE
#define GD32_GPT_USE_TIMER43                FALSE
#define GD32_GPT_USE_TIMER44                FALSE
#define GD32_GPT_USE_TIMER50                FALSE
#define GD32_GPT_USE_TIMER51                FALSE
#define GD32_GPT_TIMER0_IRQ_PRIORITY        7
#define GD32_GPT_TIMER1_IRQ_PRIORITY        7
#define GD32_GPT_TIMER2_IRQ_PRIORITY        7
#define GD32_GPT_TIMER3_IRQ_PRIORITY        7
#define GD32_GPT_TIMER4_IRQ_PRIORITY        7
#define GD32_GPT_TIMER5_IRQ_PRIORITY        7
#define GD32_GPT_TIMER6_IRQ_PRIORITY        7
#define GD32_GPT_TIMER7_IRQ_PRIORITY        7
#define GD32_GPT_TIMER14_IRQ_PRIORITY       7
#define GD32_GPT_TIMER15_IRQ_PRIORITY       7
#define GD32_GPT_TIMER16_IRQ_PRIORITY       7
#define GD32_GPT_TIMER22_IRQ_PRIORITY       7
#define GD32_GPT_TIMER23_IRQ_PRIORITY       7
#define GD32_GPT_TIMER30_IRQ_PRIORITY       7
#define GD32_GPT_TIMER31_IRQ_PRIORITY       7
#define GD32_GPT_TIMER40_IRQ_PRIORITY       7
#define GD32_GPT_TIMER41_IRQ_PRIORITY       7
#define GD32_GPT_TIMER42_IRQ_PRIORITY       7
#define GD32_GPT_TIMER43_IRQ_PRIORITY       7
#define GD32_GPT_TIMER44_IRQ_PRIORITY       7
#define GD32_GPT_TIMER50_IRQ_PRIORITY       7
#define GD32_GPT_TIMER51_IRQ_PRIORITY       7

/*
 * PWM driver system settings.
 * TIMER5, TIMER6, TIMER50 and TIMER51 have no capture/compare channel.
 */
#define GD32_PWM_USE_TIMER0                 FALSE
#define GD32_PWM_USE_TIMER1                 FALSE
#define GD32_PWM_USE_TIMER2                 FALSE
#define GD32_PWM_USE_TIMER3                 FALSE
#define GD32_PWM_USE_TIMER4                 FALSE
#define GD32_PWM_USE_TIMER7                 FALSE
#define GD32_PWM_USE_TIMER14                FALSE
#define GD32_PWM_USE_TIMER15                FALSE
#define GD32_PWM_USE_TIMER16                FALSE
#define GD32_PWM_USE_TIMER22                FALSE
#define GD32_PWM_USE_TIMER23                FALSE
#define GD32_PWM_USE_TIMER30                FALSE
#define GD32_PWM_USE_TIMER31                FALSE
#define GD32_PWM_USE_TIMER40                FALSE
#define GD32_PWM_USE_TIMER41                FALSE
#define GD32_PWM_USE_TIMER42                FALSE
#define GD32_PWM_USE_TIMER43                FALSE
#define GD32_PWM_USE_TIMER44                FALSE
#define GD32_PWM_TIMER0_IRQ_PRIORITY        7
#define GD32_PWM_TIMER1_IRQ_PRIORITY        7
#define GD32_PWM_TIMER2_IRQ_PRIORITY        7
#define GD32_PWM_TIMER3_IRQ_PRIORITY        7
#define GD32_PWM_TIMER4_IRQ_PRIORITY        7
#define GD32_PWM_TIMER7_IRQ_PRIORITY        7
#define GD32_PWM_TIMER14_IRQ_PRIORITY       7
#define GD32_PWM_TIMER15_IRQ_PRIORITY       7
#define GD32_PWM_TIMER16_IRQ_PRIORITY       7
#define GD32_PWM_TIMER22_IRQ_PRIORITY       7
#define GD32_PWM_TIMER23_IRQ_PRIORITY       7
#define GD32_PWM_TIMER30_IRQ_PRIORITY       7
#define GD32_PWM_TIMER31_IRQ_PRIORITY       7
#define GD32_PWM_TIMER40_IRQ_PRIORITY       7
#define GD32_PWM_TIMER41_IRQ_PRIORITY       7
#define GD32_PWM_TIMER42_IRQ_PRIORITY       7
#define GD32_PWM_TIMER43_IRQ_PRIORITY       7
#define GD32_PWM_TIMER44_IRQ_PRIORITY       7

/*
 * ICU driver system settings.
 * TIMER15 and TIMER16 have no SYSCFG_TIMERxCFG register and cannot support
 * the restart (slave) mode required by input capture.
 */
#define GD32_ICU_USE_TIMER0                 FALSE
#define GD32_ICU_USE_TIMER1                 FALSE
#define GD32_ICU_USE_TIMER2                 FALSE
#define GD32_ICU_USE_TIMER3                 FALSE
#define GD32_ICU_USE_TIMER4                 FALSE
#define GD32_ICU_USE_TIMER7                 FALSE
#define GD32_ICU_USE_TIMER14                FALSE
#define GD32_ICU_USE_TIMER22                FALSE
#define GD32_ICU_USE_TIMER23                FALSE
#define GD32_ICU_USE_TIMER30                FALSE
#define GD32_ICU_USE_TIMER31                FALSE
#define GD32_ICU_USE_TIMER40                FALSE
#define GD32_ICU_USE_TIMER41                FALSE
#define GD32_ICU_USE_TIMER42                FALSE
#define GD32_ICU_USE_TIMER43                FALSE
#define GD32_ICU_USE_TIMER44                FALSE
#define GD32_ICU_TIMER0_IRQ_PRIORITY        7
#define GD32_ICU_TIMER1_IRQ_PRIORITY        7
#define GD32_ICU_TIMER2_IRQ_PRIORITY        7
#define GD32_ICU_TIMER3_IRQ_PRIORITY        7
#define GD32_ICU_TIMER4_IRQ_PRIORITY        7
#define GD32_ICU_TIMER7_IRQ_PRIORITY        7
#define GD32_ICU_TIMER14_IRQ_PRIORITY       7
#define GD32_ICU_TIMER22_IRQ_PRIORITY       7
#define GD32_ICU_TIMER23_IRQ_PRIORITY       7
#define GD32_ICU_TIMER30_IRQ_PRIORITY       7
#define GD32_ICU_TIMER31_IRQ_PRIORITY       7
#define GD32_ICU_TIMER40_IRQ_PRIORITY       7
#define GD32_ICU_TIMER41_IRQ_PRIORITY       7
#define GD32_ICU_TIMER42_IRQ_PRIORITY       7
#define GD32_ICU_TIMER43_IRQ_PRIORITY       7
#define GD32_ICU_TIMER44_IRQ_PRIORITY       7

/*
 * EICU driver system settings.
 * TIMER15 and TIMER16 have no slave mode, which EICU does not require.
 */
#define GD32_EICU_USE_TIMER0                FALSE
#define GD32_EICU_USE_TIMER1                FALSE
#define GD32_EICU_USE_TIMER2                FALSE
#define GD32_EICU_USE_TIMER3                FALSE
#define GD32_EICU_USE_TIMER4                FALSE
#define GD32_EICU_USE_TIMER7                FALSE
#define GD32_EICU_USE_TIMER14               FALSE
#define GD32_EICU_USE_TIMER15               FALSE
#define GD32_EICU_USE_TIMER16               FALSE
#define GD32_EICU_USE_TIMER22               FALSE
#define GD32_EICU_USE_TIMER23               FALSE
#define GD32_EICU_USE_TIMER30               FALSE
#define GD32_EICU_USE_TIMER31               FALSE
#define GD32_EICU_USE_TIMER40               FALSE
#define GD32_EICU_USE_TIMER41               FALSE
#define GD32_EICU_USE_TIMER42               FALSE
#define GD32_EICU_USE_TIMER43               FALSE
#define GD32_EICU_USE_TIMER44               FALSE
#define GD32_EICU_TIMER0_IRQ_PRIORITY       7
#define GD32_EICU_TIMER1_IRQ_PRIORITY       7
#define GD32_EICU_TIMER2_IRQ_PRIORITY       7
#define GD32_EICU_TIMER3_IRQ_PRIORITY       7
#define GD32_EICU_TIMER4_IRQ_PRIORITY       7
#define GD32_EICU_TIMER7_IRQ_PRIORITY       7
#define GD32_EICU_TIMER14_IRQ_PRIORITY      7
#define GD32_EICU_TIMER15_IRQ_PRIORITY      7
#define GD32_EICU_TIMER16_IRQ_PRIORITY      7
#define GD32_EICU_TIMER22_IRQ_PRIORITY      7
#define GD32_EICU_TIMER23_IRQ_PRIORITY      7
#define GD32_EICU_TIMER30_IRQ_PRIORITY      7
#define GD32_EICU_TIMER31_IRQ_PRIORITY      7
#define GD32_EICU_TIMER40_IRQ_PRIORITY      7
#define GD32_EICU_TIMER41_IRQ_PRIORITY      7
#define GD32_EICU_TIMER42_IRQ_PRIORITY      7
#define GD32_EICU_TIMER43_IRQ_PRIORITY      7
#define GD32_EICU_TIMER44_IRQ_PRIORITY      7

/*
 * Drivers that are not ported yet.
 * The blocks below document the macro names each driver is expected to
 * use once its LLD lands, so board files can be prepared in advance.
 * They must stay commented out: enabling them has no effect today and
 * would only hide the fact that the driver is missing.
 */

/* ADC driver system settings.*/
/*#define GD32_ADC_USE_ADC0                   FALSE*/
/*#define GD32_ADC_USE_ADC1                   FALSE*/
/*#define GD32_ADC_USE_ADC2                   FALSE*/
/*#define GD32_ADC_ADC0_DMA_STREAM            GD32_DMA_STREAM_ID(0, 0)*/
/*#define GD32_ADC_ADC0_DMA_PRIORITY          2*/
/*#define GD32_ADC_ADC0_IRQ_PRIORITY          5*/

/* CAN driver system settings.*/
/*#define GD32_CAN_USE_CAN0                   FALSE*/
/*#define GD32_CAN_USE_CAN1                   FALSE*/
/*#define GD32_CAN_USE_CAN2                   FALSE*/
/*#define GD32_CAN_CAN0_IRQ_PRIORITY          10*/

/* DAC driver system settings.*/
/*#define GD32_DAC_USE_DAC_CH0                FALSE*/
/*#define GD32_DAC_USE_DAC_CH1                FALSE*/
/*#define GD32_DAC_DAC_CH0_DMA_STREAM         GD32_DMA_STREAM_ID(0, 1)*/
/*#define GD32_DAC_DAC_CH0_DMA_PRIORITY       2*/

/* GPT driver system settings.*/
/*#define GD32_GPT_USE_TIMER0                 FALSE*/
/*#define GD32_GPT_USE_TIMER1                 FALSE*/
/*#define GD32_GPT_TIMER0_IRQ_PRIORITY        7*/

/* I2C driver system settings.*/
/*#define GD32_I2C_USE_I2C0                   FALSE*/
/*#define GD32_I2C_USE_I2C1                   FALSE*/
/*#define GD32_I2C_USE_I2C2                   FALSE*/
/*#define GD32_I2C_USE_I2C3                   FALSE*/
/*#define GD32_I2C_I2C0_RX_DMA_STREAM         GD32_DMA_STREAM_ID(0, 2)*/
/*#define GD32_I2C_I2C0_TX_DMA_STREAM         GD32_DMA_STREAM_ID(0, 3)*/
/*#define GD32_I2C_I2C0_IRQ_PRIORITY          5*/
/*#define GD32_I2C_I2C0_DMA_PRIORITY          3*/
/*#define GD32_I2C_DMA_ERROR_HOOK(i2cp)       osalSysHalt("DMA failure")*/

/* ICU driver system settings.*/
/*#define GD32_ICU_USE_TIMER0                 FALSE*/
/*#define GD32_ICU_TIMER0_IRQ_PRIORITY        7*/

/* MAC driver system settings.*/
/*#define GD32_MAC_TRANSMIT_BUFFERS           2*/
/*#define GD32_MAC_RECEIVE_BUFFERS            4*/
/*#define GD32_MAC_BUFFERS_SIZE               1522*/
/*#define GD32_MAC_ENET0_IRQ_PRIORITY         13*/

/* PWM driver system settings.*/
/*#define GD32_PWM_USE_TIMER0                 FALSE*/
/*#define GD32_PWM_USE_TIMER7                 FALSE*/
/*#define GD32_PWM_TIMER0_IRQ_PRIORITY        7*/

/* SDC driver system settings.*/
/*#define GD32_SDC_USE_SDIO0                  FALSE*/
/*#define GD32_SDC_USE_SDIO1                  FALSE*/
/*#define GD32_SDC_SDIO0_IRQ_PRIORITY         9*/
/*#define GD32_SDC_SDIO0_DMA_PRIORITY         3*/

/* SPI driver system settings.*/
/*#define GD32_SPI_USE_SPI0                   FALSE*/
/*#define GD32_SPI_USE_SPI1                   FALSE*/
/*#define GD32_SPI_USE_SPI2                   FALSE*/
/*#define GD32_SPI_USE_SPI3                   FALSE*/
/*#define GD32_SPI_USE_SPI4                   FALSE*/
/*#define GD32_SPI_USE_SPI5                   FALSE*/
/*#define GD32_SPI_SPI0_RX_DMA_STREAM         GD32_DMA_STREAM_ID(0, 4)*/
/*#define GD32_SPI_SPI0_TX_DMA_STREAM         GD32_DMA_STREAM_ID(0, 5)*/
/*#define GD32_SPI_SPI0_IRQ_PRIORITY          10*/
/*#define GD32_SPI_SPI0_DMA_PRIORITY          1*/
/*#define GD32_SPI_DMA_ERROR_HOOK(spip)       osalSysHalt("DMA failure")*/

/* TRNG driver system settings.*/
/*#define GD32_TRNG_USE_TRNG                  FALSE*/

/* USB driver system settings.*/
/*#define GD32_USB_USE_USBHS0                 FALSE*/
/*#define GD32_USB_USE_USBHS1                 FALSE*/
/*#define GD32_USB_USBHS0_IRQ_PRIORITY        14*/

/* WDG driver system settings.*/
/*#define GD32_WDG_USE_FWDGT                  FALSE*/

#endif /* MCUCONF_H */