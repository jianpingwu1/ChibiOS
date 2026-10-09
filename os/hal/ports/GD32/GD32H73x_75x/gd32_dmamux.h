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
 * @file    GD32H73x_75x/gd32_dmamux.h
 * @brief   GD32H73x_75x DMAMUX handler header.
 *
 * @addtogroup GD32H73x_75x_DMAMUX
 * @{
 */

#ifndef GD32_DMAMUX_H
#define GD32_DMAMUX_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * The GD32H73x/75x vendor header (gd32h73x_75x_dma.h) already provides the
 * complete set of DMA_REQUEST_* multiplexer input identification macros,
 * included transitively through gd32h73x_75x_libopt.h. This file provides
 * ChibiOS-style aliases for all of them, plus the request generator trigger
 * input IDs. IDs match the vendor firmware library exactly.
 */

/**
 * @name    DMAMUX request sources (generic)
 * @{
 */
#define GD32_DMAMUX1_REQ_M2M            DMA_REQUEST_M2M
#define GD32_DMAMUX1_REQ_GEN0           DMA_REQUEST_GENERATOR0
#define GD32_DMAMUX1_REQ_GEN1           DMA_REQUEST_GENERATOR1
#define GD32_DMAMUX1_REQ_GEN2           DMA_REQUEST_GENERATOR2
#define GD32_DMAMUX1_REQ_GEN3           DMA_REQUEST_GENERATOR3
#define GD32_DMAMUX1_REQ_GEN4           DMA_REQUEST_GENERATOR4
#define GD32_DMAMUX1_REQ_GEN5           DMA_REQUEST_GENERATOR5
#define GD32_DMAMUX1_REQ_GEN6           DMA_REQUEST_GENERATOR6
#define GD32_DMAMUX1_REQ_GEN7           DMA_REQUEST_GENERATOR7
/** @} */

/**
 * @name    DMAMUX request sources (ADC)
 * @{
 */
#define GD32_DMAMUX1_ADC0               DMA_REQUEST_ADC0
#define GD32_DMAMUX1_ADC1               DMA_REQUEST_ADC1
#define GD32_DMAMUX1_ADC2               DMA_REQUEST_ADC2
/** @} */

/**
 * @name    DMAMUX request sources (TIMER0)
 * @{
 */
#define GD32_DMAMUX1_TIMER0_CH0         DMA_REQUEST_TIMER0_CH0
#define GD32_DMAMUX1_TIMER0_CH1         DMA_REQUEST_TIMER0_CH1
#define GD32_DMAMUX1_TIMER0_CH2         DMA_REQUEST_TIMER0_CH2
#define GD32_DMAMUX1_TIMER0_CH3         DMA_REQUEST_TIMER0_CH3
#define GD32_DMAMUX1_TIMER0_MCH0        DMA_REQUEST_TIMER0_MCH0
#define GD32_DMAMUX1_TIMER0_MCH1        DMA_REQUEST_TIMER0_MCH1
#define GD32_DMAMUX1_TIMER0_MCH2        DMA_REQUEST_TIMER0_MCH2
#define GD32_DMAMUX1_TIMER0_MCH3        DMA_REQUEST_TIMER0_MCH3
#define GD32_DMAMUX1_TIMER0_UP          DMA_REQUEST_TIMER0_UP
#define GD32_DMAMUX1_TIMER0_TRG         DMA_REQUEST_TIMER0_TRG
#define GD32_DMAMUX1_TIMER0_CMT         DMA_REQUEST_TIMER0_CMT
/** @} */

/**
 * @name    DMAMUX request sources (TIMER1)
 * @{
 */
#define GD32_DMAMUX1_TIMER1_CH0         DMA_REQUEST_TIMER1_CH0
#define GD32_DMAMUX1_TIMER1_CH1         DMA_REQUEST_TIMER1_CH1
#define GD32_DMAMUX1_TIMER1_CH2         DMA_REQUEST_TIMER1_CH2
#define GD32_DMAMUX1_TIMER1_CH3         DMA_REQUEST_TIMER1_CH3
#define GD32_DMAMUX1_TIMER1_UP          DMA_REQUEST_TIMER1_UP
#define GD32_DMAMUX1_TIMER1_TRG         DMA_REQUEST_TIMER1_TRG
/** @} */

/**
 * @name    DMAMUX request sources (TIMER2)
 * @{
 */
#define GD32_DMAMUX1_TIMER2_CH0         DMA_REQUEST_TIMER2_CH0
#define GD32_DMAMUX1_TIMER2_CH1         DMA_REQUEST_TIMER2_CH1
#define GD32_DMAMUX1_TIMER2_CH2         DMA_REQUEST_TIMER2_CH2
#define GD32_DMAMUX1_TIMER2_CH3         DMA_REQUEST_TIMER2_CH3
#define GD32_DMAMUX1_TIMER2_UP          DMA_REQUEST_TIMER2_UP
#define GD32_DMAMUX1_TIMER2_TRG         DMA_REQUEST_TIMER2_TRG
/** @} */

/**
 * @name    DMAMUX request sources (TIMER3)
 * @{
 */
#define GD32_DMAMUX1_TIMER3_CH0         DMA_REQUEST_TIMER3_CH0
#define GD32_DMAMUX1_TIMER3_CH1         DMA_REQUEST_TIMER3_CH1
#define GD32_DMAMUX1_TIMER3_CH2         DMA_REQUEST_TIMER3_CH2
#define GD32_DMAMUX1_TIMER3_CH3         DMA_REQUEST_TIMER3_CH3
#define GD32_DMAMUX1_TIMER3_UP          DMA_REQUEST_TIMER3_UP
#define GD32_DMAMUX1_TIMER3_TRG         DMA_REQUEST_TIMER3_TRG
/** @} */

/**
 * @name    DMAMUX request sources (TIMER4)
 * @{
 */
#define GD32_DMAMUX1_TIMER4_CH0         DMA_REQUEST_TIMER4_CH0
#define GD32_DMAMUX1_TIMER4_CH1         DMA_REQUEST_TIMER4_CH1
#define GD32_DMAMUX1_TIMER4_CH2         DMA_REQUEST_TIMER4_CH2
#define GD32_DMAMUX1_TIMER4_CH3         DMA_REQUEST_TIMER4_CH3
#define GD32_DMAMUX1_TIMER4_UP          DMA_REQUEST_TIMER4_UP
#define GD32_DMAMUX1_TIMER4_TRG         DMA_REQUEST_TIMER4_TRG
/** @} */

/**
 * @name    DMAMUX request sources (TIMER5/TIMER6)
 * @{
 */
#define GD32_DMAMUX1_TIMER5_UP          DMA_REQUEST_TIMER5_UP
#define GD32_DMAMUX1_TIMER6_UP          DMA_REQUEST_TIMER6_UP
/** @} */

/**
 * @name    DMAMUX request sources (TIMER7)
 * @{
 */
#define GD32_DMAMUX1_TIMER7_CH0         DMA_REQUEST_TIMER7_CH0
#define GD32_DMAMUX1_TIMER7_CH1         DMA_REQUEST_TIMER7_CH1
#define GD32_DMAMUX1_TIMER7_CH2         DMA_REQUEST_TIMER7_CH2
#define GD32_DMAMUX1_TIMER7_CH3         DMA_REQUEST_TIMER7_CH3
#define GD32_DMAMUX1_TIMER7_MCH0        DMA_REQUEST_TIMER7_MCH0
#define GD32_DMAMUX1_TIMER7_MCH1        DMA_REQUEST_TIMER7_MCH1
#define GD32_DMAMUX1_TIMER7_MCH2        DMA_REQUEST_TIMER7_MCH2
#define GD32_DMAMUX1_TIMER7_MCH3        DMA_REQUEST_TIMER7_MCH3
#define GD32_DMAMUX1_TIMER7_UP          DMA_REQUEST_TIMER7_UP
#define GD32_DMAMUX1_TIMER7_TRG         DMA_REQUEST_TIMER7_TRG
#define GD32_DMAMUX1_TIMER7_CMT         DMA_REQUEST_TIMER7_CMT
/** @} */

/**
 * @name    DMAMUX request sources (TIMER14/TIMER15/TIMER16)
 * @{
 */
#define GD32_DMAMUX1_TIMER14_CH0        DMA_REQUEST_TIMER14_CH0
#define GD32_DMAMUX1_TIMER14_CH1        DMA_REQUEST_TIMER14_CH1
#define GD32_DMAMUX1_TIMER14_MCH0       DMA_REQUEST_TIMER14_MCH0
#define GD32_DMAMUX1_TIMER14_UP         DMA_REQUEST_TIMER14_UP
#define GD32_DMAMUX1_TIMER14_TRG        DMA_REQUEST_TIMER14_TRG
#define GD32_DMAMUX1_TIMER14_CMT        DMA_REQUEST_TIMER14_CMT
#define GD32_DMAMUX1_TIMER15_CH0        DMA_REQUEST_TIMER15_CH0
#define GD32_DMAMUX1_TIMER15_MCH0       DMA_REQUEST_TIMER15_MCH0
#define GD32_DMAMUX1_TIMER15_UP         DMA_REQUEST_TIMER15_UP
#define GD32_DMAMUX1_TIMER16_CH0        DMA_REQUEST_TIMER16_CH0
#define GD32_DMAMUX1_TIMER16_MCH0       DMA_REQUEST_TIMER16_MCH0
#define GD32_DMAMUX1_TIMER16_UP         DMA_REQUEST_TIMER16_UP
/** @} */

/**
 * @name    DMAMUX request sources (TIMER22/TIMER23)
 * @{
 */
#define GD32_DMAMUX1_TIMER22_CH0        DMA_REQUEST_TIMER22_CH0
#define GD32_DMAMUX1_TIMER22_CH1        DMA_REQUEST_TIMER22_CH1
#define GD32_DMAMUX1_TIMER22_CH2        DMA_REQUEST_TIMER22_CH2
#define GD32_DMAMUX1_TIMER22_CH3        DMA_REQUEST_TIMER22_CH3
#define GD32_DMAMUX1_TIMER22_UP         DMA_REQUEST_TIMER22_UP
#define GD32_DMAMUX1_TIMER22_TRG        DMA_REQUEST_TIMER22_TRG
#define GD32_DMAMUX1_TIMER23_CH0        DMA_REQUEST_TIMER23_CH0
#define GD32_DMAMUX1_TIMER23_CH1        DMA_REQUEST_TIMER23_CH1
#define GD32_DMAMUX1_TIMER23_CH2        DMA_REQUEST_TIMER23_CH2
#define GD32_DMAMUX1_TIMER23_CH3        DMA_REQUEST_TIMER23_CH3
#define GD32_DMAMUX1_TIMER23_UP         DMA_REQUEST_TIMER23_UP
#define GD32_DMAMUX1_TIMER23_TRG        DMA_REQUEST_TIMER23_TRG
/** @} */

/**
 * @name    DMAMUX request sources (TIMER30/TIMER31)
 * @{
 */
#if !defined(GD32H75E)
#define GD32_DMAMUX1_TIMER30_CH0        DMA_REQUEST_TIMER30_CH0
#define GD32_DMAMUX1_TIMER30_CH1        DMA_REQUEST_TIMER30_CH1
#define GD32_DMAMUX1_TIMER30_CH2        DMA_REQUEST_TIMER30_CH2
#define GD32_DMAMUX1_TIMER30_CH3        DMA_REQUEST_TIMER30_CH3
#define GD32_DMAMUX1_TIMER30_UP         DMA_REQUEST_TIMER30_UP
#define GD32_DMAMUX1_TIMER30_TRG        DMA_REQUEST_TIMER30_TRG
#define GD32_DMAMUX1_TIMER31_CH0        DMA_REQUEST_TIMER31_CH0
#define GD32_DMAMUX1_TIMER31_CH1        DMA_REQUEST_TIMER31_CH1
#define GD32_DMAMUX1_TIMER31_CH2        DMA_REQUEST_TIMER31_CH2
#define GD32_DMAMUX1_TIMER31_CH3        DMA_REQUEST_TIMER31_CH3
#define GD32_DMAMUX1_TIMER31_UP         DMA_REQUEST_TIMER31_UP
#define GD32_DMAMUX1_TIMER31_TRG        DMA_REQUEST_TIMER31_TRG
#endif
/** @} */

/**
 * @name    DMAMUX request sources (TIMER40..TIMER44)
 * @{
 */
#define GD32_DMAMUX1_TIMER40_CH0        DMA_REQUEST_TIMER40_CH0
#define GD32_DMAMUX1_TIMER40_CH1        DMA_REQUEST_TIMER40_CH1
#define GD32_DMAMUX1_TIMER40_MCH0       DMA_REQUEST_TIMER40_MCH0
#define GD32_DMAMUX1_TIMER40_CMT        DMA_REQUEST_TIMER40_CMT
#define GD32_DMAMUX1_TIMER40_UP         DMA_REQUEST_TIMER40_UP
#define GD32_DMAMUX1_TIMER40_TRG        DMA_REQUEST_TIMER40_TRG
#define GD32_DMAMUX1_TIMER41_CH0        DMA_REQUEST_TIMER41_CH0
#define GD32_DMAMUX1_TIMER41_CH1        DMA_REQUEST_TIMER41_CH1
#define GD32_DMAMUX1_TIMER41_MCH0       DMA_REQUEST_TIMER41_MCH0
#define GD32_DMAMUX1_TIMER41_CMT        DMA_REQUEST_TIMER41_CMT
#define GD32_DMAMUX1_TIMER41_UP         DMA_REQUEST_TIMER41_UP
#define GD32_DMAMUX1_TIMER41_TRG        DMA_REQUEST_TIMER41_TRG
#define GD32_DMAMUX1_TIMER42_CH0        DMA_REQUEST_TIMER42_CH0
#define GD32_DMAMUX1_TIMER42_CH1        DMA_REQUEST_TIMER42_CH1
#define GD32_DMAMUX1_TIMER42_MCH0       DMA_REQUEST_TIMER42_MCH0
#define GD32_DMAMUX1_TIMER42_CMT        DMA_REQUEST_TIMER42_CMT
#define GD32_DMAMUX1_TIMER42_UP         DMA_REQUEST_TIMER42_UP
#define GD32_DMAMUX1_TIMER42_TRG        DMA_REQUEST_TIMER42_TRG
#define GD32_DMAMUX1_TIMER43_CH0        DMA_REQUEST_TIMER43_CH0
#define GD32_DMAMUX1_TIMER43_CH1        DMA_REQUEST_TIMER43_CH1
#define GD32_DMAMUX1_TIMER43_MCH0       DMA_REQUEST_TIMER43_MCH0
#define GD32_DMAMUX1_TIMER43_CMT        DMA_REQUEST_TIMER43_CMT
#define GD32_DMAMUX1_TIMER43_UP         DMA_REQUEST_TIMER43_UP
#define GD32_DMAMUX1_TIMER43_TRG        DMA_REQUEST_TIMER43_TRG
#define GD32_DMAMUX1_TIMER44_CH0        DMA_REQUEST_TIMER44_CH0
#define GD32_DMAMUX1_TIMER44_CH1        DMA_REQUEST_TIMER44_CH1
#define GD32_DMAMUX1_TIMER44_MCH0       DMA_REQUEST_TIMER44_MCH0
#define GD32_DMAMUX1_TIMER44_CMT        DMA_REQUEST_TIMER44_CMT
#define GD32_DMAMUX1_TIMER44_UP         DMA_REQUEST_TIMER44_UP
#define GD32_DMAMUX1_TIMER44_TRG        DMA_REQUEST_TIMER44_TRG
/** @} */

/**
 * @name    DMAMUX request sources (TIMER50/TIMER51)
 * @{
 */
#define GD32_DMAMUX1_TIMER50_UP         DMA_REQUEST_TIMER50_UP
#define GD32_DMAMUX1_TIMER51_UP         DMA_REQUEST_TIMER51_UP
/** @} */

/**
 * @name    DMAMUX request sources (USART/UART)
 * @{
 */
#define GD32_DMAMUX1_USART0_RX          DMA_REQUEST_USART0_RX
#define GD32_DMAMUX1_USART0_TX          DMA_REQUEST_USART0_TX
#define GD32_DMAMUX1_USART1_RX          DMA_REQUEST_USART1_RX
#define GD32_DMAMUX1_USART1_TX          DMA_REQUEST_USART1_TX
#define GD32_DMAMUX1_USART2_RX          DMA_REQUEST_USART2_RX
#define GD32_DMAMUX1_USART2_TX          DMA_REQUEST_USART2_TX
#define GD32_DMAMUX1_UART3_RX           DMA_REQUEST_UART3_RX
#define GD32_DMAMUX1_UART3_TX           DMA_REQUEST_UART3_TX
#define GD32_DMAMUX1_UART4_RX           DMA_REQUEST_UART4_RX
#define GD32_DMAMUX1_UART4_TX           DMA_REQUEST_UART4_TX
#define GD32_DMAMUX1_USART5_RX          DMA_REQUEST_USART5_RX
#define GD32_DMAMUX1_USART5_TX          DMA_REQUEST_USART5_TX
#define GD32_DMAMUX1_UART6_RX           DMA_REQUEST_UART6_RX
#define GD32_DMAMUX1_UART6_TX           DMA_REQUEST_UART6_TX
#define GD32_DMAMUX1_UART7_RX           DMA_REQUEST_UART7_RX
#define GD32_DMAMUX1_UART7_TX           DMA_REQUEST_UART7_TX
/** @} */

/**
 * @name    DMAMUX request sources (SPI)
 * @{
 */
#define GD32_DMAMUX1_SPI0_RX            DMA_REQUEST_SPI0_RX
#define GD32_DMAMUX1_SPI0_TX            DMA_REQUEST_SPI0_TX
#define GD32_DMAMUX1_SPI1_RX            DMA_REQUEST_SPI1_RX
#define GD32_DMAMUX1_SPI1_TX            DMA_REQUEST_SPI1_TX
#define GD32_DMAMUX1_SPI2_RX            DMA_REQUEST_SPI2_RX
#define GD32_DMAMUX1_SPI2_TX            DMA_REQUEST_SPI2_TX
#define GD32_DMAMUX1_SPI3_RX            DMA_REQUEST_SPI3_RX
#define GD32_DMAMUX1_SPI3_TX            DMA_REQUEST_SPI3_TX
#define GD32_DMAMUX1_SPI4_RX            DMA_REQUEST_SPI4_RX
#define GD32_DMAMUX1_SPI4_TX            DMA_REQUEST_SPI4_TX
#define GD32_DMAMUX1_SPI5_RX            DMA_REQUEST_SPI5_RX
#define GD32_DMAMUX1_SPI5_TX            DMA_REQUEST_SPI5_TX
/** @} */

/**
 * @name    DMAMUX request sources (I2C)
 * @{
 */
#define GD32_DMAMUX1_I2C0_RX            DMA_REQUEST_I2C0_RX
#define GD32_DMAMUX1_I2C0_TX            DMA_REQUEST_I2C0_TX
#define GD32_DMAMUX1_I2C1_RX            DMA_REQUEST_I2C1_RX
#define GD32_DMAMUX1_I2C1_TX            DMA_REQUEST_I2C1_TX
#define GD32_DMAMUX1_I2C2_RX            DMA_REQUEST_I2C2_RX
#define GD32_DMAMUX1_I2C2_TX            DMA_REQUEST_I2C2_TX
#define GD32_DMAMUX1_I2C3_RX            DMA_REQUEST_I2C3_RX
#define GD32_DMAMUX1_I2C3_TX            DMA_REQUEST_I2C3_TX
/** @} */

/**
 * @name    DMAMUX request sources (DAC)
 * @{
 */
#define GD32_DMAMUX1_DAC_CH0            DMA_REQUEST_DAC_CH0
#define GD32_DMAMUX1_DAC_CH1            DMA_REQUEST_DAC_CH1
/** @} */

/**
 * @name    DMAMUX request sources (DCI/CAU/HAU)
 * @{
 */
#if !defined(GD32H75E)
#define GD32_DMAMUX1_DCI                DMA_REQUEST_DCI
#define GD32_DMAMUX1_CAU_IN             DMA_REQUEST_CAU_IN
#define GD32_DMAMUX1_CAU_OUT            DMA_REQUEST_CAU_OUT
#define GD32_DMAMUX1_HAU_IN             DMA_REQUEST_HAU_IN
#endif
/** @} */

/**
 * @name    DMAMUX request sources (SAI/RSPDIF)
 * @{
 */
#if !defined(GD32H75E)
#define GD32_DMAMUX1_SAI0_B0            DMA_REQUEST_SAI0_B0
#define GD32_DMAMUX1_SAI0_B1            DMA_REQUEST_SAI0_B1
#define GD32_DMAMUX1_SAI1_B0            DMA_REQUEST_SAI1_B0
#define GD32_DMAMUX1_SAI1_B1            DMA_REQUEST_SAI1_B1
#define GD32_DMAMUX1_SAI2_B0            DMA_REQUEST_SAI2_B0
#define GD32_DMAMUX1_SAI2_B1            DMA_REQUEST_SAI2_B1
#define GD32_DMAMUX1_RSPDIF_DATA        DMA_REQUEST_RSPDIF_DATA
#define GD32_DMAMUX1_RSPDIF_CS          DMA_REQUEST_RSPDIF_CS
#endif
/** @} */

/**
 * @name    DMAMUX request sources (HPDF)
 * @{
 */
#define GD32_DMAMUX1_HPDF_FLT0          DMA_REQUEST_HPDF_FLT0
#define GD32_DMAMUX1_HPDF_FLT1          DMA_REQUEST_HPDF_FLT1
#define GD32_DMAMUX1_HPDF_FLT2          DMA_REQUEST_HPDF_FLT2
#define GD32_DMAMUX1_HPDF_FLT3          DMA_REQUEST_HPDF_FLT3
/** @} */

/**
 * @name    DMAMUX request sources (FAC/TMU)
 * @{
 */
#define GD32_DMAMUX1_FAC_READ           DMA_REQUEST_FAC_READ
#define GD32_DMAMUX1_FAC_WRITE          DMA_REQUEST_FAC_WRITE
#define GD32_DMAMUX1_TMU_INPUT          DMA_REQUEST_TMU_INPUT
#define GD32_DMAMUX1_TMU_OUTPUT         DMA_REQUEST_TMU_OUTPUT
/** @} */

/**
 * @name    DMAMUX request sources (CAN)
 * @{
 */
#define GD32_DMAMUX1_CAN0               DMA_REQUEST_CAN0
#define GD32_DMAMUX1_CAN1               DMA_REQUEST_CAN1
#define GD32_DMAMUX1_CAN2               DMA_REQUEST_CAN2
/** @} */

/**
 * @name    DMAMUX request generator trigger inputs
 * @{
 */
#define GD32_DMAMUX1_TRIG_EVT0_OUT      DMAMUX_TRIGGER_EVT0_OUT
#define GD32_DMAMUX1_TRIG_EVT1_OUT      DMAMUX_TRIGGER_EVT1_OUT
#define GD32_DMAMUX1_TRIG_EVT2_OUT      DMAMUX_TRIGGER_EVT2_OUT
#define GD32_DMAMUX1_TRIG_EVT3_OUT      DMAMUX_TRIGGER_EVT3_OUT
#define GD32_DMAMUX1_TRIG_EVT4_OUT      DMAMUX_TRIGGER_EVT4_OUT
#define GD32_DMAMUX1_TRIG_EVT5_OUT      DMAMUX_TRIGGER_EVT5_OUT
#define GD32_DMAMUX1_TRIG_EVT6_OUT      DMAMUX_TRIGGER_EVT6_OUT
#define GD32_DMAMUX1_TRIG_EXTI0         DMAMUX_TRIGGER_EXTI0
#define GD32_DMAMUX1_TRIG_EXTI1         DMAMUX_TRIGGER_EXTI1
#define GD32_DMAMUX1_TRIG_EXTI2         DMAMUX_TRIGGER_EXTI2
#define GD32_DMAMUX1_TRIG_EXTI3         DMAMUX_TRIGGER_EXTI3
#define GD32_DMAMUX1_TRIG_EXTI4         DMAMUX_TRIGGER_EXTI4
#define GD32_DMAMUX1_TRIG_EXTI5         DMAMUX_TRIGGER_EXTI5
#define GD32_DMAMUX1_TRIG_EXTI6         DMAMUX_TRIGGER_EXTI6
#define GD32_DMAMUX1_TRIG_EXTI7         DMAMUX_TRIGGER_EXTI7
#define GD32_DMAMUX1_TRIG_EXTI8         DMAMUX_TRIGGER_EXTI8
#define GD32_DMAMUX1_TRIG_EXTI9         DMAMUX_TRIGGER_EXTI9
#define GD32_DMAMUX1_TRIG_EXTI10        DMAMUX_TRIGGER_EXTI10
#define GD32_DMAMUX1_TRIG_EXTI11        DMAMUX_TRIGGER_EXTI11
#define GD32_DMAMUX1_TRIG_EXTI12        DMAMUX_TRIGGER_EXTI12
#define GD32_DMAMUX1_TRIG_EXTI13        DMAMUX_TRIGGER_EXTI13
#define GD32_DMAMUX1_TRIG_EXTI14        DMAMUX_TRIGGER_EXTI14
#define GD32_DMAMUX1_TRIG_EXTI15        DMAMUX_TRIGGER_EXTI15
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

#ifdef __cplusplus
}
#endif

#endif /* GD32_DMAMUX_H */

/** @} */
