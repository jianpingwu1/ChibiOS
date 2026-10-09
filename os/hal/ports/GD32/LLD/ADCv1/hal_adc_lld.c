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
 * @file    ADCv1/hal_adc_lld.c
 * @brief   GD32 ADC subsystem low level driver source.
 * @note    The interrupt handlers live in the shared LLD/ADC .inc files,
 *          see the GD32_ADCx_SUPPRESS_ISR contract in gd32_isr.h.
 *
 * @addtogroup ADC
 * @{
 */

#include "hal.h"

#if HAL_USE_ADC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#if GD32_ADC_SAMPLES_SIZE == 8
#define GD32_ADC_DMA_WIDTH  (GD32_DMA_CTL_PWIDTH_BYTE |                     \
                             GD32_DMA_CTL_MWIDTH_BYTE)
#elif GD32_ADC_SAMPLES_SIZE == 16
#define GD32_ADC_DMA_WIDTH  (GD32_DMA_CTL_PWIDTH_HWORD |                    \
                             GD32_DMA_CTL_MWIDTH_HWORD)
#else
#define GD32_ADC_DMA_WIDTH  (GD32_DMA_CTL_PWIDTH_WORD |                     \
                             GD32_DMA_CTL_MWIDTH_WORD)
#endif

#define GD32_ADC_DMA_MODE(prio)                                             \
  (GD32_DMA_CTL_DIR_P2M | GD32_DMA_CTL_MNAGA | GD32_ADC_DMA_WIDTH |         \
   GD32_DMA_CTL_PRIO(prio) | GD32_DMA_CTL_FTFIE | GD32_DMA_CTL_TAEIE |      \
   GD32_DMA_CTL_SDEIE)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if GD32_ADC_USE_ADC0 || defined(__DOXYGEN__)
/** @brief ADC0 driver identifier.*/
ADCDriver ADCD0;
#endif

#if GD32_ADC_USE_ADC1 || defined(__DOXYGEN__)
/** @brief ADC1 driver identifier.*/
ADCDriver ADCD1;
#endif

#if GD32_ADC_USE_ADC2 || defined(__DOXYGEN__)
/** @brief ADC2 driver identifier.*/
ADCDriver ADCD2;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Configuration used when a NULL one is passed to @p adcStart().
 */
static const ADCConfig default_config = {
  ADC_CALIBRATION_OFFSET_MISMATCH,
  ADC_CALIBRATION_NUM8
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Waits for a self clearing CTL1 bit with a bounded timeout.
 *
 * @param[in] adc       base address of the ADC registers block
 * @param[in] mask      mask of the bit to wait for
 * @return              The operation status.
 */
static bool adc_lld_wait_ctl1_clear(uint32_t adc, uint32_t mask) {
  uint32_t timeout = GD32_ADC_TIMEOUT_LOOPS;

  while ((ADC_CTL1(adc) & mask) != 0U) {
    if (--timeout == 0U) {
      return false;
    }
  }
  return true;
}

/**
 * @brief   Stops an ongoing conversion, if any.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
static void adc_lld_stop_adc(ADCDriver *adcp) {

  /* Interrupts are masked first, a few conversions can still complete
     before the continuous mode is actually dropped.*/
  ADC_CTL0(adcp->adc) &= ~GD32_ADC_CTL0_RESERVED_MASK;
  ADC_CTL1(adcp->adc) &= ~(ADC_CTL1_CTN | ADC_CTL1_DMA | ADC_CTL1_DDM);
  ADC_STAT(adcp->adc) = 0U;
}

/**
 * @brief   Releases the peripheral clock and the DMA stream.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 */
static void adc_lld_release(ADCDriver *adcp) {

  if (adcp->dma != NULL) {
    dmaStreamDisable(adcp->dma);
    dmaStreamFreeI(adcp->dma);
    adcp->dma = NULL;
  }

  adc_lld_stop_adc(adcp);
  ADC_CTL1(adcp->adc) &= ~ADC_CTL1_ADCON;

#if GD32_ADC_USE_ADC0
  if (&ADCD0 == adcp) {
    rcuDisableADC0();
  }
#endif
#if GD32_ADC_USE_ADC1
  if (&ADCD1 == adcp) {
    rcuDisableADC1();
  }
#endif
#if GD32_ADC_USE_ADC2
  if (&ADCD2 == adcp) {
    rcuDisableADC2();
  }
#endif
}

/**
 * @brief   ADC DMA service routine.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @param[in] flags     pre-shifted content of the INTF register
 */
static void adc_lld_serve_dma_interrupt(ADCDriver *adcp, uint32_t flags) {

  /* The conversion group may have already been reset by the ADC error
     handler, in that case this interrupt is spurious.*/
  if (adcp->grpp == NULL) {
    return;
  }

  if ((flags & (GD32_DMA_ISR_TAEIF | GD32_DMA_ISR_SDEIF)) != 0U) {
    /* This can only happen if the DMA accesses an unmapped address space
       or violates the alignment rules.*/
    _adc_isr_error_code(adcp, ADC_ERR_DMAFAILURE);
  }
  else if ((flags & GD32_DMA_ISR_TCIF) != 0U) {
    _adc_isr_full_code(adcp);
  }
  else if ((flags & GD32_DMA_ISR_HTIF) != 0U) {
    _adc_isr_half_code(adcp);
  }
  else {
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level ADC driver initialization.
 *
 * @notapi
 */
void adc_lld_init(void) {

#if GD32_ADC_USE_ADC0
  adcObjectInit(&ADCD0);
  ADCD0.adc        = ADC0;
  ADCD0.trigsel    = &TRIGSEL_ADC0;
  ADCD0.dma        = NULL;
  ADCD0.dmamode    = GD32_ADC_DMA_MODE(GD32_ADC_ADC0_DMA_PRIORITY);
  ADCD0.ckmode     = GD32_ADC_CKMODE_BITS(GD32_ADC_ADC0_CLOCK_MODE);
  ADCD0.stabcycles = GD32_ADC_STAB_CYCLES(GD32_ADC0_CLOCK);
#endif

#if GD32_ADC_USE_ADC1
  adcObjectInit(&ADCD1);
  ADCD1.adc        = ADC1;
  ADCD1.trigsel    = &TRIGSEL_ADC1;
  ADCD1.dma        = NULL;
  ADCD1.dmamode    = GD32_ADC_DMA_MODE(GD32_ADC_ADC1_DMA_PRIORITY);
  ADCD1.ckmode     = GD32_ADC_CKMODE_BITS(GD32_ADC_ADC1_CLOCK_MODE);
  ADCD1.stabcycles = GD32_ADC_STAB_CYCLES(GD32_ADC1_CLOCK);
#endif

#if GD32_ADC_USE_ADC2
  adcObjectInit(&ADCD2);
  ADCD2.adc        = ADC2;
  ADCD2.trigsel    = &TRIGSEL_ADC2;
  ADCD2.dma        = NULL;
  ADCD2.dmamode    = GD32_ADC_DMA_MODE(GD32_ADC_ADC2_DMA_PRIORITY);
  ADCD2.ckmode     = GD32_ADC_CKMODE_BITS(GD32_ADC_ADC2_CLOCK_MODE);
  ADCD2.stabcycles = GD32_ADC_STAB_CYCLES(GD32_ADC2_CLOCK);
#endif
}

/**
 * @brief   Configures and activates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 * @return              The operation status.
 *
 * @notapi
 */
msg_t adc_lld_start(ADCDriver *adcp) {

  if (adcp->state == ADC_STOP) {

    if (adcp->config == NULL) {
      adcp->config = &default_config;
    }

    if (false) {
    }
#if GD32_ADC_USE_ADC0
    else if (&ADCD0 == adcp) {
      adcp->dma = dmaStreamAllocI(GD32_ADC_ADC0_DMA_STREAM,
                                  GD32_IRQ_ADC0_1_PRIORITY,
                                  (gd32_dmaisr_t)adc_lld_serve_dma_interrupt,
                                  (void *)adcp);
      if (adcp->dma == NULL) {
        return HAL_RET_NO_RESOURCE;
      }
      rcuEnableADC0(true);
      dmaSetRequestSource(adcp->dma, GD32_DMAMUX1_ADC0);
    }
#endif
#if GD32_ADC_USE_ADC1
    else if (&ADCD1 == adcp) {
      adcp->dma = dmaStreamAllocI(GD32_ADC_ADC1_DMA_STREAM,
                                  GD32_IRQ_ADC0_1_PRIORITY,
                                  (gd32_dmaisr_t)adc_lld_serve_dma_interrupt,
                                  (void *)adcp);
      if (adcp->dma == NULL) {
        return HAL_RET_NO_RESOURCE;
      }
      rcuEnableADC1(true);
      dmaSetRequestSource(adcp->dma, GD32_DMAMUX1_ADC1);
    }
#endif
#if GD32_ADC_USE_ADC2
    else if (&ADCD2 == adcp) {
      adcp->dma = dmaStreamAllocI(GD32_ADC_ADC2_DMA_STREAM,
                                  GD32_IRQ_ADC2_PRIORITY,
                                  (gd32_dmaisr_t)adc_lld_serve_dma_interrupt,
                                  (void *)adcp);
      if (adcp->dma == NULL) {
        return HAL_RET_NO_RESOURCE;
      }
      rcuEnableADC2(true);
      dmaSetRequestSource(adcp->dma, GD32_DMAMUX1_ADC2);
    }
#endif
    else {
      osalDbgAssert(false, "invalid ADC instance");
      return HAL_RET_CONFIG_ERROR;
    }

    dmaStreamSetPeripheral(adcp->dma, &ADC_RDATA(adcp->adc));

    /* Clock prescaler, it must be programmed before powering up the analog
       part.*/
    ADC_SYNCCTL(adcp->adc) = (ADC_SYNCCTL(adcp->adc) &
                              ~(ADC_SYNCCTL_ADCSCK | ADC_SYNCCTL_ADCCK)) |
                             adcp->ckmode;

    /* Analog part power up, 14 CK_ADC cycles are needed before the cell is
       usable.*/
    ADC_CTL1(adcp->adc) |= ADC_CTL1_ADCON;
    osalSysPolledDelayX(adcp->stabcycles);

    /* Calibration, the vendor sequence is a calibration registers reset
       followed by the calibration itself.*/
    ADC_CTL1(adcp->adc) = (ADC_CTL1(adcp->adc) &
                           ~(ADC_CTL1_CALMOD | ADC_CTL1_CALNUM)) |
                          (adcp->config->calibration & ADC_CTL1_CALMOD) |
                          (adcp->config->calnum & ADC_CTL1_CALNUM);

    ADC_CTL1(adcp->adc) |= ADC_CTL1_RSTCLB;
    if (!adc_lld_wait_ctl1_clear(adcp->adc, ADC_CTL1_RSTCLB)) {
      adc_lld_release(adcp);
      return HAL_RET_HW_FAILURE;
    }

    ADC_CTL1(adcp->adc) |= ADC_CTL1_CLB;
    if (!adc_lld_wait_ctl1_clear(adcp->adc, ADC_CTL1_CLB)) {
      adc_lld_release(adcp);
      return HAL_RET_HW_FAILURE;
    }
  }

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Deactivates the ADC peripheral.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop(ADCDriver *adcp) {

  if (adcp->state == ADC_READY) {
    adc_lld_release(adcp);
  }
}

/**
 * @brief   Starts an ADC conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_start_conversion(ADCDriver *adcp) {
  const ADCConversionGroup *grpp = adcp->grpp;
  uint32_t mode, ctl0, ctl1;

  /* DMA setup.*/
  mode = adcp->dmamode;
  if (grpp->circular) {
    mode |= GD32_DMA_CTL_CMEN;
    if (adcp->depth > 1U) {
      /* The half transfer interrupt allows streaming processing.*/
      mode |= GD32_DMA_CTL_HTIE;
    }
  }
  dmaStreamSetMemory0(adcp->dma, adcp->samples);
  dmaStreamSetTransactionSize(adcp->dma,
                              (uint32_t)grpp->num_channels *
                              (uint32_t)adcp->depth);
  dmaStreamSetMode(adcp->dma, mode);
  dmaStreamEnable(adcp->dma);

  /* Routine sequence, oversampling and differential inputs.*/
  ADC_RSQ0(adcp->adc) = grpp->rsq[0];
  ADC_RSQ1(adcp->adc) = grpp->rsq[1];
  ADC_RSQ2(adcp->adc) = grpp->rsq[2];
  ADC_RSQ3(adcp->adc) = grpp->rsq[3];
  ADC_RSQ4(adcp->adc) = grpp->rsq[4];
  ADC_RSQ5(adcp->adc) = grpp->rsq[5];
  ADC_RSQ6(adcp->adc) = grpp->rsq[6];
  ADC_RSQ7(adcp->adc) = grpp->rsq[7];
  ADC_RSQ8(adcp->adc) = grpp->rsq[8];
  ADC_OVSAMPCTL(adcp->adc) = grpp->ovsampctl;
  ADC_DIFCTL(adcp->adc) = grpp->difctl;

  /* Analog watchdogs.*/
  ADC_WDHT0(adcp->adc) = grpp->wdht0;
  ADC_WDLT0(adcp->adc) = grpp->wdlt0;
  ADC_WD1SR(adcp->adc) = grpp->wd1sr;
  ADC_WDHT1(adcp->adc) = grpp->wdht1;
  ADC_WDLT1(adcp->adc) = grpp->wdlt1;
  ADC_WD2SR(adcp->adc) = grpp->wd2sr;
  ADC_WDHT2(adcp->adc) = grpp->wdht2;
  ADC_WDLT2(adcp->adc) = grpp->wdlt2;

  /* Routine trigger routing, the inserted sequence is not used by this
     driver so only INSEL0 is touched.*/
  if (grpp->trigsel != GD32_ADC_TRIGSEL_NONE) {
    *adcp->trigsel = (*adcp->trigsel & ~TRIGSEL_TARGET_INSEL0) |
                     (grpp->trigsel & TRIGSEL_TARGET_INSEL0);
  }

  /* Conversions are tracked through the DMA, only the error conditions are
     reported by the ADC interrupt.*/
  ctl0 = (grpp->ctl0 & ~GD32_ADC_CTL0_RESERVED_MASK) | ADC_CTL0_ROVFIE;
  if (grpp->error_cb != NULL) {
    ctl0 |= ADC_CTL0_WDE0IE | ADC_CTL0_WDE1IE | ADC_CTL0_WDE2IE;
  }
  ADC_STAT(adcp->adc) = 0U;
  ADC_CTL0(adcp->adc) = ctl0;

  ctl1 = (grpp->ctl1 & ~GD32_ADC_CTL1_RESERVED_MASK) |
         (ADC_CTL1(adcp->adc) & GD32_ADC_CTL1_INTCH_MASK) |
         ADC_CTL1_ADCON | ADC_CTL1_DMA;
  if (grpp->circular) {
    /* Keeps the DMA requests going after the end of transfer.*/
    ctl1 |= ADC_CTL1_DDM;
  }
  ADC_CTL1(adcp->adc) = ctl1;

  /* A software trigger is issued when no external trigger edge has been
     selected.*/
  if ((ctl1 & ADC_CTL1_ETMRC) == 0U) {
    ADC_CTL1(adcp->adc) = ctl1 | ADC_CTL1_SWRCST;
  }
}

/**
 * @brief   Stops an ongoing conversion.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_stop_conversion(ADCDriver *adcp) {

  dmaStreamDisable(adcp->dma);
  adc_lld_stop_adc(adcp);
}

/**
 * @brief   ADC IRQ service routine.
 *
 * @param[in] adcp      pointer to the @p ADCDriver object
 *
 * @notapi
 */
void adc_lld_serve_interrupt(ADCDriver *adcp) {
  uint32_t stat;

  stat = ADC_STAT(adcp->adc);
  ADC_STAT(adcp->adc) = ~stat;

  /* Overflows raised after the DMA has been disabled produce spurious
     interrupts, they are ignored here.*/
  if (adcp->grpp != NULL) {
    adcerror_t emask = 0U;

    if (((stat & ADC_STAT_ROVF) != 0U) && (adcp->state == ADC_ACTIVE)) {
      emask |= ADC_ERR_OVERFLOW;
    }
    if ((stat & ADC_STAT_WDE0) != 0U) {
      emask |= ADC_ERR_AWD0;
    }
    if ((stat & ADC_STAT_WDE1) != 0U) {
      emask |= ADC_ERR_AWD1;
    }
    if ((stat & ADC_STAT_WDE2) != 0U) {
      emask |= ADC_ERR_AWD2;
    }
    if (emask != 0U) {
      _adc_isr_error_code(adcp, emask);
    }
  }
}

#if GD32_ADC_USE_ADC2 || defined(__DOXYGEN__)
/**
 * @brief   Enables the temperature sensor channel.
 * @note    The internal channels are controlled through ADC2 only, so ADC2
 *          must be started before calling this function.
 *
 * @api
 */
void gd32_adcEnableTS(void) {

  ADC_CTL1(ADC2) |= ADC_CTL1_TSVEN1;
}

/**
 * @brief   Disables the temperature sensor channel.
 *
 * @api
 */
void gd32_adcDisableTS(void) {

  ADC_CTL1(ADC2) &= ~ADC_CTL1_TSVEN1;
}

/**
 * @brief   Enables the high precision temperature sensor channel.
 *
 * @api
 */
void gd32_adcEnableHPTS(void) {

  ADC_CTL1(ADC2) |= ADC_CTL1_TSVEN2;
}

/**
 * @brief   Disables the high precision temperature sensor channel.
 *
 * @api
 */
void gd32_adcDisableHPTS(void) {

  ADC_CTL1(ADC2) &= ~ADC_CTL1_TSVEN2;
}

/**
 * @brief   Enables the internal reference channel.
 *
 * @api
 */
void gd32_adcEnableVREF(void) {

  ADC_CTL1(ADC2) |= ADC_CTL1_INREFEN;
}

/**
 * @brief   Disables the internal reference channel.
 *
 * @api
 */
void gd32_adcDisableVREF(void) {

  ADC_CTL1(ADC2) &= ~ADC_CTL1_INREFEN;
}

/**
 * @brief   Enables the VBAT channel.
 *
 * @api
 */
void gd32_adcEnableVBAT(void) {

  ADC_CTL1(ADC2) |= ADC_CTL1_VBATEN;
}

/**
 * @brief   Disables the VBAT channel.
 *
 * @api
 */
void gd32_adcDisableVBAT(void) {

  ADC_CTL1(ADC2) &= ~ADC_CTL1_VBATEN;
}
#endif /* GD32_ADC_USE_ADC2 */

#endif /* HAL_USE_ADC */

/** @} */
