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
 * @file    GD32H73x_75x/gd32_registry.h
 * @brief   GD32H73x_75x capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef GD32_REGISTRY_H
#define GD32_REGISTRY_H

#if !defined(GD32H73X_75X) && !defined(GD32H75E)
#error "Please select the target GD32H73X_75X or GD32H75E device used in your application (in gd32h73x_75x.h file)"
#endif

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    GD32H73x_75x capabilities
 * @{
 */

/*===========================================================================*/
/* Common.                                                                   */
/*===========================================================================*/

/* Core attributes. */
#define GD32_HAS_ICACHE                     TRUE
#define GD32_HAS_DCACHE                     TRUE
#define GD32_HAS_RAMECCMU                   TRUE

/* ADC attributes.
   ADC0 and ADC1 share a single vector, ADC2 has its own. */
#define GD32_HAS_ADC0                       TRUE
#define GD32_HAS_ADC1                       TRUE
#define GD32_HAS_ADC2                       TRUE

#define GD32_ADC0_CHANNELS                  21
#define GD32_ADC1_CHANNELS                  21
#define GD32_ADC2_CHANNELS                  21

/* The 14 bits mode is only available on ADC0/ADC1 and the 6 bits mode only
   on ADC2, so the DRES field encoding is shifted by one step on ADC2. */
#define GD32_ADC0_SUPPORTS_14BIT            TRUE
#define GD32_ADC1_SUPPORTS_14BIT            TRUE
#define GD32_ADC2_SUPPORTS_14BIT            FALSE
#define GD32_ADC0_SUPPORTS_6BIT             FALSE
#define GD32_ADC1_SUPPORTS_6BIT             FALSE
#define GD32_ADC2_SUPPORTS_6BIT             TRUE

/* Maximum RSMPN/ISMPN sample time value, in CK_ADC cycles. */
#define GD32_ADC0_MAX_SAMPLE_TIME           807
#define GD32_ADC1_MAX_SAMPLE_TIME           807
#define GD32_ADC2_MAX_SAMPLE_TIME           638

/* ADC0 and ADC1 can be paired through ADC_SYNCCTL. */
#define GD32_ADC_SUPPORTS_SYNC              TRUE

/* CAN attributes.
   The three controllers are mailbox based, use a dedicated message RAM and
   support CAN FD, so FDCAN class drivers are the closest reference. */
#define GD32_HAS_CAN0                       TRUE
#define GD32_HAS_CAN1                       TRUE
#define GD32_HAS_CAN2                       TRUE
#define GD32_CAN_SUPPORTS_FD                TRUE

/* Each controller exposes seven separate vectors instead of the more common
   TX/RX0/RX1/SCE grouping. */
#define GD32_CAN_VECTORS_PER_UNIT           7

/* CMP attributes. */
#define GD32_HAS_CMP0                       TRUE
#define GD32_HAS_CMP1                       TRUE

/* CRC attributes. */
#define GD32_HAS_CRC                        TRUE

/* CTC attributes. */
#define GD32_HAS_CTC                        TRUE

/* DAC attributes.
   A single unit with two independent DMA capable outputs. */
#define GD32_HAS_DAC0                       TRUE
#define GD32_DAC0_OUTPUTS                   2

/* DCI attributes. */
#if defined(GD32H75E)
#define GD32_HAS_DCI                        FALSE
#else
#define GD32_HAS_DCI                        TRUE
#endif

/* DMA attributes.
   Two general purpose controllers with eight channels each and one vector per
   channel. This family has no BDMA. */
#define GD32_ADVANCED_DMA                   TRUE
#define GD32_HAS_DMA0                       TRUE
#define GD32_HAS_DMA1                       TRUE
#define GD32_HAS_BDMA                       FALSE
#define GD32_DMA_NUM_UNITS                  2
#define GD32_DMA_CHANNELS_PER_UNIT          8

/* DMAMUX attributes.
   Request routing goes through DMAMUX, so channel to peripheral assignment is
   free and only the request identifier matters. */
#define GD32_HAS_DMAMUX                     TRUE
#define GD32_DMA_SUPPORTS_DMAMUX            TRUE
#define GD32_DMAMUX_REQ_GENERATORS          8

/* MDMA attributes.
   Sixteen channels sharing a single vector. */
#define GD32_HAS_MDMA                       TRUE
#define GD32_MDMA_CHANNELS                  16

/* EFUSE attributes. */
#define GD32_HAS_EFUSE                      TRUE

/* ENET attributes. */
#if defined(GD32H75E)
#define GD32_HAS_ENET0                      FALSE
#define GD32_HAS_ENET1                      FALSE
#else
#define GD32_HAS_ENET0                      TRUE
#define GD32_HAS_ENET1                      TRUE
#endif

/* EXMC attributes. */
#define GD32_HAS_EXMC                       TRUE

/* EXTI attributes.
   Lines 0..15 are the GPIO mapped ones, the rest are peripheral wakeup
   sources spread over two register groups. */
#define GD32_HAS_EXTI                       TRUE
#define GD32_EXTI_NUM_LINES                 38
#define GD32_EXTI_GPIO_LINES                16
#define GD32_EXTI_REGISTER_GROUPS           2
#define GD32_EXTI_HAS_GROUP2                TRUE
/* Lines 16..31 are peripheral wakeup sources (PVD/LVD, RTC, USB, ...) owned
   by their own drivers, so the generic EXTI helper must not configure them.
   Lines 17 (alarm), 18 (tamper/timestamp) and 19 (wakeup) are excluded from
   the mask because the RTC driver has to program their edge selection. */
#define GD32_EXTI_INTEN0_MASK               0xFFF10000U
/* Lines 32..37 are peripheral wakeup sources, same restriction. */
#define GD32_EXTI_INTEN1_MASK               0x0000003FU

/* FAC attributes. */
#define GD32_HAS_FAC                        TRUE

/* FMC attributes. */
#define GD32_HAS_FMC                        TRUE

/* GPIO attributes.
   GPIOI is not implemented on this family, the port letter is skipped. */
#define GD32_HAS_GPIOA                      TRUE
#define GD32_HAS_GPIOB                      TRUE
#define GD32_HAS_GPIOC                      TRUE
#define GD32_HAS_GPIOD                      TRUE
#define GD32_HAS_GPIOE                      TRUE
#define GD32_HAS_GPIOF                      TRUE
#define GD32_HAS_GPIOG                      TRUE
#define GD32_HAS_GPIOH                      TRUE
#define GD32_HAS_GPIOI                      FALSE
#define GD32_HAS_GPIOJ                      TRUE
#define GD32_HAS_GPIOK                      TRUE

/* HPDF attributes. */
#define GD32_HAS_HPDF                       TRUE
#define GD32_HPDF_FILTERS                   4

/* HWSEM attributes. */
#if defined(GD32H75E)
#define GD32_HAS_HWSEM                      FALSE
#else
#define GD32_HAS_HWSEM                      TRUE
#endif

/* I2C attributes.
   Every unit has separate event, error and wakeup vectors. */
#define GD32_HAS_I2C0                       TRUE
#define GD32_HAS_I2C1                       TRUE
#define GD32_HAS_I2C2                       TRUE
#define GD32_HAS_I2C3                       TRUE

/* IPA attributes. */
#if defined(GD32H75E)
#define GD32_HAS_IPA                        FALSE
#else
#define GD32_HAS_IPA                        TRUE
#endif

/* LPDTS attributes. */
#define GD32_HAS_LPDTS                      TRUE

/* MDIO attributes. */
#if defined(GD32H75E)
#define GD32_HAS_MDIO                       FALSE
#else
#define GD32_HAS_MDIO                       TRUE
#endif

/* OSPI attributes. */
#define GD32_HAS_OSPI0                      TRUE
#define GD32_HAS_OSPI1                      TRUE

/* RSPDIF attributes. */
#if defined(GD32H75E)
#define GD32_HAS_RSPDIF                     FALSE
#else
#define GD32_HAS_RSPDIF                     TRUE
#endif

/* RTC attributes. */
#define GD32_HAS_RTC                        TRUE
#define GD32_RTC_HAS_SUBSECONDS             TRUE
#define GD32_RTC_HAS_PERIODIC_WAKEUPS       TRUE
#define GD32_RTC_NUM_ALARMS                 2
#define GD32_RTC_STORAGE_SIZE               128
#define GD32_RTC_TAMP_STAMP_EXTI            18
#define GD32_RTC_WKUP_EXTI                  19
#define GD32_RTC_ALARM_EXTI                 17
#define GD32_RTC_IRQ_ENABLE() do {                                          \
  nvicEnableVector(GD32_TAMPER_STAMP_LXTAL_NUMBER,                          \
                   GD32_IRQ_EXTI18_PRIORITY);                               \
  nvicEnableVector(GD32_RTC_WKUP_NUMBER, GD32_IRQ_EXTI19_PRIORITY);         \
  nvicEnableVector(GD32_RTC_ALARM_NUMBER, GD32_IRQ_EXTI17_PRIORITY);        \
} while (false)

/* RTDEC attributes. */
#if defined(GD32H75E)
#define GD32_HAS_RTDEC0                     FALSE
#define GD32_HAS_RTDEC1                     FALSE
#else
#define GD32_HAS_RTDEC0                     TRUE
#define GD32_HAS_RTDEC1                     TRUE
#endif

/* SAI attributes. */
#if defined(GD32H75E)
#define GD32_HAS_SAI0                       FALSE
#define GD32_HAS_SAI1                       FALSE
#define GD32_HAS_SAI2                       FALSE
#else
#define GD32_HAS_SAI0                       TRUE
#define GD32_HAS_SAI1                       TRUE
#define GD32_HAS_SAI2                       TRUE
#endif

/* SDIO attributes. */
#if defined(GD32H75E)
#define GD32_HAS_SDIO0                      FALSE
#define GD32_HAS_SDIO1                      FALSE
#else
#define GD32_HAS_SDIO0                      TRUE
#define GD32_HAS_SDIO1                      TRUE
#endif

/* SPI attributes.
   I2S mode is only wired on SPI0, SPI1, SPI2 and SPI5. */
#define GD32_HAS_SPI0                       TRUE
#define GD32_SPI0_SUPPORTS_I2S              TRUE
#define GD32_HAS_SPI1                       TRUE
#define GD32_SPI1_SUPPORTS_I2S              TRUE
#define GD32_HAS_SPI2                       TRUE
#define GD32_SPI2_SUPPORTS_I2S              TRUE
#define GD32_HAS_SPI3                       TRUE
#define GD32_SPI3_SUPPORTS_I2S              FALSE
#define GD32_HAS_SPI4                       TRUE
#define GD32_SPI4_SUPPORTS_I2S              FALSE
#define GD32_HAS_SPI5                       TRUE
#define GD32_SPI5_SUPPORTS_I2S              TRUE

/* TIMER attributes.
   Advanced timers own a repetition counter, break inputs (BREAK0 and BREAK1)
   and complementary outputs. Only TIMER0 and TIMER7 belong to this class on
   this family. Basic timers have no capture/compare channel at all. */
#define GD32_TIMER_MAX_CHANNELS             4

#define GD32_HAS_TIMER0                     TRUE
#define GD32_TIMER0_IS_32BITS               FALSE
#define GD32_TIMER0_IS_ADVANCED             TRUE
#define GD32_TIMER0_CHANNELS                4
#define GD32_TIMER0_COMPLEMENTARY_CHANNELS  4

#define GD32_HAS_TIMER1                     TRUE
#define GD32_TIMER1_IS_32BITS               TRUE
#define GD32_TIMER1_IS_ADVANCED             FALSE
#define GD32_TIMER1_CHANNELS                4
#define GD32_TIMER1_COMPLEMENTARY_CHANNELS  0

#define GD32_HAS_TIMER2                     TRUE
#define GD32_TIMER2_IS_32BITS               FALSE
#define GD32_TIMER2_IS_ADVANCED             FALSE
#define GD32_TIMER2_CHANNELS                4
#define GD32_TIMER2_COMPLEMENTARY_CHANNELS  0

#define GD32_HAS_TIMER3                     TRUE
#define GD32_TIMER3_IS_32BITS               FALSE
#define GD32_TIMER3_IS_ADVANCED             FALSE
#define GD32_TIMER3_CHANNELS                4
#define GD32_TIMER3_COMPLEMENTARY_CHANNELS  0

#define GD32_HAS_TIMER4                     TRUE
#define GD32_TIMER4_IS_32BITS               TRUE
#define GD32_TIMER4_IS_ADVANCED             FALSE
#define GD32_TIMER4_CHANNELS                4
#define GD32_TIMER4_COMPLEMENTARY_CHANNELS  0

#define GD32_HAS_TIMER5                     TRUE
#define GD32_TIMER5_IS_32BITS               TRUE
#define GD32_TIMER5_IS_ADVANCED             FALSE
#define GD32_TIMER5_CHANNELS                0
#define GD32_TIMER5_COMPLEMENTARY_CHANNELS  0

#define GD32_HAS_TIMER6                     TRUE
#define GD32_TIMER6_IS_32BITS               TRUE
#define GD32_TIMER6_IS_ADVANCED             FALSE
#define GD32_TIMER6_CHANNELS                0
#define GD32_TIMER6_COMPLEMENTARY_CHANNELS  0

#define GD32_HAS_TIMER7                     TRUE
#define GD32_TIMER7_IS_32BITS               FALSE
#define GD32_TIMER7_IS_ADVANCED             TRUE
#define GD32_TIMER7_CHANNELS                4
#define GD32_TIMER7_COMPLEMENTARY_CHANNELS  4

#define GD32_HAS_TIMER14                    TRUE
#define GD32_TIMER14_IS_32BITS              FALSE
#define GD32_TIMER14_IS_ADVANCED            FALSE
#define GD32_TIMER14_CHANNELS               2
#define GD32_TIMER14_COMPLEMENTARY_CHANNELS 2

#define GD32_HAS_TIMER15                    TRUE
#define GD32_TIMER15_IS_32BITS              FALSE
#define GD32_TIMER15_IS_ADVANCED            FALSE
#define GD32_TIMER15_CHANNELS               1
#define GD32_TIMER15_COMPLEMENTARY_CHANNELS 1

#define GD32_HAS_TIMER16                    TRUE
#define GD32_TIMER16_IS_32BITS              FALSE
#define GD32_TIMER16_IS_ADVANCED            FALSE
#define GD32_TIMER16_CHANNELS               1
#define GD32_TIMER16_COMPLEMENTARY_CHANNELS 1

#define GD32_HAS_TIMER22                    TRUE
#define GD32_TIMER22_IS_32BITS              TRUE
#define GD32_TIMER22_IS_ADVANCED            FALSE
#define GD32_TIMER22_CHANNELS               4
#define GD32_TIMER22_COMPLEMENTARY_CHANNELS 0

#define GD32_HAS_TIMER23                    TRUE
#define GD32_TIMER23_IS_32BITS              TRUE
#define GD32_TIMER23_IS_ADVANCED            FALSE
#define GD32_TIMER23_CHANNELS               4
#define GD32_TIMER23_COMPLEMENTARY_CHANNELS 0

#if defined(GD32H75E)
#define GD32_HAS_TIMER30                    FALSE
#define GD32_HAS_TIMER31                    FALSE
#else
#define GD32_HAS_TIMER30                    TRUE
#define GD32_TIMER30_IS_32BITS              FALSE
#define GD32_TIMER30_IS_ADVANCED            FALSE
#define GD32_TIMER30_CHANNELS               4
#define GD32_TIMER30_COMPLEMENTARY_CHANNELS 0

#define GD32_HAS_TIMER31                    TRUE
#define GD32_TIMER31_IS_32BITS              FALSE
#define GD32_TIMER31_IS_ADVANCED            FALSE
#define GD32_TIMER31_CHANNELS               4
#define GD32_TIMER31_COMPLEMENTARY_CHANNELS 0
#endif

#define GD32_HAS_TIMER40                    TRUE
#define GD32_TIMER40_IS_32BITS              FALSE
#define GD32_TIMER40_IS_ADVANCED            FALSE
#define GD32_TIMER40_CHANNELS               2
#define GD32_TIMER40_COMPLEMENTARY_CHANNELS 2

#define GD32_HAS_TIMER41                    TRUE
#define GD32_TIMER41_IS_32BITS              FALSE
#define GD32_TIMER41_IS_ADVANCED            FALSE
#define GD32_TIMER41_CHANNELS               2
#define GD32_TIMER41_COMPLEMENTARY_CHANNELS 2

#define GD32_HAS_TIMER42                    TRUE
#define GD32_TIMER42_IS_32BITS              FALSE
#define GD32_TIMER42_IS_ADVANCED            FALSE
#define GD32_TIMER42_CHANNELS               2
#define GD32_TIMER42_COMPLEMENTARY_CHANNELS 2

#define GD32_HAS_TIMER43                    TRUE
#define GD32_TIMER43_IS_32BITS              FALSE
#define GD32_TIMER43_IS_ADVANCED            FALSE
#define GD32_TIMER43_CHANNELS               2
#define GD32_TIMER43_COMPLEMENTARY_CHANNELS 2

#define GD32_HAS_TIMER44                    TRUE
#define GD32_TIMER44_IS_32BITS              FALSE
#define GD32_TIMER44_IS_ADVANCED            FALSE
#define GD32_TIMER44_CHANNELS               2
#define GD32_TIMER44_COMPLEMENTARY_CHANNELS 2

/* TIMER50 and TIMER51 carry a 64 bit counter split over the CNTL/CNTH and
   CARL/CARH register pairs, so they do not fit the 32 bit classification. */
#define GD32_HAS_TIMER50                    TRUE
#define GD32_TIMER50_IS_32BITS              FALSE
#define GD32_TIMER50_IS_64BITS              TRUE
#define GD32_TIMER50_IS_ADVANCED            FALSE
#define GD32_TIMER50_CHANNELS               0
#define GD32_TIMER50_COMPLEMENTARY_CHANNELS 0

#define GD32_HAS_TIMER51                    TRUE
#define GD32_TIMER51_IS_32BITS              FALSE
#define GD32_TIMER51_IS_64BITS              TRUE
#define GD32_TIMER51_IS_ADVANCED            FALSE
#define GD32_TIMER51_CHANNELS               0
#define GD32_TIMER51_COMPLEMENTARY_CHANNELS 0

/* TLI attributes. */
#if defined(GD32H75E)
#define GD32_HAS_TLI                        FALSE
#else
#define GD32_HAS_TLI                        TRUE
#endif

/* TMU attributes. */
#define GD32_HAS_TMU                        TRUE

/* Cryptographic attributes.
   On GD32H73x/75x the TRNG vector is shared with HAU. */
#define GD32_HAS_TRNG                       TRUE
#if defined(GD32H75E)
#define GD32_HAS_CAU                        FALSE
#define GD32_HAS_HAU                        FALSE
#else
#define GD32_HAS_CAU                        TRUE
#define GD32_HAS_HAU                        TRUE
#endif

/* USART/UART attributes. */
#define GD32_HAS_USART0                     TRUE
#define GD32_HAS_USART1                     TRUE
#define GD32_HAS_USART2                     TRUE
#define GD32_HAS_UART3                      TRUE
#define GD32_HAS_UART4                      TRUE
#define GD32_HAS_USART5                     TRUE
#define GD32_HAS_UART6                      TRUE
#define GD32_HAS_UART7                      TRUE

/* USB attributes.
   Two high speed OTG cores, each with dedicated endpoint 1 and wakeup vectors
   on top of the global one. Endpoint counts exclude the endpoint zero, the
   FIFO memory sizes are expressed in words. */
#define GD32_HAS_USBHS0                     TRUE
#define GD32_USBHS0_ENDPOINTS               7
#define GD32_USBHS0_FIFO_MEM_SIZE           1280
#define GD32_HAS_USBHS1                     TRUE
#define GD32_USBHS1_ENDPOINTS               7
#define GD32_USBHS1_FIFO_MEM_SIZE           1280

/* WDG attributes. */
#define GD32_HAS_FWDGT                      TRUE
#define GD32_HAS_WWDGT                      TRUE

/** @} */

#endif /* GD32_REGISTRY_H */

/** @} */
