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
 * @file    hal_efl_lld.h
 * @brief   GD32H73x_75x Embedded Flash subsystem low level driver header.
 *
 * @addtogroup HAL_EFL
 * @{
 */

#ifndef HAL_EFL_LLD_H
#define HAL_EFL_LLD_H

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/* Flash descriptor attributes: erased bit is one, memory mapped. */
#define GD32_FLASH_ATTR             (FLASH_ATTR_ERASED_IS_ONE |           \
                                     FLASH_ATTR_MEMORY_MAPPED)

/* Flash line (program granularity) is one 32-bit word. */
#define GD32_FLASH_LINE_SIZE        4U
#define GD32_FLASH_LINE_MASK        (GD32_FLASH_LINE_SIZE - 1U)

/* Uniform sector (page) size: 4 KiB. */
#define GD32_FLASH_SECTOR_SIZE      4096U

/* Memory density register: encodes the total flash size in KiB,
   right-shifted by GD32_FLASH_DENSITY_OFFSET bits. */
#define GD32_FLASH_DENSITY_ADDRESS  ((uint32_t)0x1FF0F7E0U)
#define GD32_FLASH_DENSITY_OFFSET   16U
#define GD32_FLASH_DENSITY_SCALE     1024U

/* Poll timeout for busy-wait (arbitrary large value; the real FMC
   operations complete within hardware-defined limits). */
#define GD32_FMC_TIMEOUT_COUNT      ((uint32_t)0x00FFFFFFU)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    GD32H73x_75x EFL driver configuration options
 * @{
 */
/**
 * @brief   Suggested wait time during erase operations polling.
 */
#if !defined(GD32_FLASH_WAIT_TIME_MS) || defined(__DOXYGEN__)
#define GD32_FLASH_WAIT_TIME_MS      5
#endif
/** @} */

/*===========================================================================*/
/* Driver data structures and types.                                        */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Low level fields of the embedded flash driver structure.
 */
#define efl_lld_driver_fields                                              \
  /* Flash descriptor reference.*/                                         \
  const flash_descriptor_t  *descriptor;

/**
 * @brief   Low level fields of the embedded flash configuration structure.
 */
#define efl_lld_config_fields                                              \
  /* Dummy configuration, it is not needed.*/                             \
  uint32_t                  dummy;

/*===========================================================================*/
/* External declarations.                                                   */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern EFlashDriver EFLD1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void efl_lld_init(void);
  void efl_lld_start(EFlashDriver *eflp);
  void efl_lld_stop(EFlashDriver *eflp);
  const flash_descriptor_t *efl_lld_get_descriptor(void *instance);
  flash_error_t efl_lld_read(void *instance, flash_offset_t offset,
                             size_t n, uint8_t *rp);
  flash_error_t efl_lld_program(void *instance, flash_offset_t offset,
                                size_t n, const uint8_t *pp);
  flash_error_t efl_lld_start_erase_all(void *instance);
  flash_error_t efl_lld_start_erase_sector(void *instance,
                                           flash_sector_t sector);
  flash_error_t efl_lld_query_erase(void *instance, uint32_t *wait_time);
  flash_error_t efl_lld_verify_erase(void *instance, flash_sector_t sector);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_EFL == TRUE */

#endif /* HAL_EFL_LLD_H */

/** @} */
