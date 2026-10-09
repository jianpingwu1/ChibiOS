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
 * @file    USB/gd32_usbhs0.inc
 * @brief   Shared USBHS0 handler.
 * @note    Driver instance numbering follows GD32's own 0-based hardware
 *          peripheral numbering:
 *          USBHS0 (hardware) <-> USBD0 (ChibiOS driver object).
 * @note    The dedicated EP1 IN/OUT vectors are not used, this driver
 *          works with the single global core interrupt.
 *
 * @addtogroup GD32_USBHS0_HANDLER
 * @{
 */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks for robustness.*/
#if !defined(GD32_HAS_USBHS0)
#error "GD32_HAS_USBHS0 not defined in registry"
#endif

#if defined(GD32_USBHS0_IS_USED)

/* Priority settings checks.*/
#if !defined(GD32_IRQ_USBHS0_PRIORITY)
#error "GD32_IRQ_USBHS0_PRIORITY not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_USBHS0_PRIORITY)
#error "Invalid IRQ priority assigned to GD32_IRQ_USBHS0_PRIORITY"
#endif

#endif /* defined(GD32_USBHS0_IS_USED) */

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static inline void usbhs0_irq_init(void) {
#if defined(GD32_USBHS0_IS_USED)
  nvicEnableVector(GD32_USBHS0_NUMBER, GD32_IRQ_USBHS0_PRIORITY);
#endif
}

static inline void usbhs0_irq_deinit(void) {
#if defined(GD32_USBHS0_IS_USED)
  nvicDisableVector(GD32_USBHS0_NUMBER);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if defined(GD32_USBHS0_IS_USED) || defined(__DOXYGEN__)
/**
 * @brief   USBHS0 interrupt handler.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(GD32_USBHS0_HANDLER) {

  OSAL_IRQ_PROLOGUE();

#if HAL_USE_USB
#if GD32_USB_USE_USBHS0
  usb_lld_serve_interrupt(&USBD0);
#endif
#endif

  OSAL_IRQ_EPILOGUE();
}
#endif

/** @} */
