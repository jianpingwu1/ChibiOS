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
 * @file    SDMMCv2/hal_sdc_lld.h
 * @brief   GD32 SDC subsystem low level driver header.
 * @note    GD32 registers are accessed through flat REG32() macros taking
 *          the peripheral base address.
 *
 * @addtogroup SDC
 * @{
 */

#ifndef HAL_SDC_LLD_H
#define HAL_SDC_LLD_H

#if HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SDIO0 driver enable switch.
 * @details If set to @p TRUE the support for SDIO0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SDC_USE_SDIO0) || defined(__DOXYGEN__)
#define GD32_SDC_USE_SDIO0                  FALSE
#endif

/**
 * @brief   SDIO1 driver enable switch.
 * @details If set to @p TRUE the support for SDIO1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SDC_USE_SDIO1) || defined(__DOXYGEN__)
#define GD32_SDC_USE_SDIO1                  FALSE
#endif

/**
 * @brief   Support for unaligned transfers.
 * @note    Unaligned transfers are much slower.
 */
#if !defined(GD32_SDC_SDIO_UNALIGNED_SUPPORT) || defined(__DOXYGEN__)
#define GD32_SDC_SDIO_UNALIGNED_SUPPORT     TRUE
#endif

/**
 * @brief   Write timeout in milliseconds.
 */
#if !defined(GD32_SDC_SDIO_WRITE_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_SDC_SDIO_WRITE_TIMEOUT         10000
#endif

/**
 * @brief   Read timeout in milliseconds.
 */
#if !defined(GD32_SDC_SDIO_READ_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_SDC_SDIO_READ_TIMEOUT          10000
#endif

/**
 * @brief   Card clock activation delay in milliseconds.
 */
#if !defined(GD32_SDC_SDIO_CLOCK_DELAY) || defined(__DOXYGEN__)
#define GD32_SDC_SDIO_CLOCK_DELAY           10
#endif

/**
 * @brief   Card clock power saving enable.
 */
#if !defined(GD32_SDC_SDIO_PWRSAV) || defined(__DOXYGEN__)
#define GD32_SDC_SDIO_PWRSAV                TRUE
#endif

/**
 * @brief   Command/response flag-wait polling loop timeout, in polling
 *          iterations.
 * @note    Backstops the flag-wait loops in case of a hardware fault; in
 *          normal operation the CMDTMOUT hardware flag is raised well
 *          before this limit is reached.
 */
#if !defined(GD32_SDC_SDIO_CMD_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_SDC_SDIO_CMD_TIMEOUT           0x000FFFFFU
#endif

/**
 * @brief   Data transfer completion timeout in milliseconds.
 * @note    Backstops the DATATO hardware counter, which cannot expire if
 *          the data state machine stops being clocked. Must be longer than
 *          the read and write timeouts above.
 */
#if !defined(GD32_SDC_SDIO_DATA_TIMEOUT) || defined(__DOXYGEN__)
#define GD32_SDC_SDIO_DATA_TIMEOUT          15000
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks.*/
#if (GD32_SDC_USE_SDIO0 && !defined(GD32_SDIO0_HANDLER)) ||                 \
    (GD32_SDC_USE_SDIO1 && !defined(GD32_SDIO1_HANDLER))
#error "GD32_SDIOx_HANDLER not defined in registry"
#endif

#if (GD32_SDC_USE_SDIO0 && !defined(GD32_SDIO0_NUMBER)) ||                  \
    (GD32_SDC_USE_SDIO1 && !defined(GD32_SDIO1_NUMBER))
#error "GD32_SDIOx_NUMBER not defined in registry"
#endif

/* Units checks.*/
#if GD32_SDC_USE_SDIO0 && !GD32_HAS_SDIO0
#error "SDIO0 not present in the selected device"
#endif

#if GD32_SDC_USE_SDIO1 && !GD32_HAS_SDIO1
#error "SDIO1 not present in the selected device"
#endif

#if !GD32_SDC_USE_SDIO0 && !GD32_SDC_USE_SDIO1
#error "SDC driver activated but no SDIO peripheral assigned"
#endif

/* Clock related tests.*/
#if GD32_HAS_SDIO0 && !defined(GD32_SDIO0CLK)
#error "GD32_SDIO0CLK not defined"
#endif

#if GD32_HAS_SDIO1 && !defined(GD32_SDIO1CLK)
#error "GD32_SDIO1CLK not defined"
#endif

/* A stopped kernel clock makes the peripheral unreachable once enabled.*/
#if GD32_SDC_USE_SDIO0 && (GD32_SDIO0CLK == 0)
#error "SDIO0 kernel clock is stopped, check GD32_SDIO0SEL and the PLL output enables"
#endif

#if GD32_SDC_USE_SDIO1 && (GD32_SDIO1CLK == 0)
#error "SDIO1 kernel clock is stopped, check GD32_SDIO1SEL and the PLL output enables"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of card flags.
 */
typedef uint32_t sdcmode_t;

/**
 * @brief   SDC Driver condition flags type.
 */
typedef uint32_t sdcflags_t;

/**
 * @brief   Type of a structure representing an SDC driver.
 */
typedef struct SDCDriver SDCDriver;

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Bus width.
   */
  sdcbusmode_t  bus_width;
  /* End of the mandatory fields.*/
  /**
   * @brief   Bus slowdown.
   * @note    This values is added to the prescaler register in order to
   *          arbitrarily reduce clock speed.
   */
  uint32_t      slowdown;
} SDCConfig;

/**
 * @brief   @p SDCDriver specific methods.
 */
#define _sdc_driver_methods                                                 \
  _mmcsd_block_device_methods

/**
 * @extends MMCSDBlockDeviceVMT
 *
 * @brief   @p SDCDriver virtual methods table.
 */
struct SDCDriverVMT {
  _sdc_driver_methods
};

/**
 * @brief   Structure representing an SDC driver.
 */
struct SDCDriver {
  /**
   * @brief   Virtual Methods Table.
   */
  const struct SDCDriverVMT *vmt;
  _mmcsd_block_device_data
  /**
   * @brief   Current configuration data.
   */
  const SDCConfig           *config;
  /**
   * @brief   Various flags regarding the mounted card.
   */
  sdcmode_t                 cardmode;
  /**
   * @brief   Errors flags.
   */
  sdcflags_t                errors;
  /**
   * @brief   Card RCA.
   */
  uint32_t                  rca;
  /**
   * @brief   Buffer of @p MMCSD_BLOCK_SIZE bytes for internal operations.
   */
  uint8_t                   *buf;
  /* End of the mandatory fields.*/
  /**
   * @brief   Thread waiting for I/O completion IRQ.
   */
  thread_reference_t        thread;
  /**
   * @brief   Base address of the SDIO registers block.
   * @note    Needed for debugging aid.
   */
  uint32_t                  sdio;
  /**
   * @brief   Input clock frequency.
   */
  uint32_t                  clkfreq;
  /**
   * @brief   Word buffer for small transfers.
   */
  uint32_t                  *resp;
  /**
   * @brief   Bounce buffer used when the caller buffer is not reachable by
   *          the IDMA or not cache line aligned, owned by the application.
   */
  struct bouncebuffer_t     *bouncebuffer;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if GD32_SDC_USE_SDIO0 && !defined(__DOXYGEN__)
extern SDCDriver SDCD0;
#endif

#if GD32_SDC_USE_SDIO1 && !defined(__DOXYGEN__)
extern SDCDriver SDCD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sdc_lld_init(void);
  void sdc_lld_start(SDCDriver *sdcp);
  void sdc_lld_stop(SDCDriver *sdcp);
  void sdc_lld_start_clk(SDCDriver *sdcp);
  void sdc_lld_set_data_clk(SDCDriver *sdcp, sdcbusclk_t clk);
  void sdc_lld_stop_clk(SDCDriver *sdcp);
  void sdc_lld_set_bus_mode(SDCDriver *sdcp, sdcbusmode_t mode);
  void sdc_lld_send_cmd_none(SDCDriver *sdcp, uint8_t cmd, uint32_t arg);
  bool sdc_lld_send_cmd_short(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                              uint32_t *resp);
  bool sdc_lld_send_cmd_short_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                  uint32_t *resp);
  bool sdc_lld_send_cmd_long_crc(SDCDriver *sdcp, uint8_t cmd, uint32_t arg,
                                 uint32_t *resp);
  bool sdc_lld_read_special(SDCDriver *sdcp, uint8_t *buf, size_t bytes,
                            uint8_t cmd, uint32_t argument);
  bool sdc_lld_read(SDCDriver *sdcp, uint32_t startblk,
                    uint8_t *buf, uint32_t blocks);
  bool sdc_lld_write(SDCDriver *sdcp, uint32_t startblk,
                     const uint8_t *buf, uint32_t blocks);
  bool sdc_lld_sync(SDCDriver *sdcp);
  bool sdc_lld_is_card_inserted(SDCDriver *sdcp);
  bool sdc_lld_is_write_protected(SDCDriver *sdcp);
  void sdc_lld_serve_interrupt(SDCDriver *sdcp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SDC */

#endif /* HAL_SDC_LLD_H */

/** @} */
