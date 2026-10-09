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
 * @file    DACv1/hal_dac_lld.h
 * @brief   GD32H73x_75x low level DAC driver header.
 * @note    This driver models the two output channels (OUT0/OUT1) of the
 *          GD32 DAC0 unit as two independent DACDriver instances. The two
 *          outputs share the DAC_CTL0 and DAC_MDCR registers and the
 *          peripheral clock gate, accesses to those are read-modify-write
 *          on the channel bit fields.
 * @note    @p DAC_ERR_UNDERFLOW is not reported by this driver, the DMA
 *          underrun source shares the TIMER5 vector and is left to the
 *          TIMER unit. Only the DMA errors are reported.
 *
 * @addtogroup DAC
 * @{
 */

#ifndef HAL_DAC_LLD_H
#define HAL_DAC_LLD_H

#if (HAL_USE_DAC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Maximum number of DAC channels per unit.
 * @note    GD32H73x_75x DAC0 provides two independent outputs.
 */
#define DAC_MAX_CHANNELS                    2U

/**
 * @name    DAC channel indexes
 * @{
 */
#define GD32_DAC_CHANNEL_OUT0               0U
#define GD32_DAC_CHANNEL_OUT1               1U
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   DAC0 OUT0 driver enable switch.
 * @details If set to @p TRUE the support for DAC0 OUT0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_DAC_USE_DAC0_OUT0) || defined(__DOXYGEN__)
#define GD32_DAC_USE_DAC0_OUT0              FALSE
#endif

/**
 * @brief   DAC0 OUT1 driver enable switch.
 * @details If set to @p TRUE the support for DAC0 OUT1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_DAC_USE_DAC0_OUT1) || defined(__DOXYGEN__)
#define GD32_DAC_USE_DAC0_OUT1              FALSE
#endif

/**
 * @brief   DAC0 OUT0 DMA stream identifier.
 */
#if !defined(GD32_DAC_DAC0_OUT0_DMA_STREAM) || defined(__DOXYGEN__)
#define GD32_DAC_DAC0_OUT0_DMA_STREAM       GD32_DMA_STREAM_ID_ANY
#endif

/**
 * @brief   DAC0 OUT1 DMA stream identifier.
 */
#if !defined(GD32_DAC_DAC0_OUT1_DMA_STREAM) || defined(__DOXYGEN__)
#define GD32_DAC_DAC0_OUT1_DMA_STREAM       GD32_DMA_STREAM_ID_ANY
#endif

/**
 * @brief   DAC0 DMA interrupt priority level setting.
 */
#if !defined(GD32_DAC_DAC0_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_DAC_DAC0_DMA_PRIORITY          2U
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (GD32_DAC_USE_DAC0_OUT0 || GD32_DAC_USE_DAC0_OUT1) && !GD32_HAS_DAC0
#error "DAC0 not present in the selected device"
#endif

#if (GD32_DAC_USE_DAC0_OUT0 || GD32_DAC_USE_DAC0_OUT1) && !GD32_HAS_DMA0
#error "DAC0 requires DMA support"
#endif

/* This driver uses the DMA helper, make sure the DMA unit is compiled in.*/
#define GD32_DMA_REQUIRED

#if !GD32_DAC_USE_DAC0_OUT0 && !GD32_DAC_USE_DAC0_OUT1
#error "DAC driver activated but no DAC peripheral assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a DAC channel index.
 */
typedef uint32_t dacchannel_t;

/**
 * @brief   Type representing a DAC sample.
 */
typedef uint16_t dacsample_t;

/**
 * @brief   Possible DAC failure causes.
 * @note    Error codes are architecture dependent and should not relied
 *          upon.
 */
typedef enum {
  DAC_ERR_DMAFAILURE = 0,                   /**< DMA operations failure.    */
  DAC_ERR_UNDERFLOW  = 1                    /**< DAC DMA underrun condition.*/
} dacerror_t;

/**
 * @brief   Data holding register alignment mode.
 */
typedef enum {
  DAC_DHRM_12BIT_RIGHT = 0,                 /**< 12-bit right aligned.      */
  DAC_DHRM_12BIT_LEFT  = 1,                 /**< 12-bit left aligned.       */
  DAC_DHRM_8BIT_RIGHT  = 2                  /**< 8-bit right aligned.       */
} dacdhrmode_t;

/**
 * @brief   GD32 DAC parameters structure.
 * @details Static per-unit parameters, resolved at compile time from the
 *          mcuconf.h settings. One structure per DAC unit.
 */
typedef struct {
  /**
   * @brief   Base address of the DAC registers block.
   */
  uint32_t                  dac_base;
  /**
   * @brief   DMA stream identifier.
   */
  uint32_t                  dma_stream;
  /**
   * @brief   DMAMUX request source.
   */
  uint32_t                  dma_req;
  /**
   * @brief   DMA interrupt priority.
   */
  uint32_t                  dma_irq_prio;
  /**
   * @brief   DAC channel index (0 = OUT0, 1 = OUT1).
   */
  dacchannel_t              channel;
} gd32_dac_params_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Low level fields of the DAC driver structure.
 */
#define dac_lld_driver_fields                                             \
  /* DAC parameters.*/                                                    \
  const gd32_dac_params_t *params;                                        \
  /* DMA stream, NULL when unused.*/                                      \
  const gd32_dma_stream_t *dma;

/**
 * @brief   Low level fields of the DAC configuration structure.
 */
#define dac_lld_config_fields                                             \
  /* Initial output value, written to both channels at start.*/           \
  uint32_t                  init;                                         \
  /* Data holding register alignment mode.*/                              \
  dacdhrmode_t              datamode;                                     \
  /* User CTL0 register bits (wave mode, bit width etc.).*/               \
  uint32_t                  ctl0;                                         \
  /* User MDCR register bits (buffer, sample & keep modes).*/             \
  uint32_t                  mdcr;

/**
 * @brief   Low level fields of the DAC group configuration structure.
 */
#define dac_lld_conversion_group_fields                                   \
  /* TRIGSEL input source for the external trigger, must not be zero.*/   \
  /* The software trigger does not generate DMA requests on this   */     \
  /* device, so DMA mode always requires a hardware trigger.*/            \
  uint32_t                  trigger;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (GD32_DAC_USE_DAC0_OUT0 == TRUE) && !defined(__DOXYGEN__)
extern DACDriver DACD0;
#endif

#if (GD32_DAC_USE_DAC0_OUT1 == TRUE) && !defined(__DOXYGEN__)
extern DACDriver DACD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dac_lld_init(void);
  void dac_lld_start(DACDriver *dacp);
  void dac_lld_stop(DACDriver *dacp);
  void dac_lld_put_channel(DACDriver *dacp,
                           dacchannel_t channel,
                           dacsample_t sample);
  void dac_lld_start_conversion(DACDriver *dacp);
  void dac_lld_stop_conversion(DACDriver *dacp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_DAC == TRUE */

#endif /* HAL_DAC_LLD_H */

/** @} */
