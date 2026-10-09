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
 * @file    SDMMCv2/hal_sdc_lld.c
 * @brief   GD32 SDC subsystem low level driver source.
 * @note    Software timeouts have been added to all flag-wait polling
 *          loops.
 *
 * @addtogroup SDC
 * @{
 */

#include <string.h>

#include "hal.h"

#if HAL_USE_SDC || defined(__DOXYGEN__)

#include "bouncebuffer.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define SDIO_INTC_ALL_FLAGS     0xFFFFFFFFU

#define SDIO_STAT_ERROR_MASK                                                \
  (SDIO_STAT_CCRCERR | SDIO_STAT_DTCRCERR |                                 \
   SDIO_STAT_CMDTMOUT | SDIO_STAT_DTTMOUT |                                 \
   SDIO_STAT_TXURE | SDIO_STAT_RXORE)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief SDCD0 driver identifier.*/
#if GD32_SDC_USE_SDIO0 || defined(__DOXYGEN__)
SDCDriver SDCD0;
#endif

/** @brief SDCD1 driver identifier.*/
#if GD32_SDC_USE_SDIO1 || defined(__DOXYGEN__)
SDCDriver SDCD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   SDIO default configuration.
 */
static const SDCConfig sdc_default_cfg = {
  SDC_MODE_4BIT,
  0U
};

/* Internal scratch buffers. They never reach the IDMA directly, every DMA
   visible buffer goes through the bouncebuffer layer, so normal cached RAM
   is used. The block buffer is IDMA aligned to keep it from bouncing.*/
#if GD32_SDC_USE_SDIO0 || defined(__DOXYGEN__)
static uint8_t sd0_buf[MMCSD_BLOCK_SIZE] CC_ALIGN_DATA(32);
static uint32_t sd0_wbuf[1];
#endif

#if GD32_SDC_USE_SDIO1 || defined(__DOXYGEN__)
static uint8_t sd1_buf[MMCSD_BLOCK_SIZE] CC_ALIGN_DATA(32);
static uint32_t sd1_wbuf[1];
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Calculates a clock divider for the specified frequency.
 * @note    The divider is calculated to not exceed the required frequency
 *          in case of non-integer division.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] f         required frequency
 * @return              The CLKCTL value.
 */
static uint32_t sdc_lld_clkdiv(SDCDriver *sdcp, uint32_t f) {

#if defined(GD32_SDC_MAX_CLOCK)
  /* Optional enforcement of an arbitrary frequency limit.*/
  if (f > GD32_SDC_MAX_CLOCK) {
    f = GD32_SDC_MAX_CLOCK;
  }
#endif

  if (f >= sdcp->clkfreq) {
    return sdcp->config->slowdown;
  }

  return sdcp->config->slowdown + ((sdcp->clkfreq + (f * 2U) - 1U) / (f * 2U));
}

/**
 * @brief   Calculates the value to be put in DATATO for timeout.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] ms        timeout in milliseconds
 * @return              The DATATO value.
 */
__STATIC_FORCEINLINE uint32_t sdc_lld_get_timeout(SDCDriver *sdcp,
                                                  uint32_t ms) {
  uint32_t div = (SDIO_CLKCTL(sdcp->sdio) & SDIO_CLKCTL_DIV) + 1U;
  return (((sdcp->clkfreq / (div * 2U)) / 1000U) * ms);
}

/* GD32 needs CMDCTL_TREN set on the data command itself to tie the CSM to
   the DSM.*/
static bool sdc_lld_send_cmd_short_crc_ex(SDCDriver *sdcp, uint8_t cmd,
                                          uint32_t arg, uint32_t *resp,
                                          uint32_t cmdctl_extra);

/**
 * @brief   Prepares to handle read transaction.
 * @details Designed for read special registers from card.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[out] buf      pointer to the read buffer
 * @param[in] bytes     number of bytes to read
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_lld_prepare_read_bytes(SDCDriver *sdcp,
                                       uint8_t *buf, uint32_t bytes) {

  osalDbgCheck(bytes < 0x1000000U);

  SDIO_DATATO(sdcp->sdio) = sdc_lld_get_timeout(sdcp, GD32_SDC_SDIO_READ_TIMEOUT);

  /* Checks for errors and waits for the card to be ready for reading.*/
  if (_sdc_wait_for_transfer_state(sdcp)) {
    return HAL_FAILED;
  }

  /* Prepares IDMA.*/
  SDIO_IDMAADDR0(sdcp->sdio) = (uint32_t)buf;
  SDIO_IDMACTL(sdcp->sdio)   = SDIO_IDMACTL_IDMAEN;

  /* Setting up data transfer.*/
  SDIO_INTC(sdcp->sdio)   = SDIO_INTC_ALL_FLAGS;
  SDIO_INTEN(sdcp->sdio)  = SDIO_INTEN_DTCRCERRIE |
                            SDIO_INTEN_DTTMOUTIE |
                            SDIO_INTEN_RXOREIE |
                            SDIO_INTEN_DTENDIE;
  SDIO_DATALEN(sdcp->sdio) = bytes;

  /* Transfer modes, must be set before the data command below. FIFOREST and
     DATAEN are not used, see the note in sdc_lld_read_aligned().*/
  SDIO_DATACTL(sdcp->sdio) = SDIO_DATACTL_DATADIR |
                             SDIO_TRANSMODE_MULTIBYTE;

  return HAL_SUCCESS;
}

/**
 * @brief   Prepares card to handle read transaction.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[in] n         number of blocks to read
 * @param[in] resp      pointer to the response buffer
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_lld_prepare_read(SDCDriver *sdcp, uint32_t startblk,
                                 uint32_t n, uint32_t *resp) {

  /* Driver handles data in 512 bytes blocks (just like HC cards). But if we
     have not HC card than we must convert address from blocks to bytes.*/
  if (!(sdcp->cardmode & SDC_MODE_HIGH_CAPACITY))
    startblk *= MMCSD_BLOCK_SIZE;

  if (n > 1U) {
    /* Send read multiple blocks command to card.*/
    if (sdc_lld_send_cmd_short_crc_ex(sdcp, MMCSD_CMD_READ_MULTIPLE_BLOCK,
                                     startblk, resp, SDIO_CMDCTL_TREN) ||
        MMCSD_R1_ERROR(resp[0])) {
      return HAL_FAILED;
    }
  }
  else {
    /* Send read single block command.*/
    if (sdc_lld_send_cmd_short_crc_ex(sdcp, MMCSD_CMD_READ_SINGLE_BLOCK,
                                     startblk, resp, SDIO_CMDCTL_TREN) ||
        MMCSD_R1_ERROR(resp[0])) {
      return HAL_FAILED;
    }
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Prepares card to handle write transaction.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[in] n         number of blocks to write
 * @param[in] resp      pointer to the response buffer
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_lld_prepare_write(SDCDriver *sdcp, uint32_t startblk,
                                  uint32_t n, uint32_t *resp) {

  /* Driver handles data in 512 bytes blocks (just like HC cards). But if we
     have not HC card than we must convert address from blocks to bytes.*/
  if (!(sdcp->cardmode & SDC_MODE_HIGH_CAPACITY))
    startblk *= MMCSD_BLOCK_SIZE;

  if (n > 1U) {
    /* Write multiple blocks command.*/
    if (sdc_lld_send_cmd_short_crc_ex(sdcp, MMCSD_CMD_WRITE_MULTIPLE_BLOCK,
                                     startblk, resp, SDIO_CMDCTL_TREN) ||
        MMCSD_R1_ERROR(resp[0])) {
      return HAL_FAILED;
    }
  }
  else {
    /* Write single block command.*/
    if (sdc_lld_send_cmd_short_crc_ex(sdcp, MMCSD_CMD_WRITE_BLOCK,
                                     startblk, resp, SDIO_CMDCTL_TREN) ||
        MMCSD_R1_ERROR(resp[0])) {
      return HAL_FAILED;
    }
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Wait end of data transaction and performs finalizations.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] n         number of blocks in transaction
 * @param[in] resp      pointer to the response buffer
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 */
static bool sdc_lld_wait_transaction_end(SDCDriver *sdcp, uint32_t n,
                                         uint32_t *resp) {

  /* Note the mask is checked before going to sleep because the interrupt
     may have occurred before reaching the critical zone.*/
  osalSysLock();

  if (SDIO_INTEN(sdcp->sdio) != 0U) {
    /* Timed suspension, the DATATO counter cannot expire if the data state
       machine stops being clocked. On timeout DTEND is not set below and
       the transaction is reported as failed.*/
    if (osalThreadSuspendTimeoutS(&sdcp->thread,
                                  OSAL_MS2I(GD32_SDC_SDIO_DATA_TIMEOUT)) ==
        MSG_TIMEOUT) {
      sdcp->errors |= SDC_DATA_TIMEOUT;
    }
  }

  /* Stopping operations.*/
  SDIO_IDMACTL(sdcp->sdio) = 0U;
  SDIO_INTEN(sdcp->sdio)   = 0U;
  SDIO_DATACTL(sdcp->sdio) = 0U;

  if ((SDIO_STAT(sdcp->sdio) & SDIO_STAT_DTEND) == 0U) {
    osalSysUnlock();
    return HAL_FAILED;
  }

  /* Clearing status.*/
  SDIO_INTC(sdcp->sdio) = SDIO_INTC_ALL_FLAGS;

  osalSysUnlock();

  /* Finalize transaction.*/
  if (n > 1U) {
    return sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_STOP_TRANSMISSION,
                                      0, resp);
  }

  return HAL_SUCCESS;
}

/**
 * @brief   Gets SDC errors.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] sta       value of the STAT register
 *
 * @notapi
 */
static void sdc_lld_collect_errors(SDCDriver *sdcp, uint32_t sta) {
  uint32_t errors = SDC_NO_ERROR;

  if (sta & SDIO_STAT_CCRCERR)
    errors |= SDC_CMD_CRC_ERROR;
  if (sta & SDIO_STAT_DTCRCERR)
    errors |= SDC_DATA_CRC_ERROR;
  if (sta & SDIO_STAT_CMDTMOUT)
    errors |= SDC_COMMAND_TIMEOUT;
  if (sta & SDIO_STAT_DTTMOUT)
    errors |= SDC_DATA_TIMEOUT;
  if (sta & SDIO_STAT_TXURE)
    errors |= SDC_TX_UNDERRUN;
  if (sta & SDIO_STAT_RXORE)
    errors |= SDC_RX_OVERRUN;

  sdcp->errors |= errors;
}

/**
 * @brief   Performs clean transaction stopping in case of errors.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] n         number of blocks in transaction
 * @param[in] resp      pointer to the response buffer
 *
 * @notapi
 */
static void sdc_lld_error_cleanup(SDCDriver *sdcp,
                                  uint32_t n,
                                  uint32_t *resp) {
  uint32_t sta;

  /* Clearing status.*/
  sta                      = SDIO_STAT(sdcp->sdio);
  SDIO_INTC(sdcp->sdio)    = sta;
  SDIO_IDMACTL(sdcp->sdio) = 0U;

  /* Discards the leftover FIFO content, FIFOREST does not self-clear so it
     has to be released explicitly.*/
  SDIO_DATACTL(sdcp->sdio) = SDIO_DATACTL_FIFOREST;
  SDIO_DATACTL(sdcp->sdio) = 0U;

  sdc_lld_collect_errors(sdcp, sta);

  if (n > 1U) {
    sdc_lld_send_cmd_short_crc(sdcp, MMCSD_CMD_STOP_TRANSMISSION, 0, resp);
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SDC driver initialization.
 *
 * @notapi
 */
void sdc_lld_init(void) {

#if GD32_SDC_USE_SDIO0
  sdcObjectInit(&SDCD0);
  SDCD0.thread  = NULL;
  SDCD0.sdio    = SDIO0;
  SDCD0.clkfreq = GD32_SDIO0CLK;
  SDCD0.buf     = sd0_buf;
  SDCD0.resp    = sd0_wbuf;
#endif

#if GD32_SDC_USE_SDIO1
  sdcObjectInit(&SDCD1);
  SDCD1.thread  = NULL;
  SDCD1.sdio    = SDIO1;
  SDCD1.clkfreq = GD32_SDIO1CLK;
  SDCD1.buf     = sd1_buf;
  SDCD1.resp    = sd1_wbuf;
#endif
}

/**
 * @brief   Configures and activates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_start(SDCDriver *sdcp) {

  /* Checking configuration, using a default if NULL has been passed.*/
  if (sdcp->config == NULL) {
    sdcp->config = &sdc_default_cfg;
  }

  /* If in stopped state then clocks are enabled and DMA initialized.*/
  if (sdcp->state == BLK_STOP) {
#if GD32_SDC_USE_SDIO0
    if (&SDCD0 == sdcp) {
      rcuEnableSDIO0(true);
    }
#endif /* GD32_SDC_USE_SDIO0 */

#if GD32_SDC_USE_SDIO1
    if (&SDCD1 == sdcp) {
      rcuEnableSDIO1(true);
    }
#endif /* GD32_SDC_USE_SDIO1 */
  }

  /* Configuration, card clock is initially stopped.*/
  SDIO_IDMACTL(sdcp->sdio) = 0U;
  SDIO_DATACTL(sdcp->sdio) = 0U;
  SDIO_PWRCTL(sdcp->sdio)  = 0U;
  SDIO_CLKCTL(sdcp->sdio)  = 0U;
  SDIO_DATATO(sdcp->sdio)  = 0U;
  SDIO_INTC(sdcp->sdio)    = SDIO_INTC_ALL_FLAGS;
}

/**
 * @brief   Deactivates the SDC peripheral.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_stop(SDCDriver *sdcp) {

  if (sdcp->state != BLK_STOP) {

    /* SDIO deactivation.*/
    SDIO_IDMACTL(sdcp->sdio) = 0U;
    SDIO_DATACTL(sdcp->sdio) = 0U;
    SDIO_PWRCTL(sdcp->sdio)  = 0U;
    SDIO_CLKCTL(sdcp->sdio)  = 0U;
    SDIO_DATATO(sdcp->sdio)  = 0U;
    SDIO_INTC(sdcp->sdio)    = SDIO_INTC_ALL_FLAGS;

    /* Clock deactivation.*/
#if GD32_SDC_USE_SDIO0
    if (&SDCD0 == sdcp) {
      /* Reset un-wedges the data state machine without using the command
         state machine to send CMDSTOP.*/
      rcuResetSDIO0();
      rcuDisableSDIO0();
    }
#endif

#if GD32_SDC_USE_SDIO1
    if (&SDCD1 == sdcp) {
      /* Reset un-wedges the data state machine without using the command
         state machine to send CMDSTOP.*/
      rcuResetSDIO1();
      rcuDisableSDIO1();
    }
#endif
  }
}

/**
 * @brief   Starts the SDIO clock and sets it to init mode (400kHz or less).
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_start_clk(SDCDriver *sdcp) {

  /* Initial clock setting: 400kHz, 1bit mode.*/
  SDIO_CLKCTL(sdcp->sdio) = sdc_lld_clkdiv(sdcp, 400000U);
  SDIO_PWRCTL(sdcp->sdio) |= SDIO_POWER_ON;

  /* Clock activation delay.*/
  osalThreadSleep(OSAL_MS2I(GD32_SDC_SDIO_CLOCK_DELAY));
}

/**
 * @brief   Sets the SDIO clock to data mode (25/50 MHz or less).
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] clk       the clock mode
 *
 * @notapi
 */
void sdc_lld_set_data_clk(SDCDriver *sdcp, sdcbusclk_t clk) {

  if (SDC_CLK_50MHz == clk) {
    SDIO_CLKCTL(sdcp->sdio) = (SDIO_CLKCTL(sdcp->sdio) &
                               ~(SDIO_CLKCTL_CLKPWRSAV | SDIO_CLKCTL_DIV)) |
#if GD32_SDC_SDIO_PWRSAV
                              sdc_lld_clkdiv(sdcp, 50000000U) |
                              SDIO_CLOCKPWRSAVE_ENABLE;
#else
                              sdc_lld_clkdiv(sdcp, 50000000U);
#endif
  }
  else {
    SDIO_CLKCTL(sdcp->sdio) = (SDIO_CLKCTL(sdcp->sdio) &
                               ~(SDIO_CLKCTL_CLKPWRSAV | SDIO_CLKCTL_DIV)) |
#if GD32_SDC_SDIO_PWRSAV
                              sdc_lld_clkdiv(sdcp, 25000000U) |
                              SDIO_CLOCKPWRSAVE_ENABLE;
#else
                              sdc_lld_clkdiv(sdcp, 25000000U);
#endif
  }
}

/**
 * @brief   Stops the SDIO clock.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @notapi
 */
void sdc_lld_stop_clk(SDCDriver *sdcp) {

  SDIO_CLKCTL(sdcp->sdio) = 0U;
  SDIO_PWRCTL(sdcp->sdio) = 0U;
}

/**
 * @brief   Switches the bus to 1, 4 or 8 bits mode.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] mode      bus mode
 *
 * @notapi
 */
void sdc_lld_set_bus_mode(SDCDriver *sdcp, sdcbusmode_t mode) {
  uint32_t clk = SDIO_CLKCTL(sdcp->sdio) & ~SDIO_CLKCTL_BUSMODE;

  switch (mode) {
  case SDC_MODE_1BIT:
    SDIO_CLKCTL(sdcp->sdio) = clk | SDIO_BUSMODE_1BIT;
    break;
  case SDC_MODE_4BIT:
    SDIO_CLKCTL(sdcp->sdio) = clk | SDIO_BUSMODE_4BIT;
    break;
  case SDC_MODE_8BIT:
    SDIO_CLKCTL(sdcp->sdio) = clk | SDIO_BUSMODE_8BIT;
    break;
  }
}

/**
 * @brief   Sends an SDIO command with no response expected.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
 * @param[in] arg       command argument
 *
 * @notapi
 */
void sdc_lld_send_cmd_none(SDCDriver *sdcp, uint8_t cmd, uint32_t arg) {
  uint32_t timeout = 0U;

  SDIO_CMDAGMT(sdcp->sdio) = arg;
  SDIO_CMDCTL(sdcp->sdio)  = (uint32_t)cmd | SDIO_CMDCTL_CSMEN;
  while (((SDIO_STAT(sdcp->sdio) & SDIO_STAT_CMDSEND) == 0U) &&
         (timeout != GD32_SDC_SDIO_CMD_TIMEOUT)) {
    timeout++;
  }
  SDIO_INTC(sdcp->sdio) = SDIO_INTC_CMDSENDC;
}

/**
 * @brief   Sends an SDIO command with a short response expected.
 * @note    The CRC is not verified.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
 * @param[in] arg       command argument
 * @param[out] resp     pointer to the response buffer (one word)
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_send_cmd_short(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                            uint32_t *resp) {
  uint32_t sta;
  uint32_t timeout = 0U;

  SDIO_CMDAGMT(sdcp->sdio) = arg;
  SDIO_CMDCTL(sdcp->sdio)  = (uint32_t)cmd | SDIO_RESPONSETYPE_SHORT |
                            SDIO_CMDCTL_CSMEN;
  while (((sta = SDIO_STAT(sdcp->sdio)) &
          (SDIO_STAT_CMDRECV | SDIO_STAT_CMDTMOUT | SDIO_STAT_CCRCERR)) == 0U) {
    if (++timeout == GD32_SDC_SDIO_CMD_TIMEOUT) {
      sdc_lld_collect_errors(sdcp, SDIO_STAT_CMDTMOUT);
      return HAL_FAILED;
    }
  }
  SDIO_INTC(sdcp->sdio) = sta & (SDIO_STAT_CMDRECV | SDIO_STAT_CMDTMOUT |
                                 SDIO_STAT_CCRCERR);
  if ((sta & (SDIO_STAT_CMDTMOUT)) != 0U) {
    sdc_lld_collect_errors(sdcp, sta);
    return HAL_FAILED;
  }
  *resp = SDIO_RESP0(sdcp->sdio);
  return HAL_SUCCESS;
}

/**
 * @brief   Sends an SDIO command with a short response expected and CRC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
 * @param[in] arg       command argument
 * @param[out] resp     pointer to the response buffer (one word)
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_send_cmd_short_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                uint32_t *resp) {
  return sdc_lld_send_cmd_short_crc_ex(sdcp, cmd, arg, resp, 0U);
}

/**
 * @brief   Sends an SDIO command with a short response expected and CRC,
 *          with extra bits to be OR-ed into CMDCTL (e.g. CMDCTL_TREN).
 *
 * @notapi
 */
static bool sdc_lld_send_cmd_short_crc_ex(SDCDriver *sdcp, uint8_t cmd,
                                          uint32_t arg, uint32_t *resp,
                                          uint32_t cmdctl_extra) {
  uint32_t sta;
  uint32_t timeout = 0U;

  SDIO_CMDAGMT(sdcp->sdio) = arg;
  SDIO_CMDCTL(sdcp->sdio)  = (uint32_t)cmd | SDIO_RESPONSETYPE_SHORT |
                            SDIO_CMDCTL_CSMEN | cmdctl_extra;
  while (((sta = SDIO_STAT(sdcp->sdio)) &
          (SDIO_STAT_CMDRECV | SDIO_STAT_CMDTMOUT | SDIO_STAT_CCRCERR)) == 0U) {
    if (++timeout == GD32_SDC_SDIO_CMD_TIMEOUT) {
      sdc_lld_collect_errors(sdcp, SDIO_STAT_CMDTMOUT);
      return HAL_FAILED;
    }
  }
  SDIO_INTC(sdcp->sdio) = sta & (SDIO_STAT_CMDRECV | SDIO_STAT_CMDTMOUT |
                                 SDIO_STAT_CCRCERR);
  if ((sta & (SDIO_STAT_CMDTMOUT | SDIO_STAT_CCRCERR)) != 0U) {
    sdc_lld_collect_errors(sdcp, sta);
    return HAL_FAILED;
  }
  *resp = SDIO_RESP0(sdcp->sdio);
  return HAL_SUCCESS;
}

/**
 * @brief   Sends an SDIO command with a long response expected and CRC.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] cmd       card command
 * @param[in] arg       command argument
 * @param[out] resp     pointer to the response buffer (four words)
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_send_cmd_long_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                               uint32_t *resp) {
  uint32_t sta;
  uint32_t timeout = 0U;

  SDIO_CMDAGMT(sdcp->sdio) = arg;
  SDIO_CMDCTL(sdcp->sdio)  = (uint32_t)cmd | SDIO_RESPONSETYPE_LONG |
                            SDIO_CMDCTL_CSMEN;
  while (((sta = SDIO_STAT(sdcp->sdio)) &
          (SDIO_STAT_CMDRECV | SDIO_STAT_CMDTMOUT | SDIO_STAT_CCRCERR)) == 0U) {
    if (++timeout == GD32_SDC_SDIO_CMD_TIMEOUT) {
      sdc_lld_collect_errors(sdcp, SDIO_STAT_CMDTMOUT);
      return HAL_FAILED;
    }
  }
  SDIO_INTC(sdcp->sdio) = sta & (SDIO_STAT_CMDRECV | SDIO_STAT_CMDTMOUT |
                                 SDIO_STAT_CCRCERR);
  if ((sta & (SDIO_STAT_ERROR_MASK)) != 0U) {
    sdc_lld_collect_errors(sdcp, sta);
    return HAL_FAILED;
  }
  /* Save bytes in reverse order because MSB in response comes first.*/
  *resp++ = SDIO_RESP3(sdcp->sdio);
  *resp++ = SDIO_RESP2(sdcp->sdio);
  *resp++ = SDIO_RESP1(sdcp->sdio);
  *resp   = SDIO_RESP0(sdcp->sdio);
  return HAL_SUCCESS;
}

/**
 * @brief   Reads special registers using data bus.
 * @details Needs only during card detection procedure.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[out] buf      pointer to the read buffer
 * @param[in] bytes     number of bytes to read
 * @param[in] cmd       card command
 * @param[in] arg       argument for command
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_read_special(SDCDriver *sdcp, uint8_t *buf, size_t bytes,
                          uint8_t cmd, uint32_t arg) {

  if (!bouncebuffer_setup_read(sdcp->bouncebuffer, &buf, bytes)) {
      return HAL_FAILED;
  }

  if (sdc_lld_prepare_read_bytes(sdcp, buf, bytes))
    goto error;

  if (sdc_lld_send_cmd_short_crc_ex(sdcp, cmd, arg, sdcp->resp,
                                    SDIO_CMDCTL_TREN) ||
      MMCSD_R1_ERROR(sdcp->resp[0]))
    goto error;

  if (sdc_lld_wait_transaction_end(sdcp, 1, sdcp->resp))
    goto error;

  bouncebuffer_finish_read(sdcp->bouncebuffer, buf, bytes);

  return HAL_SUCCESS;

error:
  sdc_lld_error_cleanup(sdcp, 1, sdcp->resp);
  bouncebuffer_finish_read(sdcp->bouncebuffer, buf, bytes);
  return HAL_FAILED;
}

/**
 * @brief   Reads one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[out] buf      pointer to the read buffer
 * @param[in] blocks    number of blocks to read
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_lld_read_aligned(SDCDriver *sdcp, uint32_t startblk,
                                 uint8_t *buf, uint32_t blocks) {

  osalDbgCheck(blocks < 0x1000000 / MMCSD_BLOCK_SIZE);

  SDIO_DATATO(sdcp->sdio) = sdc_lld_get_timeout(sdcp, GD32_SDC_SDIO_READ_TIMEOUT);

  /* Checks for errors and waits for the card to be ready for reading.*/
  if (_sdc_wait_for_transfer_state(sdcp))
    return HAL_FAILED;

  if (!bouncebuffer_setup_read(sdcp->bouncebuffer, &buf, blocks * MMCSD_BLOCK_SIZE)) {
      return HAL_FAILED;
  }

  /* Prepares IDMA.*/
  SDIO_IDMAADDR0(sdcp->sdio) = (uint32_t)buf;
  SDIO_IDMACTL(sdcp->sdio)   = SDIO_IDMACTL_IDMAEN;

  /* Setting up data transfer.*/
  SDIO_INTC(sdcp->sdio)    = SDIO_INTC_ALL_FLAGS;
  SDIO_INTEN(sdcp->sdio)   = SDIO_INTEN_DTCRCERRIE |
                             SDIO_INTEN_DTTMOUTIE |
                             SDIO_INTEN_RXOREIE |
                             SDIO_INTEN_DTENDIE;
  SDIO_DATALEN(sdcp->sdio) = blocks * MMCSD_BLOCK_SIZE;

  /* Transfer modes, set before the data command because CMDCTL_TREN makes
     the CSM trigger the DSM. DATAEN is left out, it starts a transfer with
     no CSM involved (not meant for SD/eMMC cards) and would be a second
     trigger against TREN, causing IDMERR. FIFOREST is left out too, it does
     not self-clear and the FIFO would keep discarding data while the
     transfer still ends normally, silently returning a wrong buffer.*/
  SDIO_DATACTL(sdcp->sdio) = SDIO_DATACTL_DATADIR |
                             SDIO_DATABLOCKSIZE_512BYTES;

  if (sdc_lld_prepare_read(sdcp, startblk, blocks, sdcp->resp) == true)
    goto error;

  if (sdc_lld_wait_transaction_end(sdcp, blocks, sdcp->resp) == true)
    goto error;

  bouncebuffer_finish_read(sdcp->bouncebuffer, buf, blocks * MMCSD_BLOCK_SIZE);

  return HAL_SUCCESS;

error:
  sdc_lld_error_cleanup(sdcp, blocks, sdcp->resp);
  bouncebuffer_finish_read(sdcp->bouncebuffer, buf, blocks * MMCSD_BLOCK_SIZE);
  return HAL_FAILED;
}

/**
 * @brief   Writes one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to write
 * @param[out] buf      pointer to the write buffer
 * @param[in] n         number of blocks to write
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
static bool sdc_lld_write_aligned(SDCDriver *sdcp, uint32_t startblk,
                                  const uint8_t *buf, uint32_t blocks) {

  osalDbgCheck(blocks < 0x1000000 / MMCSD_BLOCK_SIZE);

  SDIO_DATATO(sdcp->sdio) = sdc_lld_get_timeout(sdcp, GD32_SDC_SDIO_WRITE_TIMEOUT);

  /* Checks for errors and waits for the card to be ready for writing.*/
  if (_sdc_wait_for_transfer_state(sdcp))
    return HAL_FAILED;

  if (!bouncebuffer_setup_write(sdcp->bouncebuffer, &buf, blocks * MMCSD_BLOCK_SIZE)) {
      return HAL_FAILED;
  }

  /* Prepares IDMA.*/
  SDIO_IDMAADDR0(sdcp->sdio) = (uint32_t)buf;
  SDIO_IDMACTL(sdcp->sdio)   = SDIO_IDMACTL_IDMAEN;

  /* Setting up data transfer.*/
  SDIO_INTC(sdcp->sdio)    = SDIO_INTC_ALL_FLAGS;
  SDIO_INTEN(sdcp->sdio)   = SDIO_INTEN_DTCRCERRIE |
                             SDIO_INTEN_DTTMOUTIE |
                             SDIO_INTEN_TXUREIE |
                             SDIO_INTEN_DTENDIE;
  SDIO_DATALEN(sdcp->sdio) = blocks * MMCSD_BLOCK_SIZE;

  /* Transfer modes, see the note in sdc_lld_read_aligned().*/
  SDIO_DATACTL(sdcp->sdio) = SDIO_DATABLOCKSIZE_512BYTES;

  if (sdc_lld_prepare_write(sdcp, startblk, blocks, sdcp->resp) == true)
    goto error;

  if (sdc_lld_wait_transaction_end(sdcp, blocks, sdcp->resp) == true)
    goto error;

  bouncebuffer_finish_write(sdcp->bouncebuffer, buf);

  return HAL_SUCCESS;

error:
  sdc_lld_error_cleanup(sdcp, blocks, sdcp->resp);
  bouncebuffer_finish_write(sdcp->bouncebuffer, buf);
  return HAL_FAILED;
}

/**
 * @brief   Reads one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to read
 * @param[out] buf      pointer to the read buffer
 * @param[in] blocks    number of blocks to read
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_read(SDCDriver *sdcp, uint32_t startblk,
                  uint8_t *buf, uint32_t blocks) {

#if GD32_SDC_SDIO_UNALIGNED_SUPPORT
  if (((unsigned)buf & 3U) != 0U) {
    uint32_t i;
    for (i = 0U; i < blocks; i++) {
      if (sdc_lld_read_aligned(sdcp, startblk, sdcp->buf, 1)) {
        return HAL_FAILED;
      }
      memcpy(buf, sdcp->buf, MMCSD_BLOCK_SIZE);
      buf += MMCSD_BLOCK_SIZE;
      startblk++;
    }
    return HAL_SUCCESS;
  }
#else /* !GD32_SDC_SDIO_UNALIGNED_SUPPORT */
  osalDbgAssert((((unsigned)buf & 3U) == 0U), "unaligned buffer");
#endif /* !GD32_SDC_SDIO_UNALIGNED_SUPPORT */
  return sdc_lld_read_aligned(sdcp, startblk, buf, blocks);
}

/**
 * @brief   Writes one or more blocks.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 * @param[in] startblk  first block to write
 * @param[out] buf      pointer to the write buffer
 * @param[in] blocks    number of blocks to write
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS operation succeeded.
 * @retval HAL_FAILED   operation failed.
 *
 * @notapi
 */
bool sdc_lld_write(SDCDriver *sdcp, uint32_t startblk,
                   const uint8_t *buf, uint32_t blocks) {

#if GD32_SDC_SDIO_UNALIGNED_SUPPORT
  if (((unsigned)buf & 3U) != 0U) {
    uint32_t i;
    for (i = 0U; i < blocks; i++) {
      memcpy(sdcp->buf, buf, MMCSD_BLOCK_SIZE);
      buf += MMCSD_BLOCK_SIZE;
      if (sdc_lld_write_aligned(sdcp, startblk, sdcp->buf, 1))
        return HAL_FAILED;
      startblk++;
    }
    return HAL_SUCCESS;
  }
#else /* !GD32_SDC_SDIO_UNALIGNED_SUPPORT */
  osalDbgAssert((((unsigned)buf & 3U) == 0U), "unaligned buffer");
#endif /* !GD32_SDC_SDIO_UNALIGNED_SUPPORT */
  return sdc_lld_write_aligned(sdcp, startblk, buf, blocks);
}

/**
 * @brief   Waits for card idle condition.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 *
 * @return              The operation status.
 * @retval HAL_SUCCESS  the operation succeeded.
 * @retval HAL_FAILED   the operation failed.
 *
 * @api
 */
bool sdc_lld_sync(SDCDriver *sdcp) {

  /* CHTODO: Implement.*/
  (void)sdcp;
  return HAL_SUCCESS;
}

/**
 * @brief   Shared service routine.
 *
 * @param[in] sdcp      pointer to the @p SDCDriver object
 */
void sdc_lld_serve_interrupt(SDCDriver *sdcp) {

  osalSysLockFromISR();

  /* Disables the source but the status flags are not reset because the
     read/write functions needs to check them.*/
  SDIO_INTEN(sdcp->sdio) = 0U;
  osalThreadResumeI(&sdcp->thread, MSG_OK);

  osalSysUnlockFromISR();
}

#endif /* HAL_USE_SDC */

/** @} */
