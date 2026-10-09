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
 * @file    ADCv1/gd32_adc.h
 * @brief   GD32 ADC registers helper header.
 * @note    Register access relies on the flat REG32 accessors exported by
 *          the vendor header @p gd32h73x_75x_adc.h, the bit definitions of
 *          that header are reused whenever possible.
 *
 * @addtogroup GD32_ADCv1
 * @{
 */

#ifndef GD32_ADC_H
#define GD32_ADC_H

#if (HAL_USE_ADC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    Clock mode selectors
 * @details A selector is a bare token, for example @p ASYNC_DIV4, expanding
 *          into three attributes: the SYNCCTL register bits, the divider
 *          value and a flag telling whether the source is HCLK or the ADC
 *          kernel clock.
 * @{
 */
#define GD32_ADC_CKMODE_ASYNC_DIV1_BITS         ADC_CLK_ASYNC_DIV1
#define GD32_ADC_CKMODE_ASYNC_DIV1_DIV          1U
#define GD32_ADC_CKMODE_ASYNC_DIV1_SYNC         FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV2_BITS         ADC_CLK_ASYNC_DIV2
#define GD32_ADC_CKMODE_ASYNC_DIV2_DIV          2U
#define GD32_ADC_CKMODE_ASYNC_DIV2_SYNC         FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV4_BITS         ADC_CLK_ASYNC_DIV4
#define GD32_ADC_CKMODE_ASYNC_DIV4_DIV          4U
#define GD32_ADC_CKMODE_ASYNC_DIV4_SYNC         FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV6_BITS         ADC_CLK_ASYNC_DIV6
#define GD32_ADC_CKMODE_ASYNC_DIV6_DIV          6U
#define GD32_ADC_CKMODE_ASYNC_DIV6_SYNC         FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV8_BITS         ADC_CLK_ASYNC_DIV8
#define GD32_ADC_CKMODE_ASYNC_DIV8_DIV          8U
#define GD32_ADC_CKMODE_ASYNC_DIV8_SYNC         FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV10_BITS        ADC_CLK_ASYNC_DIV10
#define GD32_ADC_CKMODE_ASYNC_DIV10_DIV         10U
#define GD32_ADC_CKMODE_ASYNC_DIV10_SYNC        FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV12_BITS        ADC_CLK_ASYNC_DIV12
#define GD32_ADC_CKMODE_ASYNC_DIV12_DIV         12U
#define GD32_ADC_CKMODE_ASYNC_DIV12_SYNC        FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV16_BITS        ADC_CLK_ASYNC_DIV16
#define GD32_ADC_CKMODE_ASYNC_DIV16_DIV         16U
#define GD32_ADC_CKMODE_ASYNC_DIV16_SYNC        FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV32_BITS        ADC_CLK_ASYNC_DIV32
#define GD32_ADC_CKMODE_ASYNC_DIV32_DIV         32U
#define GD32_ADC_CKMODE_ASYNC_DIV32_SYNC        FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV64_BITS        ADC_CLK_ASYNC_DIV64
#define GD32_ADC_CKMODE_ASYNC_DIV64_DIV         64U
#define GD32_ADC_CKMODE_ASYNC_DIV64_SYNC        FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV128_BITS       ADC_CLK_ASYNC_DIV128
#define GD32_ADC_CKMODE_ASYNC_DIV128_DIV        128U
#define GD32_ADC_CKMODE_ASYNC_DIV128_SYNC       FALSE

#define GD32_ADC_CKMODE_ASYNC_DIV256_BITS       ADC_CLK_ASYNC_DIV256
#define GD32_ADC_CKMODE_ASYNC_DIV256_DIV        256U
#define GD32_ADC_CKMODE_ASYNC_DIV256_SYNC       FALSE

#define GD32_ADC_CKMODE_SYNC_HCLK_DIV2_BITS     ADC_CLK_SYNC_HCLK_DIV2
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV2_DIV      2U
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV2_SYNC     TRUE

#define GD32_ADC_CKMODE_SYNC_HCLK_DIV4_BITS     ADC_CLK_SYNC_HCLK_DIV4
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV4_DIV      4U
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV4_SYNC     TRUE

#define GD32_ADC_CKMODE_SYNC_HCLK_DIV6_BITS     ADC_CLK_SYNC_HCLK_DIV6
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV6_DIV      6U
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV6_SYNC     TRUE

#define GD32_ADC_CKMODE_SYNC_HCLK_DIV8_BITS     ADC_CLK_SYNC_HCLK_DIV8
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV8_DIV      8U
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV8_SYNC     TRUE

#define GD32_ADC_CKMODE_SYNC_HCLK_DIV10_BITS    ADC_CLK_SYNC_HCLK_DIV10
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV10_DIV     10U
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV10_SYNC    TRUE

#define GD32_ADC_CKMODE_SYNC_HCLK_DIV12_BITS    ADC_CLK_SYNC_HCLK_DIV12
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV12_DIV     12U
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV12_SYNC    TRUE

#define GD32_ADC_CKMODE_SYNC_HCLK_DIV14_BITS    ADC_CLK_SYNC_HCLK_DIV14
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV14_DIV     14U
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV14_SYNC    TRUE

#define GD32_ADC_CKMODE_SYNC_HCLK_DIV16_BITS    ADC_CLK_SYNC_HCLK_DIV16
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV16_DIV     16U
#define GD32_ADC_CKMODE_SYNC_HCLK_DIV16_SYNC    TRUE
/** @} */

/**
 * @name    Resolution field values
 * @note    ADC2 lacks the 14 bits mode and adds the 6 bits one, so its DRES
 *          encoding is shifted by one step.
 * @{
 */
#define GD32_ADC01_DRES_14B     CTL0_DRES(ADC_RESOLUTION_14B)
#define GD32_ADC01_DRES_12B     CTL0_DRES(ADC_RESOLUTION_12B)
#define GD32_ADC01_DRES_10B     CTL0_DRES(ADC_RESOLUTION_10B)
#define GD32_ADC01_DRES_8B      CTL0_DRES(ADC_RESOLUTION_8B)

#define GD32_ADC2_DRES_12B      CTL0_DRES(ADC_RESOLUTION_12B - 1U)
#define GD32_ADC2_DRES_10B      CTL0_DRES(ADC_RESOLUTION_10B - 1U)
#define GD32_ADC2_DRES_8B       CTL0_DRES(ADC_RESOLUTION_8B - 1U)
#define GD32_ADC2_DRES_6B       CTL0_DRES(ADC_RESOLUTION_6B - 1U)
/** @} */

/**
 * @name    Bits owned by the driver
 * @details These bits must be left cleared in the conversion group fields,
 *          the driver enforces them according to the requested operation.
 * @{
 */
#define GD32_ADC_CTL0_RESERVED_MASK                                         \
  (ADC_CTL0_EOCIE | ADC_CTL0_WDE0IE | ADC_CTL0_EOICIE |                     \
   ADC_CTL0_ROVFIE | ADC_CTL0_WDE1IE | ADC_CTL0_WDE2IE)

/**
 * @brief   Internal channel enables, owned by the gd32_adcEnableXXX() API.
 * @note    They live in ADC2's CTL1 and must survive a group switch.
 */
#define GD32_ADC_CTL1_INTCH_MASK                                            \
  (ADC_CTL1_TSVEN1 | ADC_CTL1_TSVEN2 | ADC_CTL1_INREFEN | ADC_CTL1_VBATEN)

#define GD32_ADC_CTL1_RESERVED_MASK                                         \
  (ADC_CTL1_ADCON | ADC_CTL1_CLB | ADC_CTL1_RSTCLB | ADC_CTL1_CALNUM |      \
   ADC_CTL1_DMA | ADC_CTL1_DDM | ADC_CTL1_CALMOD |                          \
   ADC_CTL1_SWICST | ADC_CTL1_SWRCST | GD32_ADC_CTL1_INTCH_MASK)
/** @} */

/**
 * @name    Routine sequence building helper macros
 * @details The ranks are stored in the @p rsq[] array in reverse register
 *          order, each register holding two ranks, 16 bits each:
 *          rsq[8]=SQ1, rsq[7]=SQ2/SQ3, rsq[6]=SQ4/SQ5, rsq[5]=SQ6/SQ7,
 *          rsq[4]=SQ8/SQ9, rsq[3]=SQ10/SQ11, rsq[2]=SQ12/SQ13,
 *          rsq[1]=SQ14/SQ15, rsq[0]=SQ16 plus the sequence length.
 * @note    Unlike most other devices the sample time is not held in a
 *          separate register, it is encoded in the rank itself as a number
 *          of CK_ADC cycles.
 * @{
 */
#define GD32_ADC_SQ_N(ch, smp)  ((uint32_t)(ch) | SQX_SMP(smp))

/**
 * @brief   Number of channels in the routine sequence.
 */
#define GD32_ADC_RSQ0_RL(n)             RSQ0_RL((uint32_t)(n) - 1U)

#define GD32_ADC_RSQ8_SQ1(ch, smp)      GD32_ADC_SQ_N(ch, smp)

#define GD32_ADC_RSQ7_SQ2(ch, smp)      GD32_ADC_SQ_N(ch, smp)
#define GD32_ADC_RSQ7_SQ3(ch, smp)      (GD32_ADC_SQ_N(ch, smp) << 16U)

#define GD32_ADC_RSQ6_SQ4(ch, smp)      GD32_ADC_SQ_N(ch, smp)
#define GD32_ADC_RSQ6_SQ5(ch, smp)      (GD32_ADC_SQ_N(ch, smp) << 16U)

#define GD32_ADC_RSQ5_SQ6(ch, smp)      GD32_ADC_SQ_N(ch, smp)
#define GD32_ADC_RSQ5_SQ7(ch, smp)      (GD32_ADC_SQ_N(ch, smp) << 16U)

#define GD32_ADC_RSQ4_SQ8(ch, smp)      GD32_ADC_SQ_N(ch, smp)
#define GD32_ADC_RSQ4_SQ9(ch, smp)      (GD32_ADC_SQ_N(ch, smp) << 16U)

#define GD32_ADC_RSQ3_SQ10(ch, smp)     GD32_ADC_SQ_N(ch, smp)
#define GD32_ADC_RSQ3_SQ11(ch, smp)     (GD32_ADC_SQ_N(ch, smp) << 16U)

#define GD32_ADC_RSQ2_SQ12(ch, smp)     GD32_ADC_SQ_N(ch, smp)
#define GD32_ADC_RSQ2_SQ13(ch, smp)     (GD32_ADC_SQ_N(ch, smp) << 16U)

#define GD32_ADC_RSQ1_SQ14(ch, smp)     GD32_ADC_SQ_N(ch, smp)
#define GD32_ADC_RSQ1_SQ15(ch, smp)     (GD32_ADC_SQ_N(ch, smp) << 16U)

#define GD32_ADC_RSQ0_SQ16(ch, smp)     GD32_ADC_SQ_N(ch, smp)
/** @} */

/**
 * @name    External trigger helper macros
 * @{
 */
#define GD32_ADC_CTL1_ETMRC(n)  ((uint32_t)(n) << 28U)
#define GD32_ADC_CTL1_ETMIC(n)  ((uint32_t)(n) << 20U)
/** @} */

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
 * @name    Clock mode selector accessors
 * @{
 */
#define GD32_ADC_CKMODE_BITS_(mode)     GD32_ADC_CKMODE_##mode##_BITS
#define GD32_ADC_CKMODE_DIV_(mode)      GD32_ADC_CKMODE_##mode##_DIV
#define GD32_ADC_CKMODE_SYNC_(mode)     GD32_ADC_CKMODE_##mode##_SYNC

#define GD32_ADC_CKMODE_BITS(mode)      GD32_ADC_CKMODE_BITS_(mode)
#define GD32_ADC_CKMODE_DIV(mode)       GD32_ADC_CKMODE_DIV_(mode)
#define GD32_ADC_CKMODE_SYNC(mode)      GD32_ADC_CKMODE_SYNC_(mode)

/**
 * @brief   Effective CK_ADC frequency for a clock mode selector.
 *
 * @param[in] mode      clock mode selector token
 * @param[in] kclk      ADC kernel clock frequency
 */
#define GD32_ADC_CKMODE_CLOCK(mode, kclk)                                   \
  ((GD32_ADC_CKMODE_SYNC(mode) == TRUE ? (GD32_HCLK) : (kclk)) /            \
   GD32_ADC_CKMODE_DIV(mode))
/** @} */

/**
 * @brief   ADC stabilization delay, 14 CK_ADC cycles after ADCON is set.
 *
 * @param[in] adcclk    effective CK_ADC frequency
 */
#define GD32_ADC_STAB_CYCLES(adcclk)                                        \
  ((rtcnt_t)(((14ULL * (uint64_t)GD32_SYSCLK) / (uint64_t)(adcclk)) + 1ULL))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#endif /* HAL_USE_ADC == TRUE */

#endif /* GD32_ADC_H */

/** @} */
