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
 * @file    GD32H73x_75x/hal_lld.c
 * @brief   GD32H73x_75x HAL subsystem low level driver source.
 *
 * @addtogroup HAL
 * @{
 */

#include "hal.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   HXTAL stabilization wait timeout, in polling loop iterations.
 * @note    Matches HXTAL_STARTUP_TIMEOUT semantics from the GD32 CMSIS
 *          reference system file.
 */
#if !defined(GD32_HXTAL_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_HXTAL_TIMEOUT          0xFFFFU
#endif

/**
 * @brief   LPIRC4M stabilization wait timeout, in polling loop iterations.
 */
#if !defined(GD32_LPIRC4M_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_LPIRC4M_TIMEOUT        0x0500U
#endif

/**
 * @brief   LXTAL stabilization wait timeout, in polling loop iterations.
 */
#if !defined(GD32_LXTAL_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_LXTAL_TIMEOUT          0x0500U
#endif

/**
 * @brief   IRC32K stabilization wait timeout, in polling loop iterations.
 */
#if !defined(GD32_IRC32K_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_IRC32K_TIMEOUT         0x0500U
#endif

/**
 * @brief   IRC64M stabilization wait timeout, in polling loop iterations.
 * @note    Matches IRC64M_STARTUP_TIMEOUT semantics from the GD32 CMSIS
 *          reference system file.
 */
#if !defined(GD32_IRC64M_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_IRC64M_TIMEOUT         0xFFFFU
#endif

/**
 * @brief   PLL lock wait timeout, in polling loop iterations.
 */
#if !defined(GD32_PLL_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_PLL_TIMEOUT            0xFFFFU
#endif

/**
 * @brief   System clock switch wait timeout, in polling loop iterations.
 */
#if !defined(GD32_SWITCH_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_SWITCH_TIMEOUT         0xFFFFU
#endif

/**
 * @brief   Tells if CK_IRC64MDIV is needed by a USART kernel clock.
 */
#if (GD32_USART0SEL == GD32_USARTSRC_IRC64MDIV) ||                          \
    (GD32_USART1SEL == GD32_USARTSRC_IRC64MDIV) ||                          \
    (GD32_USART2SEL == GD32_USARTSRC_IRC64MDIV) ||                          \
    (GD32_USART5SEL == GD32_USARTSRC_IRC64MDIV)
#define GD32_IRC64M_USART_REQUIRED  TRUE
#else
#define GD32_IRC64M_USART_REQUIRED  FALSE
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   CMSIS system core clock variable.
 * @note    It is declared in system_gd32h73x_75x.h. Unlike the vendor
 *          system_gd32h73x_75x.c (not used by this port, see
 *          gd32_clock_init() below for the actual clock tree setup),
 *          this is a static initialization matching GD32_HCLK.
 */
uint32_t SystemCoreClock = GD32_SYSCLK;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if !GD32_NO_INIT || defined(__DOXYGEN__)
/**
 * @brief   Brings the RCU into a state where the clock tree can be
 *          reconfigured from scratch.
 *
 * @notapi
 */
static void gd32_rcu_reset(void) {
  uint32_t timeout;

  /* IRC64M is the reset default system clock source and the fallback used
     while the PLLs are reconfigured, so it is always started.*/
  RCU_CTL |= RCU_CTL_IRC64MEN;
  timeout = 0U;
  while ((0U == (RCU_CTL & RCU_CTL_IRC64MSTB)) &&
         (timeout != GD32_IRC64M_TIMEOUT)) {
    timeout++;
  }
  osalDbgAssert((RCU_CTL & RCU_CTL_IRC64MSTB) != 0U, "IRC64M failed to start");

  /* Falling back to CK_IRC64MDIV before touching any other clock.*/
  RCU_CFG0 &= ~RCU_CFG0_SCS;
  timeout = 0U;
  while ((RCU_SCSS_IRC64MDIV != (RCU_CFG0 & RCU_CFG0_SCSS)) &&
         (timeout != GD32_SWITCH_TIMEOUT)) {
    timeout++;
  }
  osalDbgAssert(RCU_SCSS_IRC64MDIV == (RCU_CFG0 & RCU_CFG0_SCSS),
                "fallback to IRC64MDIV failed");

  /* Clock monitor disarmed first, stopping HXTAL while it is armed would
     trigger a CKM event.*/
  RCU_CTL &= ~RCU_CTL_CKMEN;

  /* Stopping HXTAL and every PLL, including the USB ones a DFU capable
     bootloader may have left running.*/
  RCU_CTL &= ~(RCU_CTL_HXTALEN | RCU_CTL_HXTALBPS |
               RCU_CTL_PLL0EN | RCU_CTL_PLL1EN | RCU_CTL_PLL2EN);
  RCU_ADDCTL1 &= ~(RCU_ADDCTL1_LPIRC4MEN |
                   RCU_ADDCTL1_PLLUSBHS0EN | RCU_ADDCTL1_PLLUSBHS1EN);

  /* The PLL configuration registers reset below only accept writes once
     the matching stabilization flag has dropped, so the PLLs must be
     given time to actually stop.*/
  timeout = 0U;
  while ((0U != (RCU_CTL & (RCU_CTL_PLL0STB | RCU_CTL_PLL1STB | RCU_CTL_PLL2STB))) &&
         (timeout != GD32_PLL_TIMEOUT)) {
    timeout++;
  }
  osalDbgAssert(0U == (RCU_CTL & (RCU_CTL_PLL0STB | RCU_CTL_PLL1STB |
                                  RCU_CTL_PLL2STB)),
                "PLLs failed to stop");

  /* reset CFG0, CFG1, CFG2, CFG3 registers */
  RCU_CFG0 &= ~(RCU_CFG0_APB1PSC | RCU_CFG0_APB2PSC | RCU_CFG0_APB3PSC |
                RCU_CFG0_APB4PSC | RCU_CFG0_AHBPSC |
                RCU_CFG0_I2C0SEL | RCU_CFG0_SCS | RCU_CFG0_RTCDIV);
#ifndef GD32H75E
  RCU_CFG1 &= ~(RCU_CFG1_HPDFSEL | RCU_CFG1_TIMERSEL | RCU_CFG1_PERSEL |
                RCU_CFG1_RSPDIFSEL | RCU_CFG1_USART0SEL | RCU_CFG1_USART1SEL |
                RCU_CFG1_USART2SEL | RCU_CFG1_USART5SEL | RCU_CFG1_PLL2RDIV |
                RCU_CFG1_CAN0SEL | RCU_CFG1_CAN1SEL | RCU_CFG1_CAN2SEL);
#else
  RCU_CFG1 &= ~(RCU_CFG1_HPDFSEL | RCU_CFG1_TIMERSEL | RCU_CFG1_PERSEL |
                RCU_CFG1_USART0SEL | RCU_CFG1_USART1SEL | RCU_CFG1_USART2SEL |
                RCU_CFG1_USART5SEL | RCU_CFG1_PLL2RDIV |
                RCU_CFG1_CAN0SEL | RCU_CFG1_CAN1SEL | RCU_CFG1_CAN2SEL);
#endif /* GD32H75E */
#ifndef GD32H75E
  RCU_CFG2 &= ~(RCU_CFG2_SAI2B1SEL | RCU_CFG2_SAI2B0SEL | RCU_CFG2_SAI1SEL |
                RCU_CFG2_SAI0SEL | RCU_CFG2_CKOUT0SEL | RCU_CFG2_CKOUT1SEL |
                RCU_CFG2_CKOUT0DIV | RCU_CFG2_CKOUT1DIV);
#else
  RCU_CFG2 &= ~(RCU_CFG2_CKOUT0SEL | RCU_CFG2_CKOUT1SEL | RCU_CFG2_CKOUT0DIV |
                RCU_CFG2_CKOUT1DIV);
#endif /* GD32H75E */
#ifndef GD32H75E
  RCU_CFG3 &= ~(RCU_CFG3_ADC01SEL | RCU_CFG3_ADC2SEL | RCU_CFG3_SDIO1SEL
                | RCU_CFG3_I2C3SEL | RCU_CFG3_I2C2SEL | RCU_CFG3_I2C1SEL);
#else
  RCU_CFG3 &= ~(RCU_CFG3_ADC01SEL | RCU_CFG3_ADC2SEL
                | RCU_CFG3_I2C3SEL | RCU_CFG3_I2C2SEL | RCU_CFG3_I2C1SEL);
#endif /* GD32H75E */
#ifndef GD32H75E
  RCU_CFG4 &= ~(RCU_CFG4_EXMCSEL | RCU_CFG4_SDIO0SEL);
#else
  RCU_CFG4 &= ~(RCU_CFG4_EXMCSEL);
#endif /* GD32H75E */
  RCU_CFG5 &= ~(RCU_CFG5_SPI0SEL | RCU_CFG5_SPI1SEL | RCU_CFG5_SPI2SEL |
                RCU_CFG5_SPI3SEL | RCU_CFG5_SPI4SEL | RCU_CFG5_SPI5SEL);

  /* disable all interrupts */
  RCU_INT = 0x14FF0000U;
  RCU_ADDINT = 0x00700000U;

  /* Restoring the reset values of every PLL configuration register.*/
  RCU_PLL0 = 0x01002020U;
  RCU_PLL1 = 0x01012020U;
  RCU_PLL2 = 0x01012020U;
  RCU_PLLALL = 0x00000000U;
  RCU_PLLADDCTL = 0x00010101U;
  RCU_PLLUSBCFG = 0x00000000U;
  RCU_PLL0FRA = 0x00000000U;
  RCU_PLL1FRA = 0x00000000U;
  RCU_PLL2FRA = 0x00000000U;

  RCU_APB4EN |= RCU_APB4EN_PMUEN;
}

/**
 * @brief   Clock tree bring-up: RCU reset, oscillators, bus prescalers,
 *          PLLs, SYSCLK switch and peripheral clock selectors.
 *
 * @notapi
 */
static void gd32_clock_init(void) {

  /* Making the clock tree reconfigurable regardless of the entry state.*/
  gd32_rcu_reset();

#if (GD32_PLLSRC == GD32_PLLSRC_IRC64MDIV) ||                               \
    (GD32_SYSCLK_SRC == GD32_SYSCLKSRC_IRC64MDIV) ||                        \
    (GD32_IRC64M_USART_REQUIRED == TRUE)
  /* CK_IRC64MDIV divider setup, IRC64M itself is already running.*/
  RCU_ADDCTL1 &= ~RCU_ADDCTL1_IRC64MDIV;
  RCU_ADDCTL1 |= ADDCTL1_IRC64MDIV(GD32_IRC64MDIV);
#endif

#if (GD32_PLLSRC == GD32_PLLSRC_HXTAL) ||                                   \
    (GD32_SYSCLK_SRC == GD32_SYSCLKSRC_HXTAL) ||                            \
    ((GD32_RTC_ENABLED == TRUE) && (GD32_RTCSEL == GD32_RTCSEL_HXTAL_DIV_RTCDIV))
  /* Enabling HXTAL when required by PLL, SYSCLK, or RTC(HXTAL/RTCDIV).*/
  RCU_CTL |= RCU_CTL_HXTALEN;

  /* Waiting for HXTAL stabilization, or halting on timeout (a stuck
     oscillator is not something we can safely recover from here).*/
  {
    uint32_t timeout = 0U;

    while ((0U == (RCU_CTL & RCU_CTL_HXTALSTB)) &&
           (timeout != GD32_HXTAL_TIMEOUT)) {
      timeout++;
    }
    osalDbgAssert((RCU_CTL & RCU_CTL_HXTALSTB) != 0U, "HXTAL failed to start");
  }
#endif

#if (GD32_PLLSRC == GD32_PLLSRC_LPIRC4M) ||                                 \
    (GD32_SYSCLK_SRC == GD32_SYSCLKSRC_LPIRC4M)
  /* Enabling LPIRC4M when required by PLL or SYSCLK source.*/
  RCU_ADDCTL1 |= RCU_ADDCTL1_LPIRC4MEN;
  {
    uint32_t timeout = 0U;

    while ((0U == (RCU_ADDCTL1 & RCU_ADDCTL1_LPIRC4MSTB)) &&
           (timeout != GD32_LPIRC4M_TIMEOUT)) {
      timeout++;
    }
    osalDbgAssert((RCU_ADDCTL1 & RCU_ADDCTL1_LPIRC4MSTB) != 0U,
                  "LPIRC4M failed to start");
  }
#endif

#if (GD32_LXTAL_ENABLED == TRUE) ||                                          \
    ((GD32_RTC_ENABLED == TRUE) && (GD32_RTCSEL == GD32_RTCSEL_LXTAL)) ||   \
    ((GD32_RTC_ENABLED == TRUE) && (GD32_RTCSEL == GD32_RTCSEL_HXTAL_DIV_RTCDIV)) || \
    ((GD32_RTC_ENABLED == TRUE) && (GD32_RTCSEL == GD32_RTCSEL_IRC32K))
  /* Backup domain write access for RTC/LXTAL configuration.*/
  PMU_CTL0 |= PMU_CTL0_BKPWEN;
#endif

#if (GD32_LXTAL_ENABLED == TRUE) ||                                          \
    ((GD32_RTC_ENABLED == TRUE) && (GD32_RTCSEL == GD32_RTCSEL_LXTAL))
  /* Optional LXTAL branch for RTC and low-power use cases.*/
  RCU_BDCTL |= RCU_BDCTL_LXTALEN;
  {
    uint32_t timeout = 0U;

    while ((0U == (RCU_BDCTL & RCU_BDCTL_LXTALSTB)) &&
           (timeout != GD32_LXTAL_TIMEOUT)) {
      timeout++;
    }
    osalDbgAssert((RCU_BDCTL & RCU_BDCTL_LXTALSTB) != 0U, "LXTAL failed to start");
  }
#endif

#if (GD32_IRC32K_ENABLED == TRUE) ||                                         \
    ((GD32_RTC_ENABLED == TRUE) && (GD32_RTCSEL == GD32_RTCSEL_IRC32K))
  /* Optional IRC32K branch for RTC and low-power use cases.*/
  RCU_RSTSCK |= RCU_RSTSCK_IRC32KEN;
  {
    uint32_t timeout = 0U;

    while ((0U == (RCU_RSTSCK & RCU_RSTSCK_IRC32KSTB)) &&
           (timeout != GD32_IRC32K_TIMEOUT)) {
      timeout++;
    }
    osalDbgAssert((RCU_RSTSCK & RCU_RSTSCK_IRC32KSTB) != 0U, "IRC32K failed to start");
  }
#endif

  /* Adding the TCM SRAM wait state required at the target SYSCLK, as
     done by the GD32 CMSIS reference init for 480/600MHz configurations.*/
  RCU_APB4EN |= RCU_APB4EN_SYSCFGEN;
  SYSCFG_SRAMCFG1 |= SYSCFG_SRAMCFG1_TCM_WAITSTATE;

  /* AHB/APBx bus prescalers. */
  RCU_CFG0 &= ~(RCU_CFG0_AHBPSC | RCU_CFG0_APB1PSC | RCU_CFG0_APB2PSC |
                RCU_CFG0_APB3PSC | RCU_CFG0_APB4PSC);
  RCU_CFG0 |= CFG0_AHBPSC(GD32_AHBPSC) |
              CFG0_APB1PSC(GD32_APB1PSC) |
              CFG0_APB2PSC(GD32_APB2PSC) |
              CFG0_APB3PSC(GD32_APB3PSC) |
              CFG0_APB4PSC(GD32_APB4PSC);

  /* USART kernel clock sources.  */
  RCU_CFG1 &= ~(RCU_CFG1_USART0SEL | RCU_CFG1_USART1SEL |
                RCU_CFG1_USART2SEL | RCU_CFG1_USART5SEL);
  RCU_CFG1 |= ((uint32_t)GD32_USART0SEL)         |
              ((uint32_t)GD32_USART1SEL << 18U)  |
              ((uint32_t)GD32_USART2SEL << 20U)  |
              ((uint32_t)GD32_USART5SEL << 22U);

  /* CAN kernel clock sources. */
  RCU_CFG1 &= ~(RCU_CFG1_CAN0SEL | RCU_CFG1_CAN1SEL | RCU_CFG1_CAN2SEL);
  RCU_CFG1 |= ((uint32_t)GD32_CAN0SEL << 8U)  |
              ((uint32_t)GD32_CAN1SEL << 10U) |
              ((uint32_t)GD32_CAN2SEL << 12U);

  /* SPI/I2S kernel clock sources.*/
  RCU_CFG5 &= ~(RCU_CFG5_SPI0SEL | RCU_CFG5_SPI1SEL | RCU_CFG5_SPI2SEL |
                RCU_CFG5_SPI3SEL | RCU_CFG5_SPI4SEL | RCU_CFG5_SPI5SEL);
  RCU_CFG5 |= ((uint32_t)GD32_SPI0SEL)        |
              ((uint32_t)GD32_SPI1SEL << 4U)  |
              ((uint32_t)GD32_SPI2SEL << 8U)  |
              ((uint32_t)GD32_SPI3SEL << 12U) |
              ((uint32_t)GD32_SPI4SEL << 16U) |
              ((uint32_t)GD32_SPI5SEL << 20U);

  /* I2C kernel clock sources, I2C0 through RCU_CFG0, I2C1..3 through
     RCU_CFG3.*/
  RCU_CFG0 &= ~RCU_CFG0_I2C0SEL;
  RCU_CFG0 |= ((uint32_t)GD32_I2C0SEL << 30U);
  RCU_CFG3 &= ~(RCU_CFG3_I2C1SEL | RCU_CFG3_I2C2SEL | RCU_CFG3_I2C3SEL);
  RCU_CFG3 |= ((uint32_t)GD32_I2C1SEL)        |
              ((uint32_t)GD32_I2C2SEL << 2U)  |
              ((uint32_t)GD32_I2C3SEL << 4U);

#ifndef GD32H75E
  /* SDIO kernel clock sources, one bit each in two different registers.*/
  RCU_CFG4 &= ~RCU_CFG4_SDIO0SEL;
  RCU_CFG4 |= ((uint32_t)GD32_SDIO0SEL << 0U);
  RCU_CFG3 &= ~RCU_CFG3_SDIO1SEL;
  RCU_CFG3 |= ((uint32_t)GD32_SDIO1SEL << 12U);
#endif /* GD32H75E */

  /* PLL source selection, shared by PLL0/PLL1/PLL2 through the single
     PLLSEL field, plus the per-PLL VCO input ranges.*/
  RCU_PLLALL &= ~(RCU_PLLALL_PLLSEL |
                  RCU_PLLALL_PLL0VCOSEL | RCU_PLLALL_PLL0RNG |
                  RCU_PLLALL_PLL1VCOSEL | RCU_PLLALL_PLL1RNG |
                  RCU_PLLALL_PLL2VCOSEL | RCU_PLLALL_PLL2RNG);
  RCU_PLLALL |= PLLALL_PLLSEL(GD32_PLLSRC);

#if GD32_PLL0_ENABLED == TRUE
  RCU_PLLALL |= GD32_PLL0RNG | GD32_PLL0VCOSEL;
#endif

#if GD32_PLL1_ENABLED == TRUE
  RCU_PLLALL |= GD32_PLL1RNG | GD32_PLL1VCOSEL;
#endif

#if GD32_PLL2_ENABLED == TRUE
  RCU_PLLALL |= GD32_PLL2RNG | GD32_PLL2VCOSEL;
#endif

#if GD32_PLL0_ENABLED == TRUE
  /* PLL0 PSC/N/P configuration (register fields store N-1/P-1, PSC
     is stored directly.*/
  RCU_PLL0 &= ~(RCU_PLL0_PLL0PSC | RCU_PLL0_PLL0N | RCU_PLL0_PLL0P |
                RCU_PLL0_PLL0R | RCU_PLL0_PLLSTBSRC);
  RCU_PLL0 |= GD32_PLL0_PSC |
              ((GD32_PLL0_N - 1U) << 6) |
              ((GD32_PLL0_P - 1U) << 16) |
              ((GD32_PLL0_R - 1U) << 24);
#endif

#if GD32_PLL1_ENABLED == TRUE
  RCU_PLL1 &= ~(RCU_PLL1_PLL1PSC | RCU_PLL1_PLL1N | RCU_PLL1_PLL1P | RCU_PLL1_PLL1R);
  RCU_PLL1 |= GD32_PLL1_PSC |
              ((GD32_PLL1_N - 1U) << 6) |
              ((GD32_PLL1_P - 1U) << 16) |
              ((GD32_PLL1_R - 1U) << 24);
#endif

#if GD32_PLL2_ENABLED == TRUE
  RCU_PLL2 &= ~(RCU_PLL2_PLL2PSC | RCU_PLL2_PLL2N | RCU_PLL2_PLL2P | RCU_PLL2_PLL2R);
  RCU_PLL2 |= GD32_PLL2_PSC |
              ((GD32_PLL2_N - 1U) << 6) |
              ((GD32_PLL2_P - 1U) << 16) |
              ((GD32_PLL2_R - 1U) << 24);
#endif

  /* PLL output enables and the PLL0/PLL1/PLL2 Q divisors.*/
  RCU_PLLADDCTL &= ~(RCU_PLLADDCTL_PLL0Q | RCU_PLLADDCTL_PLL1Q | RCU_PLLADDCTL_PLL2Q |
                     RCU_PLLADDCTL_PLL0PEN | RCU_PLLADDCTL_PLL0QEN | RCU_PLLADDCTL_PLL0REN |
                     RCU_PLLADDCTL_PLL1PEN | RCU_PLLADDCTL_PLL1QEN | RCU_PLLADDCTL_PLL1REN |
                     RCU_PLLADDCTL_PLL2PEN | RCU_PLLADDCTL_PLL2QEN | RCU_PLLADDCTL_PLL2REN);

#if GD32_PLL0_ENABLED == TRUE
  RCU_PLLADDCTL |= (GD32_PLL0_Q - 1U);
#if GD32_PLL0_P_ENABLED == TRUE
  RCU_PLLADDCTL |= RCU_PLLADDCTL_PLL0PEN;
#endif
#if GD32_PLL0_Q_ENABLED == TRUE
  RCU_PLLADDCTL |= RCU_PLLADDCTL_PLL0QEN;
#endif
#if GD32_PLL0_R_ENABLED == TRUE
  RCU_PLLADDCTL |= RCU_PLLADDCTL_PLL0REN;
#endif
#endif

#if GD32_PLL1_ENABLED == TRUE
  RCU_PLLADDCTL |= ((GD32_PLL1_Q - 1U) << 8U);
#if GD32_PLL1_P_ENABLED == TRUE
  RCU_PLLADDCTL |= RCU_PLLADDCTL_PLL1PEN;
#endif
#if GD32_PLL1_Q_ENABLED == TRUE
  RCU_PLLADDCTL |= RCU_PLLADDCTL_PLL1QEN;
#endif
#if GD32_PLL1_R_ENABLED == TRUE
  RCU_PLLADDCTL |= RCU_PLLADDCTL_PLL1REN;
#endif
#endif

#if GD32_PLL2_ENABLED == TRUE
  RCU_PLLADDCTL |= ((GD32_PLL2_Q - 1U) << 16U);
#if GD32_PLL2_P_ENABLED == TRUE
  RCU_PLLADDCTL |= RCU_PLLADDCTL_PLL2PEN;
#endif
#if GD32_PLL2_Q_ENABLED == TRUE
  RCU_PLLADDCTL |= RCU_PLLADDCTL_PLL2QEN;
#endif
#if GD32_PLL2_R_ENABLED == TRUE
  RCU_PLLADDCTL |= RCU_PLLADDCTL_PLL2REN;
#endif
#endif

#if (GD32_PLL0_ENABLED == TRUE) || (GD32_PLL1_ENABLED == TRUE) ||           \
    (GD32_PLL2_ENABLED == TRUE)
  /* Enabled PLLs are started with a single write and then waited for
     together, so that their lock times overlap instead of adding up.*/
  {
    uint32_t onmask  = 0U;
    uint32_t rdymask = 0U;
    uint32_t timeout = 0U;

#if GD32_PLL0_ENABLED == TRUE
    onmask  |= RCU_CTL_PLL0EN;
    rdymask |= RCU_CTL_PLL0STB;
#endif
#if GD32_PLL1_ENABLED == TRUE
    onmask  |= RCU_CTL_PLL1EN;
    rdymask |= RCU_CTL_PLL1STB;
#endif
#if GD32_PLL2_ENABLED == TRUE
    onmask  |= RCU_CTL_PLL2EN;
    rdymask |= RCU_CTL_PLL2STB;
#endif

    RCU_CTL |= onmask;

    while (((RCU_CTL & rdymask) != rdymask) &&
           (timeout != GD32_PLL_TIMEOUT)) {
      timeout++;
    }
    osalDbgAssert((RCU_CTL & rdymask) == rdymask, "PLL failed to lock");
  }
#endif

  /* Switching the system clock source according to GD32_SYSCLK_SRC.*/
  {
    uint32_t timeout = 0U;

    RCU_CFG0 &= ~RCU_CFG0_SCS;
#if GD32_SYSCLK_SRC == GD32_SYSCLKSRC_PLL0P
    RCU_CFG0 |= RCU_CKSYSSRC_PLL0P;
    while ((RCU_SCSS_PLL0P != (RCU_CFG0 & RCU_CFG0_SCSS)) &&
           (timeout != GD32_SWITCH_TIMEOUT)) {
      timeout++;
    }
    osalDbgAssert(RCU_SCSS_PLL0P == (RCU_CFG0 & RCU_CFG0_SCSS),
                  "SYSCLK switch failed");
#elif GD32_SYSCLK_SRC == GD32_SYSCLKSRC_HXTAL
    RCU_CFG0 |= RCU_CKSYSSRC_HXTAL;
    while ((RCU_SCSS_HXTAL != (RCU_CFG0 & RCU_CFG0_SCSS)) &&
           (timeout != GD32_SWITCH_TIMEOUT)) {
      timeout++;
    }
    osalDbgAssert(RCU_SCSS_HXTAL == (RCU_CFG0 & RCU_CFG0_SCSS),
                  "SYSCLK switch failed");
#elif GD32_SYSCLK_SRC == GD32_SYSCLKSRC_IRC64MDIV
    RCU_CFG0 |= RCU_CKSYSSRC_IRC64MDIV;
    while ((RCU_SCSS_IRC64MDIV != (RCU_CFG0 & RCU_CFG0_SCSS)) &&
           (timeout != GD32_SWITCH_TIMEOUT)) {
      timeout++;
    }
    osalDbgAssert(RCU_SCSS_IRC64MDIV == (RCU_CFG0 & RCU_CFG0_SCSS),
                  "SYSCLK switch failed");
#elif GD32_SYSCLK_SRC == GD32_SYSCLKSRC_LPIRC4M
    RCU_CFG0 |= RCU_CKSYSSRC_LPIRC4M;
    while ((RCU_SCSS_LPIRC4M != (RCU_CFG0 & RCU_CFG0_SCSS)) &&
           (timeout != GD32_SWITCH_TIMEOUT)) {
      timeout++;
    }
    osalDbgAssert(RCU_SCSS_LPIRC4M == (RCU_CFG0 & RCU_CFG0_SCSS),
                  "SYSCLK switch failed");
#else
#error "invalid GD32_SYSCLK_SRC value"
#endif
  }

#if GD32_RTC_ENABLED == TRUE
  /* RTC clock source selection branch (LXTAL/IRC32K/HXTAL/RTCDIV).*/
  RCU_BDCTL &= ~RCU_BDCTL_RTCSRC;
#if GD32_RTCSEL == GD32_RTCSEL_NONE
  RCU_BDCTL |= RCU_RTCSRC_NONE;
#elif GD32_RTCSEL == GD32_RTCSEL_LXTAL
  RCU_BDCTL |= RCU_RTCSRC_LXTAL;
#elif GD32_RTCSEL == GD32_RTCSEL_IRC32K
  RCU_BDCTL |= RCU_RTCSRC_IRC32K;
#elif GD32_RTCSEL == GD32_RTCSEL_HXTAL_DIV_RTCDIV
  RCU_CFG0 &= ~RCU_CFG0_RTCDIV;
  RCU_CFG0 |= CFG0_RTCDIV(GD32_RTCDIV);
  RCU_BDCTL |= RCU_RTCSRC_HXTAL_DIV_RTCDIV;
#else
#error "invalid GD32_RTCSEL value"
#endif
  RCU_BDCTL |= RCU_BDCTL_RTCEN;
#endif

  /* Keeping the CMSIS SystemCoreClock global in sync with the clock
     tree actually configured above.*/
  SystemCoreClock = GD32_SYSCLK;


#if 0
  RCU_AHB3EN |= RCU_AHB3EN_OSPI0EN | RCU_AHB3EN_OSPI1EN | \
                RCU_AHB3EN_OSPIMEN | RCU_AHB3EN_EXMCEN;
#endif
}
#endif /* !GD32_NO_INIT */

/*===========================================================================*/
/* Driver interrupt handlers.                                               */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                               */
/*===========================================================================*/

/**
 * @brief   Early HAL initialization (called from board's __early_init).
 * @note    Must run immediately after reset, before clock-tree/HAL init,
 *          to block speculative prefetch into unclocked external-memory
 *          space before the Cortex-M7 branch predictor warms up. See
 *          GD32H73x_75x_Firmware_Library/Template/main.c's mpu_config().
 *
 * @notapi
 */
void gd32_lld_early_init(void) {

  /*
   * Subregions 0/1/2/7 (code, SRAM, peripherals, system/PPB) keep the
   * default memory map; 3-6 (0x60000000-0xDFFFFFFF, OSPI/EXMC space)
   * remain No-Access so any speculative access faults instead of
   * stalling the AHB bus on an unclocked controller.
   */
  mpuConfigureRegion(MPU_REGION_0,
                     0x00000000U,
                     MPU_RASR_ATTR_AP_NA_NA |
                     MPU_RASR_ATTR_STRONGLY_ORDERED |
                     MPU_RASR_ATTR_XN |
                     MPU_RASR_SRD_DISABLE_SUB0 | /* 0x00000000-0x1FFFFFFF: code (Flash/ITCM) */
                     MPU_RASR_SRD_DISABLE_SUB1 | /* 0x20000000-0x3FFFFFFF: SRAM (DTCM/AXI-SRAM) */
                     MPU_RASR_SRD_DISABLE_SUB2 | /* 0x40000000-0x5FFFFFFF: peripherals */
                     MPU_RASR_SRD_DISABLE_SUB7 | /* 0xE0000000-0xFFFFFFFF: system/PPB */
                     MPU_RASR_SIZE_4G |
                     MPU_RASR_ENABLE);
  mpuEnable(MPU_CTRL_PRIVDEFENA);
}

/**
 * @brief   Low level HAL driver initialization.
 *
 * @notapi
 */
void hal_lld_init(void) {

#if !GD32_NO_INIT
  gd32_clock_init();
#endif

  /* DMA subsystems initialization.*/
#if defined(GD32_DMA_REQUIRED)
  dmaInit();
#endif

  /* IRQ subsystem initialization (EXTI + USART/UART vectors so far).*/
  irqInit();

  /* MPU initialization if required.*/
#if GD32_NOCACHE_ENABLE == TRUE
  {
    mpuConfigureRegion(GD32_NOCACHE_MPU_REGION,
                       GD32_NOCACHE_RBAR,
                       MPU_RASR_ATTR_AP_RW_RW |
                       MPU_RASR_ATTR_NON_CACHEABLE |
                       MPU_RASR_ATTR_S |
                       GD32_NOCACHE_RASR |
                       MPU_RASR_ENABLE);
    mpuEnable(MPU_CTRL_PRIVDEFENA);

    /* Invalidating data cache to make sure that the MPU settings are taken
       immediately. */
    SCB_CleanInvalidateDCache();
  }
#endif
}


/** @} */
