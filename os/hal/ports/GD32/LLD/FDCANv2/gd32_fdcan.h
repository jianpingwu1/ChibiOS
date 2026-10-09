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
 * @file    FDCANv2/gd32_fdcan.h
 * @brief   GD32H73x/75x CAN-FD helper macros.
 *
 * @addtogroup CAN
 * @{
 */

#ifndef GD32_FDCAN_H
#define GD32_FDCAN_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/* The GD32 nominal timing splits the segment before the sample point into
    PTS and PBS1 fields.*/
#define CAN_CONFIG_BT_PBS2(n)            BT_PBS2(n)
#define CAN_CONFIG_BT_PBS1(n)            BT_PBS1(n)
#define CAN_CONFIG_BT_PTS(n)             BT_PTS(n)
#define CAN_CONFIG_BT_BAUDPSC(n)         BT_BAUDPSC(n)
#define CAN_CONFIG_BT_SJW(n)             BT_SJW(n)

/* The same split exists in the FD data phase between DPTS and DPBS1.*/
#define CAN_CONFIG_FDBT_DPBS2(n)         FDBT_DPBS2(n)
#define CAN_CONFIG_FDBT_DPBS1(n)         FDBT_DPBS1(n)
#define CAN_CONFIG_FDBT_DPTS(n)          FDBT_DPTS(n)
#define CAN_CONFIG_FDBT_DBAUDPSC(n)      FDBT_DBAUDPSC(n)
#define CAN_CONFIG_FDBT_DSJW(n)          FDBT_DSJW(n)

/* Filter masks select the bits that must match, a cleared mask accepts
   all identifiers.*/
#define GD32_CAN_ACCEPT_ALL_FILTER         0x00000000U

#define GD32_CAN_RAM_BYTES                 512U
#define GD32_CAN_MAILBOX_COUNT             7U
#define GD32_CAN_RX_MAILBOX_COUNT          4U
#define GD32_CAN_TX_MAILBOX_COUNT          3U
#define GD32_CAN_TX_MAILBOX_FIRST          GD32_CAN_RX_MAILBOX_COUNT
#define GD32_CAN_MAILBOX_WORDS             18U
#define GD32_CAN_MAILBOX_BYTES             (GD32_CAN_MAILBOX_WORDS * 4U)
#define GD32_CAN_MAILBOX_INT_MASK           ((1UL << GD32_CAN_MAILBOX_COUNT) - 1UL)
#define GD32_CAN_RX_INT_MASK                ((1UL << GD32_CAN_RX_MAILBOX_COUNT) - 1UL)
#define GD32_CAN_TX_INT_MASK                (GD32_CAN_MAILBOX_INT_MASK &              \
                                             ~GD32_CAN_RX_INT_MASK)

#if (GD32_CAN_RX_MAILBOX_COUNT + GD32_CAN_TX_MAILBOX_COUNT) >                \
    GD32_CAN_MAILBOX_COUNT
#error "GD32 CAN mailbox allocation exceeds the 64-byte CAN-FD RAM window"
#endif

#if (GD32_CAN_MAILBOX_COUNT * GD32_CAN_MAILBOX_BYTES) > GD32_CAN_RAM_BYTES
#error "GD32 CAN mailbox allocation exceeds the 512-byte CAN RAM"
#endif

#define GD32_CAN_MAILBOX_ADDRESS(can, index)                                \
  (CAN_RAM(can) + ((uint32_t)(index) * GD32_CAN_MAILBOX_BYTES))

#endif /* GD32_FDCAN_H */

/** @} */