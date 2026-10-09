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
 * @file    USARTv1/hal_sio_lld.h
 * @brief   GD32H73x_75x SIO subsystem low level driver header.
 *
 * @addtogroup SIO
 * @{
 */

#ifndef HAL_SIO_LLD_H
#define HAL_SIO_LLD_H

#if (HAL_USE_SIO == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Mask of RX-related errors in the STAT register.
 */
#define SIO_LLD_STAT_RX_ERRORS          (USART_STAT_PERR  |                 \
                                         USART_STAT_FERR  |                 \
                                         USART_STAT_NERR  |                 \
                                         USART_STAT_ORERR |                 \
                                         USART_STAT_LBDF)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   SIO driver on USART0 enable switch.
 * @details If set to @p TRUE the support for USART0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SIO_USE_USART0) || defined(__DOXYGEN__)
#define GD32_SIO_USE_USART0                 FALSE
#endif

/**
 * @brief   SIO driver on USART1 enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SIO_USE_USART1) || defined(__DOXYGEN__)
#define GD32_SIO_USE_USART1                 FALSE
#endif

/**
 * @brief   SIO driver on USART2 enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SIO_USE_USART2) || defined(__DOXYGEN__)
#define GD32_SIO_USE_USART2                 FALSE
#endif

/**
 * @brief   SIO driver on UART3 enable switch.
 * @details If set to @p TRUE the support for UART3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SIO_USE_UART3) || defined(__DOXYGEN__)
#define GD32_SIO_USE_UART3                  FALSE
#endif

/**
 * @brief   SIO driver on UART4 enable switch.
 * @details If set to @p TRUE the support for UART4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SIO_USE_UART4) || defined(__DOXYGEN__)
#define GD32_SIO_USE_UART4                  FALSE
#endif

/**
 * @brief   SIO driver on USART5 enable switch.
 * @details If set to @p TRUE the support for USART5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SIO_USE_USART5) || defined(__DOXYGEN__)
#define GD32_SIO_USE_USART5                 FALSE
#endif

/**
 * @brief   SIO driver on UART6 enable switch.
 * @details If set to @p TRUE the support for UART6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SIO_USE_UART6) || defined(__DOXYGEN__)
#define GD32_SIO_USE_UART6                  FALSE
#endif

/**
 * @brief   SIO driver on UART7 enable switch.
 * @details If set to @p TRUE the support for UART7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SIO_USE_UART7) || defined(__DOXYGEN__)
#define GD32_SIO_USE_UART7                  FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if GD32_SIO_USE_USART0 && !GD32_HAS_USART0
#error "USART0 not present in the selected device"
#endif

#if GD32_SIO_USE_USART1 && !GD32_HAS_USART1
#error "USART1 not present in the selected device"
#endif

#if GD32_SIO_USE_USART2 && !GD32_HAS_USART2
#error "USART2 not present in the selected device"
#endif

#if GD32_SIO_USE_UART3 && !GD32_HAS_UART3
#error "UART3 not present in the selected device"
#endif

#if GD32_SIO_USE_UART4 && !GD32_HAS_UART4
#error "UART4 not present in the selected device"
#endif

#if GD32_SIO_USE_USART5 && !GD32_HAS_USART5
#error "USART5 not present in the selected device"
#endif

#if GD32_SIO_USE_UART6 && !GD32_HAS_UART6
#error "UART6 not present in the selected device"
#endif

#if GD32_SIO_USE_UART7 && !GD32_HAS_UART7
#error "UART7 not present in the selected device"
#endif

#if !GD32_SIO_USE_USART0 && !GD32_SIO_USE_USART1 &&                         \
    !GD32_SIO_USE_USART2 && !GD32_SIO_USE_UART3  &&                         \
    !GD32_SIO_USE_UART4  && !GD32_SIO_USE_USART5 &&                         \
    !GD32_SIO_USE_UART6  && !GD32_SIO_USE_UART7
#error "SIO driver activated but no USART/UART peripheral assigned"
#endif

/* Checks on allocation of USARTx/UARTx units, shared with the SERIAL/UART
   LLDs that may claim the same hardware instance (see gd32_usartN.inc /
   gd32_uartN.inc in GD32/LLD/USART).*/
#if GD32_SIO_USE_USART0
#if defined(GD32_USART0_IS_USED)
#error "SIOD0 requires USART0 but it is already used"
#else
#define GD32_USART0_IS_USED
#endif
#endif

#if GD32_SIO_USE_USART1
#if defined(GD32_USART1_IS_USED)
#error "SIOD1 requires USART1 but it is already used"
#else
#define GD32_USART1_IS_USED
#endif
#endif

#if GD32_SIO_USE_USART2
#if defined(GD32_USART2_IS_USED)
#error "SIOD2 requires USART2 but it is already used"
#else
#define GD32_USART2_IS_USED
#endif
#endif

#if GD32_SIO_USE_UART3
#if defined(GD32_UART3_IS_USED)
#error "SIOD3 requires UART3 but it is already used"
#else
#define GD32_UART3_IS_USED
#endif
#endif

#if GD32_SIO_USE_UART4
#if defined(GD32_UART4_IS_USED)
#error "SIOD4 requires UART4 but it is already used"
#else
#define GD32_UART4_IS_USED
#endif
#endif

#if GD32_SIO_USE_USART5
#if defined(GD32_USART5_IS_USED)
#error "SIOD5 requires USART5 but it is already used"
#else
#define GD32_USART5_IS_USED
#endif
#endif

#if GD32_SIO_USE_UART6
#if defined(GD32_UART6_IS_USED)
#error "SIOD6 requires UART6 but it is already used"
#else
#define GD32_UART6_IS_USED
#endif
#endif

#if GD32_SIO_USE_UART7
#if defined(GD32_UART7_IS_USED)
#error "SIOD7 requires UART7 but it is already used"
#else
#define GD32_UART7_IS_USED
#endif
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Low level fields of the SIO driver structure.
 */
#define sio_lld_driver_fields                                               \
  /* Base address of the USART/UART registers block.*/                      \
  uint32_t                  usart;                                          \
  /* Clock frequency for the associated USART/UART.*/                       \
  uint32_t                  clock;

/**
 * @brief   Low level fields of the SIO configuration structure.
 * @note    The peripheral runs in non-FIFO mode, data events are taken from
 *          the RBNE/TBE flags.
 */
#define sio_lld_config_fields                                               \
  /* Desired baud rate.*/                                                   \
  uint32_t                  baud;                                           \
  /* USART CTL0 register initialization data.*/                             \
  uint32_t                  ctl0;                                           \
  /* USART CTL1 register initialization data.*/                             \
  uint32_t                  ctl1;                                           \
  /* USART CTL2 register initialization data.*/                             \
  uint32_t                  ctl2;

/**
 * @brief   Determines the state of the RX FIFO.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The RX FIFO state.
 * @retval false        if RX FIFO is not empty
 * @retval true         if RX FIFO is empty
 *
 * @notapi
 */
#define sio_lld_is_rx_empty(siop)                                           \
  (bool)((USART_STAT((siop)->usart) & USART_STAT_RBNE) == 0U)

/**
 * @brief   Determines the activity state of the receiver.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The RX activity state.
 * @retval false        if RX is in active state.
 * @retval true         if RX is in idle state.
 *
 * @notapi
 */
#define sio_lld_is_rx_idle(siop)                                            \
  (bool)((USART_STAT((siop)->usart) & USART_STAT_IDLEF) != 0U)

/**
 * @brief   Determines if RX has pending error events to be read and cleared.
 * @note    Only error and protocol errors are handled, data events are not
 *          considered.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The RX error events.
 * @retval false        if RX has no pending events
 * @retval true         if RX has pending events
 *
 * @notapi
 */
#define sio_lld_has_rx_errors(siop)                                         \
  (bool)((USART_STAT((siop)->usart) & SIO_LLD_STAT_RX_ERRORS) != 0U)

/**
 * @brief   Determines the state of the TX FIFO.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The TX FIFO state.
 * @retval false        if TX FIFO is not full
 * @retval true         if TX FIFO is full
 *
 * @notapi
 */
#define sio_lld_is_tx_full(siop)                                            \
  (bool)((USART_STAT((siop)->usart) & USART_STAT_TBE) == 0U)

/**
 * @brief   Determines the transmission state.
 *
 * @param[in] siop      pointer to the @p SIODriver object
 * @return              The TX FIFO state.
 * @retval false        if transmission is idle
 * @retval true         if transmission is ongoing
 *
 * @notapi
 */
#define sio_lld_is_tx_ongoing(siop)                                         \
  (bool)((USART_STAT((siop)->usart) & USART_STAT_TC) == 0U)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if (GD32_SIO_USE_USART0 == TRUE) && !defined(__DOXYGEN__)
extern SIODriver SIOD0;
#endif

#if (GD32_SIO_USE_USART1 == TRUE) && !defined(__DOXYGEN__)
extern SIODriver SIOD1;
#endif

#if (GD32_SIO_USE_USART2 == TRUE) && !defined(__DOXYGEN__)
extern SIODriver SIOD2;
#endif

#if (GD32_SIO_USE_UART3 == TRUE) && !defined(__DOXYGEN__)
extern SIODriver SIOD3;
#endif

#if (GD32_SIO_USE_UART4 == TRUE) && !defined(__DOXYGEN__)
extern SIODriver SIOD4;
#endif

#if (GD32_SIO_USE_USART5 == TRUE) && !defined(__DOXYGEN__)
extern SIODriver SIOD5;
#endif

#if (GD32_SIO_USE_UART6 == TRUE) && !defined(__DOXYGEN__)
extern SIODriver SIOD6;
#endif

#if (GD32_SIO_USE_UART7 == TRUE) && !defined(__DOXYGEN__)
extern SIODriver SIOD7;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sio_lld_init(void);
  msg_t sio_lld_start(SIODriver *siop);
  void sio_lld_stop(SIODriver *siop);
  void sio_lld_update_enable_flags(SIODriver *siop);
  sioevents_t sio_lld_get_and_clear_errors(SIODriver *siop);
  sioevents_t sio_lld_get_and_clear_events(SIODriver *siop);
  sioevents_t sio_lld_get_events(SIODriver *siop);
  size_t sio_lld_read(SIODriver *siop, uint8_t *buffer, size_t n);
  size_t sio_lld_write(SIODriver *siop, const uint8_t *buffer, size_t n);
  msg_t sio_lld_get(SIODriver *siop);
  void sio_lld_put(SIODriver *siop, uint_fast16_t data);
  msg_t sio_lld_control(SIODriver *siop, unsigned int operation, void *arg);
  void sio_lld_serve_interrupt(SIODriver *siop);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SIO == TRUE */

#endif /* HAL_SIO_LLD_H */

/** @} */
