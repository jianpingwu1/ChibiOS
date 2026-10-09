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
 * @file    FDCANv2/hal_can_lld.h
 * @brief   GD32H73x/75x CAN-FD subsystem low level driver header.
 *
 * @addtogroup CAN
 * @{
 */

#ifndef HAL_CAN_LLD_H
#define HAL_CAN_LLD_H

#if HAL_USE_CAN || defined(__DOXYGEN__)

#include "gd32_fdcan.h"

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define CAN_MAX_DLC_BYTES           64U
#define CAN_TX_MAILBOXES            1U
#define CAN_RX_MAILBOXES            1U
#define CAN_SUPPORTS_SLEEP          TRUE

#define FDCAN_0BYTES_TO_DLC         0U
#define FDCAN_1BYTES_TO_DLC         1U
#define FDCAN_2BYTES_TO_DLC         2U
#define FDCAN_3BYTES_TO_DLC         3U
#define FDCAN_4BYTES_TO_DLC         4U
#define FDCAN_5BYTES_TO_DLC         5U
#define FDCAN_6BYTES_TO_DLC         6U
#define FDCAN_7BYTES_TO_DLC         7U
#define FDCAN_8BYTES_TO_DLC         8U
#define FDCAN_12BYTES_TO_DLC        9U
#define FDCAN_16BYTES_TO_DLC        10U
#define FDCAN_20BYTES_TO_DLC        11U
#define FDCAN_24BYTES_TO_DLC        12U
#define FDCAN_32BYTES_TO_DLC        13U
#define FDCAN_48BYTES_TO_DLC        14U
#define FDCAN_64BYTES_TO_DLC        15U

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

#if !defined(GD32_CAN_USE_CAN0) || defined(__DOXYGEN__)
#define GD32_CAN_USE_CAN0                    FALSE
#endif

#if !defined(GD32_CAN_USE_CAN1) || defined(__DOXYGEN__)
#define GD32_CAN_USE_CAN1                    FALSE
#endif

#if !defined(GD32_CAN_USE_CAN2) || defined(__DOXYGEN__)
#define GD32_CAN_USE_CAN2                    FALSE
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !defined(GD32_HAS_CAN0) || !defined(GD32_HAS_CAN1) ||                   \
    !defined(GD32_HAS_CAN2)
#error "GD32 CAN capabilities not defined in registry"
#endif

#if GD32_CAN_USE_CAN0 && !GD32_HAS_CAN0
#error "CAN0 not present in the selected device"
#endif

#if GD32_CAN_USE_CAN1 && !GD32_HAS_CAN1
#error "CAN1 not present in the selected device"
#endif

#if GD32_CAN_USE_CAN2 && !GD32_HAS_CAN2
#error "CAN2 not present in the selected device"
#endif

#if !GD32_CAN_USE_CAN0 && !GD32_CAN_USE_CAN1 && !GD32_CAN_USE_CAN2
#error "CAN driver activated but no CAN peripheral assigned"
#endif

#if !defined(GD32_CAN_SUPPORTS_FD) || !GD32_CAN_SUPPORTS_FD
#error "Selected device does not support CAN-FD"
#endif

#if GD32_CAN_USE_CAN0
#define GD32_CAN0_IS_USED
#endif
#if GD32_CAN_USE_CAN1
#define GD32_CAN1_IS_USED
#endif
#if GD32_CAN_USE_CAN2
#define GD32_CAN2_IS_USED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef enum {
  OPMODE_FDCAN = 1,
  OPMODE_CAN = 2
} fdcanopmode_t;

typedef struct hal_can_driver CANDriver;
typedef uint32_t canmbx_t;

#if (CAN_ENFORCE_USE_CALLBACKS == TRUE) || defined(__DOXYGEN__)
typedef void (*can_callback_t)(CANDriver *canp, uint32_t flags);
#endif

typedef struct {
  union {
    struct {
      union {
        struct {
          uint32_t          EID:29;
        } ext;
        struct {
          uint32_t          _R1:18;
          uint32_t          SID:11;
        } std;
        struct {
          uint32_t          _R1:29;
          uint32_t          RTR:1;
          uint32_t          XTD:1;
          uint32_t          ESI:1;
        } common;
      };
      uint32_t              _R2:16;
      uint32_t              DLC:4;
      uint32_t              BPS:1;
      uint32_t              FDF:1;
      uint32_t              _R3:1;
      uint32_t              EFC:1;
      uint32_t              MM:8;
    };
    uint32_t                header32[2];
  };
  union {
    uint8_t                 data8[CAN_MAX_DLC_BYTES];
    uint16_t                data16[CAN_MAX_DLC_BYTES / 2U];
    uint32_t                data32[CAN_MAX_DLC_BYTES / 4U];
  };
} CANTxFrame;

typedef struct {
  union {
    struct {
      union {
        struct {
          uint32_t          EID:29;
        } ext;
        struct {
          uint32_t          _R1:18;
          uint32_t          SID:11;
        } std;
        struct {
          uint32_t          _R1:29;
          uint32_t          RTR:1;
          uint32_t          XTD:1;
          uint32_t          ESI:1;
        } common;
      };
      uint32_t              RXTS:16;
      uint32_t              DLC:4;
      uint32_t              BRS:1;
      uint32_t              FDF:1;
      uint32_t              _R2:2;
      uint32_t              FIDX:7;
      uint32_t              ANMF:1;
    };
    uint32_t                header32[2];
  };
  union {
    uint8_t                 data8[CAN_MAX_DLC_BYTES];
    uint16_t                data16[CAN_MAX_DLC_BYTES / 2U];
    uint32_t                data32[CAN_MAX_DLC_BYTES / 4U];
  };
} CANRxFrame;

typedef enum {
  CAN_FILTER_TYPE_STD = 0x00,
  CAN_FILTER_TYPE_EXT = 0x01
} filter_type_t;

typedef enum {
  CAN_FILTER_MODE_RANGE = 0x00,
  CAN_FILTER_MODE_DUAL = 0x01,
  CAN_FILTER_MODE_CLASSIC = 0x02
} filter_mode_t;

typedef enum {
  CAN_FILTER_CFG_FIFO_0 = 0x01,
  CAN_FILTER_CFG_FIFO_1 = 0x02,
  CAN_FILTER_CFG_REJECT = 0x03
} filter_cfg_t;

typedef struct {
  filter_type_t             filter_type;
  filter_mode_t             filter_mode;
  filter_cfg_t              filter_cfg;
  uint32_t                  identifier1;
  uint32_t                  identifier2;
} CANFilter;

typedef struct hal_can_config {
  fdcanopmode_t             op_mode;
  uint32_t                  bt;
  uint32_t                  fdbt;
  uint32_t                  fdctl;
  uint32_t                  ctl1;
  uint32_t                  ctl2;
  uint32_t                  rx_public_filter;
} CANConfig;

struct hal_can_driver {
  canstate_t                state;
  const CANConfig           *config;
  threads_queue_t           txqueue;
  threads_queue_t           rxqueue;
#if (CAN_ENFORCE_USE_CALLBACKS == FALSE) || defined(__DOXYGEN__)
  event_source_t            rxfull_event;
  event_source_t            txempty_event;
  event_source_t            error_event;
#if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
  event_source_t            sleep_event;
  event_source_t            wakeup_event;
#endif
#else
  can_callback_t            rxfull_cb;
  can_callback_t            txempty_cb;
  can_callback_t            error_cb;
#if CAN_USE_SLEEP_MODE || defined(__DOXYGEN__)
  can_callback_t            wakeup_cb;
#endif
#endif
  uint32_t                  can;
  uint32_t                  clock;
};

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if GD32_CAN_USE_CAN0 && !defined(__DOXYGEN__)
extern CANDriver CAND0;
#endif
#if GD32_CAN_USE_CAN1 && !defined(__DOXYGEN__)
extern CANDriver CAND1;
#endif
#if GD32_CAN_USE_CAN2 && !defined(__DOXYGEN__)
extern CANDriver CAND2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void can_lld_init(void);
  bool can_lld_start(CANDriver *canp);
  void can_lld_stop(CANDriver *canp);
  bool can_lld_is_tx_empty(CANDriver *canp, canmbx_t mailbox);
  void can_lld_transmit(CANDriver *canp,
                        canmbx_t mailbox,
                        const CANTxFrame *ctfp);
  bool can_lld_is_rx_nonempty(CANDriver *canp, canmbx_t mailbox);
  void can_lld_receive(CANDriver *canp,
                       canmbx_t mailbox,
                       CANRxFrame *crfp);
  void can_lld_abort(CANDriver *canp, canmbx_t mailbox);
#if CAN_USE_SLEEP_MODE
  void can_lld_sleep(CANDriver *canp);
  void can_lld_wakeup(CANDriver *canp);
#endif
  void can_lld_serve_interrupt(CANDriver *canp);
  void canGD32SetFilters(CANDriver *canp,
                         uint8_t num,
                         const CANFilter *cfp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_CAN */

#endif /* HAL_CAN_LLD_H */

/** @} */