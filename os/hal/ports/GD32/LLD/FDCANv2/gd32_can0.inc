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
 * @file    FDCANv2/gd32_can0.inc
 * @brief   GD32 CAN0 interrupt handlers.
 *
 * @addtogroup GD32_CAN0_HANDLER
 * @{
 */

#if defined(GD32_CAN0_IS_USED)

#if !defined(GD32_IRQ_CAN0_WKUP_PRIORITY) ||                              \
    !defined(GD32_IRQ_CAN0_MSG_PRIORITY) ||                               \
    !defined(GD32_IRQ_CAN0_BUSOFF_PRIORITY) ||                            \
    !defined(GD32_IRQ_CAN0_ERR_PRIORITY) ||                               \
    !defined(GD32_IRQ_CAN0_FASTERR_PRIORITY) ||                           \
    !defined(GD32_IRQ_CAN0_TEC_PRIORITY) ||                               \
    !defined(GD32_IRQ_CAN0_REC_PRIORITY)
#error "CAN0 IRQ priorities not defined in mcuconf.h"
#endif

#if !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_CAN0_WKUP_PRIORITY) ||            \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_CAN0_MSG_PRIORITY) ||             \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_CAN0_BUSOFF_PRIORITY) ||          \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_CAN0_ERR_PRIORITY) ||             \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_CAN0_FASTERR_PRIORITY) ||         \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_CAN0_TEC_PRIORITY) ||             \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_IRQ_CAN0_REC_PRIORITY)
#error "Invalid CAN0 IRQ priority"
#endif

#endif /* defined(GD32_CAN0_IS_USED) */

static inline void gd32_can0_irq_init(void) {
#if defined(GD32_CAN0_IS_USED)
  nvicEnableVector(GD32_CAN0_WKUP_NUMBER, GD32_IRQ_CAN0_WKUP_PRIORITY);
  nvicEnableVector(GD32_CAN0_MSG_NUMBER, GD32_IRQ_CAN0_MSG_PRIORITY);
  nvicEnableVector(GD32_CAN0_BUSOFF_NUMBER, GD32_IRQ_CAN0_BUSOFF_PRIORITY);
  nvicEnableVector(GD32_CAN0_ERR_NUMBER, GD32_IRQ_CAN0_ERR_PRIORITY);
  nvicEnableVector(GD32_CAN0_FASTERR_NUMBER, GD32_IRQ_CAN0_FASTERR_PRIORITY);
  nvicEnableVector(GD32_CAN0_TEC_NUMBER, GD32_IRQ_CAN0_TEC_PRIORITY);
  nvicEnableVector(GD32_CAN0_REC_NUMBER, GD32_IRQ_CAN0_REC_PRIORITY);
#endif
}

static inline void gd32_can0_irq_deinit(void) {
#if defined(GD32_CAN0_IS_USED)
  nvicDisableVector(GD32_CAN0_WKUP_NUMBER);
  nvicDisableVector(GD32_CAN0_MSG_NUMBER);
  nvicDisableVector(GD32_CAN0_BUSOFF_NUMBER);
  nvicDisableVector(GD32_CAN0_ERR_NUMBER);
  nvicDisableVector(GD32_CAN0_FASTERR_NUMBER);
  nvicDisableVector(GD32_CAN0_TEC_NUMBER);
  nvicDisableVector(GD32_CAN0_REC_NUMBER);
#endif
}

#if defined(GD32_CAN0_IS_USED) || defined(__DOXYGEN__)

#define GD32_CAN0_IRQ_HANDLER(handler)                                      \
  OSAL_IRQ_HANDLER(handler) {                                               \
    OSAL_IRQ_PROLOGUE();                                                    \
    can_lld_serve_interrupt(&CAND0);                                        \
    OSAL_IRQ_EPILOGUE();                                                    \
  }

GD32_CAN0_IRQ_HANDLER(GD32_CAN0_WKUP_HANDLER)
GD32_CAN0_IRQ_HANDLER(GD32_CAN0_MSG_HANDLER)
GD32_CAN0_IRQ_HANDLER(GD32_CAN0_BUSOFF_HANDLER)
GD32_CAN0_IRQ_HANDLER(GD32_CAN0_ERR_HANDLER)
GD32_CAN0_IRQ_HANDLER(GD32_CAN0_FASTERR_HANDLER)
GD32_CAN0_IRQ_HANDLER(GD32_CAN0_TEC_HANDLER)
GD32_CAN0_IRQ_HANDLER(GD32_CAN0_REC_HANDLER)

#undef GD32_CAN0_IRQ_HANDLER

#endif /* defined(GD32_CAN0_IS_USED) */

/** @} */