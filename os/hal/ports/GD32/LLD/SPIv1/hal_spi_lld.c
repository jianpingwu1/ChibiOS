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
 * @file    SPIv1/hal_spi_lld.c
 * @brief   GD32 SPI subsystem low level driver source.
 *
 * @addtogroup SPI
 * @{
 */

#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/**
 * @brief   Software timeout for busy-flag polling loops (loop iterations).
 * @note    Sized for the slowest usable frame rate.
 */
#define SPI_BUSY_TIMEOUT                    0x00010000U

#if !defined(SPI_SPID0_MEMORY)
#define SPI_SPID0_MEMORY
#endif

#if !defined(SPI_SPID1_MEMORY)
#define SPI_SPID1_MEMORY
#endif

#if !defined(SPI_SPID2_MEMORY)
#define SPI_SPID2_MEMORY
#endif

#if !defined(SPI_SPID3_MEMORY)
#define SPI_SPID3_MEMORY
#endif

#if !defined(SPI_SPID4_MEMORY)
#define SPI_SPID4_MEMORY
#endif

#if !defined(SPI_SPID5_MEMORY)
#define SPI_SPID5_MEMORY
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/** @brief SPI0 driver identifier.*/
#if GD32_SPI_USE_SPI0 || defined(__DOXYGEN__)
SPI_SPID0_MEMORY SPIDriver SPID0;
#endif

/** @brief SPI1 driver identifier.*/
#if GD32_SPI_USE_SPI1 || defined(__DOXYGEN__)
SPI_SPID1_MEMORY SPIDriver SPID1;
#endif

/** @brief SPI2 driver identifier.*/
#if GD32_SPI_USE_SPI2 || defined(__DOXYGEN__)
SPI_SPID2_MEMORY SPIDriver SPID2;
#endif

/** @brief SPI3 driver identifier.*/
#if GD32_SPI_USE_SPI3 || defined(__DOXYGEN__)
SPI_SPID3_MEMORY SPIDriver SPID3;
#endif

/** @brief SPI4 driver identifier.*/
#if GD32_SPI_USE_SPI4 || defined(__DOXYGEN__)
SPI_SPID4_MEMORY SPIDriver SPID4;
#endif

/** @brief SPI5 driver identifier.*/
#if GD32_SPI_USE_SPI5 || defined(__DOXYGEN__)
SPI_SPID5_MEMORY SPIDriver SPID5;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const uint32_t dummytx = GD32_SPI_FILLER_PATTERN;
static uint32_t dummyrx;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Waits for the peripheral to stop.
 * @note    A suspend request is only served while frames are in flight, past
 *          the last frame MSTART stays set and TC marks the idle state.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
static void spi_lld_wait_complete(SPIDriver *spip) {
  uint32_t timeout = SPI_BUSY_TIMEOUT;

  while (((SPI_CTL0(spip->spi) & SPI_CTL0_MSTART) != 0U) &&
         ((SPI_STAT(spip->spi) & SPI_STAT_TC) == 0U)) {
    if (--timeout == 0U) {
      osalSysHalt("SPI transfer timeout");
    }
  }

  SPI_STATC(spip->spi) = 0xFFFFFFFFU;
}

/**
 * @brief   Shared DMA end-of-rx service routine.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void spi_lld_serve_dma_rx_interrupt(SPIDriver *spip, uint32_t flags) {

  /* DMA errors handling.*/
#if defined(GD32_SPI_DMA_ERROR_HOOK)
  if ((flags & (GD32_DMA_ISR_TAEIF | GD32_DMA_ISR_SDEIF)) != 0U) {
    GD32_SPI_DMA_ERROR_HOOK(spip);
  }
#else
  (void)flags;
#endif

  if (spip->config->circular) {
    if ((flags & GD32_DMA_ISR_HTIF) != 0U) {
      /* Half buffer interrupt.*/
      _spi_isr_half_code(spip);
    }
    if ((flags & GD32_DMA_ISR_TCIF) != 0U) {
      /* End buffer interrupt.*/
      _spi_isr_full_code(spip);
    }
  }
  else {
    /* Stopping SPI.*/
    SPI_CTL0(spip->spi) |= SPI_CTL0_MSPDR;

    /* Stopping DMAs.*/
    dmaStreamDisable(spip->dmatx);
    dmaStreamDisable(spip->dmarx);

    /* Portable SPI ISR code defined in the high level driver, note, it is
       a macro.*/
    _spi_isr_code(spip);
  }
}

/**
 * @brief   Shared DMA end-of-tx service routine.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void spi_lld_serve_dma_tx_interrupt(SPIDriver *spip, uint32_t flags) {

  /* DMA errors handling.*/
#if defined(GD32_SPI_DMA_ERROR_HOOK)
  (void)spip;
  if ((flags & (GD32_DMA_ISR_TAEIF | GD32_DMA_ISR_SDEIF)) != 0U) {
    GD32_SPI_DMA_ERROR_HOOK(spip);
  }
#else
  (void)spip;
  (void)flags;
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   Shared SPI service routine.
 * @note    Invoked from the shared vector dispatcher in gd32_isr.c.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 */
void spi_lld_serve_interrupt(SPIDriver *spip) {
  uint32_t stat;

  stat = SPI_STAT(spip->spi) & SPI_INT(spip->spi);
  SPI_STATC(spip->spi) = stat;

  if ((stat & SPI_STAT_RXORERR) != 0U) {
    /* CHTODO: fault notification.*/
  }
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @notapi
 */
void spi_lld_init(void) {

#if GD32_SPI_USE_SPI0
  spiObjectInit(&SPID0);
  SPID0.spi        = SPI0;
  SPID0.dmarx      = NULL;
  SPID0.dmatx      = NULL;
  SPID0.dmarxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI0_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_P2M |
                     GD32_DMA_CTL_FTFIE |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
  SPID0.dmatxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI0_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_M2P |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
#endif

#if GD32_SPI_USE_SPI1
  spiObjectInit(&SPID1);
  SPID1.spi        = SPI1;
  SPID1.dmarx      = NULL;
  SPID1.dmatx      = NULL;
  SPID1.dmarxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI1_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_P2M |
                     GD32_DMA_CTL_FTFIE |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
  SPID1.dmatxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI1_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_M2P |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
#endif

#if GD32_SPI_USE_SPI2
  spiObjectInit(&SPID2);
  SPID2.spi        = SPI2;
  SPID2.dmarx      = NULL;
  SPID2.dmatx      = NULL;
  SPID2.dmarxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI2_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_P2M |
                     GD32_DMA_CTL_FTFIE |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
  SPID2.dmatxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI2_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_M2P |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
#endif

#if GD32_SPI_USE_SPI3
  spiObjectInit(&SPID3);
  SPID3.spi        = SPI3;
  SPID3.dmarx      = NULL;
  SPID3.dmatx      = NULL;
  SPID3.dmarxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI3_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_P2M |
                     GD32_DMA_CTL_FTFIE |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
  SPID3.dmatxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI3_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_M2P |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
#endif

#if GD32_SPI_USE_SPI4
  spiObjectInit(&SPID4);
  SPID4.spi        = SPI4;
  SPID4.dmarx      = NULL;
  SPID4.dmatx      = NULL;
  SPID4.dmarxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI4_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_P2M |
                     GD32_DMA_CTL_FTFIE |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
  SPID4.dmatxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI4_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_M2P |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
#endif

#if GD32_SPI_USE_SPI5
  spiObjectInit(&SPID5);
  SPID5.spi        = SPI5;
  SPID5.dmarx      = NULL;
  SPID5.dmatx      = NULL;
  SPID5.dmarxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI5_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_P2M |
                     GD32_DMA_CTL_FTFIE |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
  SPID5.dmatxmode  = GD32_DMA_CTL_PRIO(GD32_SPI_SPI5_DMA_PRIORITY) |
                     GD32_DMA_CTL_DIR_M2P |
                     GD32_DMA_CTL_SDEIE |
                     GD32_DMA_CTL_TAEIE;
#endif
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {
  uint32_t dsize, accmode;

  /* If in stopped state then enables the SPI and DMA clocks.*/
  if (spip->state == SPI_STOP) {
#if GD32_SPI_USE_SPI0
    if (&SPID0 == spip) {
      spip->dmarx = dmaStreamAllocI(GD32_SPI_SPI0_RX_DMA_STREAM,
                                    GD32_SPI_SPI0_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_rx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "unable to allocate stream");
      spip->dmatx = dmaStreamAllocI(GD32_SPI_SPI0_TX_DMA_STREAM,
                                    GD32_SPI_SPI0_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_tx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "unable to allocate stream");
      rcuResetSPI0();
      rcuEnableSPI0(true);
      dmaSetRequestSource(spip->dmarx, GD32_DMAMUX1_SPI0_RX);
      dmaSetRequestSource(spip->dmatx, GD32_DMAMUX1_SPI0_TX);
    }
#endif
#if GD32_SPI_USE_SPI1
    if (&SPID1 == spip) {
      spip->dmarx = dmaStreamAllocI(GD32_SPI_SPI1_RX_DMA_STREAM,
                                    GD32_SPI_SPI1_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_rx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "unable to allocate stream");
      spip->dmatx = dmaStreamAllocI(GD32_SPI_SPI1_TX_DMA_STREAM,
                                    GD32_SPI_SPI1_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_tx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "unable to allocate stream");
      rcuResetSPI1();
      rcuEnableSPI1(true);
      dmaSetRequestSource(spip->dmarx, GD32_DMAMUX1_SPI1_RX);
      dmaSetRequestSource(spip->dmatx, GD32_DMAMUX1_SPI1_TX);
    }
#endif
#if GD32_SPI_USE_SPI2
    if (&SPID2 == spip) {
      spip->dmarx = dmaStreamAllocI(GD32_SPI_SPI2_RX_DMA_STREAM,
                                    GD32_SPI_SPI2_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_rx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "unable to allocate stream");
      spip->dmatx = dmaStreamAllocI(GD32_SPI_SPI2_TX_DMA_STREAM,
                                    GD32_SPI_SPI2_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_tx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "unable to allocate stream");
      rcuResetSPI2();
      rcuEnableSPI2(true);
      dmaSetRequestSource(spip->dmarx, GD32_DMAMUX1_SPI2_RX);
      dmaSetRequestSource(spip->dmatx, GD32_DMAMUX1_SPI2_TX);
    }
#endif
#if GD32_SPI_USE_SPI3
    if (&SPID3 == spip) {
      spip->dmarx = dmaStreamAllocI(GD32_SPI_SPI3_RX_DMA_STREAM,
                                    GD32_SPI_SPI3_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_rx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "unable to allocate stream");
      spip->dmatx = dmaStreamAllocI(GD32_SPI_SPI3_TX_DMA_STREAM,
                                    GD32_SPI_SPI3_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_tx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "unable to allocate stream");
      rcuResetSPI3();
      rcuEnableSPI3(true);
      dmaSetRequestSource(spip->dmarx, GD32_DMAMUX1_SPI3_RX);
      dmaSetRequestSource(spip->dmatx, GD32_DMAMUX1_SPI3_TX);
    }
#endif
#if GD32_SPI_USE_SPI4
    if (&SPID4 == spip) {
      spip->dmarx = dmaStreamAllocI(GD32_SPI_SPI4_RX_DMA_STREAM,
                                    GD32_SPI_SPI4_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_rx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "unable to allocate stream");
      spip->dmatx = dmaStreamAllocI(GD32_SPI_SPI4_TX_DMA_STREAM,
                                    GD32_SPI_SPI4_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_tx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "unable to allocate stream");
      rcuResetSPI4();
      rcuEnableSPI4(true);
      dmaSetRequestSource(spip->dmarx, GD32_DMAMUX1_SPI4_RX);
      dmaSetRequestSource(spip->dmatx, GD32_DMAMUX1_SPI4_TX);
    }
#endif
#if GD32_SPI_USE_SPI5
    if (&SPID5 == spip) {
      spip->dmarx = dmaStreamAllocI(GD32_SPI_SPI5_RX_DMA_STREAM,
                                    GD32_SPI_SPI5_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_rx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "unable to allocate stream");
      spip->dmatx = dmaStreamAllocI(GD32_SPI_SPI5_TX_DMA_STREAM,
                                    GD32_SPI_SPI5_IRQ_PRIORITY,
                                    (gd32_dmaisr_t)spi_lld_serve_dma_tx_interrupt,
                                    (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "unable to allocate stream");
      rcuResetSPI5();
      rcuEnableSPI5(true);
      dmaSetRequestSource(spip->dmarx, GD32_DMAMUX1_SPI5_RX);
      dmaSetRequestSource(spip->dmatx, GD32_DMAMUX1_SPI5_TX);
    }
#endif

    /* DMA setup.*/
    dmaStreamSetPeripheral(spip->dmarx, &SPI_RDATA(spip->spi));
    dmaStreamSetPeripheral(spip->dmatx, &SPI_TDATA(spip->spi));
  }

  /* Configuration-specific DMA setup.*/
  dsize = (spip->config->cfg0 & SPI_CFG0_DZ) + 1U;
  if (dsize <= 8U) {
    /* Frame width is between 4 and 8 bits.*/
    accmode = SPI_CFG0_BYTEN;
    spip->dmarxmode = (spip->dmarxmode & ~GD32_DMA_CTL_SIZE_MASK) |
                      GD32_DMA_CTL_PWIDTH_BYTE | GD32_DMA_CTL_MWIDTH_BYTE;
    spip->dmatxmode = (spip->dmatxmode & ~GD32_DMA_CTL_SIZE_MASK) |
                      GD32_DMA_CTL_PWIDTH_BYTE | GD32_DMA_CTL_MWIDTH_BYTE;
  }
  else if (dsize <= 16U) {
    /* Frame width is between 9 and 16 bits.*/
    accmode = 0U;
    spip->dmarxmode = (spip->dmarxmode & ~GD32_DMA_CTL_SIZE_MASK) |
                      GD32_DMA_CTL_PWIDTH_HWORD | GD32_DMA_CTL_MWIDTH_HWORD;
    spip->dmatxmode = (spip->dmatxmode & ~GD32_DMA_CTL_SIZE_MASK) |
                      GD32_DMA_CTL_PWIDTH_HWORD | GD32_DMA_CTL_MWIDTH_HWORD;
  }
  else {
    /* Frame width is between 17 and 32 bits.*/
    accmode = SPI_CFG0_WORDEN;
    spip->dmarxmode = (spip->dmarxmode & ~GD32_DMA_CTL_SIZE_MASK) |
                      GD32_DMA_CTL_PWIDTH_WORD | GD32_DMA_CTL_MWIDTH_WORD;
    spip->dmatxmode = (spip->dmatxmode & ~GD32_DMA_CTL_SIZE_MASK) |
                      GD32_DMA_CTL_PWIDTH_WORD | GD32_DMA_CTL_MWIDTH_WORD;
  }
  if (spip->config->circular) {
    spip->dmarxmode |= (GD32_DMA_CTL_CMEN | GD32_DMA_CTL_HTIE);
    spip->dmatxmode |= (GD32_DMA_CTL_CMEN | GD32_DMA_CTL_HTIE);
  }
  else {
    spip->dmarxmode &= ~(GD32_DMA_CTL_CMEN | GD32_DMA_CTL_HTIE);
    spip->dmatxmode &= ~(GD32_DMA_CTL_CMEN | GD32_DMA_CTL_HTIE);
  }

  /* SPI setup and enable. NSS is handled externally through the standard
     SPI_SELECT_MODE pad/line/port mechanism, so the peripheral runs in
     software NSS mode (NSSIM) with the internal NSS signal forced high
     (CTL0_NSSI), otherwise the master would detect a mode fault. BDEN/RO
     are always cleared to force full-duplex (2-line) operation, half-duplex
     is not supported by this driver.*/
  SPI_CTL0(spip->spi) &= ~SPI_CTL0_SPIEN;
  SPI_CTL0(spip->spi)  = SPI_CTL0_MASP | SPI_CTL0_NSSI;
  SPI_CTL1(spip->spi)  = 0U;
  SPI_CFG0(spip->spi)  = (spip->config->cfg0 &
                          ~(SPI_CFG0_FIFOLVL | SPI_CFG0_BYTEN |
                            SPI_CFG0_WORDEN)) |
                         accmode | SPI_CFG0_DMAREN | SPI_CFG0_DMATEN;
  SPI_CFG1(spip->spi)  = (spip->config->cfg1 | SPI_CFG1_MSTMOD |
                         SPI_CFG1_NSSIM) & ~(SPI_CFG1_BDEN | SPI_CFG1_RO);
  SPI_INT(spip->spi)   = SPI_INT_RXOREIE;
  SPI_STATC(spip->spi) = 0xFFFFFFFFU;
  SPI_CTL0(spip->spi) |= SPI_CTL0_SPIEN;
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip) {

  /* If in ready state then disables the SPI clock.*/
  if (spip->state == SPI_READY) {

    /* SPI disable.*/
    SPI_CTL0(spip->spi) &= ~SPI_CTL0_SPIEN;
    SPI_CTL0(spip->spi) = 0U;
    SPI_CTL1(spip->spi) = 0U;
    SPI_CFG0(spip->spi) = 0U;
    SPI_CFG1(spip->spi) = 0U;
    SPI_INT(spip->spi)  = 0U;

    dmaStreamFreeI(spip->dmarx);
    dmaStreamFreeI(spip->dmatx);

#if GD32_SPI_USE_SPI0
    if (&SPID0 == spip)
      rcuDisableSPI0();
#endif
#if GD32_SPI_USE_SPI1
    if (&SPID1 == spip)
      rcuDisableSPI1();
#endif
#if GD32_SPI_USE_SPI2
    if (&SPID2 == spip)
      rcuDisableSPI2();
#endif
#if GD32_SPI_USE_SPI3
    if (&SPID3 == spip)
      rcuDisableSPI3();
#endif
#if GD32_SPI_USE_SPI4
    if (&SPID4 == spip)
      rcuDisableSPI4();
#endif
#if GD32_SPI_USE_SPI5
    if (&SPID5 == spip)
      rcuDisableSPI5();
#endif
  }
}

#if (SPI_SELECT_MODE == SPI_SELECT_MODE_LLD) || defined(__DOXYGEN__)
/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_select(SPIDriver *spip) {

  /* No implementation on GD32.*/
  (void)spip;
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_unselect(SPIDriver *spip) {

  /* No implementation on GD32.*/
  (void)spip;
}
#endif

/**
 * @brief   Ignores data on the SPI bus.
 * @details This asynchronous function starts the transmission of a series of
 *          idle words on the SPI bus and ignores the received data.
 * @post    At the end of the operation the configured callback is invoked.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be ignored
 *
 * @notapi
 */
void spi_lld_ignore(SPIDriver *spip, size_t n) {

  osalDbgAssert(n <= GD32_DMA_MAX_TRANSFER, "unsupported DMA transfer size");

  spi_lld_wait_complete(spip);

  dmaStreamSetMemory0(spip->dmarx, spip->config->dummyrx ?
                                   spip->config->dummyrx : &dummyrx);
  dmaStreamSetTransactionSize(spip->dmarx, n);
  dmaStreamSetMode(spip->dmarx, spip->dmarxmode);

  dmaStreamSetMemory0(spip->dmatx, spip->config->dummytx ?
                                   spip->config->dummytx :
                                   (uint32_t *)&dummytx);
  dmaStreamSetTransactionSize(spip->dmatx, n);
  dmaStreamSetMode(spip->dmatx, spip->dmatxmode);

  dmaStreamEnable(spip->dmarx);
  dmaStreamEnable(spip->dmatx);

  SPI_CTL0(spip->spi) |= SPI_CTL0_MSTART;
}

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf     the pointer to the transmit buffer
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {

  osalDbgAssert(n <= GD32_DMA_MAX_TRANSFER, "unsupported DMA transfer size");

  spi_lld_wait_complete(spip);

  dmaStreamSetMemory0(spip->dmarx, rxbuf);
  dmaStreamSetTransactionSize(spip->dmarx, n);
  dmaStreamSetMode(spip->dmarx, spip->dmarxmode | GD32_DMA_CTL_MNAGA);

  dmaStreamSetMemory0(spip->dmatx, txbuf);
  dmaStreamSetTransactionSize(spip->dmatx, n);
  dmaStreamSetMode(spip->dmatx, spip->dmatxmode | GD32_DMA_CTL_MNAGA);

  dmaStreamEnable(spip->dmarx);
  dmaStreamEnable(spip->dmatx);

  SPI_CTL0(spip->spi) |= SPI_CTL0_MSTART;
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {

  osalDbgAssert(n <= GD32_DMA_MAX_TRANSFER, "unsupported DMA transfer size");

  spi_lld_wait_complete(spip);

  dmaStreamSetMemory0(spip->dmarx, spip->config->dummyrx ?
                                   spip->config->dummyrx : &dummyrx);
  dmaStreamSetTransactionSize(spip->dmarx, n);
  dmaStreamSetMode(spip->dmarx, spip->dmarxmode);

  dmaStreamSetMemory0(spip->dmatx, txbuf);
  dmaStreamSetTransactionSize(spip->dmatx, n);
  dmaStreamSetMode(spip->dmatx, spip->dmatxmode | GD32_DMA_CTL_MNAGA);

  dmaStreamEnable(spip->dmarx);
  dmaStreamEnable(spip->dmatx);

  SPI_CTL0(spip->spi) |= SPI_CTL0_MSTART;
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out] rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

  osalDbgAssert(n <= GD32_DMA_MAX_TRANSFER, "unsupported DMA transfer size");

  spi_lld_wait_complete(spip);

  dmaStreamSetMemory0(spip->dmarx, rxbuf);
  dmaStreamSetTransactionSize(spip->dmarx, n);
  dmaStreamSetMode(spip->dmarx, spip->dmarxmode | GD32_DMA_CTL_MNAGA);

  dmaStreamSetMemory0(spip->dmatx, spip->config->dummytx ?
                                   spip->config->dummytx :
                                   (uint32_t *)&dummytx);
  dmaStreamSetTransactionSize(spip->dmatx, n);
  dmaStreamSetMode(spip->dmatx, spip->dmatxmode);

  dmaStreamEnable(spip->dmarx);
  dmaStreamEnable(spip->dmatx);

  SPI_CTL0(spip->spi) |= SPI_CTL0_MSTART;
}

#if (SPI_SUPPORTS_CIRCULAR == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Aborts the ongoing SPI operation, if any.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_abort(SPIDriver *spip) {
  uint32_t dsize = (SPI_CFG0(spip->spi) & SPI_CFG0_DZ) + 1U;
  uint32_t i;

  /* Stopping SPI, frames may still be in flight so a graceful suspension is
     requested first.*/
  SPI_CTL0(spip->spi) |= SPI_CTL0_MSPDR;

  spi_lld_wait_complete(spip);

  /* Stopping DMAs.*/
  dmaStreamDisable(spip->dmatx);
  dmaStreamDisable(spip->dmarx);

  /* Draining frames received but not moved out by the DMA, the next transfer
     would consume them first and shift all the following data. The receive
     FIFO is 16 frames deep.*/
  for (i = 0U; i < 16U; i++) {
    if ((SPI_STAT(spip->spi) & SPI_STAT_RP) == 0U) {
      break;
    }
    if (dsize <= 8U) {
      (void)*(volatile uint8_t *)&SPI_RDATA(spip->spi);
    }
    else if (dsize <= 16U) {
      (void)*(volatile uint16_t *)&SPI_RDATA(spip->spi);
    }
    else {
      (void)SPI_RDATA(spip->spi);
    }
  }

  /* Disabling and re-enabling flushes both FIFOs, the configuration
     registers are not affected.*/
  SPI_CTL0(spip->spi) &= ~SPI_CTL0_SPIEN;
  SPI_STATC(spip->spi) = 0xFFFFFFFFU;
  SPI_CTL0(spip->spi) |= SPI_CTL0_SPIEN;
}
#endif /* SPI_SUPPORTS_CIRCULAR == TRUE */

/**
 * @brief   Exchanges one frame using a polled wait.
 * @details This synchronous function exchanges one frame using a polled
 *          synchronization method. This function is useful when exchanging
 *          small amount of data on high speed channels, usually in this
 *          situation is much more efficient just wait for completion using
 *          polling than suspending the thread waiting for an interrupt.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] frame     the data frame to send over the SPI bus
 * @return              The received data frame from the SPI bus.
 *
 * @notapi
 */
uint32_t spi_lld_polled_exchange(SPIDriver *spip, uint32_t frame) {
  uint32_t dsize = (SPI_CFG0(spip->spi) & SPI_CFG0_DZ) + 1U;
  uint32_t rxframe;
  uint32_t timeout;

  spi_lld_wait_complete(spip);

  SPI_CTL0(spip->spi) |= SPI_CTL0_MSTART;

  /* Wait for room in TX FIFO.*/
  timeout = SPI_BUSY_TIMEOUT;
  while ((SPI_STAT(spip->spi) & SPI_STAT_TP) == 0U) {
    if (--timeout == 0U) {
      osalSysHalt("SPI TX FIFO timeout");
    }
  }

  /* Data register must be accessed with the appropriate data size.
     Byte size access (uint8_t *) for transactions that are <= 8-bit etc.*/
  if (dsize <= 8U) {
    /* Frame width is between 4 and 8 bits.*/
    volatile uint8_t *txdrp8 = (volatile uint8_t *)&SPI_TDATA(spip->spi);
    volatile uint8_t *rxdrp8 = (volatile uint8_t *)&SPI_RDATA(spip->spi);
    *txdrp8 = (uint8_t)frame;
    timeout = SPI_BUSY_TIMEOUT;
    while ((SPI_STAT(spip->spi) & SPI_STAT_RP) == 0U) {
      if (--timeout == 0U) {
        osalSysHalt("SPI RX FIFO timeout");
      }
    }
    rxframe = (uint32_t)*rxdrp8;
  }
  else if (dsize <= 16U) {
    /* Frame width is between 9 and 16 bits.*/
    volatile uint16_t *txdrp16 = (volatile uint16_t *)&SPI_TDATA(spip->spi);
    volatile uint16_t *rxdrp16 = (volatile uint16_t *)&SPI_RDATA(spip->spi);
    *txdrp16 = (uint16_t)frame;
    timeout = SPI_BUSY_TIMEOUT;
    while ((SPI_STAT(spip->spi) & SPI_STAT_RP) == 0U) {
      if (--timeout == 0U) {
        osalSysHalt("SPI RX FIFO timeout");
      }
    }
    rxframe = (uint32_t)*rxdrp16;
  }
  else {
    /* Frame width is between 17 and 32 bits.*/
    SPI_TDATA(spip->spi) = frame;
    timeout = SPI_BUSY_TIMEOUT;
    while ((SPI_STAT(spip->spi) & SPI_STAT_RP) == 0U) {
      if (--timeout == 0U) {
        osalSysHalt("SPI RX FIFO timeout");
      }
    }
    rxframe = SPI_RDATA(spip->spi);
  }

  SPI_CTL0(spip->spi) |= SPI_CTL0_MSPDR;

  return rxframe;
}

#endif /* HAL_USE_SPI */

/** @} */
