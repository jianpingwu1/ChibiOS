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
 * @file    I2Cv1/hal_i2c_lld.h
 * @brief   GD32 I2C subsystem low level driver header.
 *
 * @addtogroup I2C
 * @{
 */

#ifndef HAL_I2C_LLD_H
#define HAL_I2C_LLD_H

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   This I2C LLD supports slave mode.
 */
#define I2C_SUPPORTS_SLAVE_MODE             TRUE

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   I2C0 driver enable switch.
 * @details If set to @p TRUE the support for I2C0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_I2C_USE_I2C0) || defined(__DOXYGEN__)
#define GD32_I2C_USE_I2C0                   FALSE
#endif

/**
 * @brief   I2C1 driver enable switch.
 * @details If set to @p TRUE the support for I2C1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_I2C_USE_I2C1) || defined(__DOXYGEN__)
#define GD32_I2C_USE_I2C1                   FALSE
#endif

/**
 * @brief   I2C2 driver enable switch.
 * @details If set to @p TRUE the support for I2C2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_I2C_USE_I2C2) || defined(__DOXYGEN__)
#define GD32_I2C_USE_I2C2                   FALSE
#endif

/**
 * @brief   I2C3 driver enable switch.
 * @details If set to @p TRUE the support for I2C3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_I2C_USE_I2C3) || defined(__DOXYGEN__)
#define GD32_I2C_USE_I2C3                   FALSE
#endif

/**
 * @brief   I2C timeout on busy condition in milliseconds.
 */
#if !defined(GD32_I2C_BUSY_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_I2C_BUSY_TIMEOUT               50
#endif

/**
 * @brief   Number of loops spent waiting for the peripheral to settle.
 * @note    Used by the abort sequence, it bounds the wait on the I2CEN bit
 *          so that a stuck peripheral cannot lock the driver.
 */
#if !defined(GD32_I2C_SYNC_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_I2C_SYNC_TIMEOUT               1000
#endif

/**
 * @brief   I2C0 interrupt priority level setting.
 */
#if !defined(GD32_I2C_I2C0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_I2C_I2C0_IRQ_PRIORITY          10
#endif

/**
 * @brief   I2C1 interrupt priority level setting.
 */
#if !defined(GD32_I2C_I2C1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_I2C_I2C1_IRQ_PRIORITY          10
#endif

/**
 * @brief   I2C2 interrupt priority level setting.
 */
#if !defined(GD32_I2C_I2C2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_I2C_I2C2_IRQ_PRIORITY          10
#endif

/**
 * @brief   I2C3 interrupt priority level setting.
 */
#if !defined(GD32_I2C_I2C3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_I2C_I2C3_IRQ_PRIORITY          10
#endif

/**
 * @brief   DMA use switch.
 */
#if !defined(GD32_I2C_USE_DMA) || defined(__DOXYGEN__)
#define GD32_I2C_USE_DMA                    TRUE
#endif

/**
 * @brief   I2C0 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA streams.
 */
#if !defined(GD32_I2C_I2C0_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_I2C_I2C0_DMA_PRIORITY          1
#endif

/**
 * @brief   I2C1 DMA priority (0..3|lowest..highest).
 */
#if !defined(GD32_I2C_I2C1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_I2C_I2C1_DMA_PRIORITY          1
#endif

/**
 * @brief   I2C2 DMA priority (0..3|lowest..highest).
 */
#if !defined(GD32_I2C_I2C2_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_I2C_I2C2_DMA_PRIORITY          1
#endif

/**
 * @brief   I2C3 DMA priority (0..3|lowest..highest).
 */
#if !defined(GD32_I2C_I2C3_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_I2C_I2C3_DMA_PRIORITY          1
#endif

/**
 * @brief   I2C DMA error hook.
 * @note    The default action for DMA errors is a system halt because DMA
 *          error can only happen because programming errors.
 */
#if !defined(GD32_I2C_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define GD32_I2C_DMA_ERROR_HOOK(i2cp)       osalSysHalt("DMA failure")
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks.*/
#if !defined(GD32_HAS_I2C0)
#error "GD32_HAS_I2C0 not defined in registry"
#endif

#if !defined(GD32_HAS_I2C1)
#error "GD32_HAS_I2C1 not defined in registry"
#endif

#if !defined(GD32_HAS_I2C2)
#error "GD32_HAS_I2C2 not defined in registry"
#endif

#if !defined(GD32_HAS_I2C3)
#error "GD32_HAS_I2C3 not defined in registry"
#endif

/* Peripheral availability checks.*/
#if GD32_I2C_USE_I2C0 && !GD32_HAS_I2C0
#error "I2C0 not present in the selected device"
#endif

#if GD32_I2C_USE_I2C1 && !GD32_HAS_I2C1
#error "I2C1 not present in the selected device"
#endif

#if GD32_I2C_USE_I2C2 && !GD32_HAS_I2C2
#error "I2C2 not present in the selected device"
#endif

#if GD32_I2C_USE_I2C3 && !GD32_HAS_I2C3
#error "I2C3 not present in the selected device"
#endif

#if !GD32_I2C_USE_I2C0 && !GD32_I2C_USE_I2C1 &&                             \
    !GD32_I2C_USE_I2C2 && !GD32_I2C_USE_I2C3
#error "I2C driver activated but no I2C peripheral assigned"
#endif

/* Checks on allocation of I2Cx units, the interrupt dispatch files in
   GD32/LLD/I2C are keyed on these symbols.*/
#if GD32_I2C_USE_I2C0
#if defined(GD32_I2C0_IS_USED)
#error "I2CD0 requires I2C0 but it is already used"
#else
#define GD32_I2C0_IS_USED
#endif
#endif

#if GD32_I2C_USE_I2C1
#if defined(GD32_I2C1_IS_USED)
#error "I2CD1 requires I2C1 but it is already used"
#else
#define GD32_I2C1_IS_USED
#endif
#endif

#if GD32_I2C_USE_I2C2
#if defined(GD32_I2C2_IS_USED)
#error "I2CD2 requires I2C2 but it is already used"
#else
#define GD32_I2C2_IS_USED
#endif
#endif

#if GD32_I2C_USE_I2C3
#if defined(GD32_I2C3_IS_USED)
#error "I2CD3 requires I2C3 but it is already used"
#else
#define GD32_I2C3_IS_USED
#endif
#endif

/* IRQ priority checks.*/
#if GD32_I2C_USE_I2C0 &&                                                    \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_I2C_I2C0_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to I2C0"
#endif

#if GD32_I2C_USE_I2C1 &&                                                    \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_I2C_I2C1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to I2C1"
#endif

#if GD32_I2C_USE_I2C2 &&                                                    \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_I2C_I2C2_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to I2C2"
#endif

#if GD32_I2C_USE_I2C3 &&                                                    \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_I2C_I2C3_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to I2C3"
#endif

#if GD32_I2C_USE_DMA == TRUE

#if GD32_I2C_USE_I2C0
#if !defined(GD32_I2C_I2C0_RX_DMA_STREAM)
#error "GD32_I2C_I2C0_RX_DMA_STREAM not defined"
#endif

#if !defined(GD32_I2C_I2C0_TX_DMA_STREAM)
#error "GD32_I2C_I2C0_TX_DMA_STREAM not defined"
#endif

#if !GD32_DMA_IS_VALID_PRIORITY(GD32_I2C_I2C0_DMA_PRIORITY)
#error "Invalid DMA priority assigned to I2C0"
#endif
#endif

#if GD32_I2C_USE_I2C1
#if !defined(GD32_I2C_I2C1_RX_DMA_STREAM)
#error "GD32_I2C_I2C1_RX_DMA_STREAM not defined"
#endif

#if !defined(GD32_I2C_I2C1_TX_DMA_STREAM)
#error "GD32_I2C_I2C1_TX_DMA_STREAM not defined"
#endif

#if !GD32_DMA_IS_VALID_PRIORITY(GD32_I2C_I2C1_DMA_PRIORITY)
#error "Invalid DMA priority assigned to I2C1"
#endif
#endif

#if GD32_I2C_USE_I2C2
#if !defined(GD32_I2C_I2C2_RX_DMA_STREAM)
#error "GD32_I2C_I2C2_RX_DMA_STREAM not defined"
#endif

#if !defined(GD32_I2C_I2C2_TX_DMA_STREAM)
#error "GD32_I2C_I2C2_TX_DMA_STREAM not defined"
#endif

#if !GD32_DMA_IS_VALID_PRIORITY(GD32_I2C_I2C2_DMA_PRIORITY)
#error "Invalid DMA priority assigned to I2C2"
#endif
#endif

#if GD32_I2C_USE_I2C3
#if !defined(GD32_I2C_I2C3_RX_DMA_STREAM)
#error "GD32_I2C_I2C3_RX_DMA_STREAM not defined"
#endif

#if !defined(GD32_I2C_I2C3_TX_DMA_STREAM)
#error "GD32_I2C_I2C3_TX_DMA_STREAM not defined"
#endif

#if !GD32_DMA_IS_VALID_PRIORITY(GD32_I2C_I2C3_DMA_PRIORITY)
#error "Invalid DMA priority assigned to I2C3"
#endif
#endif

/* At least one active instance requires the DMA driver to be built in.*/
#if !defined(GD32_DMA_REQUIRED)
#define GD32_DMA_REQUIRED
#endif

#endif /* GD32_I2C_USE_DMA == TRUE */

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type representing an I2C address.
 */
typedef uint16_t i2caddr_t;

/**
 * @brief   Type of I2C driver condition flags.
 */
typedef uint32_t i2cflags_t;

/**
 * @brief   I2C driver configuration structure.
 */
struct hal_i2c_config {
  /**
   * @brief   I2C_TIMING register initialization.
   * @note    Refer to the GD32H73x_75x User Manual, the values are affected
   *          by the I2C kernel clock selected in mcuconf.h.
   */
  uint32_t        timing;
  /**
   * @brief   I2C_CTL0 register initialization.
   * @note    Leave to zero unless you know what you are doing.
   */
  uint32_t        ctl0;
  /**
   * @brief   I2C_CTL1 register initialization.
   * @note    Only the ADD10EN bit can eventually be specified here.
   */
  uint32_t        ctl1;
};

/**
 * @brief   Type of a structure representing an I2C configuration.
 */
typedef struct hal_i2c_config I2CConfig;

/**
 * @brief   Type of a structure representing an I2C driver.
 */
typedef struct hal_i2c_driver I2CDriver;

/**
 * @brief   Structure representing an I2C driver.
 */
struct hal_i2c_driver {
  /**
   * @brief   Driver state.
   */
  i2cstate_t                state;
  /**
   * @brief   Current configuration data.
   */
  const I2CConfig           *config;
  /**
   * @brief   Error flags.
   */
  i2cflags_t                errors;
#if I2C_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
  mutex_t                   mutex;
#endif /* I2C_USE_MUTUAL_EXCLUSION */
#if defined(I2C_DRIVER_EXT_FIELDS)
  I2C_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Thread waiting for I/O completion.
   */
  thread_reference_t        thread;
  /**
   * @brief   Number of bytes in TX phase.
   */
  size_t                    txbytes;
  /**
   * @brief   Number of bytes in RX phase.
   */
  size_t                    rxbytes;
#if (GD32_I2C_USE_DMA == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   RX DMA mode bit mask.
   */
  uint32_t                  rxdmamode;
  /**
   * @brief   TX DMA mode bit mask.
   */
  uint32_t                  txdmamode;
  /**
   * @brief   Receive DMA stream.
   */
  const gd32_dma_stream_t   *rx;
  /**
   * @brief   Transmit DMA stream.
   */
  const gd32_dma_stream_t   *tx;
#else /* GD32_I2C_USE_DMA == FALSE */
  /**
   * @brief   Pointer to the next TX buffer location.
   */
  const uint8_t             *txptr;
  /**
   * @brief   Pointer to the next RX buffer location.
   */
  uint8_t                   *rxptr;
#endif /* GD32_I2C_USE_DMA == FALSE */
  /**
   * @brief   Base address of the I2Cx registers block.
   */
  uint32_t                  i2c;
#if (I2C_ENABLE_SLAVE_MODE == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Master needed a reply.
   */
  bool                      reply_required;
  /**
   * @brief   Master/Slave mode.
   */
  bool                      isMaster;
#endif /* I2C_ENABLE_SLAVE_MODE == TRUE */
#if defined(GD32_I2C_ISR_LIMIT) || defined(__DOXYGEN__)
  /**
   * @brief   Count of interrupts since transfer start.
   */
  uint32_t                  isr_count;
  /**
   * @brief   Limit of interrupts for this transfer.
   */
  uint32_t                  isr_limit;
#endif
  /**
   * @brief   True when a transaction is in progress.
   */
  bool                      in_transaction;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Get errors from I2C driver.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
#define i2c_lld_get_errors(i2cp) ((i2cp)->errors)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
#if GD32_I2C_USE_I2C0
extern I2CDriver I2CD0;
#endif

#if GD32_I2C_USE_I2C1
extern I2CDriver I2CD1;
#endif

#if GD32_I2C_USE_I2C2
extern I2CDriver I2CD2;
#endif

#if GD32_I2C_USE_I2C3
extern I2CDriver I2CD3;
#endif
#endif /* !defined(__DOXYGEN__) */

#ifdef __cplusplus
extern "C" {
#endif
  void i2c_lld_init(void);
  void i2c_lld_start(I2CDriver *i2cp);
  void i2c_lld_stop(I2CDriver *i2cp);
  void i2c_lld_soft_stop(I2CDriver *i2cp);
  void i2c_lld_serve_event_interrupt(I2CDriver *i2cp);
  void i2c_lld_serve_error_interrupt(I2CDriver *i2cp);
  msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                        const uint8_t *txbuf, size_t txbytes,
                                        uint8_t *rxbuf, size_t rxbytes,
                                        sysinterval_t timeout);
  msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                       uint8_t *rxbuf, size_t rxbytes,
                                       sysinterval_t timeout);
#if (I2C_ENABLE_SLAVE_MODE == TRUE)
  msg_t i2c_lld_match_address(I2CDriver *i2cp, i2caddr_t addr);
  msg_t i2c_lld_slave_receive_timeout(I2CDriver *i2cp, uint8_t *rxbuf,
                                      size_t rxbytes, sysinterval_t timeout);
  msg_t i2c_lld_slave_transmit_timeout(I2CDriver *i2cp, const uint8_t *txbuf,
                                       size_t txbytes, sysinterval_t timeout);
#endif /* I2C_ENABLE_SLAVE_MODE == TRUE */
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_I2C */

#endif /* HAL_I2C_LLD_H */

/** @} */
