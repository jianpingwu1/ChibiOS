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
 * @file    I2Cv1/hal_i2c_lld.c
 * @brief   GD32 I2C subsystem low level driver source.
 *
 * @addtogroup I2C
 * @{
 */

#include "hal.h"

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if GD32_I2C_USE_DMA == TRUE
#define DMAMODE_COMMON                                                      \
  (GD32_DMA_CTL_PWIDTH_BYTE | GD32_DMA_CTL_MWIDTH_BYTE |                    \
   GD32_DMA_CTL_MNAGA       | GD32_DMA_CTL_SDEIE       |                    \
   GD32_DMA_CTL_TAEIE       | GD32_DMA_CTL_FTFIE)
#endif

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief I2C0 driver identifier.*/
#if GD32_I2C_USE_I2C0 || defined(__DOXYGEN__)
I2CDriver I2CD0;
#endif

/** @brief I2C1 driver identifier.*/
#if GD32_I2C_USE_I2C1 || defined(__DOXYGEN__)
I2CDriver I2CD1;
#endif

/** @brief I2C2 driver identifier.*/
#if GD32_I2C_USE_I2C2 || defined(__DOXYGEN__)
I2CDriver I2CD2;
#endif

/** @brief I2C3 driver identifier.*/
#if GD32_I2C_USE_I2C3 || defined(__DOXYGEN__)
I2CDriver I2CD3;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void i2c_lld_reset(I2CDriver *i2cp);

static inline size_t i2c_lld_get_rxbytes(I2CDriver *i2cp) {

#if GD32_I2C_USE_DMA == TRUE
  return dmaStreamGetTransactionSize(i2cp->rx);
#else
  return i2cp->rxbytes;
#endif
}

static inline size_t i2c_lld_get_txbytes(I2CDriver *i2cp) {

#if GD32_I2C_USE_DMA == TRUE
  return dmaStreamGetTransactionSize(i2cp->tx);
#else
  return i2cp->txbytes;
#endif
}

#if GD32_I2C_USE_DMA == TRUE
static inline void i2c_lld_start_rx_dma(I2CDriver *i2cp) {

  dmaStreamEnable(i2cp->rx);
}

static inline void i2c_lld_start_tx_dma(I2CDriver *i2cp) {

  dmaStreamEnable(i2cp->tx);
}

static inline void i2c_lld_stop_rx_dma(I2CDriver *i2cp) {

  if (i2cp->rx != NULL) {
    dmaStreamDisable(i2cp->rx);
  }
}

static inline void i2c_lld_stop_tx_dma(I2CDriver *i2cp) {

  if (i2cp->tx != NULL) {
    dmaStreamDisable(i2cp->tx);
  }
}

/* The DMA requests are level driven and stay asserted while the data
   register is empty, they are removed at the end of each transfer phase so
   that a stale request cannot feed the following one.*/
static inline void i2c_lld_dma_request_disable(I2CDriver *i2cp) {

  I2C_CTL0(i2cp->i2c) &= ~(I2C_CTL0_DENT | I2C_CTL0_DENR);
}
#endif /* GD32_I2C_USE_DMA == TRUE */

/**
 * @brief   Slave address setup.
 * @note    The RW bit is set to zero internally.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 *
 * @notapi
 */
static void i2c_lld_set_address(I2CDriver *i2cp, i2caddr_t addr) {

  /* Address alignment depends on the addressing mode selected.*/
  if ((i2cp->config->ctl1 & I2C_CTL1_ADD10EN) == 0U) {
    I2C_CTL1(i2cp->i2c) = (uint32_t)addr << 1U;
  }
  else {
    I2C_CTL1(i2cp->i2c) = (uint32_t)addr;
  }
}

/**
 * @brief   I2C RX transfer setup.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static void i2c_lld_setup_rx_transfer(I2CDriver *i2cp) {
  uint32_t reload;
  size_t n;

  /* The unit can transfer 255 bytes maximum in a single operation.*/
  n = i2c_lld_get_rxbytes(i2cp);
  if (n > 255U) {
    n = 255U;
    reload = I2C_CTL1_RELOAD;
  }
  else {
    reload = 0U;
  }

  /* Configures CTL1 with both the calculated and the static settings.*/
  I2C_CTL1(i2cp->i2c) = (I2C_CTL1(i2cp->i2c) &
                         ~(I2C_CTL1_BYTENUM | I2C_CTL1_RELOAD)) |
                        i2cp->config->ctl1 | I2C_CTL1_TRDIR |
                        ((uint32_t)n << 16U) | reload;
}

/**
 * @brief   I2C TX transfer setup.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static void i2c_lld_setup_tx_transfer(I2CDriver *i2cp) {
  uint32_t reload;
  size_t n;

  /* The unit can transfer 255 bytes maximum in a single operation.*/
  n = i2c_lld_get_txbytes(i2cp);
  if (n > 255U) {
    n = 255U;
    reload = I2C_CTL1_RELOAD;
  }
  else {
    reload = 0U;
  }

  /* Configures CTL1 with both the calculated and the static settings.*/
  I2C_CTL1(i2cp->i2c) = (I2C_CTL1(i2cp->i2c) &
                         ~(I2C_CTL1_BYTENUM | I2C_CTL1_RELOAD)) |
                        i2cp->config->ctl1 |
                        ((uint32_t)n << 16U) | reload;
}

/**
 * @brief   Aborts an I2C transaction.
 * @note    The wait on the I2CEN bit is bounded, a stuck peripheral cannot
 *          block the caller.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static void i2c_lld_abort_operation(I2CDriver *i2cp) {

  if ((I2C_CTL0(i2cp->i2c) & I2C_CTL0_I2CEN) != 0U) {
    uint32_t n = GD32_I2C_SYNC_TIMEOUT;

    /* Stops the I2C peripheral.*/
    I2C_CTL0(i2cp->i2c) &= ~I2C_CTL0_I2CEN;
    while (((I2C_CTL0(i2cp->i2c) & I2C_CTL0_I2CEN) != 0U) && (n > 0U)) {
      I2C_CTL0(i2cp->i2c) &= ~I2C_CTL0_I2CEN;
      n--;
    }
    I2C_CTL0(i2cp->i2c) |= I2C_CTL0_I2CEN;
  }

#if GD32_I2C_USE_DMA == TRUE
  /* Stops the associated DMA streams.*/
  i2c_lld_dma_request_disable(i2cp);
  i2c_lld_stop_rx_dma(i2cp);
  i2c_lld_stop_tx_dma(i2cp);
#else
  I2C_CTL0(i2cp->i2c) &= ~(I2C_CTL0_TIE | I2C_CTL0_RBNEIE);
#endif
}

/**
 * @brief   I2C shared ISR code.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] stat      content of the STAT register to be decoded
 *
 * @notapi
 */
static void i2c_lld_serve_isr(I2CDriver *i2cp, uint32_t stat) {
  uint32_t i2c = i2cp->i2c;

#if (GD32_I2C_USE_DMA == FALSE) || (I2C_ENABLE_SLAVE_MODE == TRUE)
  uint32_t ctl0 = I2C_CTL0(i2c);
#endif

  if (!i2cp->in_transaction) {
    I2C_CTL0(i2c) &= ~(I2C_CTL0_TCIE | I2C_CTL0_TIE | I2C_CTL0_RBNEIE);
    i2c_lld_reset(i2cp);
    return;
  }

  /* Special case of a received NACK, the transfer is aborted.*/
  if ((stat & I2C_STAT_NACK) != 0U) {

#if GD32_I2C_USE_DMA == TRUE
    /* Stops the associated DMA streams.*/
    i2c_lld_dma_request_disable(i2cp);
    i2c_lld_stop_rx_dma(i2cp);
    i2c_lld_stop_tx_dma(i2cp);
#endif

#if (I2C_ENABLE_SLAVE_MODE == TRUE)
    /* The master signals the end of a read operation with a NACK, in slave
       mode this is not an error, the STOP event is next.*/
    if (!i2cp->isMaster && (i2cp->state == I2C_ACTIVE_TX)) {
      if (((stat & I2C_STAT_TR) != 0U) && ((stat & I2C_STAT_TI) != 0U)) {
        return;
      }
    }
#endif

    /* Error flag.*/
    i2cp->errors |= I2C_ACK_FAILURE;

    /* Transaction finished sending the STOP.*/
    I2C_CTL1(i2c) |= I2C_CTL1_STOP;

    /* Make sure no more interrupts.*/
    I2C_CTL0(i2c) &= ~(I2C_CTL0_TCIE | I2C_CTL0_TIE | I2C_CTL0_RBNEIE);

    /* Errors are signaled to the upper layer.*/
    _i2c_wakeup_error_isr(i2cp);

    return;
  }

#if (I2C_ENABLE_SLAVE_MODE == TRUE)
  if (!i2cp->isMaster) {
    /* Slave mode handling, note, TC is not asserted in slave mode.*/

    /* Communication completed.*/
    if ((stat & I2C_STAT_STPDET) != 0U) {

      I2C_CTL0(i2c) &= ~(I2C_CTL0_TIE | I2C_CTL0_RBNEIE);
#if GD32_I2C_USE_DMA == TRUE
      i2c_lld_dma_request_disable(i2cp);
      i2c_lld_stop_rx_dma(i2cp);
      i2c_lld_stop_tx_dma(i2cp);
#endif

      /* Normal transaction end.*/
      _i2c_wakeup_isr(i2cp);

      return;
    }

    /* Slave address match.*/
    if ((stat & I2C_STAT_ADDSEND) != 0U) {
      /* Checking the requested direction.*/
      if ((stat & I2C_STAT_TR) != 0U) {
        /* Reply required.*/
        i2cp->reply_required = true;

        if (i2cp->state == I2C_ACTIVE_RX) {
          I2C_CTL0(i2c) &= ~I2C_CTL0_RBNEIE;
#if GD32_I2C_USE_DMA == TRUE
          i2c_lld_dma_request_disable(i2cp);
          i2c_lld_stop_rx_dma(i2cp);
#endif
          _i2c_wakeup_isr(i2cp);
        }
      }
      return;
    }

    if (i2cp->state == I2C_ACTIVE_TX) {
      /* Transmission phase.*/
      if (((ctl0 & I2C_CTL0_TIE) != 0U) && ((stat & I2C_STAT_TI) != 0U)) {
#if GD32_I2C_USE_DMA == FALSE
        I2C_TDATA(i2c) = (uint32_t)*i2cp->txptr;
        i2cp->txptr++;
        i2cp->txbytes--;
        if (i2cp->txbytes == 0U) {
          I2C_CTL0(i2c) &= ~I2C_CTL0_TIE;
        }
#else
        /* Enabling TX DMA.*/
        i2c_lld_start_tx_dma(i2cp);
        I2C_CTL0(i2c) |= I2C_CTL0_DENT;
#endif
      }
    }
    else {
      /* Receive phase.*/
      if (((ctl0 & I2C_CTL0_RBNEIE) != 0U) && ((stat & I2C_STAT_RBNE) != 0U)) {
#if GD32_I2C_USE_DMA == FALSE
        *i2cp->rxptr = (uint8_t)I2C_RDATA(i2c);
        i2cp->rxptr++;
        i2cp->rxbytes--;
        if (i2cp->rxbytes == 0U) {
          I2C_CTL0(i2c) &= ~I2C_CTL0_RBNEIE;
        }
#else
        /* Enabling RX DMA.*/
        i2c_lld_start_rx_dma(i2cp);
        I2C_CTL0(i2c) |= I2C_CTL0_DENR;
#endif
      }
    }

    return;
  }
#endif /* I2C_ENABLE_SLAVE_MODE == TRUE */

#if GD32_I2C_USE_DMA == FALSE
  /* Handling of data transfer if the DMA mode is disabled.*/
  if (i2cp->state == I2C_ACTIVE_TX) {
    /* Transmission phase.*/
    if (((ctl0 & I2C_CTL0_TIE) != 0U) && ((stat & I2C_STAT_TI) != 0U)) {
      I2C_TDATA(i2c) = (uint32_t)*i2cp->txptr;
      i2cp->txptr++;
      i2cp->txbytes--;
      if (i2cp->txbytes == 0U) {
        I2C_CTL0(i2c) &= ~I2C_CTL0_TIE;
      }
    }
  }
  else {
    /* Receive phase.*/
    if (((ctl0 & I2C_CTL0_RBNEIE) != 0U) && ((stat & I2C_STAT_RBNE) != 0U)) {
      *i2cp->rxptr = (uint8_t)I2C_RDATA(i2c);
      i2cp->rxptr++;
      i2cp->rxbytes--;
      if (i2cp->rxbytes == 0U) {
        I2C_CTL0(i2c) &= ~I2C_CTL0_RBNEIE;
      }
    }
  }
#endif

  /* Partial transfer handling, restarting the transfer and returning.*/
  if ((stat & I2C_STAT_TCR) != 0U) {
    if (i2cp->state == I2C_ACTIVE_TX) {
      i2c_lld_setup_tx_transfer(i2cp);
    }
    else {
      i2c_lld_setup_rx_transfer(i2cp);
    }
    return;
  }

  /* The following condition is true if a transfer phase has been completed.*/
  if ((stat & I2C_STAT_TC) != 0U) {
    if (i2cp->state == I2C_ACTIVE_TX) {
      /* End of the transmit phase.*/

#if GD32_I2C_USE_DMA == TRUE
      i2c_lld_dma_request_disable(i2cp);
      i2c_lld_stop_tx_dma(i2cp);
#endif

      /* Starting receive phase if necessary.*/
      if (i2c_lld_get_rxbytes(i2cp) > 0U) {
        /* Setting up the peripheral.*/
        i2c_lld_setup_rx_transfer(i2cp);

#if GD32_I2C_USE_DMA == TRUE
        /* Enabling RX DMA.*/
        i2c_lld_start_rx_dma(i2cp);
        I2C_CTL0(i2c) |= I2C_CTL0_DENR;
#else
        /* RX interrupt enabled.*/
        I2C_CTL0(i2c) |= I2C_CTL0_RBNEIE;
#endif

        /* Starts the read operation.*/
        I2C_CTL1(i2c) |= I2C_CTL1_START;

        /* State change.*/
        i2cp->state = I2C_ACTIVE_RX;

        /* Note, returning because the transaction is not over yet.*/
        return;
      }
    }
    else {
      /* End of the receive phase.*/
#if GD32_I2C_USE_DMA == TRUE
      i2c_lld_dma_request_disable(i2cp);
      i2c_lld_stop_rx_dma(i2cp);
#endif
    }

    /* Transaction finished sending the STOP.*/
    I2C_CTL1(i2c) |= I2C_CTL1_STOP;

    /* Make sure no more 'Transfer Complete' interrupts.*/
    I2C_CTL0(i2c) &= ~I2C_CTL0_TCIE;

    /* Normal transaction end.*/
    _i2c_wakeup_isr(i2cp);
  }
}

/**
 * @brief   I2C error handler.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] stat      content of the STAT register to be decoded
 *
 * @notapi
 */
static void i2c_lld_serve_error_isr(I2CDriver *i2cp, uint32_t stat) {

  if (!i2cp->in_transaction) {
    I2C_CTL0(i2cp->i2c) &= ~(I2C_CTL0_TCIE | I2C_CTL0_TIE | I2C_CTL0_RBNEIE);
    i2c_lld_reset(i2cp);
    return;
  }

#if GD32_I2C_USE_DMA == TRUE
  /* Stopping the DMA streams just to be safe.*/
  i2c_lld_dma_request_disable(i2cp);
  i2c_lld_stop_rx_dma(i2cp);
  i2c_lld_stop_tx_dma(i2cp);
#else
  I2C_CTL0(i2cp->i2c) &= ~(I2C_CTL0_TIE | I2C_CTL0_RBNEIE);
#endif

  if ((stat & I2C_STAT_BERR) != 0U) {
    i2cp->errors |= I2C_BUS_ERROR;
  }

  if ((stat & I2C_STAT_LOSTARB) != 0U) {
    i2cp->errors |= I2C_ARBITRATION_LOST;
  }

  if ((stat & I2C_STAT_OUERR) != 0U) {
    i2cp->errors |= I2C_OVERRUN;
  }

  if ((stat & I2C_STAT_PECERR) != 0U) {
    i2cp->errors |= I2C_PEC_ERROR;
  }

  if ((stat & I2C_STAT_TIMEOUT) != 0U) {
    i2cp->errors |= I2C_TIMEOUT;
  }

  if ((stat & I2C_STAT_SMBALT) != 0U) {
    i2cp->errors |= I2C_SMB_ALERT;
  }

  /* If some error has been identified then wake the waiting thread.*/
  if (i2cp->errors != I2C_NO_ERROR) {
    /* The bus is released with a STOP, without it the busy flag would stay
       set and every following transfer would fail on the busy timeout.*/
    I2C_CTL1(i2cp->i2c) |= I2C_CTL1_STOP;

    _i2c_wakeup_error_isr(i2cp);
  }
}

/**
 * @brief   Resets the I2C peripheral through the RCU.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
static void i2c_lld_reset(I2CDriver *i2cp) {

#if GD32_I2C_USE_I2C0
  if (&I2CD0 == i2cp) {
    rcuResetI2C0();
  }
#endif

#if GD32_I2C_USE_I2C1
  if (&I2CD1 == i2cp) {
    rcuResetI2C1();
  }
#endif

#if GD32_I2C_USE_I2C2
  if (&I2CD2 == i2cp) {
    rcuResetI2C2();
  }
#endif

#if GD32_I2C_USE_I2C3
  if (&I2CD3 == i2cp) {
    rcuResetI2C3();
  }
#endif
}

/**
 * @brief   Checks the ISR count limit for the current operation.
 * @details Prevents interrupt storms in case the peripheral misbehaves and
 *          the normal interrupt acknowledge does not work.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @return              The limit exceeded flag.
 *
 * @notapi
 */
static bool i2c_lld_check_isr_limit(I2CDriver *i2cp) {

#if defined(GD32_I2C_ISR_LIMIT)
  if (i2cp->isr_count++ > i2cp->isr_limit) {
    i2cp->errors |= I2C_ISR_LIMIT;
    i2c_lld_reset(i2cp);
#if GD32_I2C_USE_DMA == TRUE
    i2c_lld_stop_rx_dma(i2cp);
    i2c_lld_stop_tx_dma(i2cp);
#endif
    _i2c_wakeup_error_isr(i2cp);
    return true;
  }
#else
  (void)i2cp;
#endif
  return false;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   I2C event interrupt service routine.
 * @note    Called by the interrupt dispatch code in GD32/LLD/I2C.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_serve_event_interrupt(I2CDriver *i2cp) {
  uint32_t stat = I2C_STAT(i2cp->i2c);

  if (i2c_lld_check_isr_limit(i2cp)) {
    return;
  }

  /* Clearing the served flags.*/
  I2C_STATC(i2cp->i2c) = stat & GD32_I2C_STAT_INT_MASK;

  i2c_lld_serve_isr(i2cp, stat);
}

/**
 * @brief   I2C error interrupt service routine.
 * @note    Called by the interrupt dispatch code in GD32/LLD/I2C.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_serve_error_interrupt(I2CDriver *i2cp) {
  uint32_t stat = I2C_STAT(i2cp->i2c);

  if (i2c_lld_check_isr_limit(i2cp)) {
    return;
  }

  /* Clearing the served flags.*/
  I2C_STATC(i2cp->i2c) = stat & GD32_I2C_STAT_ERROR_MASK;

  i2c_lld_serve_error_isr(i2cp, stat);
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level I2C driver initialization.
 *
 * @notapi
 */
void i2c_lld_init(void) {

#if GD32_I2C_USE_I2C0
  i2cObjectInit(&I2CD0);
  I2CD0.thread = NULL;
  I2CD0.i2c    = I2C0;
#if GD32_I2C_USE_DMA == TRUE
  I2CD0.rx     = NULL;
  I2CD0.tx     = NULL;
#endif
#endif

#if GD32_I2C_USE_I2C1
  i2cObjectInit(&I2CD1);
  I2CD1.thread = NULL;
  I2CD1.i2c    = I2C1;
#if GD32_I2C_USE_DMA == TRUE
  I2CD1.rx     = NULL;
  I2CD1.tx     = NULL;
#endif
#endif

#if GD32_I2C_USE_I2C2
  i2cObjectInit(&I2CD2);
  I2CD2.thread = NULL;
  I2CD2.i2c    = I2C2;
#if GD32_I2C_USE_DMA == TRUE
  I2CD2.rx     = NULL;
  I2CD2.tx     = NULL;
#endif
#endif

#if GD32_I2C_USE_I2C3
  i2cObjectInit(&I2CD3);
  I2CD3.thread = NULL;
  I2CD3.i2c    = I2C3;
#if GD32_I2C_USE_DMA == TRUE
  I2CD3.rx     = NULL;
  I2CD3.tx     = NULL;
#endif
#endif
}

/**
 * @brief   Configures and activates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_start(I2CDriver *i2cp) {
  uint32_t i2c = i2cp->i2c;

  /* Makes sure the I2C peripheral is disabled.*/
  I2C_CTL0(i2c) &= ~I2C_CTL0_I2CEN;

  /* If in stopped state then enables the I2C and DMA clocks.*/
  if (i2cp->state == I2C_STOP) {

#if GD32_I2C_USE_DMA == TRUE
    /* Common DMA modes.*/
    i2cp->txdmamode = DMAMODE_COMMON | GD32_DMA_CTL_DIR_M2P;
    i2cp->rxdmamode = DMAMODE_COMMON | GD32_DMA_CTL_DIR_P2M;
#endif

#if GD32_I2C_USE_I2C0
    if (&I2CD0 == i2cp) {
      rcuResetI2C0();
      rcuEnableI2C0(true);
#if GD32_I2C_USE_DMA == TRUE
      i2cp->rx = dmaStreamAllocI(GD32_I2C_I2C0_RX_DMA_STREAM,
                                 GD32_I2C_I2C0_IRQ_PRIORITY, NULL, NULL);
      osalDbgAssert(i2cp->rx != NULL, "unable to allocate stream");
      i2cp->tx = dmaStreamAllocI(GD32_I2C_I2C0_TX_DMA_STREAM,
                                 GD32_I2C_I2C0_IRQ_PRIORITY, NULL, NULL);
      osalDbgAssert(i2cp->tx != NULL, "unable to allocate stream");

      i2cp->rxdmamode |= GD32_DMA_CTL_PRIO(GD32_I2C_I2C0_DMA_PRIORITY);
      i2cp->txdmamode |= GD32_DMA_CTL_PRIO(GD32_I2C_I2C0_DMA_PRIORITY);
      dmaSetRequestSource(i2cp->rx, GD32_DMAMUX1_I2C0_RX);
      dmaSetRequestSource(i2cp->tx, GD32_DMAMUX1_I2C0_TX);
#endif
    }
#endif /* GD32_I2C_USE_I2C0 */

#if GD32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {
      rcuResetI2C1();
      rcuEnableI2C1(true);
#if GD32_I2C_USE_DMA == TRUE
      i2cp->rx = dmaStreamAllocI(GD32_I2C_I2C1_RX_DMA_STREAM,
                                 GD32_I2C_I2C1_IRQ_PRIORITY, NULL, NULL);
      osalDbgAssert(i2cp->rx != NULL, "unable to allocate stream");
      i2cp->tx = dmaStreamAllocI(GD32_I2C_I2C1_TX_DMA_STREAM,
                                 GD32_I2C_I2C1_IRQ_PRIORITY, NULL, NULL);
      osalDbgAssert(i2cp->tx != NULL, "unable to allocate stream");

      i2cp->rxdmamode |= GD32_DMA_CTL_PRIO(GD32_I2C_I2C1_DMA_PRIORITY);
      i2cp->txdmamode |= GD32_DMA_CTL_PRIO(GD32_I2C_I2C1_DMA_PRIORITY);
      dmaSetRequestSource(i2cp->rx, GD32_DMAMUX1_I2C1_RX);
      dmaSetRequestSource(i2cp->tx, GD32_DMAMUX1_I2C1_TX);
#endif
    }
#endif /* GD32_I2C_USE_I2C1 */

#if GD32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {
      rcuResetI2C2();
      rcuEnableI2C2(true);
#if GD32_I2C_USE_DMA == TRUE
      i2cp->rx = dmaStreamAllocI(GD32_I2C_I2C2_RX_DMA_STREAM,
                                 GD32_I2C_I2C2_IRQ_PRIORITY, NULL, NULL);
      osalDbgAssert(i2cp->rx != NULL, "unable to allocate stream");
      i2cp->tx = dmaStreamAllocI(GD32_I2C_I2C2_TX_DMA_STREAM,
                                 GD32_I2C_I2C2_IRQ_PRIORITY, NULL, NULL);
      osalDbgAssert(i2cp->tx != NULL, "unable to allocate stream");

      i2cp->rxdmamode |= GD32_DMA_CTL_PRIO(GD32_I2C_I2C2_DMA_PRIORITY);
      i2cp->txdmamode |= GD32_DMA_CTL_PRIO(GD32_I2C_I2C2_DMA_PRIORITY);
      dmaSetRequestSource(i2cp->rx, GD32_DMAMUX1_I2C2_RX);
      dmaSetRequestSource(i2cp->tx, GD32_DMAMUX1_I2C2_TX);
#endif
    }
#endif /* GD32_I2C_USE_I2C2 */

#if GD32_I2C_USE_I2C3
    if (&I2CD3 == i2cp) {
      rcuResetI2C3();
      rcuEnableI2C3(true);
#if GD32_I2C_USE_DMA == TRUE
      i2cp->rx = dmaStreamAllocI(GD32_I2C_I2C3_RX_DMA_STREAM,
                                 GD32_I2C_I2C3_IRQ_PRIORITY, NULL, NULL);
      osalDbgAssert(i2cp->rx != NULL, "unable to allocate stream");
      i2cp->tx = dmaStreamAllocI(GD32_I2C_I2C3_TX_DMA_STREAM,
                                 GD32_I2C_I2C3_IRQ_PRIORITY, NULL, NULL);
      osalDbgAssert(i2cp->tx != NULL, "unable to allocate stream");

      i2cp->rxdmamode |= GD32_DMA_CTL_PRIO(GD32_I2C_I2C3_DMA_PRIORITY);
      i2cp->txdmamode |= GD32_DMA_CTL_PRIO(GD32_I2C_I2C3_DMA_PRIORITY);
      dmaSetRequestSource(i2cp->rx, GD32_DMAMUX1_I2C3_RX);
      dmaSetRequestSource(i2cp->tx, GD32_DMAMUX1_I2C3_TX);
#endif
    }
#endif /* GD32_I2C_USE_I2C3 */
  }

#if GD32_I2C_USE_DMA == TRUE
  /* I2C data registers pointed by the DMA.*/
  dmaStreamSetPeripheral(i2cp->rx, &I2C_RDATA(i2c));
  dmaStreamSetPeripheral(i2cp->tx, &I2C_TDATA(i2c));
#endif

  /* Peripheral setup, the TCIE bit and the DMA request enables are handled
     separately, see i2c_lld_dma_request_disable().*/
  I2C_CTL0(i2c) = i2cp->config->ctl0 | I2C_CTL0_ERRIE | I2C_CTL0_NACKIE;

  /* Timing setup.*/
  I2C_TIMING(i2c) = i2cp->config->timing;

  /* Ready to go.*/
  I2C_CTL0(i2c) |= I2C_CTL0_I2CEN;
}

/**
 * @brief   Deactivates the I2C peripheral.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_stop(I2CDriver *i2cp) {

  /* If not in stopped state then disables the I2C clock.*/
  if (i2cp->state != I2C_STOP) {

    /* I2C disable.*/
    i2c_lld_abort_operation(i2cp);

#if GD32_I2C_USE_DMA == TRUE
    dmaStreamFreeI(i2cp->rx);
    i2cp->rx = NULL;
    dmaStreamFreeI(i2cp->tx);
    i2cp->tx = NULL;
#endif

#if GD32_I2C_USE_I2C0
    if (&I2CD0 == i2cp) {
      rcuDisableI2C0();
    }
#endif

#if GD32_I2C_USE_I2C1
    if (&I2CD1 == i2cp) {
      rcuDisableI2C1();
    }
#endif

#if GD32_I2C_USE_I2C2
    if (&I2CD2 == i2cp) {
      rcuDisableI2C2();
    }
#endif

#if GD32_I2C_USE_I2C3
    if (&I2CD3 == i2cp) {
      rcuDisableI2C3();
    }
#endif
  }
}

/**
 * @brief   Releases the DMA streams leaving the peripheral enabled.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 *
 * @notapi
 */
void i2c_lld_soft_stop(I2CDriver *i2cp) {

  if (i2cp->state != I2C_STOP) {
#if GD32_I2C_USE_DMA == TRUE
    dmaStreamFreeI(i2cp->rx);
    i2cp->rx = NULL;
    dmaStreamFreeI(i2cp->tx);
    i2cp->tx = NULL;
#else
    (void)i2cp;
#endif
  }
}

/**
 * @brief   Receives data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end.
 *
 * @notapi
 */
msg_t i2c_lld_master_receive_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                     uint8_t *rxbuf, size_t rxbytes,
                                     sysinterval_t timeout) {
  msg_t msg;
  uint32_t i2c = i2cp->i2c;
  systime_t start, end;

#if (I2C_ENABLE_SLAVE_MODE == TRUE)
  i2cp->isMaster = true;
#endif

  /* Resetting error flags for this transfer.*/
  i2cp->errors = I2C_NO_ERROR;

  /* Releases the lock from high level driver.*/
  osalSysUnlock();

#if defined(GD32_I2C_ISR_LIMIT)
  i2cp->isr_limit = rxbytes * GD32_I2C_ISR_LIMIT;
  i2cp->isr_count = 0;
#endif

#if GD32_I2C_USE_DMA == TRUE
  /* RX DMA setup.*/
  dmaStreamSetMode(i2cp->rx, i2cp->rxdmamode);
  dmaStreamSetMemory0(i2cp->rx, rxbuf);
  dmaStreamSetTransactionSize(i2cp->rx, rxbytes);
#else
  /* Sizes of transfer phases.*/
  i2cp->txbytes = 0U;
  i2cp->rxbytes = rxbytes;
  i2cp->rxptr = rxbuf;
#endif

  /* Calculating the time window for the timeout on the busy bus condition.*/
  start = osalOsGetSystemTimeX();
  end = osalTimeAddX(start, OSAL_MS2I(GD32_I2C_BUSY_TIMEOUT));

  /* Waits until the busy flag is reset or, alternatively, for a timeout
     condition.*/
  while (true) {
    osalSysLock();

    /* If the bus is not busy then the operation can continue, note, the
       loop is exited in the locked state.*/
    if ((I2C_STAT(i2c) & I2C_STAT_I2CBSY) == 0U) {
      break;
    }

    /* If the system time went outside the allowed window then a timeout
       condition is returned.*/
    if (!osalTimeIsInRangeX(osalOsGetSystemTimeX(), start, end)) {
      return MSG_TIMEOUT;
    }

    osalSysUnlock();
  }

  i2cp->in_transaction = true;

  /* Residual flags of the previous transaction are dropped, the STOP
     detection flag in particular is never acknowledged by the transfer
     that generated it.*/
  I2C_STATC(i2c) = GD32_I2C_STATC_MASK;

  /* Setting up the slave address.*/
  i2c_lld_set_address(i2cp, addr);

  /* Setting up the peripheral.*/
  i2c_lld_setup_rx_transfer(i2cp);

#if GD32_I2C_USE_DMA == TRUE
  /* Enabling RX DMA, the peripheral request is enabled last so that the
     data register cannot be fed before the stream is ready.*/
  i2c_lld_start_rx_dma(i2cp);
  I2C_CTL0(i2c) |= I2C_CTL0_DENR;

  /* Transfer complete interrupt enabled.*/
  I2C_CTL0(i2c) |= I2C_CTL0_TCIE;
#else
  /* Transfer complete and RX interrupts enabled.*/
  I2C_CTL0(i2c) |= I2C_CTL0_TCIE | I2C_CTL0_RBNEIE;
#endif

  /* Starts the operation.*/
  I2C_CTL1(i2c) |= I2C_CTL1_START;

  /* Waits for the operation completion or a timeout.*/
  msg = osalThreadSuspendTimeoutS(&i2cp->thread, timeout);

  /* In case of a software timeout a STOP is sent as an extreme attempt
     to release the bus and DMA is forcibly disabled.*/
  if (msg == MSG_TIMEOUT) {
    I2C_CTL1(i2c) |= I2C_CTL1_STOP;
  }

#if GD32_I2C_USE_DMA == TRUE
  i2c_lld_dma_request_disable(i2cp);
  i2c_lld_stop_rx_dma(i2cp);
#endif

  i2cp->in_transaction = false;

  return msg;
}

/**
 * @brief   Transmits data via the I2C bus as master.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @param[in] txbuf     pointer to the transmit buffer
 * @param[in] txbytes   number of bytes to be transmitted
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   number of bytes to be received
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if one or more I2C errors occurred, the errors can
 *                      be retrieved using @p i2cGetErrors().
 * @retval MSG_TIMEOUT  if a timeout occurred before operation end.
 *
 * @notapi
 */
msg_t i2c_lld_master_transmit_timeout(I2CDriver *i2cp, i2caddr_t addr,
                                      const uint8_t *txbuf, size_t txbytes,
                                      uint8_t *rxbuf, size_t rxbytes,
                                      sysinterval_t timeout) {
  msg_t msg;
  uint32_t i2c = i2cp->i2c;
  systime_t start, end;

#if (I2C_ENABLE_SLAVE_MODE == TRUE)
  i2cp->isMaster = true;
#endif

  /* Resetting error flags for this transfer.*/
  i2cp->errors = I2C_NO_ERROR;

  /* Releases the lock from high level driver.*/
  osalSysUnlock();

#if defined(GD32_I2C_ISR_LIMIT)
  i2cp->isr_limit = (txbytes + rxbytes) * GD32_I2C_ISR_LIMIT;
  i2cp->isr_count = 0;
#endif

#if GD32_I2C_USE_DMA == TRUE
  /* TX and RX DMA setup.*/
  dmaStreamSetMode(i2cp->tx, i2cp->txdmamode);
  dmaStreamSetMemory0(i2cp->tx, txbuf);
  dmaStreamSetTransactionSize(i2cp->tx, txbytes);

  dmaStreamSetMode(i2cp->rx, i2cp->rxdmamode);
  dmaStreamSetMemory0(i2cp->rx, rxbuf);
  dmaStreamSetTransactionSize(i2cp->rx, rxbytes);
#else
  /* Sizes of transfer phases.*/
  i2cp->txbytes = txbytes;
  i2cp->rxbytes = rxbytes;
  i2cp->txptr = txbuf;
  i2cp->rxptr = rxbuf;
#endif

  /* Calculating the time window for the timeout on the busy bus condition.*/
  start = osalOsGetSystemTimeX();
  end = osalTimeAddX(start, OSAL_MS2I(GD32_I2C_BUSY_TIMEOUT));

  /* Waits until the busy flag is reset or, alternatively, for a timeout
     condition.*/
  while (true) {
    osalSysLock();

    /* If the bus is not busy then the operation can continue, note, the
       loop is exited in the locked state.*/
    if ((I2C_STAT(i2c) & I2C_STAT_I2CBSY) == 0U) {
      break;
    }

    /* If the system time went outside the allowed window then a timeout
       condition is returned.*/
    if (!osalTimeIsInRangeX(osalOsGetSystemTimeX(), start, end)) {
      return MSG_TIMEOUT;
    }

    osalSysUnlock();
  }

  i2cp->in_transaction = true;

  /* Residual flags of the previous transaction are dropped, the STOP
     detection flag in particular is never acknowledged by the transfer
     that generated it.*/
  I2C_STATC(i2c) = GD32_I2C_STATC_MASK;

  /* Setting up the slave address.*/
  i2c_lld_set_address(i2cp, addr);

  /* Preparing the transfer.*/
  i2c_lld_setup_tx_transfer(i2cp);

#if GD32_I2C_USE_DMA == TRUE
  /* Enabling TX DMA, the peripheral request is enabled last so that the
     data register cannot be fed before the stream is ready.*/
  i2c_lld_start_tx_dma(i2cp);
  I2C_CTL0(i2c) |= I2C_CTL0_DENT;

  /* Transfer complete interrupt enabled.*/
  I2C_CTL0(i2c) |= I2C_CTL0_TCIE;
#else
  /* Transfer complete and TX interrupts enabled.*/
  I2C_CTL0(i2c) |= I2C_CTL0_TCIE | I2C_CTL0_TIE;
#endif

  /* Starts the operation.*/
  I2C_CTL1(i2c) |= I2C_CTL1_START;

  /* Waits for the operation completion or a timeout.*/
  msg = osalThreadSuspendTimeoutS(&i2cp->thread, timeout);

  /* In case of a software timeout a STOP is sent as an extreme attempt
     to release the bus and DMA is forcibly disabled.*/
  if (msg == MSG_TIMEOUT) {
    I2C_CTL1(i2c) |= I2C_CTL1_STOP;
  }

#if GD32_I2C_USE_DMA == TRUE
  i2c_lld_dma_request_disable(i2cp);
  i2c_lld_stop_rx_dma(i2cp);
  i2c_lld_stop_tx_dma(i2cp);
#endif

  i2cp->in_transaction = false;

  return msg;
}

#if (I2C_ENABLE_SLAVE_MODE == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Listens the I2C bus for an address match.
 * @details Only 7 bits addresses are supported.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] addr      slave device address
 * @return              The operation status.
 * @retval MSG_OK       if the function succeeded.
 * @retval MSG_RESET    if the address cannot be matched.
 *
 * @notapi
 */
msg_t i2c_lld_match_address(I2CDriver *i2cp, i2caddr_t addr) {

  i2cp->isMaster = false;

  if ((addr >> 7) != 0U) {
    /* Cannot add this address to the set of those matched.*/
    return MSG_RESET;
  }

  /* Address bits can be written only when the address is disabled.*/
  I2C_SADDR0(i2cp->i2c) = 0U;
  I2C_SADDR0(i2cp->i2c) = ((uint32_t)addr << 1U) | I2C_SADDR0_ADDRESSEN;

  return MSG_OK;
}

/**
 * @brief   Receives data via the I2C bus as slave.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[out] rxbuf    pointer to the receive buffer
 * @param[in] rxbytes   size of the receive buffer
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 *
 * @notapi
 */
msg_t i2c_lld_slave_receive_timeout(I2CDriver *i2cp, uint8_t *rxbuf,
                                    size_t rxbytes, sysinterval_t timeout) {

  i2cp->isMaster = false;

  /* Resetting the reply flag.*/
  i2cp->reply_required = false;

#if GD32_I2C_USE_DMA == TRUE
  /* RX DMA setup.*/
  dmaStreamSetMode(i2cp->rx, i2cp->rxdmamode);
  dmaStreamSetMemory0(i2cp->rx, rxbuf);
  dmaStreamSetTransactionSize(i2cp->rx, rxbytes);
#else
  i2cp->rxptr = rxbuf;
  i2cp->rxbytes = rxbytes;
#endif

  i2cp->in_transaction = true;

  /* Address match, RX and STOP interrupts enabled.*/
  I2C_CTL0(i2cp->i2c) |= I2C_CTL0_ADDMIE | I2C_CTL0_RBNEIE |
                         I2C_CTL0_STPDETIE;

  /* Waits for the operation completion or a timeout.*/
  return osalThreadSuspendTimeoutS(&i2cp->thread, timeout);
}

/**
 * @brief   Transmits data via the I2C bus as slave.
 * @details Call this function when the master requests data.
 *
 * @param[in] i2cp      pointer to the @p I2CDriver object
 * @param[in] txbuf     pointer to the transmit buffer
 * @param[in] txbytes   number of bytes to be transmitted
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The operation status.
 *
 * @notapi
 */
msg_t i2c_lld_slave_transmit_timeout(I2CDriver *i2cp, const uint8_t *txbuf,
                                     size_t txbytes, sysinterval_t timeout) {

  i2cp->isMaster = false;

#if GD32_I2C_USE_DMA == TRUE
  /* TX DMA setup.*/
  dmaStreamSetMode(i2cp->tx, i2cp->txdmamode);
  dmaStreamSetMemory0(i2cp->tx, txbuf);
  dmaStreamSetTransactionSize(i2cp->tx, txbytes);
#else
  i2cp->txptr = txbuf;
  i2cp->txbytes = txbytes;
#endif

  i2cp->in_transaction = true;

  /* Address match, TX and STOP interrupts enabled.*/
  I2C_CTL0(i2cp->i2c) |= I2C_CTL0_ADDMIE | I2C_CTL0_TIE |
                         I2C_CTL0_STPDETIE;

  /* Waits for the operation completion or a timeout.*/
  return osalThreadSuspendTimeoutS(&i2cp->thread, timeout);
}
#endif /* I2C_ENABLE_SLAVE_MODE == TRUE */

#endif /* HAL_USE_I2C */

/** @} */
