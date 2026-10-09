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
 * @file    hal_efl_lld.c
 * @brief   GD32H73x_75x Embedded Flash subsystem low level driver.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#include "hal.h"

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

#include <string.h>
#include "gd32h73x_75x_fmc.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* FMC status register error mask: all error flags. */
#define GD32_FMC_ERR_MASK           (FMC_STAT_ENDF    | FMC_STAT_WPERR   |  \
                                     FMC_STAT_PGSERR  | FMC_STAT_RPERR   |  \
                                     FMC_STAT_RSERR   | FMC_STAT_ECCCOR  |  \
                                     FMC_STAT_ECCDET  | FMC_STAT_OBMERR)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EFL1 driver identifier.
 */
EFlashDriver EFLD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/* Unified flash descriptor: uniform 4 KiB sectors, filled at init. */
static flash_descriptor_t gd32_efl_descriptor;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Unlocks the FMC control register.
 * @note    Uses the GD32H7 unlock key sequence defined in gd32h73x_75x_fmc.h.
 */
static inline void gd32_fmc_unlock(void) {

  if ((FMC_CTL & FMC_CTL_LK) != 0U) {
    FMC_KEY = UNLOCK_KEY0;
    FMC_KEY = UNLOCK_KEY1;
  }
}

/**
 * @brief   Locks the FMC control register.
 */
static inline void gd32_fmc_lock(void) {

  FMC_CTL |= FMC_CTL_LK;
}

/**
 * @brief   Clears all pending FMC status flags.
 */
static inline void gd32_fmc_clear_flags(void) {

  FMC_STAT = GD32_FMC_ERR_MASK;
}

/**
 * @brief   Waits for FMC to become ready, with timeout exit.
 * @return  true if ready, false if timed out.
 */
static inline bool gd32_fmc_wait_ready(void) {
  uint32_t timeout = GD32_FMC_TIMEOUT_COUNT;

  while ((FMC_STAT & FMC_STAT_BUSY) != 0U) {
    if (timeout == 0U) {
      return false;
    }
    timeout--;
  }

  return true;
}

/**
 * @brief   Checks and clears FMC error flags.
 * @return  Mapped flash_error_t value.
 */
static inline flash_error_t gd32_fmc_check_errors(void) {
  uint32_t stat = FMC_STAT;

  /* Clearing error conditions. */
  gd32_fmc_clear_flags();

  if ((stat & FMC_STAT_WPERR) != 0U) {
    return FLASH_ERROR_HW_FAILURE;
  }

  if ((stat & FMC_STAT_PGSERR) != 0U) {
    return FLASH_ERROR_PROGRAM;
  }

  if ((stat & (FMC_STAT_RPERR | FMC_STAT_RSERR)) != 0U) {
    return FLASH_ERROR_HW_FAILURE;
  }

  if ((stat & FMC_STAT_ECCDET) != 0U) {
    return FLASH_ERROR_READ;
  }

  return FLASH_NO_ERROR;
}

/**
 * @brief   Reads the total main flash size in bytes from the density register.
 */
static inline uint32_t gd32_flash_get_size(void) {

  return (REG32(GD32_FLASH_DENSITY_ADDRESS) >> GD32_FLASH_DENSITY_OFFSET)
         * GD32_FLASH_DENSITY_SCALE;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level Embedded Flash driver initialization.
 *
 * @notapi
 */
void efl_lld_init(void) {
  uint32_t flash_size;
  uint32_t sector_count;

  /* Driver initialization. */
  eflObjectInit(&EFLD1);

  flash_size   = gd32_flash_get_size();
  sector_count = flash_size / GD32_FLASH_SECTOR_SIZE;

  /* Uniform 4 KiB sectors: no sector table needed (sectors == NULL).
     The hal_flash.c helpers compute offsets/sizes from sectors_size. */
  gd32_efl_descriptor.attributes     = GD32_FLASH_ATTR;
  gd32_efl_descriptor.page_size      = GD32_FLASH_LINE_SIZE;
  gd32_efl_descriptor.sectors_count  = sector_count;
  gd32_efl_descriptor.sectors        = NULL;
  gd32_efl_descriptor.sectors_size  = GD32_FLASH_SECTOR_SIZE;
  gd32_efl_descriptor.address        = (uint8_t *)FLASH_BASE;
  gd32_efl_descriptor.size           = flash_size;

  EFLD1.descriptor = &gd32_efl_descriptor;
}

/**
 * @brief   Configures and activates the Embedded Flash peripheral.
 *
 * @param[in] eflp      pointer to a @p EFlashDriver structure
 *
 * @notapi
 */
void efl_lld_start(EFlashDriver *eflp) {

  (void)eflp;

  gd32_fmc_unlock();

  /* Clear any residual operation bits (PG/SER/MER) from a prior session. */
  FMC_CTL &= ~(FMC_CTL_PG | FMC_CTL_SER | FMC_CTL_MER | FMC_CTL_PGCHEN);

  gd32_fmc_clear_flags();
}

/**
 * @brief   Deactivates the Embedded Flash peripheral.
 *
 * @param[in] eflp      pointer to a @p EFlashDriver structure
 *
 * @notapi
 */
void efl_lld_stop(EFlashDriver *eflp) {

  (void)eflp;

  gd32_fmc_lock();
}

/**
 * @brief   Gets the flash descriptor structure.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @return                          A flash device descriptor.
 *
 * @notapi
 */
const flash_descriptor_t *efl_lld_get_descriptor(void *instance) {
  EFlashDriver *devp = (EFlashDriver *)instance;

  return devp->descriptor;
}

/**
 * @brief   Read operation.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @param[in] offset                offset within full flash address space
 * @param[in] n                     number of bytes to be read
 * @param[out] rp                   pointer to the data buffer
 * @return                          An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_read(void *instance, flash_offset_t offset,
                           size_t n, uint8_t *rp) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  const flash_descriptor_t *desc = efl_lld_get_descriptor(instance);

  osalDbgCheck((instance != NULL) && (rp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)desc->size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No reading while erasing. */
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  devp->state = FLASH_READ;

  /* Clearing error status bits. */
  gd32_fmc_clear_flags();

  /* Memory-mapped read: just a memcpy. */
  memcpy((void *)rp,
         (const void *)(desc->address + offset), n);

  devp->state = FLASH_READY;

  return FLASH_NO_ERROR;
}

/**
 * @brief   Program operation.
 * @note    The device supports word programming; each call programs
 *          32-bit words aligned to word boundaries.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @param[in] offset                offset within full flash address space
 * @param[in] n                     number of bytes to be programmed
 * @param[in] pp                    pointer to the data buffer
 * @return                          An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_program(void *instance, flash_offset_t offset,
                              size_t n, const uint8_t *pp) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  const flash_descriptor_t *desc = efl_lld_get_descriptor(instance);
  flash_error_t err = FLASH_NO_ERROR;

  osalDbgCheck((instance != NULL) && (pp != NULL) && (n > 0U));
  osalDbgCheck((size_t)offset + n <= (size_t)desc->size);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  /* No programming while erasing. */
  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  devp->state = FLASH_PGM;

  /* Clearing error status bits. */
  gd32_fmc_clear_flags();

  /* Enable programming. */
  FMC_CTL |= FMC_CTL_PG;

  /* Program word by word. Unaligned trailing bytes are padded with 0xFF. */
  while (n > 0U) {
    uint32_t addr = (uint32_t)(desc->address + offset);
    uint32_t word;
    uint8_t  i;

    /* Prepare the word: start from all-ones (erased state). */
    word = 0xFFFFFFFFU;
    for (i = 0U; (i < 4U) && (n > 0U); i++) {
      ((uint8_t *)&word)[i] = *pp++;
      offset++;
      n--;
    }

    /* Write the word to flash. */
    __ISB();
    __DSB();
    REG32(addr) = word;
    __ISB();
    __DSB();

    /* Wait for completion (with timeout). */
    if (!gd32_fmc_wait_ready()) {
      FMC_CTL &= ~FMC_CTL_PG;
      devp->state = FLASH_READY;
      return FLASH_ERROR_HW_FAILURE;
    }

    err = gd32_fmc_check_errors();
    if (err != FLASH_NO_ERROR) {
      break;
    }
  }

  /* Disable programming. */
  FMC_CTL &= ~FMC_CTL_PG;

  devp->state = FLASH_READY;

  return err;
}

/**
 * @brief   Starts a whole-device erase operation.
 * @note    Mass erase of the entire main flash is supported;
 *          it is the caller's responsibility to use it safely.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @return                          An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_all(void *instance) {
  EFlashDriver *devp = (EFlashDriver *)instance;

  osalDbgCheck(instance != NULL);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  devp->state = FLASH_ERASE;

  /* Clearing error status bits. */
  gd32_fmc_clear_flags();

  /* Enable mass erase and start. */
  FMC_CTL |= FMC_CTL_MER;
  FMC_CTL |= FMC_CTL_START;

  __DSB();

  return FLASH_NO_ERROR;
}

/**
 * @brief   Starts a sector erase operation.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @param[in] sector                sector index to be erased
 * @return                          An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_start_erase_sector(void *instance,
                                         flash_sector_t sector) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  const flash_descriptor_t *desc = efl_lld_get_descriptor(instance);
  flash_offset_t sector_offset;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < desc->sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  sector_offset = flashGetSectorOffset(getBaseFlash(devp), sector);

  devp->state = FLASH_ERASE;

  /* Invalidate cache lines that may overlap the erasing sector. */
  cacheBufferInvalidate(desc->address + sector_offset,
                        flashGetSectorSize(getBaseFlash(devp), sector));

  /* Clearing error status bits. */
  gd32_fmc_clear_flags();

  /* Enable sector erase. */
  FMC_CTL |= FMC_CTL_SER;

  /* Write the sector address and start. */
  FMC_ADDR = (uint32_t)(desc->address + sector_offset);
  FMC_CTL |= FMC_CTL_START;

  __DSB();

  return FLASH_NO_ERROR;
}

/**
 * @brief   Queries the driver for erase operation progress.
 *
 * @param[in]  instance             pointer to a @p EFlashDriver instance
 * @param[out] msec                 recommended time in ms before calling again
 * @return                          An error code.
 *
 * @api
 */
flash_error_t efl_lld_query_erase(void *instance, uint32_t *msec) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  flash_error_t err = FLASH_NO_ERROR;

  if (devp->state == FLASH_ERASE) {

    /* Checking for operation in progress. */
    if ((FMC_STAT & FMC_STAT_BUSY) == 0U) {

      /* Disabling the erase control bits. */
      FMC_CTL &= ~(FMC_CTL_MER | FMC_CTL_SER);

      /* Check for errors. */
      err = gd32_fmc_check_errors();

      devp->state = FLASH_READY;
    }
    else {
      if (msec != NULL) {
        *msec = (uint32_t)GD32_FLASH_WAIT_TIME_MS;
      }
      err = FLASH_BUSY_ERASING;
    }
  }

  return err;
}

/**
 * @brief   Returns the erase state of a sector.
 *
 * @param[in] ip                    pointer to a @p EFlashDriver instance
 * @param[in] sector                sector to be verified
 * @return                          An error code.
 *
 * @notapi
 */
flash_error_t efl_lld_verify_erase(void *instance, flash_sector_t sector) {
  EFlashDriver *devp = (EFlashDriver *)instance;
  const flash_descriptor_t *desc = efl_lld_get_descriptor(instance);
  uint32_t *address;
  flash_error_t err = FLASH_NO_ERROR;
  uint32_t sector_size;
  uint32_t i;

  osalDbgCheck(instance != NULL);
  osalDbgCheck(sector < desc->sectors_count);
  osalDbgAssert((devp->state == FLASH_READY) || (devp->state == FLASH_ERASE),
                "invalid state");

  if (devp->state == FLASH_ERASE) {
    return FLASH_BUSY_ERASING;
  }

  address = (uint32_t *)(desc->address +
                         flashGetSectorOffset(getBaseFlash(devp), sector));

  devp->state = FLASH_READ;

  sector_size = flashGetSectorSize(getBaseFlash(devp), sector);
  for (i = 0U; i < sector_size / sizeof(uint32_t); i++) {
    if (*address != 0xFFFFFFFFU) {
      err = FLASH_ERROR_VERIFY;
      break;
    }
    address++;
  }

  devp->state = FLASH_READY;

  return err;
}

#endif /* HAL_USE_EFL == TRUE */

/** @} */
