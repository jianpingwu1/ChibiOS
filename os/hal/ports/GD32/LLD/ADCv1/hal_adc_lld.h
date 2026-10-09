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
 * @file    ADCv1/hal_adc_lld.h
 * @brief   GD32 ADC subsystem low level driver header.
 * @note    ADC0, ADC1 and ADC2 are exposed as three independent drivers,
 *          the ADC_SYNCCTL pairing of ADC0 and ADC1 is not used.
 *
 * @addtogroup ADC
 * @{
 */

#ifndef HAL_ADC_LLD_H
#define HAL_ADC_LLD_H

#if HAL_USE_ADC || defined(__DOXYGEN__)

#include "gd32_adc.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define ADC_LLD_ENHANCED_API

/**
 * @name    Possible ADC errors mask bits.
 * @{
 */
#define ADC_ERR_DMAFAILURE      1U  /**< DMA operations failure.            */
#define ADC_ERR_OVERFLOW        2U  /**< ADC overflow condition.            */
#define ADC_ERR_AWD0            4U  /**< Watchdog 0 triggered.              */
#define ADC_ERR_AWD1            8U  /**< Watchdog 1 triggered.              */
#define ADC_ERR_AWD2            16U /**< Watchdog 2 triggered.              */
/** @} */

/**
 * @name    Available analog channels
 * @{
 */
#define ADC_CHANNEL_IN0         0U  /**< @brief External analog input 0.    */
#define ADC_CHANNEL_IN1         1U  /**< @brief External analog input 1.    */
#define ADC_CHANNEL_IN2         2U  /**< @brief External analog input 2.    */
#define ADC_CHANNEL_IN3         3U  /**< @brief External analog input 3.    */
#define ADC_CHANNEL_IN4         4U  /**< @brief External analog input 4.    */
#define ADC_CHANNEL_IN5         5U  /**< @brief External analog input 5.    */
#define ADC_CHANNEL_IN6         6U  /**< @brief External analog input 6.    */
#define ADC_CHANNEL_IN7         7U  /**< @brief External analog input 7.    */
#define ADC_CHANNEL_IN8         8U  /**< @brief External analog input 8.    */
#define ADC_CHANNEL_IN9         9U  /**< @brief External analog input 9.    */
#define ADC_CHANNEL_IN10        10U /**< @brief External analog input 10.   */
#define ADC_CHANNEL_IN11        11U /**< @brief External analog input 11.   */
#define ADC_CHANNEL_IN12        12U /**< @brief External analog input 12.   */
#define ADC_CHANNEL_IN13        13U /**< @brief External analog input 13.   */
#define ADC_CHANNEL_IN14        14U /**< @brief External analog input 14.   */
#define ADC_CHANNEL_IN15        15U /**< @brief External analog input 15.   */
#define ADC_CHANNEL_IN16        16U /**< @brief External analog input 16.   */
#define ADC_CHANNEL_IN17        17U /**< @brief External analog input 17.   */
#define ADC_CHANNEL_IN18        18U /**< @brief External analog input 18.   */
#define ADC_CHANNEL_IN19        19U /**< @brief External analog input 19.   */
#define ADC_CHANNEL_IN20        20U /**< @brief External analog input 20.   */
/** @} */

/**
 * @brief   No TRIGSEL programming is performed for the conversion group.
 */
#define GD32_ADC_TRIGSEL_NONE   0xFFFFFFFFU

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   Specifies the ADC samples width.
 * @note    Must be 8, 16 or 32.
 * @note    The 10, 12 and 14 bits resolutions must not be used when this
 *          option is set to 8.
 * @note    32 is useful when oversampling is activated without shift.
 */
#if !defined(GD32_ADC_SAMPLES_SIZE) || defined(__DOXYGEN__)
#define GD32_ADC_SAMPLES_SIZE               16
#endif

/**
 * @brief   Number of polling iterations before a register handshake is
 *          declared failed.
 */
#if !defined(GD32_ADC_TIMEOUT_LOOPS) || defined(__DOXYGEN__)
#define GD32_ADC_TIMEOUT_LOOPS              1000000
#endif

/**
 * @brief   ADC0 driver enable switch.
 */
#if !defined(GD32_ADC_USE_ADC0) || defined(__DOXYGEN__)
#define GD32_ADC_USE_ADC0                   FALSE
#endif

/**
 * @brief   ADC1 driver enable switch.
 */
#if !defined(GD32_ADC_USE_ADC1) || defined(__DOXYGEN__)
#define GD32_ADC_USE_ADC1                   FALSE
#endif

/**
 * @brief   ADC2 driver enable switch.
 */
#if !defined(GD32_ADC_USE_ADC2) || defined(__DOXYGEN__)
#define GD32_ADC_USE_ADC2                   FALSE
#endif

/**
 * @brief   ADC0 DMA priority (0..3|lowest..highest).
 */
#if !defined(GD32_ADC_ADC0_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ADC_ADC0_DMA_PRIORITY          2
#endif

/**
 * @brief   ADC1 DMA priority (0..3|lowest..highest).
 */
#if !defined(GD32_ADC_ADC1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ADC_ADC1_DMA_PRIORITY          2
#endif

/**
 * @brief   ADC2 DMA priority (0..3|lowest..highest).
 */
#if !defined(GD32_ADC_ADC2_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_ADC_ADC2_DMA_PRIORITY          2
#endif

/**
 * @brief   ADC0 DMA stream selector.
 * @note    Defaults to @p GD32_DMA_STREAM_ID_ANY, boards generating a static
 *          DMA allocation (ArduPilot's hwdef.h) override it.
 */
#if !defined(GD32_ADC_ADC0_DMA_STREAM) || defined(__DOXYGEN__)
#define GD32_ADC_ADC0_DMA_STREAM            GD32_DMA_STREAM_ID_ANY
#endif

/**
 * @brief   ADC1 DMA stream selector.
 */
#if !defined(GD32_ADC_ADC1_DMA_STREAM) || defined(__DOXYGEN__)
#define GD32_ADC_ADC1_DMA_STREAM            GD32_DMA_STREAM_ID_ANY
#endif

/**
 * @brief   ADC2 DMA stream selector.
 */
#if !defined(GD32_ADC_ADC2_DMA_STREAM) || defined(__DOXYGEN__)
#define GD32_ADC_ADC2_DMA_STREAM            GD32_DMA_STREAM_ID_ANY
#endif

/**
 * @brief   ADC0 clock mode selector.
 * @note    Must be a bare selector token, see @p gd32_adc.h.
 */
#if !defined(GD32_ADC_ADC0_CLOCK_MODE) || defined(__DOXYGEN__)
#define GD32_ADC_ADC0_CLOCK_MODE            ASYNC_DIV4
#endif

/**
 * @brief   ADC1 clock mode selector.
 */
#if !defined(GD32_ADC_ADC1_CLOCK_MODE) || defined(__DOXYGEN__)
#define GD32_ADC_ADC1_CLOCK_MODE            ASYNC_DIV4
#endif

/**
 * @brief   ADC2 clock mode selector.
 */
#if !defined(GD32_ADC_ADC2_CLOCK_MODE) || defined(__DOXYGEN__)
#define GD32_ADC_ADC2_CLOCK_MODE            ASYNC_DIV4
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks.*/
#if !defined(GD32_HAS_ADC0) || !defined(GD32_HAS_ADC1) ||                   \
    !defined(GD32_HAS_ADC2)
#error "GD32_HAS_ADCx not defined in registry"
#endif

/* Units checks.*/
#if GD32_ADC_USE_ADC0 && !GD32_HAS_ADC0
#error "ADC0 not present in the selected device"
#endif

#if GD32_ADC_USE_ADC1 && !GD32_HAS_ADC1
#error "ADC1 not present in the selected device"
#endif

#if GD32_ADC_USE_ADC2 && !GD32_HAS_ADC2
#error "ADC2 not present in the selected device"
#endif

/* At least one ADC must be assigned.*/
#if !GD32_ADC_USE_ADC0 && !GD32_ADC_USE_ADC1 && !GD32_ADC_USE_ADC2
#error "ADC driver activated but no ADC peripheral assigned"
#endif

/* IRQ handlers checks.*/
#if (GD32_ADC_USE_ADC0 || GD32_ADC_USE_ADC1) &&                             \
    !defined(GD32_ADC0_1_HANDLER)
#error "GD32_ADC0_1_HANDLER not defined in registry"
#endif

#if (GD32_ADC_USE_ADC0 || GD32_ADC_USE_ADC1) && !defined(GD32_ADC0_1_NUMBER)
#error "GD32_ADC0_1_NUMBER not defined in registry"
#endif

#if GD32_ADC_USE_ADC2 && !defined(GD32_ADC2_HANDLER)
#error "GD32_ADC2_HANDLER not defined in registry"
#endif

#if GD32_ADC_USE_ADC2 && !defined(GD32_ADC2_NUMBER)
#error "GD32_ADC2_NUMBER not defined in registry"
#endif

/* DMA priority tests.*/
#if GD32_ADC_USE_ADC0 &&                                                    \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_ADC_ADC0_DMA_PRIORITY)
#error "Invalid DMA priority assigned to ADC0"
#endif

#if GD32_ADC_USE_ADC1 &&                                                    \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_ADC_ADC1_DMA_PRIORITY)
#error "Invalid DMA priority assigned to ADC1"
#endif

#if GD32_ADC_USE_ADC2 &&                                                    \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_ADC_ADC2_DMA_PRIORITY)
#error "Invalid DMA priority assigned to ADC2"
#endif

/* Samples width checks.*/
#if ((GD32_ADC_SAMPLES_SIZE != 8)  &&                                       \
     (GD32_ADC_SAMPLES_SIZE != 16) &&                                       \
     (GD32_ADC_SAMPLES_SIZE != 32))
#error "GD32_ADC_SAMPLES_SIZE must be 8, 16 or 32"
#endif

/* Effective CK_ADC frequencies and range checks. The limits apply after the
   ADC_SYNCCTL prescaler.*/
#define GD32_ADC0_CLOCK                                                     \
  GD32_ADC_CKMODE_CLOCK(GD32_ADC_ADC0_CLOCK_MODE, GD32_ADC01CLK)
#define GD32_ADC1_CLOCK                                                     \
  GD32_ADC_CKMODE_CLOCK(GD32_ADC_ADC1_CLOCK_MODE, GD32_ADC01CLK)
#define GD32_ADC2_CLOCK                                                     \
  GD32_ADC_CKMODE_CLOCK(GD32_ADC_ADC2_CLOCK_MODE, GD32_ADC2CLK)

/* A null clock means the selected kernel clock source is not enabled.*/
#if GD32_ADC_USE_ADC0 && (GD32_ADC0_CLOCK == 0)
#error "GD32_ADC0_CLOCK is zero, check GD32_ADC01SEL and its clock source"
#endif

#if GD32_ADC_USE_ADC1 && (GD32_ADC1_CLOCK == 0)
#error "GD32_ADC1_CLOCK is zero, check GD32_ADC01SEL and its clock source"
#endif

#if GD32_ADC_USE_ADC2 && (GD32_ADC2_CLOCK == 0)
#error "GD32_ADC2_CLOCK is zero, check GD32_ADC2SEL and its clock source"
#endif

#if GD32_ADC_USE_ADC0 && (GD32_ADC0_CLOCK > GD32_ADC01CLK_MAX)
#error "GD32_ADC0_CLOCK exceeding maximum frequency (GD32_ADC01CLK_MAX)"
#endif

#if GD32_ADC_USE_ADC1 && (GD32_ADC1_CLOCK > GD32_ADC01CLK_MAX)
#error "GD32_ADC1_CLOCK exceeding maximum frequency (GD32_ADC01CLK_MAX)"
#endif

#if GD32_ADC_USE_ADC2 && (GD32_ADC2_CLOCK > GD32_ADC2CLK_MAX)
#error "GD32_ADC2_CLOCK exceeding maximum frequency (GD32_ADC2CLK_MAX)"
#endif

/* ISR ownership, the handlers live in the shared LLD/ADC .inc files.*/
#if GD32_ADC_USE_ADC0
#if defined(GD32_ADC0_IS_USED)
#error "ADC0 already in use"
#endif
#define GD32_ADC0_IS_USED
#endif

#if GD32_ADC_USE_ADC1
#if defined(GD32_ADC1_IS_USED)
#error "ADC1 already in use"
#endif
#define GD32_ADC1_IS_USED
#endif

#if GD32_ADC_USE_ADC2
#if defined(GD32_ADC2_IS_USED)
#error "ADC2 already in use"
#endif
#define GD32_ADC2_IS_USED
#endif

#if !defined(GD32_DMA_REQUIRED)
#define GD32_DMA_REQUIRED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   ADC sample data type.
 */
#if (GD32_ADC_SAMPLES_SIZE == 16) || defined(__DOXYGEN__)
typedef uint16_t adcsample_t;
#elif (GD32_ADC_SAMPLES_SIZE == 8)
typedef uint8_t adcsample_t;
#elif (GD32_ADC_SAMPLES_SIZE == 32)
typedef uint32_t adcsample_t;
#endif

/**
 * @brief   Channels number in a conversion group.
 */
typedef uint32_t adc_channels_num_t;

/**
 * @brief   Type of an ADC error mask.
 */
typedef uint32_t adcerror_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Low level fields of the ADC driver structure.
 */
#define adc_lld_driver_fields                                               \
  /* Base address of the ADCx registers block.*/                            \
  uint32_t                  adc;                                            \
  /* Associated TRIGSEL target register.*/                                  \
  volatile uint32_t         *trigsel;                                       \
  /* Pointer to the associated DMA stream.*/                                \
  const gd32_dma_stream_t   *dma;                                           \
  /* DMA mode bit mask.*/                                                   \
  uint32_t                  dmamode;                                        \
  /* ADC_SYNCCTL clock prescaler bits.*/                                    \
  uint32_t                  ckmode;                                         \
  /* Stabilization delay in CPU cycles.*/                                   \
  rtcnt_t                   stabcycles;

/**
 * @brief   Low level fields of the ADC configuration structure.
 */
#define adc_lld_config_fields                                               \
  /* Calibration mode, @p ADC_CALIBRATION_OFFSET_MISMATCH or                \
     @p ADC_CALIBRATION_OFFSET.*/                                           \
  uint32_t                  calibration;                                    \
  /* Calibration repetitions, one of the ADC_CALIBRATION_NUMx values.*/     \
  uint32_t                  calnum;

/**
 * @brief   Low level fields of the ADC group configuration structure.
 */
#define adc_lld_configuration_group_fields                                  \
  /* ADC CTL0 register initialization data.                                 \
     NOTE: the interrupt enable bits are enforced internally to the         \
           driver, keep them to zero.*/                                     \
  uint32_t                  ctl0;                                           \
  /* ADC CTL1 register initialization data.                                 \
     NOTE: the DMA, DDM, ADCON, calibration and software trigger bits are   \
           enforced internally to the driver, keep them to zero.            \
     NOTE: @p ADC_CTL1_CTN must be specified in continuous mode.*/          \
  uint32_t                  ctl1;                                           \
  /* ADC OVSAMPCTL register initialization data, zero if unused.*/          \
  uint32_t                  ovsampctl;                                      \
  /* ADC DIFCTL register initialization data.*/                             \
  uint32_t                  difctl;                                         \
  /* ADC RSQ0..RSQ8 registers initialization data, the sequence length      \
     goes into rsq[0].*/                                                    \
  uint32_t                  rsq[9];                                         \
  /* Analog watchdog 0 thresholds.*/                                        \
  uint32_t                  wdht0;                                          \
  uint32_t                  wdlt0;                                          \
  /* Analog watchdog 1 channel selection and thresholds.*/                  \
  uint32_t                  wd1sr;                                          \
  uint32_t                  wdht1;                                          \
  uint32_t                  wdlt1;                                          \
  /* Analog watchdog 2 channel selection and thresholds.*/                  \
  uint32_t                  wd2sr;                                          \
  uint32_t                  wdht2;                                          \
  uint32_t                  wdlt2;                                          \
  /* TRIGSEL routine trigger source, one of the TRIGSEL_INPUT_x values or   \
     @p GD32_ADC_TRIGSEL_NONE to leave TRIGSEL untouched.*/                 \
  uint32_t                  trigsel;

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if GD32_ADC_USE_ADC0 && !defined(__DOXYGEN__)
extern ADCDriver ADCD0;
#endif

#if GD32_ADC_USE_ADC1 && !defined(__DOXYGEN__)
extern ADCDriver ADCD1;
#endif

#if GD32_ADC_USE_ADC2 && !defined(__DOXYGEN__)
extern ADCDriver ADCD2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void adc_lld_init(void);
  msg_t adc_lld_start(ADCDriver *adcp);
  void adc_lld_stop(ADCDriver *adcp);
  void adc_lld_start_conversion(ADCDriver *adcp);
  void adc_lld_stop_conversion(ADCDriver *adcp);
  void adc_lld_serve_interrupt(ADCDriver *adcp);
#if GD32_ADC_USE_ADC2 || defined(__DOXYGEN__)
  void gd32_adcEnableTS(void);
  void gd32_adcDisableTS(void);
  void gd32_adcEnableHPTS(void);
  void gd32_adcDisableHPTS(void);
  void gd32_adcEnableVREF(void);
  void gd32_adcDisableVREF(void);
  void gd32_adcEnableVBAT(void);
  void gd32_adcDisableVBAT(void);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_ADC */

#endif /* HAL_ADC_LLD_H */

/** @} */
