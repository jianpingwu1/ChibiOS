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
 * @file    DMAv2/gd32_dma.h
 * @brief   Enhanced-DMA helper driver header.
 *
 * @addtogroup GD32_DMA
 * @{
 */

#ifndef GD32_DMA_H
#define GD32_DMA_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   DMA capability.
 * @details if @p TRUE then the DMA is able of burst transfers, FIFOs,
 *          scatter gather and other advanced features.
 */
#define GD32_DMA_ADVANCED          TRUE

/**
 * @brief   Maximum number of transfers in a single operation.
 */
#define GD32_DMA_MAX_TRANSFER      65535

/**
 * @brief   Total number of DMA streams.
 * @details This is the total number of streams among all the DMA units.
 */
#if !defined(GD32_DMA_STREAMS) || defined(__DOXYGEN__)
#define GD32_DMA_STREAMS           16U
#endif

/**
 * @brief   Mask of the ISR bits passed to the DMA callback functions.
 */
#define GD32_DMA_ISR_MASK          0x3DU

/**
 * @brief   Returns the channel associated to the specified stream.
 *
 * @param[in] id        the unique numeric stream identifier
 * @param[in] c         a stream/channel association word, one channel per
 *                      nibble
 * @return              Returns the channel associated to the stream.
 */
#define GD32_DMA_GETCHANNEL(id, c) (((c) >> (((id) & 7U) * 4U)) & 15U)

/**
 * @brief   Checks if a DMA priority is within the valid range.
 * @param[in] prio      DMA priority
 *
 * @retval              The check result.
 * @retval false        invalid DMA priority.
 * @retval true         correct DMA priority.
 */
#define GD32_DMA_IS_VALID_PRIORITY(prio) (((prio) >= 0U) && ((prio) <= 3U))

/**
 * @name    Special stream identifiers
 * @{
 */
#define GD32_DMA_STREAM_ID_ANY         GD32_DMA_STREAMS
#define GD32_DMA_STREAM_ID_ANY_DMA0    (GD32_DMA_STREAM_ID_ANY + 1)
#if GD32_HAS_DMA1 == TRUE
#define GD32_DMA_STREAM_ID_ANY_DMA1    (GD32_DMA_STREAM_ID_ANY_DMA0 + 1)
#endif
/** @} */

/**
 * @brief   Checks if a DMA stream is valid.
 *
 * @param[in] id        DMA stream id
 * @retval              The check result.
 * @retval false        invalid DMA stream.
 * @retval true         correct DMA stream.
 */
#if GD32_HAS_DMA1 == TRUE
#define GD32_DMA_IS_VALID_STREAM(id) (((id) >= 0U) &&                      \
                                       ((id) <= GD32_DMA_STREAM_ID_ANY_DMA1))
#else
#define GD32_DMA_IS_VALID_STREAM(id) (((id) >= 0U) &&                      \
                                       ((id) <= GD32_DMA_STREAM_ID_ANY_DMA0))
#endif

/**
 * @brief   Returns an unique numeric identifier for a DMA stream.
 *
 * @param[in] dma       the DMA unit number (0 or 1)
 * @param[in] stream    the stream number (0..7)
 * @return              An unique numeric stream identifier.
 */
#define GD32_DMA_STREAM_ID(dma, stream) ((((dma) & 1U) * 8U) + (stream))

/**
 * @brief   Returns a DMA stream identifier mask.
 *
 * @param[in] dma       the DMA unit number (0 or 1)
 * @param[in] stream    the stream number (0..7)
 * @return              A DMA stream identifier mask.
 */
#define GD32_DMA_STREAM_ID_MSK(dma, stream)                                \
  (1U << GD32_DMA_STREAM_ID(dma, stream))

/**
 * @brief   Checks if a DMA stream unique identifier belongs to a mask.
 * @param[in] id        the stream numeric identifier
 * @param[in] mask      the stream numeric identifiers mask
 *
 * @retval              The check result.
 * @retval false        id does not belong to the mask.
 * @retval true         id belongs to the mask.
 */
#define GD32_DMA_IS_VALID_ID(id, mask) (((1U << (id)) & (mask)))

/**
 * @name    DMA streams identifiers
 * @{
 */
/**
 * @brief   Returns a pointer to a gd32_dma_stream_t structure.
 *
 * @param[in] id        the stream numeric identifier
 * @return              A pointer to the gd32_dma_stream_t constant structure
 *                      associated to the DMA stream.
 */
#define GD32_DMA_STREAM(id)        (&_gd32_dma_streams[id])

#define GD32_DMA0_STREAM0          GD32_DMA_STREAM(0)
#define GD32_DMA0_STREAM1          GD32_DMA_STREAM(1)
#define GD32_DMA0_STREAM2          GD32_DMA_STREAM(2)
#define GD32_DMA0_STREAM3          GD32_DMA_STREAM(3)
#define GD32_DMA0_STREAM4          GD32_DMA_STREAM(4)
#define GD32_DMA0_STREAM5          GD32_DMA_STREAM(5)
#define GD32_DMA0_STREAM6          GD32_DMA_STREAM(6)
#define GD32_DMA0_STREAM7          GD32_DMA_STREAM(7)
#define GD32_DMA1_STREAM0          GD32_DMA_STREAM(8)
#define GD32_DMA1_STREAM1          GD32_DMA_STREAM(9)
#define GD32_DMA1_STREAM2          GD32_DMA_STREAM(10)
#define GD32_DMA1_STREAM3          GD32_DMA_STREAM(11)
#define GD32_DMA1_STREAM4          GD32_DMA_STREAM(12)
#define GD32_DMA1_STREAM5          GD32_DMA_STREAM(13)
#define GD32_DMA1_STREAM6          GD32_DMA_STREAM(14)
#define GD32_DMA1_STREAM7          GD32_DMA_STREAM(15)
/** @} */

/**
 * @name    CTL register constants common to all DMA types
 * @{
 */
#define GD32_DMA_CTL_RESET_VALUE    0x00000000U
#define GD32_DMA_CTL_CHEN           DMA_CHXCTL_CHEN
#define GD32_DMA_CTL_SDEIE          DMA_CHXCTL_SDEIE
#define GD32_DMA_CTL_TAEIE          DMA_CHXCTL_TAEIE
#define GD32_DMA_CTL_HTIE           DMA_CHXCTL_HTFIE
#define GD32_DMA_CTL_FTFIE          DMA_CHXCTL_FTFIE
#define GD32_DMA_CTL_TM             DMA_CHXCTL_TM
#define GD32_DMA_CTL_CMEN           DMA_CHXCTL_CMEN
#define GD32_DMA_CTL_PNAGA          DMA_CHXCTL_PNAGA
#define GD32_DMA_CTL_MNAGA          DMA_CHXCTL_MNAGA
#define GD32_DMA_CTL_PWIDTH         DMA_CHXCTL_PWIDTH
#define GD32_DMA_CTL_MWIDTH         DMA_CHXCTL_MWIDTH
#define GD32_DMA_CTL_PAIF           DMA_CHXCTL_PAIF
#define GD32_DMA_CTL_SBMEN          DMA_CHXCTL_SBMEN
#define GD32_DMA_CTL_MBS            DMA_CHXCTL_MBS
#define GD32_DMA_CTL_PBURST         DMA_CHXCTL_PBURST
#define GD32_DMA_CTL_MBURST         DMA_CHXCTL_MBURST

#define GD32_DMA_CTL_DIR_MASK       DMA_CHXCTL_TM
#define GD32_DMA_CTL_DIR_P2M        0U
#define GD32_DMA_CTL_DIR_M2P        (1U << 6U)
#define GD32_DMA_CTL_DIR_M2M        (2U << 6U)

#define GD32_DMA_CTL_PWIDTH_MASK    DMA_CHXCTL_PWIDTH
#define GD32_DMA_CTL_PWIDTH_BYTE    0U
#define GD32_DMA_CTL_PWIDTH_HWORD   (1U << 11U)
#define GD32_DMA_CTL_PWIDTH_WORD    (2U << 11U)

#define GD32_DMA_CTL_MWIDTH_MASK    DMA_CHXCTL_MWIDTH
#define GD32_DMA_CTL_MWIDTH_BYTE    0U
#define GD32_DMA_CTL_MWIDTH_HWORD   (1U << 13U)
#define GD32_DMA_CTL_MWIDTH_WORD    (2U << 13U)

#define GD32_DMA_CTL_SIZE_MASK      (GD32_DMA_CTL_PWIDTH_MASK |              \
                                     GD32_DMA_CTL_MWIDTH_MASK)

#define GD32_DMA_CTL_PRIO_MASK      DMA_CHXCTL_PRIO
#define GD32_DMA_CTL_PRIO(n)        ((n) << 16U)

#define GD32_DMA_CTL_PBURST_MASK    DMA_CHXCTL_PBURST
#define GD32_DMA_CTL_PBURST_SINGLE  0U
#define GD32_DMA_CTL_PBURST_INCR4   (1U << 21U)
#define GD32_DMA_CTL_PBURST_INCR8   (2U << 21U)
#define GD32_DMA_CTL_PBURST_INCR16  (3U << 21U)

#define GD32_DMA_CTL_MBURST_MASK    DMA_CHXCTL_MBURST
#define GD32_DMA_CTL_MBURST_SINGLE  0U
#define GD32_DMA_CTL_MBURST_INCR4   (1U << 23U)
#define GD32_DMA_CTL_MBURST_INCR8   (2U << 23U)
#define GD32_DMA_CTL_MBURST_INCR16  (3U << 23U)
/** @} */

/**
 * @name    FCTL register constants
 * @{
 */
#define GD32_DMA_FCTL_RESET_VALUE   0x00000021U
#define GD32_DMA_FCTL_FCCV          DMA_CHXFCTL_FCCV
#define GD32_DMA_FCTL_MDMEN         DMA_CHXFCTL_MDMEN
#define GD32_DMA_FCTL_FCNT          DMA_CHXFCTL_FCNT
#define GD32_DMA_FCTL_FEEIE         DMA_CHXFCTL_FEEIE

#define GD32_DMA_FCTL_FTH_MASK      DMA_CHXFCTL_FCCV
#define GD32_DMA_FCTL_FTH_1Q        0U
#define GD32_DMA_FCTL_FTH_HALF      (1U << 0U)
#define GD32_DMA_FCTL_FTH_3Q        (2U << 0U)
#define GD32_DMA_FCTL_FTH_FULL      (3U << 0U)
/** @} */

/**
 * @name    Status flags passed to the ISR callbacks
 */
#define GD32_DMA_ISR_FEIF           DMA_INTF_FEEIF
#define GD32_DMA_ISR_SDEIF          DMA_INTF_SDEIF
#define GD32_DMA_ISR_TAEIF          DMA_INTF_TAEIF
#define GD32_DMA_ISR_HTIF           DMA_INTF_HTFIF
#define GD32_DMA_ISR_TCIF           DMA_INTF_FTFIF
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(GD32_DMA_SUPPORTS_DMAMUX)
#error "GD32_DMA_SUPPORTS_DMAMUX not defined in registry"
#endif

#if !defined(GD32_HAS_DMA0)
#error "GD32_HAS_DMA0 missing in registry"
#endif

#if !defined(GD32_HAS_DMA1)
#error "GD32_HAS_DMA1 missing in registry"
#endif

#if !defined(GD32_DMA0_CH0_HANDLER)
#error "GD32_DMA0_CH0_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA0_CH1_HANDLER)
#error "GD32_DMA0_CH1_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA0_CH2_HANDLER)
#error "GD32_DMA0_CH2_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA0_CH3_HANDLER)
#error "GD32_DMA0_CH3_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA0_CH4_HANDLER)
#error "GD32_DMA0_CH4_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA0_CH5_HANDLER)
#error "GD32_DMA0_CH5_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA0_CH6_HANDLER)
#error "GD32_DMA0_CH6_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA0_CH7_HANDLER)
#error "GD32_DMA0_CH7_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA1_CH0_HANDLER)
#error "GD32_DMA1_CH0_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA1_CH1_HANDLER)
#error "GD32_DMA1_CH1_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA1_CH2_HANDLER)
#error "GD32_DMA1_CH2_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA1_CH3_HANDLER)
#error "GD32_DMA1_CH3_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA1_CH4_HANDLER)
#error "GD32_DMA1_CH4_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA1_CH5_HANDLER)
#error "GD32_DMA1_CH5_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA1_CH6_HANDLER)
#error "GD32_DMA1_CH6_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA1_CH7_HANDLER)
#error "GD32_DMA1_CH7_HANDLER missing in registry"
#endif

#if !defined(GD32_DMA0_CH0_NUMBER)
#error "GD32_DMA0_CH0_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA0_CH1_NUMBER)
#error "GD32_DMA0_CH1_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA0_CH2_NUMBER)
#error "GD32_DMA0_CH2_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA0_CH3_NUMBER)
#error "GD32_DMA0_CH3_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA0_CH4_NUMBER)
#error "GD32_DMA0_CH4_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA0_CH5_NUMBER)
#error "GD32_DMA0_CH5_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA0_CH6_NUMBER)
#error "GD32_DMA0_CH6_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA0_CH7_NUMBER)
#error "GD32_DMA0_CH7_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA1_CH0_NUMBER)
#error "GD32_DMA1_CH0_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA1_CH1_NUMBER)
#error "GD32_DMA1_CH1_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA1_CH2_NUMBER)
#error "GD32_DMA1_CH2_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA1_CH3_NUMBER)
#error "GD32_DMA1_CH3_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA1_CH4_NUMBER)
#error "GD32_DMA1_CH4_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA1_CH5_NUMBER)
#error "GD32_DMA1_CH5_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA1_CH6_NUMBER)
#error "GD32_DMA1_CH6_NUMBER missing in registry"
#endif

#if !defined(GD32_DMA1_CH7_NUMBER)
#error "GD32_DMA1_CH7_NUMBER missing in registry"
#endif

#if (GD32_DMA_SUPPORTS_DMAMUX == TRUE) || defined(__DOXYGEN__)
#include "gd32_dmamux.h"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   GD32 DMA ISR function type.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] flags     pre-shifted content of the INTF register, the bits
 *                      are aligned to bit zero
 */
typedef void (*gd32_dmaisr_t)(void *p, uint32_t flags);

/**
 * @brief   GD32 DMA stream descriptor structure.
 * @details A "stream" in ChibiOS API terminology maps 1:1 to a GD32 DMA
 *          hardware channel. The vendor header provides generic per-channel
 *          register access macros (DMA_CHCTL(dma, ch), etc.) so only the
 *          base address and channel number need to be stored.
 */
typedef struct {
  uint32_t              dma_base;       /**< @brief DMA unit base address.  */
  uint8_t               channel;        /**< @brief Channel number (0..7).  */
  volatile uint32_t     *intc;          /**< @brief Associated INTC reg.    */
  uint8_t               shift;          /**< @brief Bits offset in INTF/INTC. */
  uint8_t               selfindex;      /**< @brief Index to self in array. */
  uint8_t               vector;         /**< @brief Associated IRQ vector.  */
} gd32_dma_stream_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro Functions
 * @{
 */
/**
 * @brief   Associates a peripheral data register to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 * @param[in] addr      value to be written in the PADDR register
 *
 * @special
 */
#define dmaStreamSetPeripheral(dmastp, addr) {                              \
  DMA_CHPADDR((dmastp)->dma_base, (dmastp)->channel) = (uint32_t)(addr);     \
}

/**
 * @brief   Associates a memory destination to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 * @param[in] addr      value to be written in the M0ADDR register
 *
 * @special
 */
#define dmaStreamSetMemory0(dmastp, addr) {                                 \
  DMA_CHM0ADDR((dmastp)->dma_base, (dmastp)->channel) = (uint32_t)(addr);    \
}

/**
 * @brief   Associates an alternate memory destination to a DMA stream.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 * @param[in] addr      value to be written in the M1ADDR register
 *
 * @special
 */
#define dmaStreamSetMemory1(dmastp, addr) {                                 \
  DMA_CHM1ADDR((dmastp)->dma_base, (dmastp)->channel) = (uint32_t)(addr);    \
}

/**
 * @brief   Sets the number of transfers to be performed.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 * @param[in] size      value to be written in the CNT register
 *
 * @special
 */
#define dmaStreamSetTransactionSize(dmastp, size) {                         \
  DMA_CHCNT((dmastp)->dma_base, (dmastp)->channel) = (uint32_t)(size);       \
}

/**
 * @brief   Returns the number of transfers to be performed.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 * @return              The number of transfers to be performed.
 *
 * @special
 */
#define dmaStreamGetTransactionSize(dmastp)                                 \
  ((size_t)(DMA_CHCNT((dmastp)->dma_base, (dmastp)->channel)))

/**
 * @brief   Programs the stream mode settings.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 * @param[in] mode      value to be written in the CTL register
 *
 * @special
 */
#define dmaStreamSetMode(dmastp, mode) {                                    \
  DMA_CHCTL((dmastp)->dma_base, (dmastp)->channel) = (uint32_t)(mode);       \
}

/**
 * @brief   Programs the stream FIFO settings.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 * @param[in] mode      value to be written in the FCTL register
 *
 * @special
 */
#define dmaStreamSetFIFO(dmastp, mode) {                                    \
  DMA_CHFCTL((dmastp)->dma_base, (dmastp)->channel) = (uint32_t)(mode);      \
}

/**
 * @brief   DMA stream enable.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 *
 * @special
 */
#define dmaStreamEnable(dmastp) {                                           \
  DMA_CHCTL((dmastp)->dma_base, (dmastp)->channel) |= GD32_DMA_CTL_CHEN;     \
}

/**
 * @brief   DMA stream disable.
 * @details The function disables the specified stream, waits for the disable
 *          operation to complete and then clears any pending interrupt.
 * @note    This function can be invoked in both ISR or thread context.
 * @note    Interrupts enabling flags are set to zero after this call.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 *
 * @special
 */
#define dmaStreamDisable(dmastp) {                                          \
  DMA_CHCTL((dmastp)->dma_base, (dmastp)->channel) &=                        \
      ~(GD32_DMA_CTL_FTFIE | GD32_DMA_CTL_HTIE |                             \
        GD32_DMA_CTL_TAEIE | GD32_DMA_CTL_SDEIE | GD32_DMA_CTL_CHEN);        \
  while ((DMA_CHCTL((dmastp)->dma_base, (dmastp)->channel) &                 \
          GD32_DMA_CTL_CHEN) != 0)                                           \
    ;                                                                       \
  dmaStreamClearInterrupt(dmastp);                                          \
}

/**
 * @brief   DMA stream interrupt sources clear.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 *
 * @special
 */
#define dmaStreamClearInterrupt(dmastp) {                                   \
  *(dmastp)->intc = GD32_DMA_ISR_MASK << (dmastp)->shift;                   \
}

/**
 * @brief   Starts a memory to memory operation using the specified stream.
 * @note    The default transfer data mode is "byte to byte" but it can be
 *          changed by specifying extra options in the @p mode parameter.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 * @param[in] mode      value to be written in the CTL register, this value
 *                      is implicitly ORed with:
 *                      - @p GD32_DMA_CTL_MNAGA
 *                      - @p GD32_DMA_CTL_PNAGA
 *                      - @p GD32_DMA_CTL_DIR_M2M
 *                      - @p GD32_DMA_CTL_CHEN
 *                      .
 * @param[in] src       source address
 * @param[in] dst       destination address
 * @param[in] n         number of data units to copy
 */
#if (GD32_DMA_SUPPORTS_DMAMUX == TRUE) || defined(__DOXYGEN__)
#define dmaStartMemCopy(dmastp, mode, src, dst, n) {                        \
  dmaStreamSetPeripheral(dmastp, src);                                      \
  dmaStreamSetMemory0(dmastp, dst);                                         \
  dmaStreamSetTransactionSize(dmastp, n);                                   \
  dmaStreamSetMode(dmastp, (mode) |                                         \
                           GD32_DMA_CTL_MNAGA | GD32_DMA_CTL_PNAGA |         \
                           GD32_DMA_CTL_DIR_M2M);                            \
  dmaSetRequestSource(dmastp, GD32_DMAMUX1_REQ_M2M);                        \
  dmaStreamEnable(dmastp);                                                  \
}
#else
#define dmaStartMemCopy(dmastp, mode, src, dst, n) {                        \
  dmaStreamSetPeripheral(dmastp, src);                                      \
  dmaStreamSetMemory0(dmastp, dst);                                         \
  dmaStreamSetTransactionSize(dmastp, n);                                   \
  dmaStreamSetMode(dmastp, (mode) |                                         \
                           GD32_DMA_CTL_MNAGA | GD32_DMA_CTL_PNAGA |         \
                           GD32_DMA_CTL_DIR_M2M);                            \
  dmaStreamEnable(dmastp);                                                  \
}
#endif

/**
 * @brief   Polled wait for DMA transfer end.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 */
#define dmaWaitCompletion(dmastp) {                                         \
  DMA_CHCTL((dmastp)->dma_base, (dmastp)->channel) &=                        \
      ~(GD32_DMA_CTL_FTFIE | GD32_DMA_CTL_HTIE |                             \
        GD32_DMA_CTL_TAEIE | GD32_DMA_CTL_SDEIE);                            \
  while (DMA_CHCTL((dmastp)->dma_base, (dmastp)->channel) &                  \
         GD32_DMA_CTL_CHEN)                                                  \
    ;                                                                       \
  dmaStreamClearInterrupt(dmastp);                                          \
}

/**
 * @brief   DMA stream current target.
 * @note    This function can be invoked in both ISR or thread context.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    After use the stream can be released using @p dmaStreamFree().
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 * @return              Current memory target index.
 *
 * @special
 */
#define dmaStreamGetCurrentTarget(dmastp)                                   \
  (((DMA_CHCTL((dmastp)->dma_base, (dmastp)->channel)) >> 19U) & 1U)
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern const gd32_dma_stream_t _gd32_dma_streams[GD32_DMA_STREAMS];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void dmaInit(void);
  const gd32_dma_stream_t *dmaStreamAllocI(uint32_t id,
                                           uint32_t priority,
                                           gd32_dmaisr_t func,
                                           void *param);
  const gd32_dma_stream_t *dmaStreamAlloc(uint32_t id,
                                          uint32_t priority,
                                          gd32_dmaisr_t func,
                                          void *param);
  void dmaStreamFreeI(const gd32_dma_stream_t *dmastp);
  void dmaStreamFree(const gd32_dma_stream_t *dmastp);
#if GD32_DMA_SUPPORTS_DMAMUX == TRUE
  void dmaSetRequestSource(const gd32_dma_stream_t *dmastp, uint32_t per);
#endif
#ifdef __cplusplus
}
#endif

#endif /* GD32_DMA_H */

/** @} */
