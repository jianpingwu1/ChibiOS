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
 * @file    I2C/gd32_i2c2.inc
 * @brief   Shared I2C2 handler.
 * @note    Driver instance numbering follows GD32's own 0-based hardware
 *          peripheral numbering: I2C2 (hardware) <-> I2CD2 (driver object).
 *
 * @addtogroup GD32_I2C2_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(GD32_HAS_I2C2)
#error "GD32_HAS_I2C2 not defined in registry"
#endif

#if defined(GD32_I2C2_IS_USED)

/* Priority settings checks.*/
#if !defined(GD32_IRQ_I2C2_PRIORITY)
#error "GD32_IRQ_I2C2_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_I2C2_PRIORITY)
#error "Invalid IRQ priority assigned to GD32_IRQ_I2C2_PRIORITY"
#endif

#endif /* defined(GD32_I2C2_IS_USED) */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void i2c2_irq_init(void) {
#if defined(GD32_I2C2_IS_USED)
  nvicEnableVector(GD32_I2C2_EV_NUMBER, GD32_IRQ_I2C2_PRIORITY);
  nvicEnableVector(GD32_I2C2_ER_NUMBER, GD32_IRQ_I2C2_PRIORITY);
#endif
}

static inline void i2c2_irq_deinit(void) {
#if defined(GD32_I2C2_IS_USED)
  nvicDisableVector(GD32_I2C2_EV_NUMBER);
  nvicDisableVector(GD32_I2C2_ER_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(GD32_I2C2_IS_USED) || defined(__DOXYGEN__)
/**
 * @brief   I2C2 event interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_I2C2_EV_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  i2c_lld_serve_event_interrupt(&I2CD2);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   I2C2 error interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_I2C2_ER_HANDLER) {

  OSAL_IRQ_PROLOGUE();

  i2c_lld_serve_error_interrupt(&I2CD2);

  OSAL_IRQ_EPILOGUE();
}
#endif /* defined(GD32_I2C2_IS_USED) */

/** @} */
