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
 * @file    GD32H73x_75x/gd32_rcu.h
 * @brief   RCU (Reset and Clock Unit) helper driver header.
 * @note    This file requires the flat REG32()-style register definitions
 *          from the GD32 CMSIS/standard-peripheral headers.
 *
 * @addtogroup GD32H73x_75x_RCU
 * @{
 */

#ifndef GD32_RCU_H
#define GD32_RCU_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Generic RCU operations
 * @{
 */
/**
 * @brief   Enables peripherals on AHB1.
 *
 * @param[in] mask              mask of peripherals to be enabled
 * @param[in] lp                low power (sleep mode) enable flag
 *
 * @api
 */
#define rcuEnableAHB1(mask, lp) {                                           \
  RCU_AHB1EN |= (mask);                                                     \
  if (lp) {                                                                 \
    RCU_AHB1SPEN |= (mask);                                                 \
  }                                                                         \
  else {                                                                    \
    RCU_AHB1SPEN &= ~(mask);                                                \
  }                                                                         \
  (void)RCU_AHB1SPEN;                                                       \
}

/**
 * @brief   Disables peripherals on AHB1.
 *
 * @param[in] mask              mask of peripherals to be disabled
 *
 * @api
 */
#define rcuDisableAHB1(mask) {                                              \
  RCU_AHB1EN &= ~(mask);                                                    \
  RCU_AHB1SPEN &= ~(mask);                                                  \
  (void)RCU_AHB1SPEN;                                                       \
}

/**
 * @brief   Resets peripherals on AHB1.
 *
 * @param[in] mask              mask of peripherals to be reset
 *
 * @api
 */
#define rcuResetAHB1(mask) {                                                \
  RCU_AHB1RST |= (mask);                                                    \
  RCU_AHB1RST &= ~(mask);                                                   \
  (void)RCU_AHB1RST;                                                        \
}

/**
 * @brief   Enables peripherals on AHB2.
 *
 * @param[in] mask              mask of peripherals to be enabled
 * @param[in] lp                low power (sleep mode) enable flag
 *
 * @api
 */
#define rcuEnableAHB2(mask, lp) {                                           \
  RCU_AHB2EN |= (mask);                                                     \
  if (lp) {                                                                 \
    RCU_AHB2SPEN |= (mask);                                                 \
  }                                                                         \
  else {                                                                    \
    RCU_AHB2SPEN &= ~(mask);                                                \
  }                                                                         \
  (void)RCU_AHB2SPEN;                                                       \
}

/**
 * @brief   Disables peripherals on AHB2.
 *
 * @param[in] mask              mask of peripherals to be disabled
 *
 * @api
 */
#define rcuDisableAHB2(mask) {                                              \
  RCU_AHB2EN &= ~(mask);                                                    \
  RCU_AHB2SPEN &= ~(mask);                                                  \
  (void)RCU_AHB2SPEN;                                                       \
}

/**
 * @brief   Resets peripherals on AHB2.
 *
 * @param[in] mask              mask of peripherals to be reset
 *
 * @api
 */
#define rcuResetAHB2(mask) {                                                \
  RCU_AHB2RST |= (mask);                                                    \
  RCU_AHB2RST &= ~(mask);                                                   \
  (void)RCU_AHB2RST;                                                        \
}

/**
 * @brief   Enables peripherals on AHB3.
 *
 * @param[in] mask              mask of peripherals to be enabled
 * @param[in] lp                low power (sleep mode) enable flag
 *
 * @api
 */
#define rcuEnableAHB3(mask, lp) {                                           \
  RCU_AHB3EN |= (mask);                                                     \
  if (lp) {                                                                 \
    RCU_AHB3SPEN |= (mask);                                                 \
  }                                                                         \
  else {                                                                    \
    RCU_AHB3SPEN &= ~(mask);                                                \
  }                                                                         \
  (void)RCU_AHB3SPEN;                                                       \
}

/**
 * @brief   Disables peripherals on AHB3.
 *
 * @param[in] mask              mask of peripherals to be disabled
 *
 * @api
 */
#define rcuDisableAHB3(mask) {                                              \
  RCU_AHB3EN &= ~(mask);                                                    \
  RCU_AHB3SPEN &= ~(mask);                                                  \
  (void)RCU_AHB3SPEN;                                                       \
}

/**
 * @brief   Resets peripherals on AHB3.
 *
 * @param[in] mask              mask of peripherals to be reset
 *
 * @api
 */
#define rcuResetAHB3(mask) {                                                \
  RCU_AHB3RST |= (mask);                                                    \
  RCU_AHB3RST &= ~(mask);                                                   \
  (void)RCU_AHB3RST;                                                        \
}

/**
 * @brief   Enables peripherals on AHB4 (GPIO, CRC, HWSEM, BKPSRAM).
 *
 * @param[in] mask              mask of peripherals to be enabled
 * @param[in] lp                low power (sleep mode) enable flag
 *
 * @api
 */
#define rcuEnableAHB4(mask, lp) {                                           \
  RCU_AHB4EN |= (mask);                                                     \
  if (lp) {                                                                 \
    RCU_AHB4SPEN |= (mask);                                                 \
  }                                                                         \
  else {                                                                    \
    RCU_AHB4SPEN &= ~(mask);                                                \
  }                                                                         \
  (void)RCU_AHB4SPEN;                                                       \
}

/**
 * @brief   Disables peripherals on AHB4.
 *
 * @param[in] mask              mask of peripherals to be disabled
 *
 * @api
 */
#define rcuDisableAHB4(mask) {                                              \
  RCU_AHB4EN &= ~(mask);                                                    \
  RCU_AHB4SPEN &= ~(mask);                                                  \
  (void)RCU_AHB4SPEN;                                                       \
}

/**
 * @brief   Resets peripherals on AHB4.
 *
 * @param[in] mask              mask of peripherals to be reset
 *
 * @api
 */
#define rcuResetAHB4(mask) {                                                \
  RCU_AHB4RST |= (mask);                                                    \
  RCU_AHB4RST &= ~(mask);                                                   \
  (void)RCU_AHB4RST;                                                        \
}

/**
 * @brief   Enables peripherals on APB1.
 *
 * @param[in] mask              mask of peripherals to be enabled
 * @param[in] lp                low power (sleep mode) enable flag
 *
 * @api
 */
#define rcuEnableAPB1(mask, lp) {                                           \
  RCU_APB1EN |= (mask);                                                     \
  if (lp) {                                                                 \
    RCU_APB1SPEN |= (mask);                                                 \
  }                                                                         \
  else {                                                                    \
    RCU_APB1SPEN &= ~(mask);                                                \
  }                                                                         \
  (void)RCU_APB1SPEN;                                                       \
}

/**
 * @brief   Disables peripherals on APB1.
 *
 * @param[in] mask              mask of peripherals to be disabled
 *
 * @api
 */
#define rcuDisableAPB1(mask) {                                              \
  RCU_APB1EN &= ~(mask);                                                    \
  RCU_APB1SPEN &= ~(mask);                                                  \
  (void)RCU_APB1SPEN;                                                       \
}

/**
 * @brief   Resets peripherals on APB1.
 *
 * @param[in] mask              mask of peripherals to be reset
 *
 * @api
 */
#define rcuResetAPB1(mask) {                                                \
  RCU_APB1RST |= (mask);                                                    \
  RCU_APB1RST &= ~(mask);                                                   \
  (void)RCU_APB1RST;                                                        \
}

/**
 * @brief   Enables peripherals on APB2.
 *
 * @param[in] mask              mask of peripherals to be enabled
 * @param[in] lp                low power (sleep mode) enable flag
 *
 * @api
 */
#define rcuEnableAPB2(mask, lp) {                                           \
  RCU_APB2EN |= (mask);                                                     \
  if (lp) {                                                                 \
    RCU_APB2SPEN |= (mask);                                                 \
  }                                                                         \
  else {                                                                    \
    RCU_APB2SPEN &= ~(mask);                                                \
  }                                                                         \
  (void)RCU_APB2SPEN;                                                       \
}

/**
 * @brief   Disables peripherals on APB2.
 *
 * @param[in] mask              mask of peripherals to be disabled
 *
 * @api
 */
#define rcuDisableAPB2(mask) {                                              \
  RCU_APB2EN &= ~(mask);                                                    \
  RCU_APB2SPEN &= ~(mask);                                                  \
  (void)RCU_APB2SPEN;                                                       \
}

/**
 * @brief   Resets peripherals on APB2.
 *
 * @param[in] mask              mask of peripherals to be reset
 *
 * @api
 */
#define rcuResetAPB2(mask) {                                                \
  RCU_APB2RST |= (mask);                                                    \
  RCU_APB2RST &= ~(mask);                                                   \
  (void)RCU_APB2RST;                                                        \
}

/**
 * @brief   Enables peripherals on APB3.
 *
 * @param[in] mask              mask of peripherals to be enabled
 * @param[in] lp                low power (sleep mode) enable flag
 *
 * @api
 */
#define rcuEnableAPB3(mask, lp) {                                           \
  RCU_APB3EN |= (mask);                                                     \
  if (lp) {                                                                 \
    RCU_APB3SPEN |= (mask);                                                 \
  }                                                                         \
  else {                                                                    \
    RCU_APB3SPEN &= ~(mask);                                                \
  }                                                                         \
  (void)RCU_APB3SPEN;                                                       \
}

/**
 * @brief   Disables peripherals on APB3.
 *
 * @param[in] mask              mask of peripherals to be disabled
 *
 * @api
 */
#define rcuDisableAPB3(mask) {                                              \
  RCU_APB3EN &= ~(mask);                                                    \
  RCU_APB3SPEN &= ~(mask);                                                  \
  (void)RCU_APB3SPEN;                                                       \
}

/**
 * @brief   Resets peripherals on APB3.
 *
 * @param[in] mask              mask of peripherals to be reset
 *
 * @api
 */
#define rcuResetAPB3(mask) {                                                \
  RCU_APB3RST |= (mask);                                                    \
  RCU_APB3RST &= ~(mask);                                                   \
  (void)RCU_APB3RST;                                                        \
}

/**
 * @brief   Enables peripherals on APB4.
 *
 * @param[in] mask              mask of peripherals to be enabled
 * @param[in] lp                low power (sleep mode) enable flag
 *
 * @api
 */
#define rcuEnableAPB4(mask, lp) {                                           \
  RCU_APB4EN |= (mask);                                                     \
  if (lp) {                                                                 \
    RCU_APB4SPEN |= (mask);                                                 \
  }                                                                         \
  else {                                                                    \
    RCU_APB4SPEN &= ~(mask);                                                \
  }                                                                         \
  (void)RCU_APB4SPEN;                                                       \
}

/**
 * @brief   Disables peripherals on APB4.
 *
 * @param[in] mask              mask of peripherals to be disabled
 *
 * @api
 */
#define rcuDisableAPB4(mask) {                                              \
  RCU_APB4EN &= ~(mask);                                                    \
  RCU_APB4SPEN &= ~(mask);                                                  \
  (void)RCU_APB4SPEN;                                                       \
}

/**
 * @brief   Resets peripherals on APB4.
 *
 * @param[in] mask              mask of peripherals to be reset
 *
 * @api
 */
#define rcuResetAPB4(mask) {                                                \
  RCU_APB4RST |= (mask);                                                    \
  RCU_APB4RST &= ~(mask);                                                   \
  (void)RCU_APB4RST;                                                        \
}

/**
 * @brief   Enables peripherals on the additional APB2 domain.
 * @note    The CAN units are not mapped in RCU_APB2EN, they have their own
 *          RCU_ADDAPB2EN/RST/SPEN register set.
 *
 * @param[in] mask              mask of peripherals to be enabled
 * @param[in] lp                low power (sleep mode) enable flag
 *
 * @api
 */
#define rcuEnableADDAPB2(mask, lp) {                                        \
  RCU_ADDAPB2EN |= (mask);                                                  \
  if (lp) {                                                                 \
    RCU_ADDAPB2SPEN |= (mask);                                              \
  }                                                                         \
  else {                                                                    \
    RCU_ADDAPB2SPEN &= ~(mask);                                             \
  }                                                                         \
  (void)RCU_ADDAPB2SPEN;                                                    \
}

/**
 * @brief   Disables peripherals on the additional APB2 domain.
 *
 * @param[in] mask              mask of peripherals to be disabled
 *
 * @api
 */
#define rcuDisableADDAPB2(mask) {                                           \
  RCU_ADDAPB2EN &= ~(mask);                                                 \
  RCU_ADDAPB2SPEN &= ~(mask);                                               \
  (void)RCU_ADDAPB2SPEN;                                                    \
}

/**
 * @brief   Resets peripherals on the additional APB2 domain.
 *
 * @param[in] mask              mask of peripherals to be reset
 *
 * @api
 */
#define rcuResetADDAPB2(mask) {                                             \
  RCU_ADDAPB2RST |= (mask);                                                 \
  RCU_ADDAPB2RST &= ~(mask);                                                \
  (void)RCU_ADDAPB2RST;                                                     \
}
/** @} */

/**
 * @name    GPIO peripherals specific RCU operations
 * @note    GPIO clocks live on AHB4. GD32_HAS_GPIOI is FALSE for this
 *          device (see gd32_registry.h), so no GPIOI macros are provided.
 * @{
 */
/**
 * @brief   Enables the GPIOA peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableGPIOA(lp) rcuEnableAHB4(RCU_AHB4EN_PAEN, lp)

/**
 * @brief   Disables the GPIOA peripheral clock.
 *
 * @api
 */
#define rcuDisableGPIOA() rcuDisableAHB4(RCU_AHB4EN_PAEN)

/**
 * @brief   Resets the GPIOA peripheral.
 *
 * @api
 */
#define rcuResetGPIOA() rcuResetAHB4(RCU_AHB4RST_PARST)

/**
 * @brief   Enables the GPIOB peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableGPIOB(lp) rcuEnableAHB4(RCU_AHB4EN_PBEN, lp)

/**
 * @brief   Disables the GPIOB peripheral clock.
 *
 * @api
 */
#define rcuDisableGPIOB() rcuDisableAHB4(RCU_AHB4EN_PBEN)

/**
 * @brief   Resets the GPIOB peripheral.
 *
 * @api
 */
#define rcuResetGPIOB() rcuResetAHB4(RCU_AHB4RST_PBRST)

/**
 * @brief   Enables the GPIOC peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableGPIOC(lp) rcuEnableAHB4(RCU_AHB4EN_PCEN, lp)

/**
 * @brief   Disables the GPIOC peripheral clock.
 *
 * @api
 */
#define rcuDisableGPIOC() rcuDisableAHB4(RCU_AHB4EN_PCEN)

/**
 * @brief   Resets the GPIOC peripheral.
 *
 * @api
 */
#define rcuResetGPIOC() rcuResetAHB4(RCU_AHB4RST_PCRST)

/**
 * @brief   Enables the GPIOD peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableGPIOD(lp) rcuEnableAHB4(RCU_AHB4EN_PDEN, lp)

/**
 * @brief   Disables the GPIOD peripheral clock.
 *
 * @api
 */
#define rcuDisableGPIOD() rcuDisableAHB4(RCU_AHB4EN_PDEN)

/**
 * @brief   Resets the GPIOD peripheral.
 *
 * @api
 */
#define rcuResetGPIOD() rcuResetAHB4(RCU_AHB4RST_PDRST)

/**
 * @brief   Enables the GPIOE peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableGPIOE(lp) rcuEnableAHB4(RCU_AHB4EN_PEEN, lp)

/**
 * @brief   Disables the GPIOE peripheral clock.
 *
 * @api
 */
#define rcuDisableGPIOE() rcuDisableAHB4(RCU_AHB4EN_PEEN)

/**
 * @brief   Resets the GPIOE peripheral.
 *
 * @api
 */
#define rcuResetGPIOE() rcuResetAHB4(RCU_AHB4RST_PERST)

/**
 * @brief   Enables the GPIOF peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableGPIOF(lp) rcuEnableAHB4(RCU_AHB4EN_PFEN, lp)

/**
 * @brief   Disables the GPIOF peripheral clock.
 *
 * @api
 */
#define rcuDisableGPIOF() rcuDisableAHB4(RCU_AHB4EN_PFEN)

/**
 * @brief   Resets the GPIOF peripheral.
 *
 * @api
 */
#define rcuResetGPIOF() rcuResetAHB4(RCU_AHB4RST_PFRST)

/**
 * @brief   Enables the GPIOG peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableGPIOG(lp) rcuEnableAHB4(RCU_AHB4EN_PGEN, lp)

/**
 * @brief   Disables the GPIOG peripheral clock.
 *
 * @api
 */
#define rcuDisableGPIOG() rcuDisableAHB4(RCU_AHB4EN_PGEN)

/**
 * @brief   Resets the GPIOG peripheral.
 *
 * @api
 */
#define rcuResetGPIOG() rcuResetAHB4(RCU_AHB4RST_PGRST)

/**
 * @brief   Enables the GPIOH peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableGPIOH(lp) rcuEnableAHB4(RCU_AHB4EN_PHEN, lp)

/**
 * @brief   Disables the GPIOH peripheral clock.
 *
 * @api
 */
#define rcuDisableGPIOH() rcuDisableAHB4(RCU_AHB4EN_PHEN)

/**
 * @brief   Resets the GPIOH peripheral.
 *
 * @api
 */
#define rcuResetGPIOH() rcuResetAHB4(RCU_AHB4RST_PHRST)

/**
 * @brief   Enables the GPIOJ peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableGPIOJ(lp) rcuEnableAHB4(RCU_AHB4EN_PJEN, lp)

/**
 * @brief   Disables the GPIOJ peripheral clock.
 *
 * @api
 */
#define rcuDisableGPIOJ() rcuDisableAHB4(RCU_AHB4EN_PJEN)

/**
 * @brief   Resets the GPIOJ peripheral.
 *
 * @api
 */
#define rcuResetGPIOJ() rcuResetAHB4(RCU_AHB4RST_PJRST)

/**
 * @brief   Enables the GPIOK peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableGPIOK(lp) rcuEnableAHB4(RCU_AHB4EN_PKEN, lp)

/**
 * @brief   Disables the GPIOK peripheral clock.
 *
 * @api
 */
#define rcuDisableGPIOK() rcuDisableAHB4(RCU_AHB4EN_PKEN)

/**
 * @brief   Resets the GPIOK peripheral.
 *
 * @api
 */
#define rcuResetGPIOK() rcuResetAHB4(RCU_AHB4RST_PKRST)
/** @} */

/**
 * @name    USART/UART peripherals specific RCU operations
 * @{
 */
/**
 * @brief   Enables the USART0 peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableUSART0(lp) rcuEnableAPB2(RCU_APB2EN_USART0EN, lp)

/**
 * @brief   Disables the USART0 peripheral clock.
 *
 * @api
 */
#define rcuDisableUSART0() rcuDisableAPB2(RCU_APB2EN_USART0EN)

/**
 * @brief   Resets the USART0 peripheral.
 *
 * @api
 */
#define rcuResetUSART0() rcuResetAPB2(RCU_APB2RST_USART0RST)

/**
 * @brief   Enables the USART1 peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableUSART1(lp) rcuEnableAPB1(RCU_APB1EN_USART1EN, lp)

/**
 * @brief   Disables the USART1 peripheral clock.
 *
 * @api
 */
#define rcuDisableUSART1() rcuDisableAPB1(RCU_APB1EN_USART1EN)

/**
 * @brief   Resets the USART1 peripheral.
 *
 * @api
 */
#define rcuResetUSART1() rcuResetAPB1(RCU_APB1RST_USART1RST)

/**
 * @brief   Enables the USART2 peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableUSART2(lp) rcuEnableAPB1(RCU_APB1EN_USART2EN, lp)

/**
 * @brief   Disables the USART2 peripheral clock.
 *
 * @api
 */
#define rcuDisableUSART2() rcuDisableAPB1(RCU_APB1EN_USART2EN)

/**
 * @brief   Resets the USART2 peripheral.
 *
 * @api
 */
#define rcuResetUSART2() rcuResetAPB1(RCU_APB1RST_USART2RST)

/**
 * @brief   Enables the UART3 peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableUART3(lp) rcuEnableAPB1(RCU_APB1EN_UART3EN, lp)

/**
 * @brief   Disables the UART3 peripheral clock.
 *
 * @api
 */
#define rcuDisableUART3() rcuDisableAPB1(RCU_APB1EN_UART3EN)

/**
 * @brief   Resets the UART3 peripheral.
 *
 * @api
 */
#define rcuResetUART3() rcuResetAPB1(RCU_APB1RST_UART3RST)

/**
 * @brief   Enables the UART4 peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableUART4(lp) rcuEnableAPB1(RCU_APB1EN_UART4EN, lp)

/**
 * @brief   Disables the UART4 peripheral clock.
 *
 * @api
 */
#define rcuDisableUART4() rcuDisableAPB1(RCU_APB1EN_UART4EN)

/**
 * @brief   Resets the UART4 peripheral.
 *
 * @api
 */
#define rcuResetUART4() rcuResetAPB1(RCU_APB1RST_UART4RST)

/**
 * @brief   Enables the USART5 peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableUSART5(lp) rcuEnableAPB2(RCU_APB2EN_USART5EN, lp)

/**
 * @brief   Disables the USART5 peripheral clock.
 *
 * @api
 */
#define rcuDisableUSART5() rcuDisableAPB2(RCU_APB2EN_USART5EN)

/**
 * @brief   Resets the USART5 peripheral.
 *
 * @api
 */
#define rcuResetUSART5() rcuResetAPB2(RCU_APB2RST_USART5RST)

/**
 * @brief   Enables the UART6 peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableUART6(lp) rcuEnableAPB1(RCU_APB1EN_UART6EN, lp)

/**
 * @brief   Disables the UART6 peripheral clock.
 *
 * @api
 */
#define rcuDisableUART6() rcuDisableAPB1(RCU_APB1EN_UART6EN)

/**
 * @brief   Resets the UART6 peripheral.
 *
 * @api
 */
#define rcuResetUART6() rcuResetAPB1(RCU_APB1RST_UART6RST)

/**
 * @brief   Enables the UART7 peripheral clock.
 *
 * @param[in] lp        low power enable flag
 *
 * @api
 */
#define rcuEnableUART7(lp) rcuEnableAPB1(RCU_APB1EN_UART7EN, lp)

/**
 * @brief   Disables the UART7 peripheral clock.
 *
 * @api
 */
#define rcuDisableUART7() rcuDisableAPB1(RCU_APB1EN_UART7EN)

/**
 * @brief   Resets the UART7 peripheral.
 *
 * @api
 */
#define rcuResetUART7() rcuResetAPB1(RCU_APB1RST_UART7RST)
/** @} */

/**
 * @name    DMA peripherals specific RCU operations
 * @{
 */
#define rcuEnableDMA0(lp) rcuEnableAHB1(RCU_AHB1EN_DMA0EN, lp)
#define rcuDisableDMA0() rcuDisableAHB1(RCU_AHB1EN_DMA0EN)
#define rcuResetDMA0() rcuResetAHB1(RCU_AHB1RST_DMA0RST)

#define rcuEnableDMA1(lp) rcuEnableAHB1(RCU_AHB1EN_DMA1EN, lp)
#define rcuDisableDMA1() rcuDisableAHB1(RCU_AHB1EN_DMA1EN)
#define rcuResetDMA1() rcuResetAHB1(RCU_AHB1RST_DMA1RST)

#define rcuEnableDMAMUX(lp) rcuEnableAHB1(RCU_AHB1EN_DMAMUXEN, lp)
#define rcuDisableDMAMUX() rcuDisableAHB1(RCU_AHB1EN_DMAMUXEN)
#define rcuResetDMAMUX() rcuResetAHB1(RCU_AHB1RST_DMAMUXRST)

#define rcuEnableMDMA(lp) rcuEnableAHB3(RCU_AHB3EN_MDMAEN, lp)
#define rcuDisableMDMA() rcuDisableAHB3(RCU_AHB3EN_MDMAEN)
#define rcuResetMDMA() rcuResetAHB3(RCU_AHB3RST_MDMARST)
/** @} */

/**
 * @name    TIMER peripherals specific RCU operations
 * @note    TIMER0/7, TIMER14..16 and TIMER40..44 live on APB2, all the
 *          remaining units live on APB1.
 * @{
 */
#define rcuEnableTIMER0(lp) rcuEnableAPB2(RCU_APB2EN_TIMER0EN, lp)
#define rcuDisableTIMER0() rcuDisableAPB2(RCU_APB2EN_TIMER0EN)
#define rcuResetTIMER0() rcuResetAPB2(RCU_APB2RST_TIMER0RST)

#define rcuEnableTIMER1(lp) rcuEnableAPB1(RCU_APB1EN_TIMER1EN, lp)
#define rcuDisableTIMER1() rcuDisableAPB1(RCU_APB1EN_TIMER1EN)
#define rcuResetTIMER1() rcuResetAPB1(RCU_APB1RST_TIMER1RST)

#define rcuEnableTIMER2(lp) rcuEnableAPB1(RCU_APB1EN_TIMER2EN, lp)
#define rcuDisableTIMER2() rcuDisableAPB1(RCU_APB1EN_TIMER2EN)
#define rcuResetTIMER2() rcuResetAPB1(RCU_APB1RST_TIMER2RST)

#define rcuEnableTIMER3(lp) rcuEnableAPB1(RCU_APB1EN_TIMER3EN, lp)
#define rcuDisableTIMER3() rcuDisableAPB1(RCU_APB1EN_TIMER3EN)
#define rcuResetTIMER3() rcuResetAPB1(RCU_APB1RST_TIMER3RST)

#define rcuEnableTIMER4(lp) rcuEnableAPB1(RCU_APB1EN_TIMER4EN, lp)
#define rcuDisableTIMER4() rcuDisableAPB1(RCU_APB1EN_TIMER4EN)
#define rcuResetTIMER4() rcuResetAPB1(RCU_APB1RST_TIMER4RST)

#define rcuEnableTIMER5(lp) rcuEnableAPB1(RCU_APB1EN_TIMER5EN, lp)
#define rcuDisableTIMER5() rcuDisableAPB1(RCU_APB1EN_TIMER5EN)
#define rcuResetTIMER5() rcuResetAPB1(RCU_APB1RST_TIMER5RST)

#define rcuEnableTIMER6(lp) rcuEnableAPB1(RCU_APB1EN_TIMER6EN, lp)
#define rcuDisableTIMER6() rcuDisableAPB1(RCU_APB1EN_TIMER6EN)
#define rcuResetTIMER6() rcuResetAPB1(RCU_APB1RST_TIMER6RST)

#define rcuEnableTIMER7(lp) rcuEnableAPB2(RCU_APB2EN_TIMER7EN, lp)
#define rcuDisableTIMER7() rcuDisableAPB2(RCU_APB2EN_TIMER7EN)
#define rcuResetTIMER7() rcuResetAPB2(RCU_APB2RST_TIMER7RST)

#define rcuEnableTIMER14(lp) rcuEnableAPB2(RCU_APB2EN_TIMER14EN, lp)
#define rcuDisableTIMER14() rcuDisableAPB2(RCU_APB2EN_TIMER14EN)
#define rcuResetTIMER14() rcuResetAPB2(RCU_APB2RST_TIMER14RST)

#define rcuEnableTIMER15(lp) rcuEnableAPB2(RCU_APB2EN_TIMER15EN, lp)
#define rcuDisableTIMER15() rcuDisableAPB2(RCU_APB2EN_TIMER15EN)
#define rcuResetTIMER15() rcuResetAPB2(RCU_APB2RST_TIMER15RST)

#define rcuEnableTIMER16(lp) rcuEnableAPB2(RCU_APB2EN_TIMER16EN, lp)
#define rcuDisableTIMER16() rcuDisableAPB2(RCU_APB2EN_TIMER16EN)
#define rcuResetTIMER16() rcuResetAPB2(RCU_APB2RST_TIMER16RST)

#define rcuEnableTIMER22(lp) rcuEnableAPB1(RCU_APB1EN_TIMER22EN, lp)
#define rcuDisableTIMER22() rcuDisableAPB1(RCU_APB1EN_TIMER22EN)
#define rcuResetTIMER22() rcuResetAPB1(RCU_APB1RST_TIMER22RST)

#define rcuEnableTIMER23(lp) rcuEnableAPB1(RCU_APB1EN_TIMER23EN, lp)
#define rcuDisableTIMER23() rcuDisableAPB1(RCU_APB1EN_TIMER23EN)
#define rcuResetTIMER23() rcuResetAPB1(RCU_APB1RST_TIMER23RST)

#define rcuEnableTIMER30(lp) rcuEnableAPB1(RCU_APB1EN_TIMER30EN, lp)
#define rcuDisableTIMER30() rcuDisableAPB1(RCU_APB1EN_TIMER30EN)
#define rcuResetTIMER30() rcuResetAPB1(RCU_APB1RST_TIMER30RST)

#define rcuEnableTIMER31(lp) rcuEnableAPB1(RCU_APB1EN_TIMER31EN, lp)
#define rcuDisableTIMER31() rcuDisableAPB1(RCU_APB1EN_TIMER31EN)
#define rcuResetTIMER31() rcuResetAPB1(RCU_APB1RST_TIMER31RST)

#define rcuEnableTIMER40(lp) rcuEnableAPB2(RCU_APB2EN_TIMER40EN, lp)
#define rcuDisableTIMER40() rcuDisableAPB2(RCU_APB2EN_TIMER40EN)
#define rcuResetTIMER40() rcuResetAPB2(RCU_APB2RST_TIMER40RST)

#define rcuEnableTIMER41(lp) rcuEnableAPB2(RCU_APB2EN_TIMER41EN, lp)
#define rcuDisableTIMER41() rcuDisableAPB2(RCU_APB2EN_TIMER41EN)
#define rcuResetTIMER41() rcuResetAPB2(RCU_APB2RST_TIMER41RST)

#define rcuEnableTIMER42(lp) rcuEnableAPB2(RCU_APB2EN_TIMER42EN, lp)
#define rcuDisableTIMER42() rcuDisableAPB2(RCU_APB2EN_TIMER42EN)
#define rcuResetTIMER42() rcuResetAPB2(RCU_APB2RST_TIMER42RST)

#define rcuEnableTIMER43(lp) rcuEnableAPB2(RCU_APB2EN_TIMER43EN, lp)
#define rcuDisableTIMER43() rcuDisableAPB2(RCU_APB2EN_TIMER43EN)
#define rcuResetTIMER43() rcuResetAPB2(RCU_APB2RST_TIMER43RST)

#define rcuEnableTIMER44(lp) rcuEnableAPB2(RCU_APB2EN_TIMER44EN, lp)
#define rcuDisableTIMER44() rcuDisableAPB2(RCU_APB2EN_TIMER44EN)
#define rcuResetTIMER44() rcuResetAPB2(RCU_APB2RST_TIMER44RST)

#define rcuEnableTIMER50(lp) rcuEnableAPB1(RCU_APB1EN_TIMER50EN, lp)
#define rcuDisableTIMER50() rcuDisableAPB1(RCU_APB1EN_TIMER50EN)
#define rcuResetTIMER50() rcuResetAPB1(RCU_APB1RST_TIMER50RST)

#define rcuEnableTIMER51(lp) rcuEnableAPB1(RCU_APB1EN_TIMER51EN, lp)
#define rcuDisableTIMER51() rcuDisableAPB1(RCU_APB1EN_TIMER51EN)
#define rcuResetTIMER51() rcuResetAPB1(RCU_APB1RST_TIMER51RST)
/** @} */

/**
 * @name    I2C peripherals specific RCU operations
 * @{
 */
#define rcuEnableI2C0(lp) rcuEnableAPB1(RCU_APB1EN_I2C0EN, lp)
#define rcuDisableI2C0() rcuDisableAPB1(RCU_APB1EN_I2C0EN)
#define rcuResetI2C0() rcuResetAPB1(RCU_APB1RST_I2C0RST)

#define rcuEnableI2C1(lp) rcuEnableAPB1(RCU_APB1EN_I2C1EN, lp)
#define rcuDisableI2C1() rcuDisableAPB1(RCU_APB1EN_I2C1EN)
#define rcuResetI2C1() rcuResetAPB1(RCU_APB1RST_I2C1RST)

#define rcuEnableI2C2(lp) rcuEnableAPB1(RCU_APB1EN_I2C2EN, lp)
#define rcuDisableI2C2() rcuDisableAPB1(RCU_APB1EN_I2C2EN)
#define rcuResetI2C2() rcuResetAPB1(RCU_APB1RST_I2C2RST)

#define rcuEnableI2C3(lp) rcuEnableAPB1(RCU_APB1EN_I2C3EN, lp)
#define rcuDisableI2C3() rcuDisableAPB1(RCU_APB1EN_I2C3EN)
#define rcuResetI2C3() rcuResetAPB1(RCU_APB1RST_I2C3RST)
/** @} */

/**
 * @name    SPI/I2S peripherals specific RCU operations
 * @note    SPI1/2 live on APB1, SPI0/3/4/5 live on APB2.
 * @{
 */
#define rcuEnableSPI0(lp) rcuEnableAPB2(RCU_APB2EN_SPI0EN, lp)
#define rcuDisableSPI0() rcuDisableAPB2(RCU_APB2EN_SPI0EN)
#define rcuResetSPI0() rcuResetAPB2(RCU_APB2RST_SPI0RST)

#define rcuEnableSPI1(lp) rcuEnableAPB1(RCU_APB1EN_SPI1EN, lp)
#define rcuDisableSPI1() rcuDisableAPB1(RCU_APB1EN_SPI1EN)
#define rcuResetSPI1() rcuResetAPB1(RCU_APB1RST_SPI1RST)

#define rcuEnableSPI2(lp) rcuEnableAPB1(RCU_APB1EN_SPI2EN, lp)
#define rcuDisableSPI2() rcuDisableAPB1(RCU_APB1EN_SPI2EN)
#define rcuResetSPI2() rcuResetAPB1(RCU_APB1RST_SPI2RST)

#define rcuEnableSPI3(lp) rcuEnableAPB2(RCU_APB2EN_SPI3EN, lp)
#define rcuDisableSPI3() rcuDisableAPB2(RCU_APB2EN_SPI3EN)
#define rcuResetSPI3() rcuResetAPB2(RCU_APB2RST_SPI3RST)

#define rcuEnableSPI4(lp) rcuEnableAPB2(RCU_APB2EN_SPI4EN, lp)
#define rcuDisableSPI4() rcuDisableAPB2(RCU_APB2EN_SPI4EN)
#define rcuResetSPI4() rcuResetAPB2(RCU_APB2RST_SPI4RST)

#define rcuEnableSPI5(lp) rcuEnableAPB2(RCU_APB2EN_SPI5EN, lp)
#define rcuDisableSPI5() rcuDisableAPB2(RCU_APB2EN_SPI5EN)
#define rcuResetSPI5() rcuResetAPB2(RCU_APB2RST_SPI5RST)
/** @} */

/**
 * @name    ADC and DAC peripherals specific RCU operations
 * @note    DACHOLD is the separate sample-and-hold clock domain of the DAC,
 *          it is only needed when the sample-and-hold mode is used.
 * @{
 */
#define rcuEnableADC0(lp) rcuEnableAPB2(RCU_APB2EN_ADC0EN, lp)
#define rcuDisableADC0() rcuDisableAPB2(RCU_APB2EN_ADC0EN)
#define rcuResetADC0() rcuResetAPB2(RCU_APB2RST_ADC0RST)

#define rcuEnableADC1(lp) rcuEnableAPB2(RCU_APB2EN_ADC1EN, lp)
#define rcuDisableADC1() rcuDisableAPB2(RCU_APB2EN_ADC1EN)
#define rcuResetADC1() rcuResetAPB2(RCU_APB2RST_ADC1RST)

#define rcuEnableADC2(lp) rcuEnableAPB2(RCU_APB2EN_ADC2EN, lp)
#define rcuDisableADC2() rcuDisableAPB2(RCU_APB2EN_ADC2EN)
#define rcuResetADC2() rcuResetAPB2(RCU_APB2RST_ADC2RST)

#define rcuEnableDAC(lp) rcuEnableAPB1(RCU_APB1EN_DACEN, lp)
#define rcuDisableDAC() rcuDisableAPB1(RCU_APB1EN_DACEN)
#define rcuResetDAC() rcuResetAPB1(RCU_APB1RST_DACRST)

#define rcuEnableDACHOLD(lp) rcuEnableAPB1(RCU_APB1EN_DACHOLDEN, lp)
#define rcuDisableDACHOLD() rcuDisableAPB1(RCU_APB1EN_DACHOLDEN)
#define rcuResetDACHOLD() rcuResetAPB1(RCU_APB1RST_DACHOLDRST)
/** @} */

/**
 * @name    CAN peripherals specific RCU operations
 * @note    The CAN units are clocked from the additional APB2 register set,
 *          not from RCU_APB2EN.
 * @{
 */
#define rcuEnableCAN0(lp) rcuEnableADDAPB2(RCU_ADDAPB2EN_CAN0EN, lp)
#define rcuDisableCAN0() rcuDisableADDAPB2(RCU_ADDAPB2EN_CAN0EN)
#define rcuResetCAN0() rcuResetADDAPB2(RCU_ADDAPB2RST_CAN0RST)

#define rcuEnableCAN1(lp) rcuEnableADDAPB2(RCU_ADDAPB2EN_CAN1EN, lp)
#define rcuDisableCAN1() rcuDisableADDAPB2(RCU_ADDAPB2EN_CAN1EN)
#define rcuResetCAN1() rcuResetADDAPB2(RCU_ADDAPB2RST_CAN1RST)

#define rcuEnableCAN2(lp) rcuEnableADDAPB2(RCU_ADDAPB2EN_CAN2EN, lp)
#define rcuDisableCAN2() rcuDisableADDAPB2(RCU_ADDAPB2EN_CAN2EN)
#define rcuResetCAN2() rcuResetADDAPB2(RCU_ADDAPB2RST_CAN2RST)
/** @} */

/**
 * @name    SDIO peripherals specific RCU operations
 * @note    SDIO0 sits on AHB3 while SDIO1 sits on AHB2.
 * @{
 */
#define rcuEnableSDIO0(lp) rcuEnableAHB3(RCU_AHB3EN_SDIO0EN, lp)
#define rcuDisableSDIO0() rcuDisableAHB3(RCU_AHB3EN_SDIO0EN)
#define rcuResetSDIO0() rcuResetAHB3(RCU_AHB3RST_SDIO0RST)

#define rcuEnableSDIO1(lp) rcuEnableAHB2(RCU_AHB2EN_SDIO1EN, lp)
#define rcuDisableSDIO1() rcuDisableAHB2(RCU_AHB2EN_SDIO1EN)
#define rcuResetSDIO1() rcuResetAHB2(RCU_AHB2RST_SDIO1RST)
/** @} */

/**
 * @name    Ethernet peripherals specific RCU operations
 * @note    The TX, RX and PTP clock gates have no reset bit of their own,
 *          they are released together with the MAC reset.
 * @{
 */
#define rcuEnableENET0(lp) rcuEnableAHB1(RCU_AHB1EN_ENET0EN, lp)
#define rcuDisableENET0() rcuDisableAHB1(RCU_AHB1EN_ENET0EN)
#define rcuResetENET0() rcuResetAHB1(RCU_AHB1RST_ENET0RST)

#define rcuEnableENET0TX(lp) rcuEnableAHB1(RCU_AHB1EN_ENET0TXEN, lp)
#define rcuDisableENET0TX() rcuDisableAHB1(RCU_AHB1EN_ENET0TXEN)

#define rcuEnableENET0RX(lp) rcuEnableAHB1(RCU_AHB1EN_ENET0RXEN, lp)
#define rcuDisableENET0RX() rcuDisableAHB1(RCU_AHB1EN_ENET0RXEN)

#define rcuEnableENET0PTP(lp) rcuEnableAHB1(RCU_AHB1EN_ENET0PTPEN, lp)
#define rcuDisableENET0PTP() rcuDisableAHB1(RCU_AHB1EN_ENET0PTPEN)

#define rcuEnableENET1(lp) rcuEnableAHB1(RCU_AHB1EN_ENET1EN, lp)
#define rcuDisableENET1() rcuDisableAHB1(RCU_AHB1EN_ENET1EN)
#define rcuResetENET1() rcuResetAHB1(RCU_AHB1RST_ENET1RST)

#define rcuEnableENET1TX(lp) rcuEnableAHB1(RCU_AHB1EN_ENET1TXEN, lp)
#define rcuDisableENET1TX() rcuDisableAHB1(RCU_AHB1EN_ENET1TXEN)

#define rcuEnableENET1RX(lp) rcuEnableAHB1(RCU_AHB1EN_ENET1RXEN, lp)
#define rcuDisableENET1RX() rcuDisableAHB1(RCU_AHB1EN_ENET1RXEN)

#define rcuEnableENET1PTP(lp) rcuEnableAHB1(RCU_AHB1EN_ENET1PTPEN, lp)
#define rcuDisableENET1PTP() rcuDisableAHB1(RCU_AHB1EN_ENET1PTPEN)

#define rcuEnableMDIO(lp) rcuEnableAPB1(RCU_APB1EN_MDIOEN, lp)
#define rcuDisableMDIO() rcuDisableAPB1(RCU_APB1EN_MDIOEN)
#define rcuResetMDIO() rcuResetAPB1(RCU_APB1RST_MDIORST)
/** @} */

/**
 * @name    USB peripherals specific RCU operations
 * @note    The ULPI gates feed the external high speed PHY interface and are
 *          only required when an external ULPI PHY is used.
 * @{
 */
#define rcuEnableUSBHS0(lp) rcuEnableAHB1(RCU_AHB1EN_USBHS0EN, lp)
#define rcuDisableUSBHS0() rcuDisableAHB1(RCU_AHB1EN_USBHS0EN)
#define rcuResetUSBHS0() rcuResetAHB1(RCU_AHB1RST_USBHS0RST)

#define rcuEnableUSBHS0ULPI(lp) rcuEnableAHB1(RCU_AHB1EN_USBHS0ULPIEN, lp)
#define rcuDisableUSBHS0ULPI() rcuDisableAHB1(RCU_AHB1EN_USBHS0ULPIEN)

#define rcuEnableUSBHS1(lp) rcuEnableAHB1(RCU_AHB1EN_USBHS1EN, lp)
#define rcuDisableUSBHS1() rcuDisableAHB1(RCU_AHB1EN_USBHS1EN)
#define rcuResetUSBHS1() rcuResetAHB1(RCU_AHB1RST_USBHS1RST)

#define rcuEnableUSBHS1ULPI(lp) rcuEnableAHB1(RCU_AHB1EN_USBHS1ULPIEN, lp)
#define rcuDisableUSBHS1ULPI() rcuDisableAHB1(RCU_AHB1EN_USBHS1ULPIEN)
/** @} */

/**
 * @name    External memory controllers specific RCU operations
 * @{
 */
#define rcuEnableEXMC(lp) rcuEnableAHB3(RCU_AHB3EN_EXMCEN, lp)
#define rcuDisableEXMC() rcuDisableAHB3(RCU_AHB3EN_EXMCEN)
#define rcuResetEXMC() rcuResetAHB3(RCU_AHB3RST_EXMCRST)

#define rcuEnableOSPIM(lp) rcuEnableAHB3(RCU_AHB3EN_OSPIMEN, lp)
#define rcuDisableOSPIM() rcuDisableAHB3(RCU_AHB3EN_OSPIMEN)
#define rcuResetOSPIM() rcuResetAHB3(RCU_AHB3RST_OSPIMRST)

#define rcuEnableOSPI0(lp) rcuEnableAHB3(RCU_AHB3EN_OSPI0EN, lp)
#define rcuDisableOSPI0() rcuDisableAHB3(RCU_AHB3EN_OSPI0EN)
#define rcuResetOSPI0() rcuResetAHB3(RCU_AHB3RST_OSPI0RST)

#define rcuEnableOSPI1(lp) rcuEnableAHB3(RCU_AHB3EN_OSPI1EN, lp)
#define rcuDisableOSPI1() rcuDisableAHB3(RCU_AHB3EN_OSPI1EN)
#define rcuResetOSPI1() rcuResetAHB3(RCU_AHB3RST_OSPI1RST)

#define rcuEnableRTDEC0(lp) rcuEnableAHB3(RCU_AHB3EN_RTDEC0EN, lp)
#define rcuDisableRTDEC0() rcuDisableAHB3(RCU_AHB3EN_RTDEC0EN)
#define rcuResetRTDEC0() rcuResetAHB3(RCU_AHB3RST_RTDEC0RST)

#define rcuEnableRTDEC1(lp) rcuEnableAHB3(RCU_AHB3EN_RTDEC1EN, lp)
#define rcuDisableRTDEC1() rcuDisableAHB3(RCU_AHB3EN_RTDEC1EN)
#define rcuResetRTDEC1() rcuResetAHB3(RCU_AHB3RST_RTDEC1RST)
/** @} */

/**
 * @name    Graphics and imaging peripherals specific RCU operations
 * @{
 */
#define rcuEnableTLI(lp) rcuEnableAPB3(RCU_APB3EN_TLIEN, lp)
#define rcuDisableTLI() rcuDisableAPB3(RCU_APB3EN_TLIEN)
#define rcuResetTLI() rcuResetAPB3(RCU_APB3RST_TLIRST)

#define rcuEnableIPA(lp) rcuEnableAHB3(RCU_AHB3EN_IPAEN, lp)
#define rcuDisableIPA() rcuDisableAHB3(RCU_AHB3EN_IPAEN)
#define rcuResetIPA() rcuResetAHB3(RCU_AHB3RST_IPARST)

#define rcuEnableDCI(lp) rcuEnableAHB2(RCU_AHB2EN_DCIEN, lp)
#define rcuDisableDCI() rcuDisableAHB2(RCU_AHB2EN_DCIEN)
#define rcuResetDCI() rcuResetAHB2(RCU_AHB2RST_DCIRST)
/** @} */

/**
 * @name    Audio peripherals specific RCU operations
 * @{
 */
#define rcuEnableSAI0(lp) rcuEnableAPB2(RCU_APB2EN_SAI0EN, lp)
#define rcuDisableSAI0() rcuDisableAPB2(RCU_APB2EN_SAI0EN)
#define rcuResetSAI0() rcuResetAPB2(RCU_APB2RST_SAI0RST)

#define rcuEnableSAI1(lp) rcuEnableAPB2(RCU_APB2EN_SAI1EN, lp)
#define rcuDisableSAI1() rcuDisableAPB2(RCU_APB2EN_SAI1EN)
#define rcuResetSAI1() rcuResetAPB2(RCU_APB2RST_SAI1RST)

#define rcuEnableSAI2(lp) rcuEnableAPB2(RCU_APB2EN_SAI2EN, lp)
#define rcuDisableSAI2() rcuDisableAPB2(RCU_APB2EN_SAI2EN)
#define rcuResetSAI2() rcuResetAPB2(RCU_APB2RST_SAI2RST)

#define rcuEnableHPDF(lp) rcuEnableAPB2(RCU_APB2EN_HPDFEN, lp)
#define rcuDisableHPDF() rcuDisableAPB2(RCU_APB2EN_HPDFEN)
#define rcuResetHPDF() rcuResetAPB2(RCU_APB2RST_HPDFRST)

#define rcuEnableRSPDIF(lp) rcuEnableAPB1(RCU_APB1EN_RSPDIFEN, lp)
#define rcuDisableRSPDIF() rcuDisableAPB1(RCU_APB1EN_RSPDIFEN)
#define rcuResetRSPDIF() rcuResetAPB1(RCU_APB1RST_RSPDIFRST)
/** @} */

/**
 * @name    Cryptography and math accelerators specific RCU operations
 * @{
 */
#define rcuEnableCAU(lp) rcuEnableAHB2(RCU_AHB2EN_CAUEN, lp)
#define rcuDisableCAU() rcuDisableAHB2(RCU_AHB2EN_CAUEN)
#define rcuResetCAU() rcuResetAHB2(RCU_AHB2RST_CAURST)

#define rcuEnableHAU(lp) rcuEnableAHB2(RCU_AHB2EN_HAUEN, lp)
#define rcuDisableHAU() rcuDisableAHB2(RCU_AHB2EN_HAUEN)
#define rcuResetHAU() rcuResetAHB2(RCU_AHB2RST_HAURST)

#define rcuEnableTRNG(lp) rcuEnableAHB2(RCU_AHB2EN_TRNGEN, lp)
#define rcuDisableTRNG() rcuDisableAHB2(RCU_AHB2EN_TRNGEN)
#define rcuResetTRNG() rcuResetAHB2(RCU_AHB2RST_TRNGRST)

#define rcuEnableFAC(lp) rcuEnableAHB2(RCU_AHB2EN_FACEN, lp)
#define rcuDisableFAC() rcuDisableAHB2(RCU_AHB2EN_FACEN)
#define rcuResetFAC() rcuResetAHB2(RCU_AHB2RST_FACRST)

#define rcuEnableTMU(lp) rcuEnableAHB2(RCU_AHB2EN_TMUEN, lp)
#define rcuDisableTMU() rcuDisableAHB2(RCU_AHB2EN_TMUEN)
#define rcuResetTMU() rcuResetAHB2(RCU_AHB2RST_TMURST)

#define rcuEnableCRC(lp) rcuEnableAHB4(RCU_AHB4EN_CRCEN, lp)
#define rcuDisableCRC() rcuDisableAHB4(RCU_AHB4EN_CRCEN)
#define rcuResetCRC() rcuResetAHB4(RCU_AHB4RST_CRCRST)
/** @} */

/**
 * @name    System peripherals specific RCU operations
 * @note    BKPSRAM, HWSEM and the RAM ECC managers have no reset bit, only
 *          a clock gate.
 * @{
 */
#define rcuEnableSYSCFG(lp) rcuEnableAPB4(RCU_APB4EN_SYSCFGEN, lp)
#define rcuDisableSYSCFG() rcuDisableAPB4(RCU_APB4EN_SYSCFGEN)
#define rcuResetSYSCFG() rcuResetAPB4(RCU_APB4RST_SYSCFGRST)

#define rcuEnablePMU(lp) rcuEnableAPB4(RCU_APB4EN_PMUEN, lp)
#define rcuDisablePMU() rcuDisableAPB4(RCU_APB4EN_PMUEN)
#define rcuResetPMU() rcuResetAPB4(RCU_APB4RST_PMURST)

#define rcuEnableCMP(lp) rcuEnableAPB4(RCU_APB4EN_CMPEN, lp)
#define rcuDisableCMP() rcuDisableAPB4(RCU_APB4EN_CMPEN)
#define rcuResetCMP() rcuResetAPB4(RCU_APB4RST_CMPRST)

#define rcuEnableVREF(lp) rcuEnableAPB4(RCU_APB4EN_VREFEN, lp)
#define rcuDisableVREF() rcuDisableAPB4(RCU_APB4EN_VREFEN)
#define rcuResetVREF() rcuResetAPB4(RCU_APB4RST_VREFRST)

#define rcuEnableLPDTS(lp) rcuEnableAPB4(RCU_APB4EN_LPDTSEN, lp)
#define rcuDisableLPDTS() rcuDisableAPB4(RCU_APB4EN_LPDTSEN)
#define rcuResetLPDTS() rcuResetAPB4(RCU_APB4RST_LPDTSRST)

#define rcuEnableWWDGT(lp) rcuEnableAPB3(RCU_APB3EN_WWDGTEN, lp)
#define rcuDisableWWDGT() rcuDisableAPB3(RCU_APB3EN_WWDGTEN)
#define rcuResetWWDGT() rcuResetAPB3(RCU_APB3RST_WWDGTRST)

#define rcuEnableCTC(lp) rcuEnableAPB1(RCU_APB1EN_CTCEN, lp)
#define rcuDisableCTC() rcuDisableAPB1(RCU_APB1EN_CTCEN)
#define rcuResetCTC() rcuResetAPB1(RCU_APB1RST_CTCRST)

#define rcuEnableEDOUT(lp) rcuEnableAPB2(RCU_APB2EN_EDOUTEN, lp)
#define rcuDisableEDOUT() rcuDisableAPB2(RCU_APB2EN_EDOUTEN)
#define rcuResetEDOUT() rcuResetAPB2(RCU_APB2RST_EDOUTRST)

#define rcuEnableTRGSEL(lp) rcuEnableAPB2(RCU_APB2EN_TRGSELEN, lp)
#define rcuDisableTRGSEL() rcuDisableAPB2(RCU_APB2EN_TRGSELEN)

#define rcuEnableBKPSRAM(lp) rcuEnableAHB4(RCU_AHB4EN_BKPSRAMEN, lp)
#define rcuDisableBKPSRAM() rcuDisableAHB4(RCU_AHB4EN_BKPSRAMEN)

#define rcuEnableHWSEM(lp) rcuEnableAHB4(RCU_AHB4EN_HWSEMEN, lp)
#define rcuDisableHWSEM() rcuDisableAHB4(RCU_AHB4EN_HWSEMEN)
#define rcuResetHWSEM() rcuResetAHB4(RCU_AHB4RST_HWSEMRST)

#define rcuEnableRAMECCMU0(lp) rcuEnableAHB3(RCU_AHB3EN_RAMECCMU0EN, lp)
#define rcuDisableRAMECCMU0() rcuDisableAHB3(RCU_AHB3EN_RAMECCMU0EN)

#define rcuEnableRAMECCMU1(lp) rcuEnableAHB2(RCU_AHB2EN_RAMECCMU1EN, lp)
#define rcuDisableRAMECCMU1() rcuDisableAHB2(RCU_AHB2EN_RAMECCMU1EN)
/** @} */

#endif /* GD32_RCU_H */

/** @} */
