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
 * @file    GD32/LLD/I2Cv1/gd32_i2c.h
 * @brief   GD32 I2C driver macros and structures.
 * @details Register access helpers shared by the I2C LLD and the interrupt
 *          dispatch files, included by hal_lld.h.
 *
 * @addtogroup GD32_I2C
 * @{
 */

#ifndef GD32_I2C_H
#define GD32_I2C_H

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
 * @name    Timeout register accessor
 * @note    The vendor header names the timeout register accessor
 *          @p I2C_TIMEOUT(i2cx), it collides with the @p I2C_TIMEOUT error
 *          flag declared later by hal_i2c.h. The accessor is re-exported
 *          with a prefix and the colliding name is removed.
 * @{
 */
#if defined(I2C_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_I2C_TIMEOUT(i2cx)          REG32((i2cx) + 0x00000014U)
#undef I2C_TIMEOUT
#endif
/** @} */

/**
 * @name    I2C_TIMING register field builders
 * @{
 */
#define GD32_I2C_TIMING_SCLL(n)         ((uint32_t)(n) << 0U)
#define GD32_I2C_TIMING_SCLH(n)         ((uint32_t)(n) << 8U)
#define GD32_I2C_TIMING_SDADELY(n)      ((uint32_t)(n) << 16U)
#define GD32_I2C_TIMING_SCLDELY(n)      ((uint32_t)(n) << 20U)
#define GD32_I2C_TIMING_PSC(n)          ((uint32_t)(n) << 28U)
/** @} */

/**
 * @name    I2C_CTL0 register field builders
 * @{
 */
#define GD32_I2C_CTL0_DNF(n)            (((uint32_t)(n) << 8U) & I2C_CTL0_DNF)
/** @} */


/**
 * @name    I2C_STAT masks
 * @{
 */
#define GD32_I2C_STAT_ERROR_MASK        ((uint32_t)(I2C_STAT_BERR    |      \
                                                    I2C_STAT_LOSTARB |      \
                                                    I2C_STAT_OUERR   |      \
                                                    I2C_STAT_PECERR  |      \
                                                    I2C_STAT_TIMEOUT |      \
                                                    I2C_STAT_SMBALT))

#define GD32_I2C_STAT_INT_MASK          ((uint32_t)(I2C_STAT_TCR     |      \
                                                    I2C_STAT_TC      |      \
                                                    I2C_STAT_STPDET  |      \
                                                    I2C_STAT_NACK    |      \
                                                    I2C_STAT_ADDSEND |      \
                                                    I2C_STAT_RBNE    |      \
                                                    I2C_STAT_TI))
/** @} */

/**
 * @name    I2C_STATC masks
 * @{
 */
#define GD32_I2C_STATC_MASK             ((uint32_t)(I2C_STATC_ADDSENDC |    \
                                                    I2C_STATC_NACKC    |    \
                                                    I2C_STATC_STPDETC  |    \
                                                    I2C_STATC_BERRC    |    \
                                                    I2C_STATC_LOSTARBC |    \
                                                    I2C_STATC_OUERRC   |    \
                                                    I2C_STATC_PECERRC  |    \
                                                    I2C_STATC_TIMEOUTC |    \
                                                    I2C_STATC_SMBALTC))
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

#endif /* GD32_I2C_H */

/** @} */
