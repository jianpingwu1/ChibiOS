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
 * @file    USB/gd32_usbhs1.inc
 * @brief   Shared USBHS1 handler.
 * @note    Driver instance numbering follows GD32's own 0-based hardware
 *          peripheral numbering:
 *          USBHS1 (hardware) <-> USBD1 (ChibiOS driver object).
 * @note    The dedicated EP1 IN/OUT vectors are not used, this driver
 *          works with the single global core interrupt.
 *
 * @addtogroup GD32_USBHS1_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(GD32_HAS_USBHS1)
#error "GD32_HAS_USBHS1 not defined in registry"
#endif

#if defined(GD32_USBHS1_IS_USED)

/* Priority settings checks.*/
#if !defined(GD32_IRQ_USBHS1_PRIORITY)
#error "GD32_IRQ_USBHS1_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_USBHS1_PRIORITY)
#error "Invalid IRQ priority assigned to GD32_IRQ_USBHS1_PRIORITY"
#endif

#endif /* defined(GD32_USBHS1_IS_USED) */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void usbhs1_irq_init(void) {
#if defined(GD32_USBHS1_IS_USED)
  nvicEnableVector(GD32_USBHS1_NUMBER, GD32_IRQ_USBHS1_PRIORITY);
#endif
}

static inline void usbhs1_irq_deinit(void) {
#if defined(GD32_USBHS1_IS_USED)
  nvicDisableVector(GD32_USBHS1_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(GD32_USBHS1_IS_USED) || defined(__DOXYGEN__)
/**
 * @brief   USBHS1 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_USBHS1_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_USB
#if GD32_USB_USE_USBHS1
  usb_lld_serve_interrupt(&USBD1);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

/** @} */
