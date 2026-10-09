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
 * @file    DMAv2/gd32_dma.c
 * @brief   Enhanced DMA helper driver code.
 *
 * @addtogroup GD32_DMA
 * @details DMA sharing helper driver. In the GD32 the DMA channels are a
 *          shared resource, this driver allows to allocate and free DMA
 *          channels at runtime in order to allow all the other device
 *          drivers to coordinate the access to the resource.
 * @note    The DMA ISR handlers are all declared into this module because
 *          sharing, the various device drivers can associate a callback to
 *          ISRs when allocating channels.
 * @{
 */

#include "hal.h"

/* The following macro is only defined if some driver requiring DMA services
   has been enabled.*/
#if defined(GD32_DMA_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Mask of the DMA0 streams in @p dma.allocated_mask.
 */
#define GD32_DMA0_STREAMS_MASK     0x000000FFU

/**
 * @brief   Mask of the DMA1 streams in @p dma.allocated_mask.
 */
#define GD32_DMA1_STREAMS_MASK     0x0000FF00U

#define DMA0_BASE                  DMA0
#define DMA1_BASE                  DMA1

/**
 * @brief   Per-channel register base offset.
 * @details Channel registers are at dma_base + 0x10 + channel * 0x18.
 *          The vendor header provides DMA_CHCTL(dma, ch) etc. macros for
 *          register access, so this offset is only used for documentation.
 */
#define GD32_DMA_CH_OFFSET(ch)     (0x10U + (ch) * 0x18U)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   DMA streams descriptors.
 * @details This table keeps the association between an unique stream
 *          identifier and the involved physical registers.
 * @note    Don't use this array directly, use the appropriate wrapper macros
 *          instead: @p GD32_DMA0_STREAM0, @p GD32_DMA0_STREAM1 etc.
 */
const gd32_dma_stream_t _gd32_dma_streams[GD32_DMA_STREAMS] = {
  {DMA0_BASE, 0, &DMA_INTC0(DMA0_BASE),  0,  0, GD32_DMA0_CH0_NUMBER},
  {DMA0_BASE, 1, &DMA_INTC0(DMA0_BASE),  6,  1, GD32_DMA0_CH1_NUMBER},
  {DMA0_BASE, 2, &DMA_INTC0(DMA0_BASE), 16,  2, GD32_DMA0_CH2_NUMBER},
  {DMA0_BASE, 3, &DMA_INTC0(DMA0_BASE), 22,  3, GD32_DMA0_CH3_NUMBER},
  {DMA0_BASE, 4, &DMA_INTC1(DMA0_BASE),  0,  4, GD32_DMA0_CH4_NUMBER},
  {DMA0_BASE, 5, &DMA_INTC1(DMA0_BASE),  6,  5, GD32_DMA0_CH5_NUMBER},
  {DMA0_BASE, 6, &DMA_INTC1(DMA0_BASE), 16,  6, GD32_DMA0_CH6_NUMBER},
  {DMA0_BASE, 7, &DMA_INTC1(DMA0_BASE), 22,  7, GD32_DMA0_CH7_NUMBER},
  {DMA1_BASE, 0, &DMA_INTC0(DMA1_BASE),  0,  8, GD32_DMA1_CH0_NUMBER},
  {DMA1_BASE, 1, &DMA_INTC0(DMA1_BASE),  6,  9, GD32_DMA1_CH1_NUMBER},
  {DMA1_BASE, 2, &DMA_INTC0(DMA1_BASE), 16, 10, GD32_DMA1_CH2_NUMBER},
  {DMA1_BASE, 3, &DMA_INTC0(DMA1_BASE), 22, 11, GD32_DMA1_CH3_NUMBER},
  {DMA1_BASE, 4, &DMA_INTC1(DMA1_BASE),  0, 12, GD32_DMA1_CH4_NUMBER},
  {DMA1_BASE, 5, &DMA_INTC1(DMA1_BASE),  6, 13, GD32_DMA1_CH5_NUMBER},
  {DMA1_BASE, 6, &DMA_INTC1(DMA1_BASE), 16, 14, GD32_DMA1_CH6_NUMBER},
  {DMA1_BASE, 7, &DMA_INTC1(DMA1_BASE), 22, 15, GD32_DMA1_CH7_NUMBER},
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Global DMA-related data structures.
 */
static struct {
  /**
   * @brief   Mask of the allocated streams.
   */
  uint32_t          allocated_mask;
  /**
   * @brief   DMA IRQ redirectors.
   */
  struct {
    /**
     * @brief   DMA callback function.
     */
    gd32_dmaisr_t    func;
    /**
     * @brief   DMA callback parameter.
     */
    void              *param;
  } streams[GD32_DMA_STREAMS];
} dma;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   DMA0 channel 0 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA0_CH0_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF0(DMA0_BASE) >> 0U) & GD32_DMA_ISR_MASK;
  DMA_INTC0(DMA0_BASE) = flags << 0U;
  if (dma.streams[0].func)
    dma.streams[0].func(dma.streams[0].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA0 channel 1 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA0_CH1_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF0(DMA0_BASE) >> 6U) & GD32_DMA_ISR_MASK;
  DMA_INTC0(DMA0_BASE) = flags << 6U;
  if (dma.streams[1].func)
    dma.streams[1].func(dma.streams[1].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA0 channel 2 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA0_CH2_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF0(DMA0_BASE) >> 16U) & GD32_DMA_ISR_MASK;
  DMA_INTC0(DMA0_BASE) = flags << 16U;
  if (dma.streams[2].func)
    dma.streams[2].func(dma.streams[2].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA0 channel 3 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA0_CH3_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF0(DMA0_BASE) >> 22U) & GD32_DMA_ISR_MASK;
  DMA_INTC0(DMA0_BASE) = flags << 22U;
  if (dma.streams[3].func)
    dma.streams[3].func(dma.streams[3].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA0 channel 4 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA0_CH4_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF1(DMA0_BASE) >> 0U) & GD32_DMA_ISR_MASK;
  DMA_INTC1(DMA0_BASE) = flags << 0U;
  if (dma.streams[4].func)
    dma.streams[4].func(dma.streams[4].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA0 channel 5 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA0_CH5_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF1(DMA0_BASE) >> 6U) & GD32_DMA_ISR_MASK;
  DMA_INTC1(DMA0_BASE) = flags << 6U;
  if (dma.streams[5].func)
    dma.streams[5].func(dma.streams[5].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA0 channel 6 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA0_CH6_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF1(DMA0_BASE) >> 16U) & GD32_DMA_ISR_MASK;
  DMA_INTC1(DMA0_BASE) = flags << 16U;
  if (dma.streams[6].func)
    dma.streams[6].func(dma.streams[6].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA0 channel 7 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA0_CH7_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF1(DMA0_BASE) >> 22U) & GD32_DMA_ISR_MASK;
  DMA_INTC1(DMA0_BASE) = flags << 22U;
  if (dma.streams[7].func)
    dma.streams[7].func(dma.streams[7].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 0 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA1_CH0_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF0(DMA1_BASE) >> 0U) & GD32_DMA_ISR_MASK;
  DMA_INTC0(DMA1_BASE) = flags << 0U;
  if (dma.streams[8].func)
    dma.streams[8].func(dma.streams[8].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 1 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA1_CH1_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF0(DMA1_BASE) >> 6U) & GD32_DMA_ISR_MASK;
  DMA_INTC0(DMA1_BASE) = flags << 6U;
  if (dma.streams[9].func)
    dma.streams[9].func(dma.streams[9].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 2 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA1_CH2_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF0(DMA1_BASE) >> 16U) & GD32_DMA_ISR_MASK;
  DMA_INTC0(DMA1_BASE) = flags << 16U;
  if (dma.streams[10].func)
    dma.streams[10].func(dma.streams[10].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 3 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA1_CH3_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF0(DMA1_BASE) >> 22U) & GD32_DMA_ISR_MASK;
  DMA_INTC0(DMA1_BASE) = flags << 22U;
  if (dma.streams[11].func)
    dma.streams[11].func(dma.streams[11].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 4 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA1_CH4_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF1(DMA1_BASE) >> 0U) & GD32_DMA_ISR_MASK;
  DMA_INTC1(DMA1_BASE) = flags << 0U;
  if (dma.streams[12].func)
    dma.streams[12].func(dma.streams[12].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 5 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA1_CH5_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF1(DMA1_BASE) >> 6U) & GD32_DMA_ISR_MASK;
  DMA_INTC1(DMA1_BASE) = flags << 6U;
  if (dma.streams[13].func)
    dma.streams[13].func(dma.streams[13].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 6 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA1_CH6_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF1(DMA1_BASE) >> 16U) & GD32_DMA_ISR_MASK;
  DMA_INTC1(DMA1_BASE) = flags << 16U;
  if (dma.streams[14].func)
    dma.streams[14].func(dma.streams[14].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   DMA1 channel 7 shared interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_DMA1_CH7_HANDLER) {
  uint32_t flags;

  OSAL_IRQ_PROLOGUE();

  flags = (DMA_INTF1(DMA1_BASE) >> 22U) & GD32_DMA_ISR_MASK;
  DMA_INTC1(DMA1_BASE) = flags << 22U;
  if (dma.streams[15].func)
    dma.streams[15].func(dma.streams[15].param, flags);

  OSAL_IRQ_EPILOGUE();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   DMA helper driver initialization.
 * @details The DMA0 and DMA1 clocks are enabled because the DMA is a shared
 *          resource.
 *
 * @init
 */
void dmaInit(void) {
  unsigned i;

  /* The DMA0 and DMA1 clocks are enabled because the DMA is a shared
     resource.*/
  RCU_AHB1EN |= (RCU_AHB1EN_DMA0EN | RCU_AHB1EN_DMA1EN |
                 RCU_AHB1EN_DMAMUXEN);

  dma.allocated_mask = 0U;
  for (i = 0U; i < GD32_DMA_STREAMS; i++) {
    const gd32_dma_stream_t *dmastp = &_gd32_dma_streams[i];

    DMA_CHCTL(dmastp->dma_base, dmastp->channel)  = GD32_DMA_CTL_RESET_VALUE;
    DMA_CHFCTL(dmastp->dma_base, dmastp->channel) = GD32_DMA_FCTL_RESET_VALUE;
    dma.streams[i].func = NULL;
  }

  /* Clearing all the residual interrupt flags. */
  DMA_INTC0(DMA0_BASE) = 0xFFFFFFFFU;
  DMA_INTC1(DMA0_BASE) = 0xFFFFFFFFU;
  DMA_INTC0(DMA1_BASE) = 0xFFFFFFFFU;
  DMA_INTC1(DMA1_BASE) = 0xFFFFFFFFU;
}

/**
 * @brief   Allocates a DMA stream.
 * @details The function allocates a DMA stream, sets the associated ISR
 *          priority and registers the callback function and parameter.
 * @pre     The stream must not be already allocated.
 * @post    The stream is allocated and its ISR is configured.
 * @post    The stream must be released using @p dmaStreamFree() before
 *          it can be reused with another peripheral.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] id        the unique numeric stream identifier
 * @param[in] priority  IRQ priority for the DMA stream
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              A pointer to the gd32_dma_stream_t structure allocated
 *                      or @p NULL if the resource is not available.
 *
 * @special
 */
const gd32_dma_stream_t *dmaStreamAllocI(uint32_t id,
                                         uint32_t priority,
                                         gd32_dmaisr_t func,
                                         void *param) {
  uint32_t i, startid = id, endid = id;

  osalDbgCheckClassI();

  if (id < GD32_DMA_STREAMS) {
    startid = id;
  }
  else if (id == GD32_DMA_STREAM_ID_ANY) {
    startid = 0U;
    endid   = GD32_DMA_STREAMS - 1U;
  }
  else if (id == GD32_DMA_STREAM_ID_ANY_DMA0) {
    startid = 0U;
    endid   = 7U;
  }
#if GD32_HAS_DMA1 == TRUE
  else if (id == GD32_DMA_STREAM_ID_ANY_DMA1) {
    startid = 8U;
    endid   = GD32_DMA_STREAMS - 1U;
  }
#endif
  else {
    osalDbgCheck(false);
    return NULL;
  }

  for (i = startid; i <= endid; i++) {
    uint32_t mask = (1U << i);
    if ((dma.allocated_mask & mask) == 0U) {
      const gd32_dma_stream_t *dmastp = &_gd32_dma_streams[i];

      /* Installs the DMA handler.*/
      dma.streams[i].func  = func;
      dma.streams[i].param = param;
      dma.allocated_mask  |= mask;

      /* Enabling DMA clocks required by the current streams set.*/
      if ((GD32_DMA0_STREAMS_MASK & mask) != 0U) {
        rcuEnableDMA0(true);
      }
      if ((GD32_DMA1_STREAMS_MASK & mask) != 0U) {
        rcuEnableDMA1(true);
      }

#if GD32_DMA_SUPPORTS_DMAMUX == TRUE
      /* Enabling DMAMUX if present.*/
      if (dma.allocated_mask != 0U) {
        rcuEnableDMAMUX(true);
      }
#endif

      /* Putting the stream in a safe state.*/
      dmaStreamDisable(dmastp);
      DMA_CHCTL(dmastp->dma_base, dmastp->channel)  = GD32_DMA_CTL_RESET_VALUE;
      DMA_CHFCTL(dmastp->dma_base, dmastp->channel) = GD32_DMA_FCTL_RESET_VALUE;

      /* Enables the associated IRQ vector if a callback is defined.*/
      if (func != NULL) {
        nvicEnableVector(dmastp->vector, priority);
      }

      return dmastp;
    }
  }

  return NULL;
}

/**
 * @brief   Allocates a DMA stream.
 * @details The function allocates a DMA stream, sets the associated ISR
 *          priority and registers the callback function and parameter.
 * @pre     The stream must not be already allocated.
 * @post    The stream is allocated and its ISR is configured.
 * @post    The stream must be released using @p dmaStreamFree() before
 *          it can be reused with another peripheral.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] id        the unique numeric stream identifier
 * @param[in] priority  IRQ priority for the DMA stream
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              A pointer to the gd32_dma_stream_t structure allocated
 *                      or @p NULL if the resource is not available.
 *
 * @api
 */
const gd32_dma_stream_t *dmaStreamAlloc(uint32_t id,
                                        uint32_t priority,
                                        gd32_dmaisr_t func,
                                        void *param) {
  const gd32_dma_stream_t *dmastp;

  osalSysLock();
  dmastp = dmaStreamAllocI(id, priority, func, param);
  osalSysUnlock();

  return dmastp;
}

/**
 * @brief   Releases a DMA stream.
 * @details The function frees the specified DMA stream and disables its
 *          interrupt.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    The stream is free and its ISR is disabled.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 *
 * @special
 */
void dmaStreamFreeI(const gd32_dma_stream_t *dmastp) {

  osalDbgCheck(dmastp != NULL);
  osalDbgCheck(dma.allocated_mask & (1U << dmastp->selfindex));

  nvicDisableVector(dmastp->vector);
  dma.allocated_mask &= ~(1U << dmastp->selfindex);
  dma.streams[dmastp->selfindex].func  = NULL;
  dma.streams[dmastp->selfindex].param = NULL;

  /* Shutting down clocks that are no more required, if any.*/
  if ((dma.allocated_mask & GD32_DMA0_STREAMS_MASK) == 0U) {
    RCU_AHB1EN &= ~RCU_AHB1EN_DMA0EN;
  }
  if ((dma.allocated_mask & GD32_DMA1_STREAMS_MASK) == 0U) {
    RCU_AHB1EN &= ~RCU_AHB1EN_DMA1EN;
  }

#if GD32_DMA_SUPPORTS_DMAMUX == TRUE
  /* Shutting down DMAMUX if no streams are allocated.*/
  if (dma.allocated_mask == 0U) {
    RCU_AHB1EN &= ~RCU_AHB1EN_DMAMUXEN;
  }
#endif
}

/**
 * @brief   Releases a DMA stream.
 * @details The function frees the specified DMA stream and disables its
 *          interrupt.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @post    The stream is free and its ISR is disabled.
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 *
 * @api
 */
void dmaStreamFree(const gd32_dma_stream_t *dmastp) {

  osalSysLock();
  dmaStreamFreeI(dmastp);
  osalSysUnlock();
}

#if (GD32_DMA_SUPPORTS_DMAMUX == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Sets the request source for a DMA stream.
 * @details The function programs the DMAMUX multiplexer to route the
 *          specified peripheral request to the DMA channel.
 * @pre     The stream must have been allocated using @p dmaStreamAlloc().
 * @note    This function can be invoked in both ISR or thread context.
 *
 * @param[in] dmastp    pointer to a gd32_dma_stream_t structure
 * @param[in] per       peripheral request identifier, one of the
 *                      GD32_DMAMUX1_* macros
 *
 * @special
 */
void dmaSetRequestSource(const gd32_dma_stream_t *dmastp, uint32_t per) {
  uint32_t mux_channel = (dmastp->dma_base == DMA0_BASE) ?
                         dmastp->channel : (dmastp->channel + 8U);

  osalDbgCheck(dmastp != NULL);

  /* The DMAMUX request multiplexer channel configuration registers are
     at fixed offsets starting from DMAMUX_BASE, 4 bytes per channel.*/
  REG32(DMAMUX_BASE + mux_channel * 4U) = per;
}
#endif /* GD32_DMA_SUPPORTS_DMAMUX == TRUE */

#endif /* GD32_DMA_REQUIRED */

/** @} */
