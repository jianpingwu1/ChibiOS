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
 * @file    DACv1/hal_dac_lld.c
 * @brief   GD32H73x_75x low level DAC driver source.
 * @note    Each physical output (OUT0/OUT1) is exposed as an independent
 *          DACDriver instance (DACD0/DACD1). The two outputs share the
 *          DAC_CTL0 and DAC_MDCR registers and the peripheral clock gate,
 *          so those accesses are read-modify-write on the channel bit
 *          fields (OUT1 fields start at bit 16).
 *
 * @addtogroup DAC
 * @{
 */

#include "hal.h"

#if (HAL_USE_DAC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/** @brief CTL0/MDCR bit shift for a channel, OUT1 fields start at bit 16.*/
#define GD32_DAC_REG_SHIFT(ch)        ((ch) * 16U)

/** @brief CTL0/MDCR bit mask of a channel fields.*/
#define GD32_DAC_REG_MASK(ch)         (0xFFFFU << GD32_DAC_REG_SHIFT(ch))

/** @brief User configuration bits mask.*/
#define GD32_DAC_CONFIG_MASK          0xFFFFU

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief DAC0 OUT0 driver identifier.*/
#if (GD32_DAC_USE_DAC0_OUT0 == TRUE) || defined(__DOXYGEN__)
DACDriver DACD0;
#endif

/** @brief DAC0 OUT1 driver identifier.*/
#if (GD32_DAC_USE_DAC0_OUT1 == TRUE) || defined(__DOXYGEN__)
DACDriver DACD1;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

#if (GD32_DAC_USE_DAC0_OUT0 == TRUE) || defined(__DOXYGEN__)
/** @brief DAC0 OUT0 parameters.*/
static const gd32_dac_params_t dac0_out0_params = {
  .dac_base       = DAC0,
  .dma_stream     = GD32_DAC_DAC0_OUT0_DMA_STREAM,
  .dma_req        = GD32_DMAMUX1_DAC_CH0,
  .dma_irq_prio   = GD32_DAC_DAC0_DMA_PRIORITY,
  .channel        = GD32_DAC_CHANNEL_OUT0
};
#endif

#if (GD32_DAC_USE_DAC0_OUT1 == TRUE) || defined(__DOXYGEN__)
/** @brief DAC0 OUT1 parameters.*/
static const gd32_dac_params_t dac0_out1_params = {
  .dac_base       = DAC0,
  .dma_stream     = GD32_DAC_DAC0_OUT1_DMA_STREAM,
  .dma_req        = GD32_DMAMUX1_DAC_CH1,
  .dma_irq_prio   = GD32_DAC_DAC0_DMA_PRIORITY,
  .channel        = GD32_DAC_CHANNEL_OUT1
};
#endif

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Selects the TRIGSEL trigger source for a DAC output.
 * @details Performs a read-modify-write of the trigger source field (low
 *          8 bits) when the target register is not locked. Uses the named
 *          TRIGSEL_DAC0OUTx registers from the device header, avoiding a
 *          link dependency on the peripheral library trigsel unit.
 *
 * @param[in] reg       pointer to the TRIGSEL target register
 * @param[in] source    TRIGSEL trigger source value
 */
static void gd32_dac_trigsel_source(volatile uint32_t *reg, uint32_t source) {

  /* Enabling the TRIGSEL clock, required for trigger source routing.*/
  RCU_APB2EN |= RCU_APB2EN_TRGSELEN;

  if ((*reg & TRIGSEL_TARGET_LK) == 0U) {
    *reg = (*reg & ~0xFFU) | (source & 0xFFU);
  }
}

/**
 * @brief   Writes a sample to the channel data holding register.
 * @details Selects the register based on the configured data mode and the
 *          channel owned by the driver instance.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] sample    value to be output
 */
static void gd32_dac_write_dhr(DACDriver *dacp, dacsample_t sample) {
  uint32_t base = dacp->params->dac_base;

  switch (dacp->config->datamode) {
  case DAC_DHRM_12BIT_RIGHT:
    if (dacp->params->channel == GD32_DAC_CHANNEL_OUT0) {
      DAC_OUT0_R12DH(base) = (uint32_t)sample;
    }
    else {
      DAC_OUT1_R12DH(base) = (uint32_t)sample;
    }
    break;
  case DAC_DHRM_12BIT_LEFT:
    if (dacp->params->channel == GD32_DAC_CHANNEL_OUT0) {
      DAC_OUT0_L12DH(base) = (uint32_t)sample;
    }
    else {
      DAC_OUT1_L12DH(base) = (uint32_t)sample;
    }
    break;
  case DAC_DHRM_8BIT_RIGHT:
    if (dacp->params->channel == GD32_DAC_CHANNEL_OUT0) {
      DAC_OUT0_R8DH(base) = (uint32_t)sample;
    }
    else {
      DAC_OUT1_R8DH(base) = (uint32_t)sample;
    }
    break;
  default:
    osalDbgAssert(false, "unexpected DAC mode");
    break;
  }
}

/**
 * @brief   Returns the address of the channel data holding register.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @return              Address of the DHR register for DMA.
 */
static volatile uint32_t *gd32_dac_dhr_addr(DACDriver *dacp) {
  uint32_t base = dacp->params->dac_base;
  bool is_out0 = (dacp->params->channel == GD32_DAC_CHANNEL_OUT0);

  switch (dacp->config->datamode) {
  case DAC_DHRM_12BIT_RIGHT:
    return is_out0 ? &DAC_OUT0_R12DH(base) : &DAC_OUT1_R12DH(base);
  case DAC_DHRM_12BIT_LEFT:
    return is_out0 ? &DAC_OUT0_L12DH(base) : &DAC_OUT1_L12DH(base);
  case DAC_DHRM_8BIT_RIGHT:
  default:
    return is_out0 ? &DAC_OUT0_R8DH(base) : &DAC_OUT1_R8DH(base);
  }
}

/**
 * @brief   Returns the DMA mode bits for the configured data mode.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @return              DMA CTL register mode value.
 */
static uint32_t gd32_dac_dma_mode(DACDriver *dacp) {
  uint32_t mode;

  /* Peripheral and memory widths follow the data alignment mode.*/
  switch (dacp->config->datamode) {
  case DAC_DHRM_8BIT_RIGHT:
    mode = GD32_DMA_CTL_PWIDTH_BYTE | GD32_DMA_CTL_MWIDTH_BYTE;
    break;
  case DAC_DHRM_12BIT_RIGHT:
  case DAC_DHRM_12BIT_LEFT:
  default:
    mode = GD32_DMA_CTL_PWIDTH_HWORD | GD32_DMA_CTL_MWIDTH_HWORD;
    break;
  }

  return mode | GD32_DMA_CTL_DIR_M2P | GD32_DMA_CTL_MNAGA |
         GD32_DMA_CTL_CMEN | GD32_DMA_CTL_PRIO(3) |
         GD32_DMA_CTL_HTIE | GD32_DMA_CTL_FTFIE |
         GD32_DMA_CTL_TAEIE | GD32_DMA_CTL_SDEIE;
}

/**
 * @brief   Shared end/half-of-tx service routine.
 *
 * @param[in] p         pointer to the @p DACDriver object
 * @param[in] flags     pre-shifted content of the INTF register
 */
static void dac_lld_serve_tx_interrupt(void *p, uint32_t flags) {
  DACDriver *dacp = (DACDriver *)p;

  /* The FIFO error flag is not an error condition here, the stream runs in
     direct mode and the flag can be set by the hardware anyway.*/
  if ((flags & (GD32_DMA_ISR_TAEIF | GD32_DMA_ISR_SDEIF)) != 0U) {
    /* DMA errors handling, the conversion is stopped by the macro.*/
    _dac_isr_error_code(dacp, DAC_ERR_DMAFAILURE);
  }
  else {
    if ((flags & GD32_DMA_ISR_HTIF) != 0U) {
      /* Half transfer processing.*/
      _dac_isr_half_code(dacp);
    }
    if ((flags & GD32_DMA_ISR_TCIF) != 0U) {
      /* Transfer complete processing.*/
      _dac_isr_full_code(dacp);
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level DAC driver initialization.
 *
 * @notapi
 */
void dac_lld_init(void) {

#if GD32_DAC_USE_DAC0_OUT0 == TRUE
  dacObjectInit(&DACD0);
  DACD0.params = &dac0_out0_params;
  DACD0.dma = NULL;
#endif

#if GD32_DAC_USE_DAC0_OUT1 == TRUE
  dacObjectInit(&DACD1);
  DACD1.params = &dac0_out1_params;
  DACD1.dma = NULL;
#endif
}

/**
 * @brief   Configures and activates the DAC peripheral.
 * @details Enables the RCU clock, applies the user CTL0/MDCR bits for the
 *          channel, enables the channel and writes its initial value.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_start(DACDriver *dacp) {
  uint32_t base = dacp->params->dac_base;
  uint32_t shift = GD32_DAC_REG_SHIFT(dacp->params->channel);
  uint32_t mask = GD32_DAC_REG_MASK(dacp->params->channel);
  uint32_t reg;

  if (dacp->state == DAC_STOP) {
    /* Enabling the clock source, it is shared by both outputs.*/
    rcuEnableDAC(false);

    /* Mode register, only the fields of this channel are touched.*/
    reg = DAC_MDCR(base) & ~mask;
    DAC_MDCR(base) = reg |
                     ((dacp->config->mdcr & GD32_DAC_CONFIG_MASK) << shift);

    /* Enabling and initializing this channel only.*/
    reg = DAC_CTL0(base) & ~mask;
    DAC_CTL0(base) = reg |
                     ((DAC_CTL0_DEN0 |
                       (dacp->config->ctl0 & GD32_DAC_CONFIG_MASK)) << shift);

    /* Write the initial output value to this channel.*/
    gd32_dac_write_dhr(dacp, (dacsample_t)dacp->config->init);
  }
}

/**
 * @brief   Deactivates the DAC peripheral.
 * @details Disables the channel and releases the RCU clock when the other
 *          output is not in use.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_stop(DACDriver *dacp) {
  uint32_t base = dacp->params->dac_base;
  uint32_t mask = GD32_DAC_REG_MASK(dacp->params->channel);

  if (dacp->state == DAC_READY) {
    /* Disabling this channel only.*/
    DAC_CTL0(base) &= ~mask;

    /* The clock gate is shared by both outputs, release it only when the
       other output is disabled too.*/
    if ((DAC_CTL0(base) & (DAC_CTL0_DEN0 | DAC_CTL0_DEN1)) == 0U) {
      rcuDisableDAC();
    }
  }
}

/**
 * @brief   Outputs a value directly on the DAC channel.
 * @details Writes the value to the channel data holding register. The
 *          channel must have been enabled by @p dac_lld_start().
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 * @param[in] channel   DAC channel index, must match the instance channel
 * @param[in] sample    value to be output
 *
 * @api
 */
void dac_lld_put_channel(DACDriver *dacp,
                         dacchannel_t channel,
                         dacsample_t sample) {

  osalDbgCheck(channel == dacp->params->channel);
  gd32_dac_write_dhr(dacp, sample);
}

/**
 * @brief   Starts a DAC conversion.
 * @details Starts a circular DMA transfer to the channel owned by the
 *          driver instance. The DMA runs in circular mode and generates
 *          half-transfer and transfer-complete callbacks.
 *
 * @note    DMA mode requires an external hardware trigger, the software
 *          trigger does not generate DMA requests on this device.
 * @note    In @p DAC_DHRM_8BIT_RIGHT mode each DMA operation transfers one
 *          byte, the buffer depth is counted in bytes.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @notapi
 */
void dac_lld_start_conversion(DACDriver *dacp) {
  uint32_t base = dacp->params->dac_base;
  uint32_t shift = GD32_DAC_REG_SHIFT(dacp->params->channel);
  uint32_t mask = GD32_DAC_REG_MASK(dacp->params->channel);
  uint32_t reg;
  size_t n;

  osalDbgAssert(dacp->grpp->num_channels == 1U,
                "invalid number of channels");
  osalDbgAssert(dacp->grpp->trigger != 0U,
                "DMA mode requires an external trigger");

  /* Number of DMA operations per buffer.*/
  n = dacp->depth * dacp->grpp->num_channels;

  /* Routing the external trigger source through TRIGSEL.*/
  if (dacp->params->channel == GD32_DAC_CHANNEL_OUT0) {
    gd32_dac_trigsel_source(&TRIGSEL_DAC0OUT0, dacp->grpp->trigger);
  }
  else {
    gd32_dac_trigsel_source(&TRIGSEL_DAC0OUT1, dacp->grpp->trigger);
  }

  /* Allocating the DMA stream.*/
  dacp->dma = dmaStreamAllocI(dacp->params->dma_stream,
                              dacp->params->dma_irq_prio,
                              dac_lld_serve_tx_interrupt,
                              (void *)dacp);
  osalDbgAssert(dacp->dma != NULL, "unable to allocate stream");

  dmaSetRequestSource(dacp->dma, dacp->params->dma_req);
  dmaStreamSetPeripheral(dacp->dma, gd32_dac_dhr_addr(dacp));
  dmaStreamSetMemory0(dacp->dma, dacp->samples);
  dmaStreamSetTransactionSize(dacp->dma, n);
  dmaStreamSetMode(dacp->dma, gd32_dac_dma_mode(dacp));
  dmaStreamEnable(dacp->dma);

  /* DAC configuration, only the fields of this channel are touched.*/
  reg = DAC_CTL0(base) & ~mask;
  DAC_CTL0(base) = reg |
                   ((DAC_CTL0_DEN0 | DAC_CTL0_DTEN0 | DAC_CTL0_DDMAEN0 |
                     DAC_TRIGGER_EXTERNAL |
                     (dacp->config->ctl0 & GD32_DAC_CONFIG_MASK)) << shift);
}

/**
 * @brief   Stops an ongoing conversion.
 * @details Disables and releases the DMA stream and restores the DAC
 *          channel to its start configuration.
 *
 * @param[in] dacp      pointer to the @p DACDriver object
 *
 * @iclass
 */
void dac_lld_stop_conversion(DACDriver *dacp) {
  uint32_t base = dacp->params->dac_base;
  uint32_t shift = GD32_DAC_REG_SHIFT(dacp->params->channel);

  /* Releasing the DMA stream, if allocated.*/
  if (dacp->dma != NULL) {
    dmaStreamDisable(dacp->dma);
    dmaStreamFreeI(dacp->dma);
    dacp->dma = NULL;
  }

  /* Clearing the DMA and trigger enable bits, the channel is left enabled
     so the last converted value is held on the output.*/
  DAC_CTL0(base) &= ~((DAC_CTL0_DDMAEN0 | DAC_CTL0_DTEN0) << shift);
}

#endif /* HAL_USE_DAC == TRUE */

/** @} */
