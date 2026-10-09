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
 * @file    GD32H73x_75x/hal_lld.h
 * @brief   GD32H73x_75x HAL subsystem low level driver header.
 * @note    This is the whole clock-tree configuration entry point for the
 *          GD32H73x_75x port: user settings, derived frequencies and the
 *          related compile-time checks all live in this file.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef HAL_LLD_H
#define HAL_LLD_H

#include "gd32h73x_75x.h"
#include "gd32_registry.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Requires a message.
 */
#if !defined(GD32H7XX)
  #define GD32H7XX
#endif
#if !defined(GD32H73X_75X)
  #define GD32H73X_75X
#endif
#if !defined(GD32H7)
  #define GD32H7
#endif

/**
 * @name    Platform identification
 * @{
 */
#define PLATFORM_NAME           "GD32H73x_75x"
/** @} */

/**
 * @name    Clock points names
 * @{
 */
#define CLK_SYSCLK              0U
#define CLK_HCLK                1U
#define CLK_PCLK1               2U
#define CLK_PCLK2               3U
#define CLK_PCLK3               4U
#define CLK_PCLK4               5U
#define CLK_PLL0PCLK            6U
#define CLK_PLL0QCLK            7U
#define CLK_PLL0RCLK            8U
#define CLK_PLL1PCLK            9U
#define CLK_PLL1QCLK            10U
#define CLK_PLL1RCLK            11U
#define CLK_PLL2PCLK            12U
#define CLK_PLL2QCLK            13U
#define CLK_PLL2RCLK            14U
#define CLK_RTCCLK              15U
#define CLK_PERCLK              16U
#define CLK_ARRAY_SIZE          17U
/** @} */

/**
 * @name    System clock source selectors.
 * @note    Raw selector values are used here so they can be compared in
 *          preprocessor expressions by the derived-constants section.
 * @{
 */
#define GD32_SYSCLKSRC_IRC64MDIV 0U
#define GD32_SYSCLKSRC_HXTAL     1U
#define GD32_SYSCLKSRC_LPIRC4M   2U
#define GD32_SYSCLKSRC_PLL0P     3U
/** @} */

/**
 * @name    PLL clock source selectors
 * @note    Plain integer literals are used here instead of the vendor
 *          RCU_PLLSRC_IRC64MDIV/LPIRC4M/HXTAL macros: those expand (via
 *          PLLALL_PLLSEL()) to expressions containing a "(uint32_t)" cast,
 *          which is invalid inside a preprocessor #if - and GD32_PLLSRC is
 *          compared against these selectors in #if directives by the
 *          derived-constants section below. The literals below match the
 *          raw regval argument accepted by the vendor PLLALL_PLLSEL()
 *          macro (0=IRC64MDIV, 1=LPIRC4M, 2=HXTAL, gd32h73x_75x_rcu.h),
 *          and are translated back to the actual shifted register value at
 *          runtime in gd32_clock_init() (hal_lld.c) via PLLALL_PLLSEL().
 * @note    The three sources are mutually exclusive: PLL0, PLL1 and PLL2
 *          share the single PLLSEL field in RCU_PLLALL.
 * @{
 */
#define GD32_PLLSRC_IRC64MDIV   0U
#define GD32_PLLSRC_LPIRC4M     1U
#define GD32_PLLSRC_HXTAL       2U
/** @} */

/**
 * @name    IRC64M divider selectors (for CK_IRC64MDIV).
 * @{
 */
#define GD32_IRC64MDIV_DIV1      0U
#define GD32_IRC64MDIV_DIV2      1U
#define GD32_IRC64MDIV_DIV4      2U
#define GD32_IRC64MDIV_DIV8      3U
/** @} */

/**
 * @name    RTC source selectors.
 * @note    Raw selector values are used for preprocessor checks.
 * @{
 */
#define GD32_RTCSEL_NONE                 0U
#define GD32_RTCSEL_LXTAL                1U
#define GD32_RTCSEL_IRC32K               2U
#define GD32_RTCSEL_HXTAL_DIV_RTCDIV     3U
/** @} */

/**
 * @name    USART clock source selectors.
 * @note    Only USART0, USART1, USART2 and USART5 have a selectable kernel
 *          clock, through the USARTxSEL fields of RCU_CFG1. UART3, UART4,
 *          UART6 and UART7 are permanently clocked from CK_APB1.
 * @{
 */
#define GD32_USARTSRC_APB                0U
#define GD32_USARTSRC_AHB                1U
#define GD32_USARTSRC_LXTAL              2U
#define GD32_USARTSRC_IRC64MDIV          3U
/** @} */

/**
 * @name    I2C clock source selectors.
 * @note    All four instances share the same encoding, I2C0 through the
 *          I2C0SEL field of RCU_CFG0 and I2C1..3 through RCU_CFG3.
 * @{
 */
#define GD32_I2CSRC_APB1                 0U
#define GD32_I2CSRC_PLL2R                1U
#define GD32_I2CSRC_IRC64MDIV            2U
/** @} */

/**
 * @name    SPI clock source selectors, SPI0/1/2 group.
 * @note    These instances double as I2S peripherals and are therefore fed
 *          from the audio capable PLL outputs, through RCU_CFG5.
 * @{
 */
#define GD32_SPISRC_PLL0Q                0U
#define GD32_SPISRC_PLL1P                1U
#define GD32_SPISRC_PLL2P                2U
#define GD32_SPISRC_I2S_CKIN             3U
#define GD32_SPISRC_PER                  4U
/** @} */

/**
 * @name    SPI clock source selectors, SPI3/4/5 group.
 * @note    SPI5 shares this mux with SPI3/4 but is the only one of the three
 *          also accepting I2S_CKIN.
 * @{
 */
#define GD32_SPISRC_APB2                 0U
#define GD32_SPISRC_PLL1Q                1U
#define GD32_SPISRC_PLL2Q                2U
#define GD32_SPISRC_IRC64MDIV            3U
#define GD32_SPISRC_LPIRC4M              4U
#define GD32_SPISRC_HXTAL                5U
#define GD32_SPISRC_SPI5_I2S_CKIN        6U
/** @} */

/**
 * @name    ADC clock source selectors.
 * @note    ADC0 and ADC1 share the ADC01SEL field of RCU_CFG3, ADC2 has its
 *          own ADC2SEL field in the same register.
 * @{
 */
#define GD32_ADCSRC_PLL1P                0U
#define GD32_ADCSRC_PLL2R                1U
#define GD32_ADCSRC_PER                  2U
/** @} */

/**
 * @name    SDIO clock source selectors.
 * @{
 */
#define GD32_SDIOSRC_PLL0Q               0U
#define GD32_SDIOSRC_PLL1R               1U
/** @} */

/**
 * @name    CAN clock source selectors.
 * @note    All three instances share the same encoding, through the CANxSEL
 *          fields of RCU_CFG1.
 * @{
 */
#define GD32_CANSRC_HXTAL                0U
#define GD32_CANSRC_APB2                 1U
#define GD32_CANSRC_APB2_DIV2            2U
/** @} */

/**
 * @name    EXMC clock source selectors.
 * @{
 */
#define GD32_EXMCSRC_AHB                 0U
#define GD32_EXMCSRC_PLL0Q               1U
#define GD32_EXMCSRC_PLL1R               2U
#define GD32_EXMCSRC_PER                 3U
/** @} */

/**
 * @name    CK_PER clock source selectors.
 * @note    CK_PER is a shared kernel clock that can be routed to the ADC and
 *          EXMC muxes, selected through the PERSEL field of RCU_CFG1.
 * @{
 */
#define GD32_PERSRC_IRC64MDIV            0U
#define GD32_PERSRC_LPIRC4M              1U
#define GD32_PERSRC_HXTAL                2U
/** @} */

/**
 * @name    TIMER clock prescaler selectors.
 * @note    Selected through the TIMERSEL bit of RCU_CFG1, it caps how far
 *          the timer kernel clock may be multiplied above CK_APBx.
 * @{
 */
#define GD32_TIMERSRC_MUL2               0U
#define GD32_TIMERSRC_MUL4               1U
/** @} */

/**
 * @name    AHB prescaler selectors
 * @note    These are the raw "regval" arguments accepted by the vendor
 *          CFG0_AHBPSC() macro (gd32h73x_75x_rcu.h), not yet shifted into
 *          the RCU_CFG0 register position.
 * @{
 */
#define GD32_AHBPSC_DIV1         0U
#define GD32_AHBPSC_DIV2         8U
#define GD32_AHBPSC_DIV4         9U
#define GD32_AHBPSC_DIV8         10U
#define GD32_AHBPSC_DIV16        11U
#define GD32_AHBPSC_DIV64        12U
#define GD32_AHBPSC_DIV128       13U
#define GD32_AHBPSC_DIV256       14U
#define GD32_AHBPSC_DIV512       15U
/** @} */

/**
 * @name    APBx prescaler selectors (APB1/APB2/APB3/APB4 share the same
 *          encoding: divider by 1/2/4/8/16 of CK_AHB).
 * @{
 */
#define GD32_APBPSC_DIV1         0U
#define GD32_APBPSC_DIV2         4U
#define GD32_APBPSC_DIV4         5U
#define GD32_APBPSC_DIV8         6U
#define GD32_APBPSC_DIV16        7U
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                        */
/*===========================================================================*/

/**
 * @brief   Disables the clock initialization in hal_lld_init().
 */
#if !defined(GD32_NO_INIT) || defined(__DOXYGEN__)
#define GD32_NO_INIT                FALSE
#endif

/**
 * @brief   HXTAL clock frequency, in Hz.
 * @note    Default value matches the 25MHz crystal used by the reference
 *          system_gd32h73x_75x.c system_clock_600m_hxtal() configuration.
 */
#if !defined(GD32_HXTALCLK) || defined(__DOXYGEN__)
#define GD32_HXTALCLK               25000000U
#endif

/**
 * @brief   Enables the LXTAL oscillator branch.
 * @note    The LXTAL frequency is not configurable, see @p GD32_LXTALCLK.
 */
#if !defined(GD32_LXTAL_ENABLED) || defined(__DOXYGEN__)
#define GD32_LXTAL_ENABLED          FALSE
#endif

/**
 * @brief   Enables the IRC32K oscillator branch.
 */
#if !defined(GD32_IRC32K_ENABLED) || defined(__DOXYGEN__)
#define GD32_IRC32K_ENABLED         FALSE
#endif

/**
 * @brief   SYSCLK source selector.
 */
#if !defined(GD32_SYSCLK_SRC) || defined(__DOXYGEN__)
#define GD32_SYSCLK_SRC             GD32_SYSCLKSRC_PLL0P
#endif

/**
 * @brief   IRC64M divider for CK_IRC64MDIV.
 */
#if !defined(GD32_IRC64MDIV) || defined(__DOXYGEN__)
#define GD32_IRC64MDIV              GD32_IRC64MDIV_DIV1
#endif

/**
 * @brief   PLLs source clock selector.
 * @note    PLL0, PLL1 and PLL2 share a single PLLSEL field in RCU_PLLALL,
 *          so this selection applies to all of them.
 */
#if !defined(GD32_PLLSRC) || defined(__DOXYGEN__)
#define GD32_PLLSRC                 GD32_PLLSRC_HXTAL
#endif

/**
 * @name    PLL0 configuration.
 * @note    Default values reproduce the reference
 *          system_clock_600m_hxtal() configuration:
 *          VCOin = 25MHz / 5 = 5MHz (in the 4-8MHz PLL input range),
 *          VCO = 5MHz * 120 = 600MHz, PLL0P = 600MHz / 1 = 600MHz SYSCLK.
 * @{
 */
#if !defined(GD32_PLL0_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL0_ENABLED           TRUE
#endif
#if !defined(GD32_PLL0_PSC) || defined(__DOXYGEN__)
#define GD32_PLL0_PSC               5U
#endif
#if !defined(GD32_PLL0_N) || defined(__DOXYGEN__)
#define GD32_PLL0_N                 120U
#endif
#if !defined(GD32_PLL0_P) || defined(__DOXYGEN__)
#define GD32_PLL0_P                 1U
#endif
#if !defined(GD32_PLL0_Q) || defined(__DOXYGEN__)
#define GD32_PLL0_Q                 2U
#endif
#if !defined(GD32_PLL0_R) || defined(__DOXYGEN__)
#define GD32_PLL0_R                 2U
#endif

/**
 * @brief   PLL0 VCO range selector.
 */
#if !defined(GD32_PLL0VCOSEL) || defined(__DOXYGEN__)
#define GD32_PLL0VCOSEL             RCU_PLL0VCO_192M_836M
#endif

/**
 * @name    PLL0 output enables.
 * @note    PLL0P is enabled by default because it is the usual SYSCLK
 *          source. PLL0Q and PLL0R only feed peripheral kernel clocks and
 *          are left off unless explicitly requested.
 * @{
 */
#if !defined(GD32_PLL0_P_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL0_P_ENABLED         TRUE
#endif
#if !defined(GD32_PLL0_Q_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL0_Q_ENABLED         FALSE
#endif
#if !defined(GD32_PLL0_R_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL0_R_ENABLED         FALSE
#endif
/** @} */
/** @} */

/**
 * @name    PLL1 configuration.
 * @note    Factors use the same logical encoding as the PLL0 ones.
 * @{
 */
#if !defined(GD32_PLL1_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL1_ENABLED           FALSE
#endif
#if !defined(GD32_PLL1_PSC) || defined(__DOXYGEN__)
#define GD32_PLL1_PSC               5U
#endif
#if !defined(GD32_PLL1_N) || defined(__DOXYGEN__)
#define GD32_PLL1_N                 120U
#endif
#if !defined(GD32_PLL1_P) || defined(__DOXYGEN__)
#define GD32_PLL1_P                 2U
#endif
#if !defined(GD32_PLL1_Q) || defined(__DOXYGEN__)
#define GD32_PLL1_Q                 2U
#endif
#if !defined(GD32_PLL1_R) || defined(__DOXYGEN__)
#define GD32_PLL1_R                 2U
#endif
#if !defined(GD32_PLL1VCOSEL) || defined(__DOXYGEN__)
#define GD32_PLL1VCOSEL             RCU_PLL1VCO_192M_836M
#endif
#if !defined(GD32_PLL1_P_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL1_P_ENABLED         FALSE
#endif
#if !defined(GD32_PLL1_Q_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL1_Q_ENABLED         FALSE
#endif
#if !defined(GD32_PLL1_R_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL1_R_ENABLED         FALSE
#endif
/** @} */

/**
 * @name    PLL2 configuration.
 * @note    Factors use the same logical encoding as the PLL0 ones.
 * @{
 */
#if !defined(GD32_PLL2_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL2_ENABLED           FALSE
#endif
#if !defined(GD32_PLL2_PSC) || defined(__DOXYGEN__)
#define GD32_PLL2_PSC               5U
#endif
#if !defined(GD32_PLL2_N) || defined(__DOXYGEN__)
#define GD32_PLL2_N                 120U
#endif
#if !defined(GD32_PLL2_P) || defined(__DOXYGEN__)
#define GD32_PLL2_P                 2U
#endif
#if !defined(GD32_PLL2_Q) || defined(__DOXYGEN__)
#define GD32_PLL2_Q                 2U
#endif
#if !defined(GD32_PLL2_R) || defined(__DOXYGEN__)
#define GD32_PLL2_R                 2U
#endif
#if !defined(GD32_PLL2VCOSEL) || defined(__DOXYGEN__)
#define GD32_PLL2VCOSEL             RCU_PLL2VCO_192M_836M
#endif
#if !defined(GD32_PLL2_P_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL2_P_ENABLED         FALSE
#endif
#if !defined(GD32_PLL2_Q_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL2_Q_ENABLED         FALSE
#endif
#if !defined(GD32_PLL2_R_ENABLED) || defined(__DOXYGEN__)
#define GD32_PLL2_R_ENABLED         FALSE
#endif
/** @} */

/**
 * @name    Bus prescalers.
 * @note    Defaults reproduce the reference system_clock_600m_hxtal()
 *          configuration: AHB = SYSCLK/2, APB1 = AHB/2, APB2 = AHB/1,
 *          APB3 = AHB/2, APB4 = AHB/2.
 * @{
 */
#if !defined(GD32_AHBPSC) || defined(__DOXYGEN__)
#define GD32_AHBPSC                  GD32_AHBPSC_DIV2
#endif
#if !defined(GD32_APB1PSC) || defined(__DOXYGEN__)
#define GD32_APB1PSC                 GD32_APBPSC_DIV2
#endif
#if !defined(GD32_APB2PSC) || defined(__DOXYGEN__)
#define GD32_APB2PSC                 GD32_APBPSC_DIV1
#endif
#if !defined(GD32_APB3PSC) || defined(__DOXYGEN__)
#define GD32_APB3PSC                 GD32_APBPSC_DIV2
#endif
#if !defined(GD32_APB4PSC) || defined(__DOXYGEN__)
#define GD32_APB4PSC                 GD32_APBPSC_DIV2
#endif
/** @} */

/**
 * @name    RTC/LXTAL/IRC32K related settings.
 * @{
 */
#if !defined(GD32_RTC_ENABLED) || defined(__DOXYGEN__)
#define GD32_RTC_ENABLED            FALSE
#endif
#if !defined(GD32_RTCSEL) || defined(__DOXYGEN__)
#define GD32_RTCSEL                 GD32_RTCSEL_NONE
#endif
#if !defined(GD32_RTCDIV) || defined(__DOXYGEN__)
#define GD32_RTCDIV                 25U
#endif
/** @} */

/**
 * @name    USART kernel clock sources.
 * @note    Defaults select CK_APB, which is the RCU_CFG1 reset state and
 *          matches the vendor rcu_usart_clock_config() default.
 * @{
 */
#if !defined(GD32_USART0SEL) || defined(__DOXYGEN__)
#define GD32_USART0SEL              GD32_USARTSRC_APB
#endif
#if !defined(GD32_USART1SEL) || defined(__DOXYGEN__)
#define GD32_USART1SEL              GD32_USARTSRC_APB
#endif
#if !defined(GD32_USART2SEL) || defined(__DOXYGEN__)
#define GD32_USART2SEL              GD32_USARTSRC_APB
#endif
#if !defined(GD32_USART5SEL) || defined(__DOXYGEN__)
#define GD32_USART5SEL              GD32_USARTSRC_APB
#endif
/** @} */

/**
 * @name    Peripheral kernel clock sources.
 * @note    Every default below is the reset state of the matching RCU
 *          selector field, so leaving them untouched reproduces the clock
 *          tree the device comes up with.
 * @{
 */
#if !defined(GD32_I2C0SEL) || defined(__DOXYGEN__)
#define GD32_I2C0SEL                GD32_I2CSRC_APB1
#endif
#if !defined(GD32_I2C1SEL) || defined(__DOXYGEN__)
#define GD32_I2C1SEL                GD32_I2CSRC_APB1
#endif
#if !defined(GD32_I2C2SEL) || defined(__DOXYGEN__)
#define GD32_I2C2SEL                GD32_I2CSRC_APB1
#endif
#if !defined(GD32_I2C3SEL) || defined(__DOXYGEN__)
#define GD32_I2C3SEL                GD32_I2CSRC_APB1
#endif

#if !defined(GD32_SPI0SEL) || defined(__DOXYGEN__)
#define GD32_SPI0SEL                GD32_SPISRC_PLL0Q
#endif
#if !defined(GD32_SPI1SEL) || defined(__DOXYGEN__)
#define GD32_SPI1SEL                GD32_SPISRC_PLL0Q
#endif
#if !defined(GD32_SPI2SEL) || defined(__DOXYGEN__)
#define GD32_SPI2SEL                GD32_SPISRC_PLL0Q
#endif
#if !defined(GD32_SPI3SEL) || defined(__DOXYGEN__)
#define GD32_SPI3SEL                GD32_SPISRC_APB2
#endif
#if !defined(GD32_SPI4SEL) || defined(__DOXYGEN__)
#define GD32_SPI4SEL                GD32_SPISRC_APB2
#endif
#if !defined(GD32_SPI5SEL) || defined(__DOXYGEN__)
#define GD32_SPI5SEL                GD32_SPISRC_APB2
#endif

#if !defined(GD32_ADC01SEL) || defined(__DOXYGEN__)
#define GD32_ADC01SEL               GD32_ADCSRC_PLL1P
#endif
#if !defined(GD32_ADC2SEL) || defined(__DOXYGEN__)
#define GD32_ADC2SEL                GD32_ADCSRC_PLL1P
#endif

#if !defined(GD32_SDIO0SEL) || defined(__DOXYGEN__)
#define GD32_SDIO0SEL               GD32_SDIOSRC_PLL0Q
#endif
#if !defined(GD32_SDIO1SEL) || defined(__DOXYGEN__)
#define GD32_SDIO1SEL               GD32_SDIOSRC_PLL0Q
#endif

#if !defined(GD32_CAN0SEL) || defined(__DOXYGEN__)
#define GD32_CAN0SEL                GD32_CANSRC_HXTAL
#endif
#if !defined(GD32_CAN1SEL) || defined(__DOXYGEN__)
#define GD32_CAN1SEL                GD32_CANSRC_HXTAL
#endif
#if !defined(GD32_CAN2SEL) || defined(__DOXYGEN__)
#define GD32_CAN2SEL                GD32_CANSRC_HXTAL
#endif

#if !defined(GD32_EXMCSEL) || defined(__DOXYGEN__)
#define GD32_EXMCSEL                GD32_EXMCSRC_AHB
#endif
#if !defined(GD32_PERSEL) || defined(__DOXYGEN__)
#define GD32_PERSEL                 GD32_PERSRC_IRC64MDIV
#endif
#if !defined(GD32_TIMERSEL) || defined(__DOXYGEN__)
#define GD32_TIMERSEL               GD32_TIMERSRC_MUL2
#endif
/** @} */

/**
 * @brief   Enables a no-cache RAM area using the MPU.
 * @note    The Cortex-M7 D-Cache is not coherent with the DMA bus masters,
 *          shared buffers must either live in this area or be maintained
 *          with @p cacheBufferFlush() and @p cacheBufferInvalidate().
 * @note    Buffers are placed here by naming them @p __nocache_xxx or
 *          @p __enet_xxx, the linker script collects those into sections
 *          that must be covered by the region configured below.
 */
#if !defined(GD32_NOCACHE_ENABLE) || defined(__DOXYGEN__)
#define GD32_NOCACHE_ENABLE                 FALSE
#endif

/**
 * @brief   MPU region to be used for no-cache RAM area.
 */
#if !defined(GD32_NOCACHE_MPU_REGION) || defined(__DOXYGEN__)
#define GD32_NOCACHE_MPU_REGION             MPU_REGION_6
#endif

/**
 * @brief   Base address to be used for no-cache RAM area.
 */
#if !defined(GD32_NOCACHE_RBAR) || defined(__DOXYGEN__)
#define GD32_NOCACHE_RBAR                   0x30000000U
#endif

/**
 * @brief   Size to be used for no-cache RAM area.
 */
#if !defined(GD32_NOCACHE_RASR) || defined(__DOXYGEN__)
#define GD32_NOCACHE_RASR                   MPU_RASR_SIZE_32K
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                      */
/*===========================================================================*/

/* Helpers required by the checks below, the GD32 specific helpers are
   included at the end of this file because they need the derived clocks.*/
#include "nvic.h"
#include "cache.h"
#include "mpu_v7m.h"

#if (GD32_NOCACHE_RASR != MPU_RASR_SIZE_32)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_64)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_128)  &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_256)  &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_512)  &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_1K)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_2K)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_4K)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_8K)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_16K)  &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_32K)  &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_64K)  &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_128K) &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_256K) &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_512K) &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_1M)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_2M)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_4M)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_8M)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_16M)  &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_32M)  &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_64M)  &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_128M) &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_256M) &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_512M) &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_1G)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_2G)   &&                            \
    (GD32_NOCACHE_RASR != MPU_RASR_SIZE_4G)
#error "invalid MPU RASR size value"
#endif

/**
 * @name    Absolute Maximum Ratings
 * @{
 */
#define GD32_HXTALCLK_MIN            4000000U
#define GD32_HXTALCLK_MAX            50000000U

#define GD32_LXTALCLK                32768U

#define GD32_IRC64MCLK               64000000U
#define GD32_LPIRC4MCLK              4000000U
#define GD32_IRC32KCLK               32000U

#define GD32_PLL_VCO_IN_MIN          1000000U
#define GD32_PLL_VCO_IN_THRESHOLD1   2000000U
#define GD32_PLL_VCO_IN_THRESHOLD2   4000000U
#define GD32_PLL_VCO_IN_THRESHOLD3   8000000U
#define GD32_PLL_VCO_IN_MAX          16000000U

/**
 * @brief   Absolute maximum system clock.
 */
#define GD32_SYSCLK_MAX              600000000U

#define GD32_PLLPSC_DIV_MIN          1U
#define GD32_PLLPSC_DIV_MAX          63U
#define GD32_PLLN_MUL_MIN            9U
#define GD32_PLLN_MUL_MAX            512U
#define GD32_PLLP_DIV_MIN            1U
#define GD32_PLLP_DIV_MAX            128U
#define GD32_PLLQ_DIV_MIN            1U
#define GD32_PLLQ_DIV_MAX            128U
#define GD32_PLLR_DIV_MIN            1U
#define GD32_PLLR_DIV_MAX            128U
/** @} */

/*
 * IRC64MDIV source.
 */
#if GD32_IRC64MDIV == GD32_IRC64MDIV_DIV1
#define GD32_IRC64MDIV_VALUE         1U
#elif GD32_IRC64MDIV == GD32_IRC64MDIV_DIV2
#define GD32_IRC64MDIV_VALUE         2U
#elif GD32_IRC64MDIV == GD32_IRC64MDIV_DIV4
#define GD32_IRC64MDIV_VALUE         4U
#elif GD32_IRC64MDIV == GD32_IRC64MDIV_DIV8
#define GD32_IRC64MDIV_VALUE         8U
#else
#error "invalid GD32_IRC64MDIV value specified"
#endif

#define GD32_IRC64MDIVCLK            (GD32_IRC64MCLK / GD32_IRC64MDIV_VALUE)

/*
 * PLL source clock, shared by PLL0/PLL1/PLL2.
 */
#if (GD32_PLLSRC == GD32_PLLSRC_HXTAL) || defined(__DOXYGEN__)
#define GD32_PLLSRCCLK               GD32_HXTALCLK

#if (GD32_HXTALCLK < GD32_HXTALCLK_MIN) || (GD32_HXTALCLK > GD32_HXTALCLK_MAX)
#error "GD32_HXTALCLK outside the acceptable GD32_HXTALCLK_MIN..GD32_HXTALCLK_MAX range"
#endif

#elif GD32_PLLSRC == GD32_PLLSRC_IRC64MDIV
#define GD32_PLLSRCCLK               GD32_IRC64MDIVCLK

#elif GD32_PLLSRC == GD32_PLLSRC_LPIRC4M
#define GD32_PLLSRCCLK               GD32_LPIRC4MCLK

#else
#error "invalid GD32_PLLSRC value specified"
#endif

/*
 * PLL0.
 */
#if (GD32_PLL0_ENABLED == TRUE) || defined(__DOXYGEN__)

#if (GD32_PLL0_PSC < GD32_PLLPSC_DIV_MIN) || (GD32_PLL0_PSC > GD32_PLLPSC_DIV_MAX)
#error "GD32_PLL0_PSC outside the acceptable range"
#endif
#if (GD32_PLL0_N < GD32_PLLN_MUL_MIN) || (GD32_PLL0_N > GD32_PLLN_MUL_MAX)
#error "GD32_PLL0_N outside the acceptable range"
#endif
#if (GD32_PLL0_P < GD32_PLLP_DIV_MIN) || (GD32_PLL0_P > GD32_PLLP_DIV_MAX)
#error "GD32_PLL0_P outside the acceptable range"
#endif
#if (GD32_PLL0_Q < GD32_PLLQ_DIV_MIN) || (GD32_PLL0_Q > GD32_PLLQ_DIV_MAX)
#error "GD32_PLL0_Q outside the acceptable range"
#endif
#if (GD32_PLL0_R < GD32_PLLR_DIV_MIN) || (GD32_PLL0_R > GD32_PLLR_DIV_MAX)
#error "GD32_PLL0_R outside the acceptable range"
#endif

#define GD32_PLL0_VCO_IN             (GD32_PLLSRCCLK / GD32_PLL0_PSC)

#if (GD32_PLL0_VCO_IN < GD32_PLL_VCO_IN_MIN) || (GD32_PLL0_VCO_IN > GD32_PLL_VCO_IN_MAX)
#error "GD32_PLL0_VCO_IN outside the acceptable GD32 PLL input range"
#endif

#if GD32_PLL0_VCO_IN <= GD32_PLL_VCO_IN_THRESHOLD1
#define GD32_PLL0RNG                 RCU_PLL0RNG_1M_2M
#elif GD32_PLL0_VCO_IN <= GD32_PLL_VCO_IN_THRESHOLD2
#define GD32_PLL0RNG                 RCU_PLL0RNG_2M_4M
#elif GD32_PLL0_VCO_IN <= GD32_PLL_VCO_IN_THRESHOLD3
#define GD32_PLL0RNG                 RCU_PLL0RNG_4M_8M
#else
#define GD32_PLL0RNG                 RCU_PLL0RNG_8M_16M
#endif

#define GD32_PLL0_VCO                (GD32_PLL0_VCO_IN * GD32_PLL0_N)

/* Disabled outputs must report 0Hz, a peripheral fed by a stopped kernel
   clock locks the bus on the first register access.*/
#if GD32_PLL0_P_ENABLED == TRUE
#define GD32_PLL0P_CLK               (GD32_PLL0_VCO / GD32_PLL0_P)
#else
#define GD32_PLL0P_CLK               0U
#endif
#if GD32_PLL0_Q_ENABLED == TRUE
#define GD32_PLL0Q_CLK               (GD32_PLL0_VCO / GD32_PLL0_Q)
#else
#define GD32_PLL0Q_CLK               0U
#endif
#if GD32_PLL0_R_ENABLED == TRUE
#define GD32_PLL0R_CLK               (GD32_PLL0_VCO / GD32_PLL0_R)
#else
#define GD32_PLL0R_CLK               0U
#endif

#if (GD32_SYSCLK_SRC == GD32_SYSCLKSRC_PLL0P) && (GD32_PLL0_P_ENABLED != TRUE)
#error "GD32_SYSCLKSRC_PLL0P requires GD32_PLL0_P_ENABLED == TRUE"
#endif

#else /* GD32_PLL0_ENABLED == FALSE */

#if GD32_SYSCLK_SRC == GD32_SYSCLKSRC_PLL0P
#error "GD32_SYSCLKSRC_PLL0P requires GD32_PLL0_ENABLED == TRUE"
#endif

#define GD32_PLL0RNG                 RCU_PLL0RNG_1M_2M
#define GD32_PLL0_VCO_IN             0U
#define GD32_PLL0_VCO                0U
#define GD32_PLL0P_CLK               0U
#define GD32_PLL0Q_CLK               0U
#define GD32_PLL0R_CLK               0U

#endif /* GD32_PLL0_ENABLED */

/*
 * PLL1.
 */
#if GD32_PLL1_ENABLED == TRUE

#if (GD32_PLL1_PSC < GD32_PLLPSC_DIV_MIN) || (GD32_PLL1_PSC > GD32_PLLPSC_DIV_MAX)
#error "GD32_PLL1_PSC outside the acceptable range"
#endif
#if (GD32_PLL1_N < GD32_PLLN_MUL_MIN) || (GD32_PLL1_N > GD32_PLLN_MUL_MAX)
#error "GD32_PLL1_N outside the acceptable range"
#endif
#if (GD32_PLL1_P < GD32_PLLP_DIV_MIN) || (GD32_PLL1_P > GD32_PLLP_DIV_MAX)
#error "GD32_PLL1_P outside the acceptable range"
#endif
#if (GD32_PLL1_Q < GD32_PLLQ_DIV_MIN) || (GD32_PLL1_Q > GD32_PLLQ_DIV_MAX)
#error "GD32_PLL1_Q outside the acceptable range"
#endif
#if (GD32_PLL1_R < GD32_PLLR_DIV_MIN) || (GD32_PLL1_R > GD32_PLLR_DIV_MAX)
#error "GD32_PLL1_R outside the acceptable range"
#endif

#define GD32_PLL1_VCO_IN             (GD32_PLLSRCCLK / GD32_PLL1_PSC)

#if (GD32_PLL1_VCO_IN < GD32_PLL_VCO_IN_MIN) || (GD32_PLL1_VCO_IN > GD32_PLL_VCO_IN_MAX)
#error "GD32_PLL1_VCO_IN outside the acceptable GD32 PLL input range"
#endif

#if GD32_PLL1_VCO_IN <= GD32_PLL_VCO_IN_THRESHOLD1
#define GD32_PLL1RNG                 RCU_PLL1RNG_1M_2M
#elif GD32_PLL1_VCO_IN <= GD32_PLL_VCO_IN_THRESHOLD2
#define GD32_PLL1RNG                 RCU_PLL1RNG_2M_4M
#elif GD32_PLL1_VCO_IN <= GD32_PLL_VCO_IN_THRESHOLD3
#define GD32_PLL1RNG                 RCU_PLL1RNG_4M_8M
#else
#define GD32_PLL1RNG                 RCU_PLL1RNG_8M_16M
#endif

#define GD32_PLL1_VCO                (GD32_PLL1_VCO_IN * GD32_PLL1_N)
#if GD32_PLL1_P_ENABLED == TRUE
#define GD32_PLL1P_CLK               (GD32_PLL1_VCO / GD32_PLL1_P)
#else
#define GD32_PLL1P_CLK               0U
#endif
#if GD32_PLL1_Q_ENABLED == TRUE
#define GD32_PLL1Q_CLK               (GD32_PLL1_VCO / GD32_PLL1_Q)
#else
#define GD32_PLL1Q_CLK               0U
#endif
#if GD32_PLL1_R_ENABLED == TRUE
#define GD32_PLL1R_CLK               (GD32_PLL1_VCO / GD32_PLL1_R)
#else
#define GD32_PLL1R_CLK               0U
#endif

#else

#define GD32_PLL1RNG                 RCU_PLL1RNG_1M_2M
#define GD32_PLL1_VCO_IN             0U
#define GD32_PLL1_VCO                0U
#define GD32_PLL1P_CLK               0U
#define GD32_PLL1Q_CLK               0U
#define GD32_PLL1R_CLK               0U

#endif

/*
 * PLL2.
 */
#if GD32_PLL2_ENABLED == TRUE

#if (GD32_PLL2_PSC < GD32_PLLPSC_DIV_MIN) || (GD32_PLL2_PSC > GD32_PLLPSC_DIV_MAX)
#error "GD32_PLL2_PSC outside the acceptable range"
#endif
#if (GD32_PLL2_N < GD32_PLLN_MUL_MIN) || (GD32_PLL2_N > GD32_PLLN_MUL_MAX)
#error "GD32_PLL2_N outside the acceptable range"
#endif
#if (GD32_PLL2_P < GD32_PLLP_DIV_MIN) || (GD32_PLL2_P > GD32_PLLP_DIV_MAX)
#error "GD32_PLL2_P outside the acceptable range"
#endif
#if (GD32_PLL2_Q < GD32_PLLQ_DIV_MIN) || (GD32_PLL2_Q > GD32_PLLQ_DIV_MAX)
#error "GD32_PLL2_Q outside the acceptable range"
#endif
#if (GD32_PLL2_R < GD32_PLLR_DIV_MIN) || (GD32_PLL2_R > GD32_PLLR_DIV_MAX)
#error "GD32_PLL2_R outside the acceptable range"
#endif

#define GD32_PLL2_VCO_IN             (GD32_PLLSRCCLK / GD32_PLL2_PSC)

#if (GD32_PLL2_VCO_IN < GD32_PLL_VCO_IN_MIN) || (GD32_PLL2_VCO_IN > GD32_PLL_VCO_IN_MAX)
#error "GD32_PLL2_VCO_IN outside the acceptable GD32 PLL input range"
#endif

#if GD32_PLL2_VCO_IN <= GD32_PLL_VCO_IN_THRESHOLD1
#define GD32_PLL2RNG                 RCU_PLL2RNG_1M_2M
#elif GD32_PLL2_VCO_IN <= GD32_PLL_VCO_IN_THRESHOLD2
#define GD32_PLL2RNG                 RCU_PLL2RNG_2M_4M
#elif GD32_PLL2_VCO_IN <= GD32_PLL_VCO_IN_THRESHOLD3
#define GD32_PLL2RNG                 RCU_PLL2RNG_4M_8M
#else
#define GD32_PLL2RNG                 RCU_PLL2RNG_8M_16M
#endif

#define GD32_PLL2_VCO                (GD32_PLL2_VCO_IN * GD32_PLL2_N)
#if GD32_PLL2_P_ENABLED == TRUE
#define GD32_PLL2P_CLK               (GD32_PLL2_VCO / GD32_PLL2_P)
#else
#define GD32_PLL2P_CLK               0U
#endif
#if GD32_PLL2_Q_ENABLED == TRUE
#define GD32_PLL2Q_CLK               (GD32_PLL2_VCO / GD32_PLL2_Q)
#else
#define GD32_PLL2Q_CLK               0U
#endif
#if GD32_PLL2_R_ENABLED == TRUE
#define GD32_PLL2R_CLK               (GD32_PLL2_VCO / GD32_PLL2_R)
#else
#define GD32_PLL2R_CLK               0U
#endif

#else

#define GD32_PLL2RNG                 RCU_PLL2RNG_1M_2M
#define GD32_PLL2_VCO_IN             0U
#define GD32_PLL2_VCO                0U
#define GD32_PLL2P_CLK               0U
#define GD32_PLL2Q_CLK               0U
#define GD32_PLL2R_CLK               0U

#endif

/*
 * System clock and bus clocks.
 */
#if GD32_SYSCLK_SRC == GD32_SYSCLKSRC_PLL0P
#define GD32_SYSCLK                  GD32_PLL0P_CLK
#elif GD32_SYSCLK_SRC == GD32_SYSCLKSRC_HXTAL
#define GD32_SYSCLK                  GD32_HXTALCLK
#elif GD32_SYSCLK_SRC == GD32_SYSCLKSRC_IRC64MDIV
#define GD32_SYSCLK                  GD32_IRC64MDIVCLK
#elif GD32_SYSCLK_SRC == GD32_SYSCLKSRC_LPIRC4M
#define GD32_SYSCLK                  GD32_LPIRC4MCLK
#else
#error "invalid GD32_SYSCLK_SRC value specified"
#endif

#if GD32_SYSCLK > GD32_SYSCLK_MAX
#error "GD32_SYSCLK exceeding maximum frequency (GD32_SYSCLK_MAX)"
#endif

/**
 * @brief   AHB prescaler divider value.
 */
#if GD32_AHBPSC == GD32_AHBPSC_DIV1
#define GD32_AHBPSC_VALUE            1U
#elif GD32_AHBPSC == GD32_AHBPSC_DIV2
#define GD32_AHBPSC_VALUE            2U
#elif GD32_AHBPSC == GD32_AHBPSC_DIV4
#define GD32_AHBPSC_VALUE            4U
#elif GD32_AHBPSC == GD32_AHBPSC_DIV8
#define GD32_AHBPSC_VALUE            8U
#elif GD32_AHBPSC == GD32_AHBPSC_DIV16
#define GD32_AHBPSC_VALUE            16U
#elif GD32_AHBPSC == GD32_AHBPSC_DIV64
#define GD32_AHBPSC_VALUE            64U
#elif GD32_AHBPSC == GD32_AHBPSC_DIV128
#define GD32_AHBPSC_VALUE            128U
#elif GD32_AHBPSC == GD32_AHBPSC_DIV256
#define GD32_AHBPSC_VALUE            256U
#elif GD32_AHBPSC == GD32_AHBPSC_DIV512
#define GD32_AHBPSC_VALUE            512U
#else
#error "invalid GD32_AHBPSC value specified"
#endif

/**
 * @brief   AHB clock (HCLK), in Hz.
 */
#define GD32_HCLK                    (GD32_SYSCLK / GD32_AHBPSC_VALUE)

/**
 * @brief   APBx prescaler divider values.
 * @{
 */
#if GD32_APB1PSC == GD32_APBPSC_DIV1
#define GD32_APB1PSC_VALUE           1U
#elif GD32_APB1PSC == GD32_APBPSC_DIV2
#define GD32_APB1PSC_VALUE           2U
#elif GD32_APB1PSC == GD32_APBPSC_DIV4
#define GD32_APB1PSC_VALUE           4U
#elif GD32_APB1PSC == GD32_APBPSC_DIV8
#define GD32_APB1PSC_VALUE           8U
#elif GD32_APB1PSC == GD32_APBPSC_DIV16
#define GD32_APB1PSC_VALUE           16U
#else
#error "invalid GD32_APB1PSC value specified"
#endif

#if GD32_APB2PSC == GD32_APBPSC_DIV1
#define GD32_APB2PSC_VALUE           1U
#elif GD32_APB2PSC == GD32_APBPSC_DIV2
#define GD32_APB2PSC_VALUE           2U
#elif GD32_APB2PSC == GD32_APBPSC_DIV4
#define GD32_APB2PSC_VALUE           4U
#elif GD32_APB2PSC == GD32_APBPSC_DIV8
#define GD32_APB2PSC_VALUE           8U
#elif GD32_APB2PSC == GD32_APBPSC_DIV16
#define GD32_APB2PSC_VALUE           16U
#else
#error "invalid GD32_APB2PSC value specified"
#endif

#if GD32_APB3PSC == GD32_APBPSC_DIV1
#define GD32_APB3PSC_VALUE           1U
#elif GD32_APB3PSC == GD32_APBPSC_DIV2
#define GD32_APB3PSC_VALUE           2U
#elif GD32_APB3PSC == GD32_APBPSC_DIV4
#define GD32_APB3PSC_VALUE           4U
#elif GD32_APB3PSC == GD32_APBPSC_DIV8
#define GD32_APB3PSC_VALUE           8U
#elif GD32_APB3PSC == GD32_APBPSC_DIV16
#define GD32_APB3PSC_VALUE           16U
#else
#error "invalid GD32_APB3PSC value specified"
#endif

#if GD32_APB4PSC == GD32_APBPSC_DIV1
#define GD32_APB4PSC_VALUE           1U
#elif GD32_APB4PSC == GD32_APBPSC_DIV2
#define GD32_APB4PSC_VALUE           2U
#elif GD32_APB4PSC == GD32_APBPSC_DIV4
#define GD32_APB4PSC_VALUE           4U
#elif GD32_APB4PSC == GD32_APBPSC_DIV8
#define GD32_APB4PSC_VALUE           8U
#elif GD32_APB4PSC == GD32_APBPSC_DIV16
#define GD32_APB4PSC_VALUE           16U
#else
#error "invalid GD32_APB4PSC value specified"
#endif

/**
 * @brief   APBx peripheral clocks, in Hz.
 * @{
 */
#define GD32_PCLK1                   (GD32_HCLK / GD32_APB1PSC_VALUE)
#define GD32_PCLK2                   (GD32_HCLK / GD32_APB2PSC_VALUE)
#define GD32_PCLK3                   (GD32_HCLK / GD32_APB3PSC_VALUE)
#define GD32_PCLK4                   (GD32_HCLK / GD32_APB4PSC_VALUE)
/** @} */

/*
 * RTC clock.
 */
#if GD32_RTC_ENABLED == TRUE

#if GD32_RTCSEL == GD32_RTCSEL_NONE
#define GD32_RTCCLK                  0U
#elif GD32_RTCSEL == GD32_RTCSEL_LXTAL

#if GD32_LXTAL_ENABLED != TRUE
#error "GD32_RTCSEL_LXTAL requires GD32_LXTAL_ENABLED == TRUE"
#endif

#define GD32_RTCCLK                  GD32_LXTALCLK

#elif GD32_RTCSEL == GD32_RTCSEL_IRC32K

#if GD32_IRC32K_ENABLED != TRUE
#error "GD32_RTCSEL_IRC32K requires GD32_IRC32K_ENABLED == TRUE"
#endif

#define GD32_RTCCLK                  GD32_IRC32KCLK

#elif GD32_RTCSEL == GD32_RTCSEL_HXTAL_DIV_RTCDIV

#if (GD32_RTCDIV < 2U) || (GD32_RTCDIV > 63U)
#error "GD32_RTCDIV must be in the 2..63 range when GD32_RTCSEL_HXTAL_DIV_RTCDIV is selected"
#endif

#define GD32_RTCCLK                  (GD32_HXTALCLK / GD32_RTCDIV)

#else
#error "invalid GD32_RTCSEL value specified"
#endif

#else
#define GD32_RTCCLK                  0U
#endif

/*
 * USART/UART kernel clocks.
 * USART0/1/2/5 follow their RCU_CFG1 USARTxSEL mux, the remaining
 * instances are hardwired to CK_APB1.
 */
#if GD32_USART0SEL == GD32_USARTSRC_APB
#define GD32_USART0CLK               GD32_PCLK2
#elif GD32_USART0SEL == GD32_USARTSRC_AHB
#define GD32_USART0CLK               GD32_HCLK
#elif GD32_USART0SEL == GD32_USARTSRC_LXTAL
#if GD32_LXTAL_ENABLED != TRUE
#error "GD32_USART0SEL requires GD32_LXTAL_ENABLED == TRUE"
#endif
#define GD32_USART0CLK               GD32_LXTALCLK
#elif GD32_USART0SEL == GD32_USARTSRC_IRC64MDIV
#define GD32_USART0CLK               GD32_IRC64MDIVCLK
#else
#error "invalid GD32_USART0SEL value specified"
#endif

#if GD32_USART1SEL == GD32_USARTSRC_APB
#define GD32_USART1CLK               GD32_PCLK1
#elif GD32_USART1SEL == GD32_USARTSRC_AHB
#define GD32_USART1CLK               GD32_HCLK
#elif GD32_USART1SEL == GD32_USARTSRC_LXTAL
#if GD32_LXTAL_ENABLED != TRUE
#error "GD32_USART1SEL requires GD32_LXTAL_ENABLED == TRUE"
#endif
#define GD32_USART1CLK               GD32_LXTALCLK
#elif GD32_USART1SEL == GD32_USARTSRC_IRC64MDIV
#define GD32_USART1CLK               GD32_IRC64MDIVCLK
#else
#error "invalid GD32_USART1SEL value specified"
#endif

#if GD32_USART2SEL == GD32_USARTSRC_APB
#define GD32_USART2CLK               GD32_PCLK1
#elif GD32_USART2SEL == GD32_USARTSRC_AHB
#define GD32_USART2CLK               GD32_HCLK
#elif GD32_USART2SEL == GD32_USARTSRC_LXTAL
#if GD32_LXTAL_ENABLED != TRUE
#error "GD32_USART2SEL requires GD32_LXTAL_ENABLED == TRUE"
#endif
#define GD32_USART2CLK               GD32_LXTALCLK
#elif GD32_USART2SEL == GD32_USARTSRC_IRC64MDIV
#define GD32_USART2CLK               GD32_IRC64MDIVCLK
#else
#error "invalid GD32_USART2SEL value specified"
#endif

#if GD32_USART5SEL == GD32_USARTSRC_APB
#define GD32_USART5CLK               GD32_PCLK2
#elif GD32_USART5SEL == GD32_USARTSRC_AHB
#define GD32_USART5CLK               GD32_HCLK
#elif GD32_USART5SEL == GD32_USARTSRC_LXTAL
#if GD32_LXTAL_ENABLED != TRUE
#error "GD32_USART5SEL requires GD32_LXTAL_ENABLED == TRUE"
#endif
#define GD32_USART5CLK               GD32_LXTALCLK
#elif GD32_USART5SEL == GD32_USARTSRC_IRC64MDIV
#define GD32_USART5CLK               GD32_IRC64MDIVCLK
#else
#error "invalid GD32_USART5SEL value specified"
#endif

#define GD32_UART3CLK                GD32_PCLK1
#define GD32_UART4CLK                GD32_PCLK1
#define GD32_UART6CLK                GD32_PCLK1
#define GD32_UART7CLK                GD32_PCLK1

/*
 * CK_PER, the shared kernel clock feeding the ADC and EXMC muxes.
 */
#if GD32_PERSEL == GD32_PERSRC_IRC64MDIV
#define GD32_PERCLK                  GD32_IRC64MDIVCLK
#elif GD32_PERSEL == GD32_PERSRC_LPIRC4M
#define GD32_PERCLK                  GD32_LPIRC4MCLK
#elif GD32_PERSEL == GD32_PERSRC_HXTAL
#define GD32_PERCLK                  GD32_HXTALCLK
#else
#error "invalid GD32_PERSEL value specified"
#endif

/*
 * I2C kernel clocks.
 */
#if GD32_I2C0SEL == GD32_I2CSRC_APB1
#define GD32_I2C0CLK                 GD32_PCLK1
#elif GD32_I2C0SEL == GD32_I2CSRC_PLL2R
#if (GD32_PLL2_ENABLED != TRUE) || (GD32_PLL2_R_ENABLED != TRUE)
#error "GD32_I2C0SEL_PLL2R requires GD32_PLL2_ENABLED and GD32_PLL2_R_ENABLED == TRUE"
#endif
#define GD32_I2C0CLK                 GD32_PLL2R_CLK
#elif GD32_I2C0SEL == GD32_I2CSRC_IRC64MDIV
#define GD32_I2C0CLK                 GD32_IRC64MDIVCLK
#else
#error "invalid GD32_I2C0SEL value specified"
#endif

#if GD32_I2C1SEL == GD32_I2CSRC_APB1
#define GD32_I2C1CLK                 GD32_PCLK1
#elif GD32_I2C1SEL == GD32_I2CSRC_PLL2R
#if (GD32_PLL2_ENABLED != TRUE) || (GD32_PLL2_R_ENABLED != TRUE)
#error "GD32_I2C1SEL_PLL2R requires GD32_PLL2_ENABLED and GD32_PLL2_R_ENABLED == TRUE"
#endif
#define GD32_I2C1CLK                 GD32_PLL2R_CLK
#elif GD32_I2C1SEL == GD32_I2CSRC_IRC64MDIV
#define GD32_I2C1CLK                 GD32_IRC64MDIVCLK
#else
#error "invalid GD32_I2C1SEL value specified"
#endif

#if GD32_I2C2SEL == GD32_I2CSRC_APB1
#define GD32_I2C2CLK                 GD32_PCLK1
#elif GD32_I2C2SEL == GD32_I2CSRC_PLL2R
#if (GD32_PLL2_ENABLED != TRUE) || (GD32_PLL2_R_ENABLED != TRUE)
#error "GD32_I2C2SEL_PLL2R requires GD32_PLL2_ENABLED and GD32_PLL2_R_ENABLED == TRUE"
#endif
#define GD32_I2C2CLK                 GD32_PLL2R_CLK
#elif GD32_I2C2SEL == GD32_I2CSRC_IRC64MDIV
#define GD32_I2C2CLK                 GD32_IRC64MDIVCLK
#else
#error "invalid GD32_I2C2SEL value specified"
#endif

#if GD32_I2C3SEL == GD32_I2CSRC_APB1
#define GD32_I2C3CLK                 GD32_PCLK1
#elif GD32_I2C3SEL == GD32_I2CSRC_PLL2R
#if (GD32_PLL2_ENABLED != TRUE) || (GD32_PLL2_R_ENABLED != TRUE)
#error "GD32_I2C3SEL_PLL2R requires GD32_PLL2_ENABLED and GD32_PLL2_R_ENABLED == TRUE"
#endif
#define GD32_I2C3CLK                 GD32_PLL2R_CLK
#elif GD32_I2C3SEL == GD32_I2CSRC_IRC64MDIV
#define GD32_I2C3CLK                 GD32_IRC64MDIVCLK
#else
#error "invalid GD32_I2C3SEL value specified"
#endif

/*
 * SPI/I2S kernel clocks.
 * SPI0/1/2 use the audio capable mux, SPI3/4/5 use the general purpose one.
 * GD32_SPISRC_I2S_CKIN maps to the external I2S_CKIN pin whose frequency is
 * board dependent, so it has to be supplied through GD32_I2S_CKIN.
 */
#if !defined(GD32_I2S_CKIN) || defined(__DOXYGEN__)
#define GD32_I2S_CKIN                0U
#endif

#if GD32_SPI0SEL == GD32_SPISRC_PLL0Q
#define GD32_SPI0CLK                 GD32_PLL0Q_CLK
#elif GD32_SPI0SEL == GD32_SPISRC_PLL1P
#define GD32_SPI0CLK                 GD32_PLL1P_CLK
#elif GD32_SPI0SEL == GD32_SPISRC_PLL2P
#define GD32_SPI0CLK                 GD32_PLL2P_CLK
#elif GD32_SPI0SEL == GD32_SPISRC_I2S_CKIN
#define GD32_SPI0CLK                 GD32_I2S_CKIN
#elif GD32_SPI0SEL == GD32_SPISRC_PER
#define GD32_SPI0CLK                 GD32_PERCLK
#else
#error "invalid GD32_SPI0SEL value specified"
#endif

#if GD32_SPI1SEL == GD32_SPISRC_PLL0Q
#define GD32_SPI1CLK                 GD32_PLL0Q_CLK
#elif GD32_SPI1SEL == GD32_SPISRC_PLL1P
#define GD32_SPI1CLK                 GD32_PLL1P_CLK
#elif GD32_SPI1SEL == GD32_SPISRC_PLL2P
#define GD32_SPI1CLK                 GD32_PLL2P_CLK
#elif GD32_SPI1SEL == GD32_SPISRC_I2S_CKIN
#define GD32_SPI1CLK                 GD32_I2S_CKIN
#elif GD32_SPI1SEL == GD32_SPISRC_PER
#define GD32_SPI1CLK                 GD32_PERCLK
#else
#error "invalid GD32_SPI1SEL value specified"
#endif

#if GD32_SPI2SEL == GD32_SPISRC_PLL0Q
#define GD32_SPI2CLK                 GD32_PLL0Q_CLK
#elif GD32_SPI2SEL == GD32_SPISRC_PLL1P
#define GD32_SPI2CLK                 GD32_PLL1P_CLK
#elif GD32_SPI2SEL == GD32_SPISRC_PLL2P
#define GD32_SPI2CLK                 GD32_PLL2P_CLK
#elif GD32_SPI2SEL == GD32_SPISRC_I2S_CKIN
#define GD32_SPI2CLK                 GD32_I2S_CKIN
#elif GD32_SPI2SEL == GD32_SPISRC_PER
#define GD32_SPI2CLK                 GD32_PERCLK
#else
#error "invalid GD32_SPI2SEL value specified"
#endif

#if GD32_SPI3SEL == GD32_SPISRC_APB2
#define GD32_SPI3CLK                 GD32_PCLK2
#elif GD32_SPI3SEL == GD32_SPISRC_PLL1Q
#define GD32_SPI3CLK                 GD32_PLL1Q_CLK
#elif GD32_SPI3SEL == GD32_SPISRC_PLL2Q
#define GD32_SPI3CLK                 GD32_PLL2Q_CLK
#elif GD32_SPI3SEL == GD32_SPISRC_IRC64MDIV
#define GD32_SPI3CLK                 GD32_IRC64MDIVCLK
#elif GD32_SPI3SEL == GD32_SPISRC_LPIRC4M
#define GD32_SPI3CLK                 GD32_LPIRC4MCLK
#elif GD32_SPI3SEL == GD32_SPISRC_HXTAL
#define GD32_SPI3CLK                 GD32_HXTALCLK
#else
#error "invalid GD32_SPI3SEL value specified"
#endif

#if GD32_SPI4SEL == GD32_SPISRC_APB2
#define GD32_SPI4CLK                 GD32_PCLK2
#elif GD32_SPI4SEL == GD32_SPISRC_PLL1Q
#define GD32_SPI4CLK                 GD32_PLL1Q_CLK
#elif GD32_SPI4SEL == GD32_SPISRC_PLL2Q
#define GD32_SPI4CLK                 GD32_PLL2Q_CLK
#elif GD32_SPI4SEL == GD32_SPISRC_IRC64MDIV
#define GD32_SPI4CLK                 GD32_IRC64MDIVCLK
#elif GD32_SPI4SEL == GD32_SPISRC_LPIRC4M
#define GD32_SPI4CLK                 GD32_LPIRC4MCLK
#elif GD32_SPI4SEL == GD32_SPISRC_HXTAL
#define GD32_SPI4CLK                 GD32_HXTALCLK
#else
#error "invalid GD32_SPI4SEL value specified"
#endif

#if GD32_SPI5SEL == GD32_SPISRC_APB2
#define GD32_SPI5CLK                 GD32_PCLK2
#elif GD32_SPI5SEL == GD32_SPISRC_PLL1Q
#define GD32_SPI5CLK                 GD32_PLL1Q_CLK
#elif GD32_SPI5SEL == GD32_SPISRC_PLL2Q
#define GD32_SPI5CLK                 GD32_PLL2Q_CLK
#elif GD32_SPI5SEL == GD32_SPISRC_IRC64MDIV
#define GD32_SPI5CLK                 GD32_IRC64MDIVCLK
#elif GD32_SPI5SEL == GD32_SPISRC_LPIRC4M
#define GD32_SPI5CLK                 GD32_LPIRC4MCLK
#elif GD32_SPI5SEL == GD32_SPISRC_HXTAL
#define GD32_SPI5CLK                 GD32_HXTALCLK
#elif GD32_SPI5SEL == GD32_SPISRC_SPI5_I2S_CKIN
#define GD32_SPI5CLK                 GD32_I2S_CKIN
#else
#error "invalid GD32_SPI5SEL value specified"
#endif

/* The I2S_CKIN pin frequency is board dependent and has no sensible
   default, so it must be declared whenever it is actually selected. */
#if ((GD32_SPI0SEL == GD32_SPISRC_I2S_CKIN) ||                              \
     (GD32_SPI1SEL == GD32_SPISRC_I2S_CKIN) ||                              \
     (GD32_SPI2SEL == GD32_SPISRC_I2S_CKIN) ||                              \
     (GD32_SPI5SEL == GD32_SPISRC_SPI5_I2S_CKIN)) && (GD32_I2S_CKIN == 0U)
#error "GD32_SPISRC_I2S_CKIN selected but GD32_I2S_CKIN is not defined"
#endif

/*
 * ADC kernel clocks, ADC0 and ADC1 share a single selector.
 * @note    The limits apply to CK_ADC, that is after the ADC_SYNCCTL
 *          prescaler, so they are enforced by the ADC driver.
 */
#define GD32_ADC01CLK_MAX            72000000U
#define GD32_ADC2CLK_MAX             80000000U

#if GD32_ADC01SEL == GD32_ADCSRC_PLL1P
#define GD32_ADC01CLK                GD32_PLL1P_CLK
#elif GD32_ADC01SEL == GD32_ADCSRC_PLL2R
#define GD32_ADC01CLK                GD32_PLL2R_CLK
#elif GD32_ADC01SEL == GD32_ADCSRC_PER
#define GD32_ADC01CLK                GD32_PERCLK
#else
#error "invalid GD32_ADC01SEL value specified"
#endif

#if GD32_ADC2SEL == GD32_ADCSRC_PLL1P
#define GD32_ADC2CLK                 GD32_PLL1P_CLK
#elif GD32_ADC2SEL == GD32_ADCSRC_PLL2R
#define GD32_ADC2CLK                 GD32_PLL2R_CLK
#elif GD32_ADC2SEL == GD32_ADCSRC_PER
#define GD32_ADC2CLK                 GD32_PERCLK
#else
#error "invalid GD32_ADC2SEL value specified"
#endif

/*
 * SDIO kernel clocks.
 */
#if GD32_SDIO0SEL == GD32_SDIOSRC_PLL0Q
#define GD32_SDIO0CLK                GD32_PLL0Q_CLK
#elif GD32_SDIO0SEL == GD32_SDIOSRC_PLL1R
#define GD32_SDIO0CLK                GD32_PLL1R_CLK
#else
#error "invalid GD32_SDIO0SEL value specified"
#endif

#if GD32_SDIO1SEL == GD32_SDIOSRC_PLL0Q
#define GD32_SDIO1CLK                GD32_PLL0Q_CLK
#elif GD32_SDIO1SEL == GD32_SDIOSRC_PLL1R
#define GD32_SDIO1CLK                GD32_PLL1R_CLK
#else
#error "invalid GD32_SDIO1SEL value specified"
#endif

/*
 * CAN kernel clocks.
 */
#if GD32_CAN0SEL == GD32_CANSRC_HXTAL
#define GD32_CAN0CLK                 GD32_HXTALCLK
#elif GD32_CAN0SEL == GD32_CANSRC_APB2
#define GD32_CAN0CLK                 GD32_PCLK2
#elif GD32_CAN0SEL == GD32_CANSRC_APB2_DIV2
#define GD32_CAN0CLK                 (GD32_PCLK2 / 2)
#else
#error "invalid GD32_CAN0SEL value specified"
#endif

#if GD32_CAN1SEL == GD32_CANSRC_HXTAL
#define GD32_CAN1CLK                 GD32_HXTALCLK
#elif GD32_CAN1SEL == GD32_CANSRC_APB2
#define GD32_CAN1CLK                 GD32_PCLK2
#elif GD32_CAN1SEL == GD32_CANSRC_APB2_DIV2
#define GD32_CAN1CLK                 (GD32_PCLK2 / 2)
#else
#error "invalid GD32_CAN1SEL value specified"
#endif

#if GD32_CAN2SEL == GD32_CANSRC_HXTAL
#define GD32_CAN2CLK                 GD32_HXTALCLK
#elif GD32_CAN2SEL == GD32_CANSRC_APB2
#define GD32_CAN2CLK                 GD32_PCLK2
#elif GD32_CAN2SEL == GD32_CANSRC_APB2_DIV2
#define GD32_CAN2CLK                 (GD32_PCLK2 / 2)
#else
#error "invalid GD32_CAN2SEL value specified"
#endif

/*
 * EXMC kernel clock.
 */
#if GD32_EXMCSEL == GD32_EXMCSRC_AHB
#define GD32_EXMCCLK                 GD32_HCLK
#elif GD32_EXMCSEL == GD32_EXMCSRC_PLL0Q
#define GD32_EXMCCLK                 GD32_PLL0Q_CLK
#elif GD32_EXMCSEL == GD32_EXMCSRC_PLL1R
#define GD32_EXMCCLK                 GD32_PLL1R_CLK
#elif GD32_EXMCSEL == GD32_EXMCSRC_PER
#define GD32_EXMCCLK                 GD32_PERCLK
#else
#error "invalid GD32_EXMCSEL value specified"
#endif

/*
 * TIMER kernel clocks.
 * The timer clock tracks CK_APBx multiplied by 2 or 4 as allowed by
 * TIMERSEL, but it is clamped to CK_AHB while the APB prescaler is still
 * small enough for the multiplied clock to reach it.
 */
#if GD32_TIMERSEL == GD32_TIMERSRC_MUL2
#if GD32_APB1PSC_VALUE <= 2
#define GD32_TIMERCLK1                 GD32_HCLK
#else
#define GD32_TIMERCLK1                 (GD32_PCLK1 * 2)
#endif
#if GD32_APB2PSC_VALUE <= 2
#define GD32_TIMERCLK2                 GD32_HCLK
#else
#define GD32_TIMERCLK2                 (GD32_PCLK2 * 2)
#endif
#elif GD32_TIMERSEL == GD32_TIMERSRC_MUL4
#if GD32_APB1PSC_VALUE <= 4
#define GD32_TIMERCLK1                 GD32_HCLK
#else
#define GD32_TIMERCLK1                 (GD32_PCLK1 * 4)
#endif
#if GD32_APB2PSC_VALUE <= 4
#define GD32_TIMERCLK2                 GD32_HCLK
#else
#define GD32_TIMERCLK2                 (GD32_PCLK2 * 4)
#endif
#else
#error "invalid GD32_TIMERSEL value specified"
#endif

/* Various helpers.*/
#include "gd32_isr.h"
#include "gd32_rcu.h"
#include "gd32_dma.h"
#include "gd32_exti.h"
#include "gd32_i2c.h"
#include "gd32_timer.h"

/*===========================================================================*/
/* Driver data structures and types.                                        */
/*===========================================================================*/

/**
 * @brief   Type of a structure representing system limits.
 */
typedef struct {
  uint32_t              zero;
} halclkcfg_t;

/*===========================================================================*/
/* Driver macros.                                                           */
/*===========================================================================*/

/**
 * @brief   Returns the frequency of a clock point in Hz.
 * @note    Static implementation: this port has no runtime clock
 *          switching, so every clock point is a compile-time constant
 *          derived above and the whole expression is constant-folded by
 *          the compiler.
 *
 * @param[in] clkpt     clock point to be returned
 * @return              The clock point frequency in Hz or zero if the
 *                      frequency is unknown.
 *
 * @notapi
 */
#define hal_lld_get_clock_point(clkpt)                                      \
  ((clkpt) == CLK_SYSCLK   ? GD32_SYSCLK    :                               \
   (clkpt) == CLK_HCLK     ? GD32_HCLK      :                               \
   (clkpt) == CLK_PCLK1    ? GD32_PCLK1     :                               \
   (clkpt) == CLK_PCLK2    ? GD32_PCLK2     :                               \
   (clkpt) == CLK_PCLK3    ? GD32_PCLK3     :                               \
   (clkpt) == CLK_PCLK4    ? GD32_PCLK4     :                               \
   (clkpt) == CLK_PLL0PCLK ? GD32_PLL0P_CLK :                               \
   (clkpt) == CLK_PLL0QCLK ? GD32_PLL0Q_CLK :                               \
   (clkpt) == CLK_PLL0RCLK ? GD32_PLL0R_CLK :                               \
   (clkpt) == CLK_PLL1PCLK ? GD32_PLL1P_CLK :                               \
   (clkpt) == CLK_PLL1QCLK ? GD32_PLL1Q_CLK :                               \
   (clkpt) == CLK_PLL1RCLK ? GD32_PLL1R_CLK :                               \
   (clkpt) == CLK_PLL2PCLK ? GD32_PLL2P_CLK :                               \
   (clkpt) == CLK_PLL2QCLK ? GD32_PLL2Q_CLK :                               \
   (clkpt) == CLK_PLL2RCLK ? GD32_PLL2R_CLK :                               \
   (clkpt) == CLK_RTCCLK   ? GD32_RTCCLK    :                               \
   (clkpt) == CLK_PERCLK   ? GD32_PERCLK    :                               \
   0U)

/*===========================================================================*/
/* External declarations.                                                   */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void gd32_lld_early_init(void);
  void hal_lld_init(void);
#ifdef __cplusplus
}
#endif

#endif /* HAL_LLD_H */

/** @} */
