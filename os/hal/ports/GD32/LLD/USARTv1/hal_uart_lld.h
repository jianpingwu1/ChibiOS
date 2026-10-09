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
 * @file    USARTv1/hal_uart_lld.h
 * @brief   GD32 low level UART driver header.
 *
 * @addtogroup UART
 * @{
 */

#ifndef HAL_UART_LLD_H
#define HAL_UART_LLD_H

#if (HAL_USE_UART == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   UART driver on USART0 enable switch.
 * @details If set to @p TRUE the support for USART0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_UART_USE_USART0) || defined(__DOXYGEN__)
#define GD32_UART_USE_USART0                FALSE
#endif

/**
 * @brief   UART driver on USART1 enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_UART_USE_USART1) || defined(__DOXYGEN__)
#define GD32_UART_USE_USART1                FALSE
#endif

/**
 * @brief   UART driver on USART2 enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_UART_USE_USART2) || defined(__DOXYGEN__)
#define GD32_UART_USE_USART2                FALSE
#endif

/**
 * @brief   UART driver on UART3 enable switch.
 * @details If set to @p TRUE the support for UART3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_UART_USE_UART3) || defined(__DOXYGEN__)
#define GD32_UART_USE_UART3                 FALSE
#endif

/**
 * @brief   UART driver on UART4 enable switch.
 * @details If set to @p TRUE the support for UART4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_UART_USE_UART4) || defined(__DOXYGEN__)
#define GD32_UART_USE_UART4                 FALSE
#endif

/**
 * @brief   UART driver on USART5 enable switch.
 * @details If set to @p TRUE the support for USART5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_UART_USE_USART5) || defined(__DOXYGEN__)
#define GD32_UART_USE_USART5                FALSE
#endif

/**
 * @brief   UART driver on UART6 enable switch.
 * @details If set to @p TRUE the support for UART6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_UART_USE_UART6) || defined(__DOXYGEN__)
#define GD32_UART_USE_UART6                 FALSE
#endif

/**
 * @brief   UART driver on UART7 enable switch.
 * @details If set to @p TRUE the support for UART7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_UART_USE_UART7) || defined(__DOXYGEN__)
#define GD32_UART_USE_UART7                 FALSE
#endif

/**
 * @brief   UART driver on USART0 interrupt priority level setting.
 */
#if !defined(GD32_UART_USART0_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_USART0_IRQ_PRIORITY       12
#endif

/**
 * @brief   UART driver on USART1 interrupt priority level setting.
 */
#if !defined(GD32_UART_USART1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_USART1_IRQ_PRIORITY       12
#endif

/**
 * @brief   UART driver on USART2 interrupt priority level setting.
 */
#if !defined(GD32_UART_USART2_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_USART2_IRQ_PRIORITY       12
#endif

/**
 * @brief   UART driver on UART3 interrupt priority level setting.
 */
#if !defined(GD32_UART_UART3_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_UART3_IRQ_PRIORITY        12
#endif

/**
 * @brief   UART driver on UART4 interrupt priority level setting.
 */
#if !defined(GD32_UART_UART4_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_UART4_IRQ_PRIORITY        12
#endif

/**
 * @brief   UART driver on USART5 interrupt priority level setting.
 */
#if !defined(GD32_UART_USART5_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_USART5_IRQ_PRIORITY       12
#endif

/**
 * @brief   UART driver on UART6 interrupt priority level setting.
 */
#if !defined(GD32_UART_UART6_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_UART6_IRQ_PRIORITY        12
#endif

/**
 * @brief   UART driver on UART7 interrupt priority level setting.
 */
#if !defined(GD32_UART_UART7_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_UART7_IRQ_PRIORITY        12
#endif

/**
 * @brief   USART0 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(GD32_UART_USART0_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_USART0_DMA_PRIORITY       0
#endif

/**
 * @brief   USART1 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(GD32_UART_USART1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_USART1_DMA_PRIORITY       0
#endif

/**
 * @brief   USART2 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(GD32_UART_USART2_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_USART2_DMA_PRIORITY       0
#endif

/**
 * @brief   UART3 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(GD32_UART_UART3_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_UART3_DMA_PRIORITY        0
#endif

/**
 * @brief   UART4 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(GD32_UART_UART4_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_UART4_DMA_PRIORITY        0
#endif

/**
 * @brief   USART5 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(GD32_UART_USART5_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_USART5_DMA_PRIORITY       0
#endif

/**
 * @brief   UART6 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(GD32_UART_UART6_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_UART6_DMA_PRIORITY        0
#endif

/**
 * @brief   UART7 DMA priority (0..3|lowest..highest).
 * @note    The priority level is used for both the TX and RX DMA channels but
 *          because of the channels ordering the RX channel has always priority
 *          over the TX channel.
 */
#if !defined(GD32_UART_UART7_DMA_PRIORITY) || defined(__DOXYGEN__)
#define GD32_UART_UART7_DMA_PRIORITY        0
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if GD32_UART_USE_USART0 && !GD32_HAS_USART0
#error "USART0 not present in the selected device"
#endif

#if GD32_UART_USE_USART1 && !GD32_HAS_USART1
#error "USART1 not present in the selected device"
#endif

#if GD32_UART_USE_USART2 && !GD32_HAS_USART2
#error "USART2 not present in the selected device"
#endif

#if GD32_UART_USE_UART3 && !GD32_HAS_UART3
#error "UART3 not present in the selected device"
#endif

#if GD32_UART_USE_UART4 && !GD32_HAS_UART4
#error "UART4 not present in the selected device"
#endif

#if GD32_UART_USE_USART5 && !GD32_HAS_USART5
#error "USART5 not present in the selected device"
#endif

#if GD32_UART_USE_UART6 && !GD32_HAS_UART6
#error "UART6 not present in the selected device"
#endif

#if GD32_UART_USE_UART7 && !GD32_HAS_UART7
#error "UART7 not present in the selected device"
#endif

#if !GD32_UART_USE_USART0 && !GD32_UART_USE_USART1 &&                     \
    !GD32_UART_USE_USART2 && !GD32_UART_USE_UART3  &&                     \
    !GD32_UART_USE_UART4  && !GD32_UART_USE_USART5 &&                     \
    !GD32_UART_USE_UART6  && !GD32_UART_USE_UART7
#error "UART driver activated but no USART/UART peripheral assigned"
#endif

#if !defined(GD32_USART0_SUPPRESS_ISR) &&                                 \
    GD32_UART_USE_USART0 &&                                               \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_UART_USART0_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to USART0"
#endif

#if !defined(GD32_USART1_SUPPRESS_ISR) &&                                 \
    GD32_UART_USE_USART1 &&                                               \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_UART_USART1_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to USART1"
#endif

#if !defined(GD32_USART2_SUPPRESS_ISR) &&                                 \
    GD32_UART_USE_USART2 &&                                               \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_UART_USART2_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to USART2"
#endif

#if !defined(GD32_UART3_SUPPRESS_ISR) &&                                  \
    GD32_UART_USE_UART3 &&                                                \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_UART_UART3_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to UART3"
#endif

#if !defined(GD32_UART4_SUPPRESS_ISR) &&                                  \
    GD32_UART_USE_UART4 &&                                                \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_UART_UART4_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to UART4"
#endif

#if !defined(GD32_USART5_SUPPRESS_ISR) &&                                 \
    GD32_UART_USE_USART5 &&                                               \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_UART_USART5_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to USART5"
#endif

#if !defined(GD32_UART6_SUPPRESS_ISR) &&                                  \
    GD32_UART_USE_UART6 &&                                                \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_UART_UART6_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to UART6"
#endif

#if !defined(GD32_UART7_SUPPRESS_ISR) &&                                  \
    GD32_UART_USE_UART7 &&                                                \
    !OSAL_IRQ_IS_VALID_PRIORITY(GD32_UART_UART7_IRQ_PRIORITY)
#error "Invalid IRQ priority assigned to UART7"
#endif

/* Check on DMA priorities.*/
#if GD32_UART_USE_USART0 &&                                               \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_UART_USART0_DMA_PRIORITY)
#error "Invalid DMA priority assigned to USART0"
#endif

#if GD32_UART_USE_USART1 &&                                               \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_UART_USART1_DMA_PRIORITY)
#error "Invalid DMA priority assigned to USART1"
#endif

#if GD32_UART_USE_USART2 &&                                               \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_UART_USART2_DMA_PRIORITY)
#error "Invalid DMA priority assigned to USART2"
#endif

#if GD32_UART_USE_UART3 &&                                                \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_UART_UART3_DMA_PRIORITY)
#error "Invalid DMA priority assigned to UART3"
#endif

#if GD32_UART_USE_UART4 &&                                                \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_UART_UART4_DMA_PRIORITY)
#error "Invalid DMA priority assigned to UART4"
#endif

#if GD32_UART_USE_USART5 &&                                               \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_UART_USART5_DMA_PRIORITY)
#error "Invalid DMA priority assigned to USART5"
#endif

#if GD32_UART_USE_UART6 &&                                                \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_UART_UART6_DMA_PRIORITY)
#error "Invalid DMA priority assigned to UART6"
#endif

#if GD32_UART_USE_UART7 &&                                                \
    !GD32_DMA_IS_VALID_PRIORITY(GD32_UART_UART7_DMA_PRIORITY)
#error "Invalid DMA priority assigned to UART7"
#endif

/* Checks on allocation of USARTx/UARTx units, shared with the SERIAL/SIO
   LLDs that may claim the same hardware instance (see gd32_usartN.inc /
   gd32_uartN.inc in GD32/LLD/USART).*/
#if GD32_UART_USE_USART0
#if defined(GD32_USART0_IS_USED)
#error "UARTD0 requires USART0 but it is already used"
#else
#define GD32_USART0_IS_USED
#endif
#endif

#if GD32_UART_USE_USART1
#if defined(GD32_USART1_IS_USED)
#error "UARTD1 requires USART1 but it is already used"
#else
#define GD32_USART1_IS_USED
#endif
#endif

#if GD32_UART_USE_USART2
#if defined(GD32_USART2_IS_USED)
#error "UARTD2 requires USART2 but it is already used"
#else
#define GD32_USART2_IS_USED
#endif
#endif

#if GD32_UART_USE_UART3
#if defined(GD32_UART3_IS_USED)
#error "UARTD3 requires UART3 but it is already used"
#else
#define GD32_UART3_IS_USED
#endif
#endif

#if GD32_UART_USE_UART4
#if defined(GD32_UART4_IS_USED)
#error "UARTD4 requires UART4 but it is already used"
#else
#define GD32_UART4_IS_USED
#endif
#endif

#if GD32_UART_USE_USART5
#if defined(GD32_USART5_IS_USED)
#error "UARTD5 requires USART5 but it is already used"
#else
#define GD32_USART5_IS_USED
#endif
#endif

#if GD32_UART_USE_UART6
#if defined(GD32_UART6_IS_USED)
#error "UARTD6 requires UART6 but it is already used"
#else
#define GD32_UART6_IS_USED
#endif
#endif

#if GD32_UART_USE_UART7
#if defined(GD32_UART7_IS_USED)
#error "UARTD7 requires UART7 but it is already used"
#else
#define GD32_UART7_IS_USED
#endif
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef struct hal_uart_driver UARTDriver;

/**
 * @brief   Type of an UART error mask.
 */
typedef uint32_t uartflags_t;

/**
 * @brief   Generic UART notification callback type.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 */
typedef void (*uartcb_t)(UARTDriver *uartp);

/**
 * @brief   Character received UART notification callback type.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] c         received character
 */
typedef void (*uartccb_t)(UARTDriver *uartp, uint16_t c);

/**
 * @brief   Receive error UART notification callback type.
 *
 * @param[in] uartp     pointer to the @p UARTDriver object
 * @param[in] e         receive error mask
 */
typedef void (*uartecb_t)(UARTDriver *uartp, uartflags_t e);

/**
 * @brief   Type of an UART configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct hal_uart_config {
  /**
   * @brief   End of transmission buffer callback.
   */
  uartcb_t                  txend1_cb;
  /**
   * @brief   Physical end of transmission callback.
   */
  uartcb_t                  txend2_cb;
  /**
   * @brief   Receive buffer filled callback.
   */
  uartcb_t                  rxend_cb;
  /**
   * @brief   Character received while out if the @p UART_RECEIVE state.
   */
  uartccb_t                 rxchar_cb;
  /**
   * @brief   Receive error callback.
   */
  uartecb_t                 rxerr_cb;
  /* End of the mandatory fields.*/
  /**
   * @brief   Receiver timeout callback.
   * @details Handles both idle and timeout interrupts depending on
   *          configured flags in CTL registers.
   */
  uartcb_t                  timeout_cb;
  /**
   * @brief   Receiver timeout value in terms of number of bit duration.
   * @details Set it to 0 when you want to handle idle interrupt instead
   *          of hardware timeout.
   */
  uint32_t                  timeout;
  /**
   * @brief   Bit rate.
   */
  uint32_t                  speed;
  /**
   * @brief   Initialization value for the CTL0 register.
   */
  uint32_t                  ctl0;
  /**
   * @brief   Initialization value for the CTL1 register.
   */
  uint32_t                  ctl1;
  /**
   * @brief   Initialization value for the CTL2 register.
   */
  uint32_t                  ctl2;
} UARTConfig;

/**
 * @brief   Structure representing an UART driver.
 */
struct hal_uart_driver {
  /**
   * @brief   Driver state.
   */
  uartstate_t               state;
  /**
   * @brief   Transmitter state.
   */
  uarttxstate_t             txstate;
  /**
   * @brief   Receiver state.
   */
  uartrxstate_t             rxstate;
  /**
   * @brief   Current configuration data.
   */
  const UARTConfig          *config;
#if (UART_USE_WAIT == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Synchronization flag for transmit operations.
   */
  bool                      early;
  /**
   * @brief   Waiting thread on RX.
   */
  thread_reference_t        threadrx;
  /**
   * @brief   Waiting thread on TX.
   */
  thread_reference_t        threadtx;
#endif /* UART_USE_WAIT */
#if (UART_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the peripheral.
   */
  mutex_t                   mutex;
#endif /* UART_USE_MUTUAL_EXCLUSION */
#if defined(UART_DRIVER_EXT_FIELDS)
  UART_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @brief   Base address of the USART/UART registers block.
   */
  uint32_t                  usart;
  /**
   * @brief   Clock frequency for the associated USART/UART.
   */
  uint32_t                  clock;
  /**
   * @brief   Receive DMA mode bit mask.
   */
  uint32_t                  dmarxmode;
  /**
   * @brief   Send DMA mode bit mask.
   */
  uint32_t                  dmatxmode;
  /**
   * @brief   Receive DMA channel.
   */
  const gd32_dma_stream_t   *dmarx;
  /**
   * @brief   Transmit DMA channel.
   */
  const gd32_dma_stream_t   *dmatx;
  /**
   * @brief   Default receive buffer while into @p UART_RX_IDLE state.
   */
  volatile uint16_t         rxbuf;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if GD32_UART_USE_USART0 && !defined(__DOXYGEN__)
extern UARTDriver UARTD0;
#endif
#if GD32_UART_USE_USART1 && !defined(__DOXYGEN__)
extern UARTDriver UARTD1;
#endif
#if GD32_UART_USE_USART2 && !defined(__DOXYGEN__)
extern UARTDriver UARTD2;
#endif
#if GD32_UART_USE_UART3 && !defined(__DOXYGEN__)
extern UARTDriver UARTD3;
#endif
#if GD32_UART_USE_UART4 && !defined(__DOXYGEN__)
extern UARTDriver UARTD4;
#endif
#if GD32_UART_USE_USART5 && !defined(__DOXYGEN__)
extern UARTDriver UARTD5;
#endif
#if GD32_UART_USE_UART6 && !defined(__DOXYGEN__)
extern UARTDriver UARTD6;
#endif
#if GD32_UART_USE_UART7 && !defined(__DOXYGEN__)
extern UARTDriver UARTD7;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void uart_lld_init(void);
  void uart_lld_start(UARTDriver *uartp);
  void uart_lld_stop(UARTDriver *uartp);
  void uart_lld_start_send(UARTDriver *uartp, size_t n, const void *txbuf);
  size_t uart_lld_stop_send(UARTDriver *uartp);
  void uart_lld_start_receive(UARTDriver *uartp, size_t n, void *rxbuf);
  size_t uart_lld_stop_receive(UARTDriver *uartp);
  void uart_lld_serve_interrupt(UARTDriver *uartp);
#ifdef __cplusplus
}
#endif

#if !defined(GD32_DMA_REQUIRED)
#define GD32_DMA_REQUIRED
#endif

#endif /* HAL_USE_UART == TRUE */

#endif /* HAL_UART_LLD_H */
