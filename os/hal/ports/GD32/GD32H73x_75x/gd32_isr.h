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
 * @file    GD32H73x_75x/gd32_isr.h
 * @brief   GD32H73x_75x ISR handler header.
 *
 * @addtogroup GD32H73x_75x_ISR
 * @{
 */

#ifndef GD32_ISR_H
#define GD32_ISR_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    ISRs suppressed in standard drivers
 * @{
 */
#define GD32_USART0_SUPPRESS_ISR
#define GD32_USART1_SUPPRESS_ISR
#define GD32_USART2_SUPPRESS_ISR
#define GD32_UART3_SUPPRESS_ISR
#define GD32_UART4_SUPPRESS_ISR
#define GD32_USART5_SUPPRESS_ISR
#define GD32_UART6_SUPPRESS_ISR
#define GD32_UART7_SUPPRESS_ISR
#define GD32_I2C0_SUPPRESS_ISR
#define GD32_I2C1_SUPPRESS_ISR
#define GD32_I2C2_SUPPRESS_ISR
#define GD32_I2C3_SUPPRESS_ISR
#define GD32_TIMER0_SUPPRESS_ISR
#define GD32_TIMER1_SUPPRESS_ISR
#define GD32_TIMER2_SUPPRESS_ISR
#define GD32_TIMER3_SUPPRESS_ISR
#define GD32_TIMER4_SUPPRESS_ISR
#define GD32_TIMER5_SUPPRESS_ISR
#define GD32_TIMER6_SUPPRESS_ISR
#define GD32_TIMER7_SUPPRESS_ISR
#define GD32_TIMER14_SUPPRESS_ISR
#define GD32_TIMER15_SUPPRESS_ISR
#define GD32_TIMER16_SUPPRESS_ISR
#define GD32_TIMER22_SUPPRESS_ISR
#define GD32_TIMER23_SUPPRESS_ISR
#define GD32_TIMER30_SUPPRESS_ISR
#define GD32_TIMER31_SUPPRESS_ISR
#define GD32_TIMER40_SUPPRESS_ISR
#define GD32_TIMER41_SUPPRESS_ISR
#define GD32_TIMER42_SUPPRESS_ISR
#define GD32_TIMER43_SUPPRESS_ISR
#define GD32_TIMER44_SUPPRESS_ISR
#define GD32_TIMER50_SUPPRESS_ISR
#define GD32_TIMER51_SUPPRESS_ISR

#define GD32_SPI0_SUPPRESS_ISR
#define GD32_SPI1_SUPPRESS_ISR
#define GD32_SPI2_SUPPRESS_ISR
#define GD32_SPI3_SUPPRESS_ISR
#define GD32_SPI4_SUPPRESS_ISR
#define GD32_SPI5_SUPPRESS_ISR
#define GD32_SDIO0_SUPPRESS_ISR
#define GD32_SDIO1_SUPPRESS_ISR

#define GD32_ADC0_SUPPRESS_ISR
#define GD32_ADC1_SUPPRESS_ISR
#define GD32_ADC2_SUPPRESS_ISR

#define GD32_USBHS0_SUPPRESS_ISR
#define GD32_USBHS1_SUPPRESS_ISR
/** @} */

/**
 * @name    ISR names and numbers
 * @{
 */
/*
 * EXTI units.
 * @note    Vector offset = 0x40 + 4 * IRQn, verified against the IRQn_Type
 *          enum in gd32h73x_75x.h (EXTI0_IRQn=6 ... EXTI10_15_IRQn=40).
 */
#define GD32_EXTI0_HANDLER                   Vector58
#define GD32_EXTI1_HANDLER                   Vector5C
#define GD32_EXTI2_HANDLER                   Vector60
#define GD32_EXTI3_HANDLER                   Vector64
#define GD32_EXTI4_HANDLER                   Vector68
#define GD32_EXTI5_9_HANDLER                 Vector9C
#define GD32_EXTI10_15_HANDLER               VectorE0

#define GD32_EXTI0_NUMBER                    6
#define GD32_EXTI1_NUMBER                    7
#define GD32_EXTI2_NUMBER                    8
#define GD32_EXTI3_NUMBER                    9
#define GD32_EXTI4_NUMBER                    10
#define GD32_EXTI5_9_NUMBER                  23
#define GD32_EXTI10_15_NUMBER                40

/*
 * USART/UART units.
 * @note    GD32H73x_75x has 8 USART/UART instances (USART0/1/2, UART3/4,
 *          USART5, UART6/7), unlike STM32H7xx which has up to 11. Vector
 *          offsets verified against the IRQn_Type enum in gd32h73x_75x.h
 *          (USART0_IRQn=37 ... UART7_IRQn=83).
 */
#define GD32_USART0_HANDLER                  VectorD4
#define GD32_USART1_HANDLER                  VectorD8
#define GD32_USART2_HANDLER                  VectorDC
#define GD32_UART3_HANDLER                   Vector110
#define GD32_UART4_HANDLER                   Vector114
#define GD32_USART5_HANDLER                  Vector15C
#define GD32_UART6_HANDLER                   Vector188
#define GD32_UART7_HANDLER                   Vector18C

#define GD32_USART0_NUMBER                   37
#define GD32_USART1_NUMBER                   38
#define GD32_USART2_NUMBER                   39
#define GD32_UART3_NUMBER                    52
#define GD32_UART4_NUMBER                    53
#define GD32_USART5_NUMBER                   71
#define GD32_UART6_NUMBER                    82
#define GD32_UART7_NUMBER                    83

/*
 * TIMER units.
 * @note    Advanced timers (TIMER0/7) split their interrupt across four
 *          vectors (break, update, trigger/commutation, capture/compare).
 *          The DEC vectors serve the quadrature decoder error event and
 *          only exist on the timers listed below.
 * @note    TIMER5 shares its vector with the DAC underrun event, hence the
 *          combined GD32_TIMER5_DAC_UDR_* naming.
 */
#define GD32_TIMER0_BRK_HANDLER              VectorA0
#define GD32_TIMER0_UP_HANDLER               VectorA4
#define GD32_TIMER0_TRG_CMT_HANDLER          VectorA8
#define GD32_TIMER0_CC_HANDLER               VectorAC
#define GD32_TIMER1_HANDLER                  VectorB0
#define GD32_TIMER2_HANDLER                  VectorB4
#define GD32_TIMER3_HANDLER                  VectorB8
#define GD32_TIMER7_BRK_HANDLER              VectorEC
#define GD32_TIMER7_UP_HANDLER               VectorF0
#define GD32_TIMER7_TRG_CMT_HANDLER          VectorF4
#define GD32_TIMER7_CC_HANDLER               VectorF8
#define GD32_TIMER4_HANDLER                  Vector108
#define GD32_TIMER5_DAC_UDR_HANDLER          Vector118
#define GD32_TIMER6_HANDLER                  Vector11C
#define GD32_TIMER14_HANDLER                 Vector210
#define GD32_TIMER15_HANDLER                 Vector214
#define GD32_TIMER16_HANDLER                 Vector218
#define GD32_TIMER22_HANDLER                 Vector2C4
#define GD32_TIMER23_HANDLER                 Vector2C8
#define GD32_TIMER30_HANDLER                 Vector2CC
#define GD32_TIMER31_HANDLER                 Vector2D0
#define GD32_TIMER40_HANDLER                 Vector2D4
#define GD32_TIMER41_HANDLER                 Vector2D8
#define GD32_TIMER42_HANDLER                 Vector2DC
#define GD32_TIMER43_HANDLER                 Vector2E0
#define GD32_TIMER44_HANDLER                 Vector2E4
#define GD32_TIMER50_HANDLER                 Vector2E8
#define GD32_TIMER51_HANDLER                 Vector2EC
#define GD32_TIMER0_DEC_HANDLER              Vector37C
#define GD32_TIMER7_DEC_HANDLER              Vector380
#define GD32_TIMER1_DEC_HANDLER              Vector384
#define GD32_TIMER2_DEC_HANDLER              Vector388
#define GD32_TIMER3_DEC_HANDLER              Vector38C
#define GD32_TIMER4_DEC_HANDLER              Vector390
#define GD32_TIMER22_DEC_HANDLER             Vector394
#define GD32_TIMER23_DEC_HANDLER             Vector398
#define GD32_TIMER30_DEC_HANDLER             Vector39C
#define GD32_TIMER31_DEC_HANDLER             Vector3A0

#define GD32_TIMER0_BRK_NUMBER               24
#define GD32_TIMER0_UP_NUMBER                25
#define GD32_TIMER0_TRG_CMT_NUMBER           26
#define GD32_TIMER0_CC_NUMBER                27
#define GD32_TIMER1_NUMBER                   28
#define GD32_TIMER2_NUMBER                   29
#define GD32_TIMER3_NUMBER                   30
#define GD32_TIMER7_BRK_NUMBER               43
#define GD32_TIMER7_UP_NUMBER                44
#define GD32_TIMER7_TRG_CMT_NUMBER           45
#define GD32_TIMER7_CC_NUMBER                46
#define GD32_TIMER4_NUMBER                   50
#define GD32_TIMER5_DAC_UDR_NUMBER           54
#define GD32_TIMER6_NUMBER                   55
#define GD32_TIMER14_NUMBER                  116
#define GD32_TIMER15_NUMBER                  117
#define GD32_TIMER16_NUMBER                  118
#define GD32_TIMER22_NUMBER                  161
#define GD32_TIMER23_NUMBER                  162
#define GD32_TIMER30_NUMBER                  163
#define GD32_TIMER31_NUMBER                  164
#define GD32_TIMER40_NUMBER                  165
#define GD32_TIMER41_NUMBER                  166
#define GD32_TIMER42_NUMBER                  167
#define GD32_TIMER43_NUMBER                  168
#define GD32_TIMER44_NUMBER                  169
#define GD32_TIMER50_NUMBER                  170
#define GD32_TIMER51_NUMBER                  171
#define GD32_TIMER0_DEC_NUMBER               207
#define GD32_TIMER7_DEC_NUMBER               208
#define GD32_TIMER1_DEC_NUMBER               209
#define GD32_TIMER2_DEC_NUMBER               210
#define GD32_TIMER3_DEC_NUMBER               211
#define GD32_TIMER4_DEC_NUMBER               212
#define GD32_TIMER22_DEC_NUMBER              213
#define GD32_TIMER23_DEC_NUMBER              214
#define GD32_TIMER30_DEC_NUMBER              215
#define GD32_TIMER31_DEC_NUMBER              216

/*
 * I2C units.
 * @note    Each instance has separate event, error and EXTI wakeup vectors.
 */
#define GD32_I2C0_EV_HANDLER                 VectorBC
#define GD32_I2C0_ER_HANDLER                 VectorC0
#define GD32_I2C1_EV_HANDLER                 VectorC4
#define GD32_I2C1_ER_HANDLER                 VectorC8
#define GD32_I2C2_EV_HANDLER                 Vector160
#define GD32_I2C2_ER_HANDLER                 Vector164
#define GD32_I2C3_EV_HANDLER                 Vector1BC
#define GD32_I2C3_ER_HANDLER                 Vector1C0
#define GD32_I2C0_WKUP_HANDLER               Vector364
#define GD32_I2C1_WKUP_HANDLER               Vector368
#define GD32_I2C2_WKUP_HANDLER               Vector36C
#define GD32_I2C3_WKUP_HANDLER               Vector370

#define GD32_I2C0_EV_NUMBER                  31
#define GD32_I2C0_ER_NUMBER                  32
#define GD32_I2C1_EV_NUMBER                  33
#define GD32_I2C1_ER_NUMBER                  34
#define GD32_I2C2_EV_NUMBER                  72
#define GD32_I2C2_ER_NUMBER                  73
#define GD32_I2C3_EV_NUMBER                  95
#define GD32_I2C3_ER_NUMBER                  96
#define GD32_I2C0_WKUP_NUMBER                201
#define GD32_I2C1_WKUP_NUMBER                202
#define GD32_I2C2_WKUP_NUMBER                203
#define GD32_I2C3_WKUP_NUMBER                204

/*
 * SPI/I2S units.
 */
#define GD32_SPI0_HANDLER                    VectorCC
#define GD32_SPI1_HANDLER                    VectorD0
#define GD32_SPI2_HANDLER                    Vector10C
#define GD32_SPI3_HANDLER                    Vector190
#define GD32_SPI4_HANDLER                    Vector194
#define GD32_SPI5_HANDLER                    Vector198

#define GD32_SPI0_NUMBER                     35
#define GD32_SPI1_NUMBER                     36
#define GD32_SPI2_NUMBER                     51
#define GD32_SPI3_NUMBER                     84
#define GD32_SPI4_NUMBER                     85
#define GD32_SPI5_NUMBER                     86

/*
 * ADC and DAC units.
 * @note    ADC0 and ADC1 share a single vector. The DAC underrun event is
 *          routed to the TIMER5 vector, see GD32_TIMER5_DAC_UDR_HANDLER.
 */
#define GD32_ADC0_1_HANDLER                  Vector88
#define GD32_ADC2_HANDLER                    Vector23C

#define GD32_ADC0_1_NUMBER                   18
#define GD32_ADC2_NUMBER                     127

/*
 * CAN units.
 * @note    Every instance exposes seven separate vectors.
 */
#define GD32_CAN0_WKUP_HANDLER               Vector30C
#define GD32_CAN0_MSG_HANDLER                Vector310
#define GD32_CAN0_BUSOFF_HANDLER             Vector314
#define GD32_CAN0_ERR_HANDLER                Vector318
#define GD32_CAN0_FASTERR_HANDLER            Vector31C
#define GD32_CAN0_TEC_HANDLER                Vector320
#define GD32_CAN0_REC_HANDLER                Vector324
#define GD32_CAN1_WKUP_HANDLER               Vector328
#define GD32_CAN1_MSG_HANDLER                Vector32C
#define GD32_CAN1_BUSOFF_HANDLER             Vector330
#define GD32_CAN1_ERR_HANDLER                Vector334
#define GD32_CAN1_FASTERR_HANDLER            Vector338
#define GD32_CAN1_TEC_HANDLER                Vector33C
#define GD32_CAN1_REC_HANDLER                Vector340
#define GD32_CAN2_WKUP_HANDLER               Vector344
#define GD32_CAN2_MSG_HANDLER                Vector348
#define GD32_CAN2_BUSOFF_HANDLER             Vector34C
#define GD32_CAN2_ERR_HANDLER                Vector350
#define GD32_CAN2_FASTERR_HANDLER            Vector354
#define GD32_CAN2_TEC_HANDLER                Vector358
#define GD32_CAN2_REC_HANDLER                Vector35C

#define GD32_CAN0_WKUP_NUMBER                179
#define GD32_CAN0_MSG_NUMBER                 180
#define GD32_CAN0_BUSOFF_NUMBER              181
#define GD32_CAN0_ERR_NUMBER                 182
#define GD32_CAN0_FASTERR_NUMBER             183
#define GD32_CAN0_TEC_NUMBER                 184
#define GD32_CAN0_REC_NUMBER                 185
#define GD32_CAN1_WKUP_NUMBER                186
#define GD32_CAN1_MSG_NUMBER                 187
#define GD32_CAN1_BUSOFF_NUMBER              188
#define GD32_CAN1_ERR_NUMBER                 189
#define GD32_CAN1_FASTERR_NUMBER             190
#define GD32_CAN1_TEC_NUMBER                 191
#define GD32_CAN1_REC_NUMBER                 192
#define GD32_CAN2_WKUP_NUMBER                193
#define GD32_CAN2_MSG_NUMBER                 194
#define GD32_CAN2_BUSOFF_NUMBER              195
#define GD32_CAN2_ERR_NUMBER                 196
#define GD32_CAN2_FASTERR_NUMBER             197
#define GD32_CAN2_TEC_NUMBER                 198
#define GD32_CAN2_REC_NUMBER                 199

/*
 * SDIO units.
 */
#define GD32_SDIO0_HANDLER                   Vector104
#define GD32_SDIO1_HANDLER                   Vector230

#define GD32_SDIO0_NUMBER                    49
#define GD32_SDIO1_NUMBER                    124

/*
 * USBHS units.
 */
#define GD32_USBHS0_EP1_OUT_HANDLER          Vector168
#define GD32_USBHS0_EP1_IN_HANDLER           Vector16C
#define GD32_USBHS0_WKUP_HANDLER             Vector170
#define GD32_USBHS0_HANDLER                  Vector174
#define GD32_USBHS1_EP1_OUT_HANDLER          Vector2F0
#define GD32_USBHS1_EP1_IN_HANDLER           Vector2F4
#define GD32_USBHS1_WKUP_HANDLER             Vector2F8
#define GD32_USBHS1_HANDLER                  Vector2FC

#define GD32_USBHS0_EP1_OUT_NUMBER           74
#define GD32_USBHS0_EP1_IN_NUMBER            75
#define GD32_USBHS0_WKUP_NUMBER              76
#define GD32_USBHS0_NUMBER                   77
#define GD32_USBHS1_EP1_OUT_NUMBER           172
#define GD32_USBHS1_EP1_IN_NUMBER            173
#define GD32_USBHS1_WKUP_NUMBER              174
#define GD32_USBHS1_NUMBER                   175

/*
 * Ethernet units.
 */
#define GD32_ENET0_HANDLER                   Vector134
#define GD32_ENET0_WKUP_HANDLER              Vector138
#define GD32_ENET1_HANDLER                   Vector300
#define GD32_ENET1_WKUP_HANDLER              Vector304
#define GD32_MDIO_HANDLER                    Vector220

#define GD32_ENET0_NUMBER                    61
#define GD32_ENET0_WKUP_NUMBER               62
#define GD32_ENET1_NUMBER                    176
#define GD32_ENET1_WKUP_NUMBER               177
#define GD32_MDIO_NUMBER                     120

/*
 * DMA, MDMA and DMAMUX.
 * @note    DMA0 channel 7 is not contiguous with channels 0..6, its IRQn
 *          is 47 while channels 0..6 use 11..17.
 */
#define GD32_DMA0_CH0_HANDLER                Vector6C
#define GD32_DMA0_CH1_HANDLER                Vector70
#define GD32_DMA0_CH2_HANDLER                Vector74
#define GD32_DMA0_CH3_HANDLER                Vector78
#define GD32_DMA0_CH4_HANDLER                Vector7C
#define GD32_DMA0_CH5_HANDLER                Vector80
#define GD32_DMA0_CH6_HANDLER                Vector84
#define GD32_DMA0_CH7_HANDLER                VectorFC
#define GD32_DMA1_CH0_HANDLER                Vector120
#define GD32_DMA1_CH1_HANDLER                Vector124
#define GD32_DMA1_CH2_HANDLER                Vector128
#define GD32_DMA1_CH3_HANDLER                Vector12C
#define GD32_DMA1_CH4_HANDLER                Vector130
#define GD32_DMA1_CH5_HANDLER                Vector150
#define GD32_DMA1_CH6_HANDLER                Vector154
#define GD32_DMA1_CH7_HANDLER                Vector158
#define GD32_MDMA_HANDLER                    Vector228
#define GD32_DMAMUX_OVR_HANDLER              Vector1D8

#define GD32_DMA0_CH0_NUMBER                 11
#define GD32_DMA0_CH1_NUMBER                 12
#define GD32_DMA0_CH2_NUMBER                 13
#define GD32_DMA0_CH3_NUMBER                 14
#define GD32_DMA0_CH4_NUMBER                 15
#define GD32_DMA0_CH5_NUMBER                 16
#define GD32_DMA0_CH6_NUMBER                 17
#define GD32_DMA0_CH7_NUMBER                 47
#define GD32_DMA1_CH0_NUMBER                 56
#define GD32_DMA1_CH1_NUMBER                 57
#define GD32_DMA1_CH2_NUMBER                 58
#define GD32_DMA1_CH3_NUMBER                 59
#define GD32_DMA1_CH4_NUMBER                 60
#define GD32_DMA1_CH5_NUMBER                 68
#define GD32_DMA1_CH6_NUMBER                 69
#define GD32_DMA1_CH7_NUMBER                 70
#define GD32_MDMA_NUMBER                     122
#define GD32_DMAMUX_OVR_NUMBER               102

/*
 * External memory and octo-SPI controllers.
 */
#define GD32_EXMC_HANDLER                    Vector100
#define GD32_OSPI0_HANDLER                   Vector1B0
#define GD32_OSPI1_HANDLER                   Vector298
#define GD32_RTDEC0_HANDLER                  Vector29C
#define GD32_RTDEC1_HANDLER                  Vector2A0

#define GD32_EXMC_NUMBER                     48
#define GD32_OSPI0_NUMBER                    92
#define GD32_OSPI1_NUMBER                    150
#define GD32_RTDEC0_NUMBER                   151
#define GD32_RTDEC1_NUMBER                   152

/*
 * Graphics and imaging units.
 */
#define GD32_TLI_HANDLER                     Vector1A0
#define GD32_TLI_ER_HANDLER                  Vector1A4
#define GD32_IPA_HANDLER                     Vector1A8
#define GD32_DCI_HANDLER                     Vector178

#define GD32_TLI_NUMBER                      88
#define GD32_TLI_ER_NUMBER                   89
#define GD32_IPA_NUMBER                      90
#define GD32_DCI_NUMBER                      78

/*
 * Audio units.
 */
#define GD32_SAI0_HANDLER                    Vector19C
#define GD32_SAI1_HANDLER                    Vector1AC
#define GD32_SAI2_HANDLER                    Vector208
#define GD32_RSPDIF_HANDLER                  Vector1C4
#define GD32_HPDF_INT0_HANDLER               Vector1F8
#define GD32_HPDF_INT1_HANDLER               Vector1FC
#define GD32_HPDF_INT2_HANDLER               Vector200
#define GD32_HPDF_INT3_HANDLER               Vector204

#define GD32_SAI0_NUMBER                     87
#define GD32_SAI1_NUMBER                     91
#define GD32_SAI2_NUMBER                     114
#define GD32_RSPDIF_NUMBER                   97
#define GD32_HPDF_INT0_NUMBER                110
#define GD32_HPDF_INT1_NUMBER                111
#define GD32_HPDF_INT2_NUMBER                112
#define GD32_HPDF_INT3_NUMBER                113

/*
 * Cryptography, random number generation and math accelerators.
 */
#define GD32_CAU_HANDLER                     Vector17C
#define GD32_HAU_TRNG_HANDLER                Vector180
#define GD32_FAC_HANDLER                     Vector2A4
#define GD32_TMU_HANDLER                     Vector2A8

#define GD32_CAU_NUMBER                      79
#define GD32_HAU_TRNG_NUMBER                 80
#define GD32_FAC_NUMBER                      153
#define GD32_TMU_NUMBER                      154

/*
 * RTC, watchdog and system-level units.
 */
#define GD32_WWDGT_HANDLER                   Vector40
#define GD32_VAVD_LVD_VOVD_HANDLER           Vector44
#define GD32_TAMPER_STAMP_LXTAL_HANDLER      Vector48
#define GD32_RTC_WKUP_HANDLER                Vector4C
#define GD32_FMC_HANDLER                     Vector50
#define GD32_RCU_HANDLER                     Vector54
#define GD32_RTC_ALARM_HANDLER               VectorE4
#define GD32_FPU_HANDLER                     Vector184
#define GD32_HWSEM_HANDLER                   Vector234
#define GD32_CMP0_1_HANDLER                  Vector264
#define GD32_CTC_HANDLER                     Vector280
#define GD32_RAMECCMU_HANDLER                Vector284
#define GD32_EFUSE_HANDLER                   Vector360
#define GD32_LPDTS_HANDLER                   Vector374
#define GD32_LPDTS_WKUP_HANDLER              Vector378

#define GD32_WWDGT_NUMBER                    0
#define GD32_VAVD_LVD_VOVD_NUMBER            1
#define GD32_TAMPER_STAMP_LXTAL_NUMBER       2
#define GD32_RTC_WKUP_NUMBER                 3
#define GD32_FMC_NUMBER                      4
#define GD32_RCU_NUMBER                      5
#define GD32_RTC_ALARM_NUMBER                41
#define GD32_FPU_NUMBER                      81
#define GD32_HWSEM_NUMBER                    125
#define GD32_CMP0_1_NUMBER                   137
#define GD32_CTC_NUMBER                      144
#define GD32_RAMECCMU_NUMBER                 145
#define GD32_EFUSE_NUMBER                    200
#define GD32_LPDTS_NUMBER                    205
#define GD32_LPDTS_WKUP_NUMBER               206
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

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void irqInit(void);
  void irqDeinit(void);
#ifdef __cplusplus
}
#endif

#endif /* GD32_ISR_H */

/** @} */
