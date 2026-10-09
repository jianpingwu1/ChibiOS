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
 * @file    USARTv1/hal_serial_lld.h
 * @brief   GD32H73x_75x low level serial driver header.
 *
 * @addtogroup SERIAL
 * @{
 */

#ifndef HAL_SERIAL_LLD_H
#define HAL_SERIAL_LLD_H

#if HAL_USE_SERIAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   Advanced buffering support switch.
 * @details This constants enables the advanced buffering support in the
 *          low level driver, the queue buffer is no more part of the
 *          @p SerialDriver structure, each driver can have a different
 *          queue size.
 */
#define SERIAL_ADVANCED_BUFFERING_SUPPORT   TRUE

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   USART0 driver enable switch.
 * @details If set to @p TRUE the support for USART0 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SERIAL_USE_USART0) || defined(__DOXYGEN__)
#define GD32_SERIAL_USE_USART0              FALSE
#endif

/**
 * @brief   USART1 driver enable switch.
 * @details If set to @p TRUE the support for USART1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SERIAL_USE_USART1) || defined(__DOXYGEN__)
#define GD32_SERIAL_USE_USART1              FALSE
#endif

/**
 * @brief   USART2 driver enable switch.
 * @details If set to @p TRUE the support for USART2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SERIAL_USE_USART2) || defined(__DOXYGEN__)
#define GD32_SERIAL_USE_USART2              FALSE
#endif

/**
 * @brief   UART3 driver enable switch.
 * @details If set to @p TRUE the support for UART3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SERIAL_USE_UART3) || defined(__DOXYGEN__)
#define GD32_SERIAL_USE_UART3               FALSE
#endif

/**
 * @brief   UART4 driver enable switch.
 * @details If set to @p TRUE the support for UART4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SERIAL_USE_UART4) || defined(__DOXYGEN__)
#define GD32_SERIAL_USE_UART4               FALSE
#endif

/**
 * @brief   USART5 driver enable switch.
 * @details If set to @p TRUE the support for USART5 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SERIAL_USE_USART5) || defined(__DOXYGEN__)
#define GD32_SERIAL_USE_USART5              FALSE
#endif

/**
 * @brief   UART6 driver enable switch.
 * @details If set to @p TRUE the support for UART6 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SERIAL_USE_UART6) || defined(__DOXYGEN__)
#define GD32_SERIAL_USE_UART6               FALSE
#endif

/**
 * @brief   UART7 driver enable switch.
 * @details If set to @p TRUE the support for UART7 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(GD32_SERIAL_USE_UART7) || defined(__DOXYGEN__)
#define GD32_SERIAL_USE_UART7               FALSE
#endif

/**
 * @brief   Input buffer size for USART0.
 */
#if !defined(GD32_SERIAL_USART0_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART0_IN_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for USART0.
 */
#if !defined(GD32_SERIAL_USART0_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART0_OUT_BUF_SIZE      SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for USART1.
 */
#if !defined(GD32_SERIAL_USART1_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART1_IN_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for USART1.
 */
#if !defined(GD32_SERIAL_USART1_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART1_OUT_BUF_SIZE      SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for USART2.
 */
#if !defined(GD32_SERIAL_USART2_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART2_IN_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for USART2.
 */
#if !defined(GD32_SERIAL_USART2_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART2_OUT_BUF_SIZE      SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for UART3.
 */
#if !defined(GD32_SERIAL_UART3_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_UART3_IN_BUF_SIZE        SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for UART3.
 */
#if !defined(GD32_SERIAL_UART3_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_UART3_OUT_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for UART4.
 */
#if !defined(GD32_SERIAL_UART4_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_UART4_IN_BUF_SIZE        SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for UART4.
 */
#if !defined(GD32_SERIAL_UART4_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_UART4_OUT_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for USART5.
 */
#if !defined(GD32_SERIAL_USART5_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART5_IN_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for USART5.
 */
#if !defined(GD32_SERIAL_USART5_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_USART5_OUT_BUF_SIZE      SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for UART6.
 */
#if !defined(GD32_SERIAL_UART6_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_UART6_IN_BUF_SIZE        SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for UART6.
 */
#if !defined(GD32_SERIAL_UART6_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_UART6_OUT_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Input buffer size for UART7.
 */
#if !defined(GD32_SERIAL_UART7_IN_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_UART7_IN_BUF_SIZE        SERIAL_BUFFERS_SIZE
#endif

/**
 * @brief   Output buffer size for UART7.
 */
#if !defined(GD32_SERIAL_UART7_OUT_BUF_SIZE) || defined(__DOXYGEN__)
#define GD32_SERIAL_UART7_OUT_BUF_SIZE       SERIAL_BUFFERS_SIZE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if GD32_SERIAL_USE_USART0 && !GD32_HAS_USART0
#error "USART0 not present in the selected device"
#endif

#if GD32_SERIAL_USE_USART1 && !GD32_HAS_USART1
#error "USART1 not present in the selected device"
#endif

#if GD32_SERIAL_USE_USART2 && !GD32_HAS_USART2
#error "USART2 not present in the selected device"
#endif

#if GD32_SERIAL_USE_UART3 && !GD32_HAS_UART3
#error "UART3 not present in the selected device"
#endif

#if GD32_SERIAL_USE_UART4 && !GD32_HAS_UART4
#error "UART4 not present in the selected device"
#endif

#if GD32_SERIAL_USE_USART5 && !GD32_HAS_USART5
#error "USART5 not present in the selected device"
#endif

#if GD32_SERIAL_USE_UART6 && !GD32_HAS_UART6
#error "UART6 not present in the selected device"
#endif

#if GD32_SERIAL_USE_UART7 && !GD32_HAS_UART7
#error "UART7 not present in the selected device"
#endif

#if !GD32_SERIAL_USE_USART0 && !GD32_SERIAL_USE_USART1 &&                   \
    !GD32_SERIAL_USE_USART2 && !GD32_SERIAL_USE_UART3  &&                   \
    !GD32_SERIAL_USE_UART4  && !GD32_SERIAL_USE_USART5 &&                   \
    !GD32_SERIAL_USE_UART6  && !GD32_SERIAL_USE_UART7
#error "SERIAL driver activated but no USART/UART peripheral assigned"
#endif

/* Checks on allocation of USARTx/UARTx units, shared with the SIO/UART
   LLDs that may claim the same hardware instance (see gd32_usartN.inc /
   gd32_uartN.inc in GD32/LLD/USART).*/
#if GD32_SERIAL_USE_USART0
#if defined(GD32_USART0_IS_USED)
#error "SD0 requires USART0 but it is already used"
#else
#define GD32_USART0_IS_USED
#endif
#endif

#if GD32_SERIAL_USE_USART1
#if defined(GD32_USART1_IS_USED)
#error "SD1 requires USART1 but it is already used"
#else
#define GD32_USART1_IS_USED
#endif
#endif

#if GD32_SERIAL_USE_USART2
#if defined(GD32_USART2_IS_USED)
#error "SD2 requires USART2 but it is already used"
#else
#define GD32_USART2_IS_USED
#endif
#endif

#if GD32_SERIAL_USE_UART3
#if defined(GD32_UART3_IS_USED)
#error "SD3 requires UART3 but it is already used"
#else
#define GD32_UART3_IS_USED
#endif
#endif

#if GD32_SERIAL_USE_UART4
#if defined(GD32_UART4_IS_USED)
#error "SD4 requires UART4 but it is already used"
#else
#define GD32_UART4_IS_USED
#endif
#endif

#if GD32_SERIAL_USE_USART5
#if defined(GD32_USART5_IS_USED)
#error "SD5 requires USART5 but it is already used"
#else
#define GD32_USART5_IS_USED
#endif
#endif

#if GD32_SERIAL_USE_UART6
#if defined(GD32_UART6_IS_USED)
#error "SD6 requires UART6 but it is already used"
#else
#define GD32_UART6_IS_USED
#endif
#endif

#if GD32_SERIAL_USE_UART7
#if defined(GD32_UART7_IS_USED)
#error "SD7 requires UART7 but it is already used"
#else
#define GD32_UART7_IS_USED
#endif
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef void (*uartirq_t)(void *sd);

#define _serial_irq_code(sd) {                                              \
  if ((sd)->config->irq_cb != NULL)                                         \
    (sd)->config->irq_cb((sd)->config->ctx);                                \
}

/**
 * @brief   GD32 Serial Driver configuration structure.
 * @details An instance of this structure must be passed to @p sdStart()
 *          in order to configure and start a serial driver operations.
 * @note    This structure content is architecture dependent, each driver
 *          implementation defines its own version and the custom static
 *          initializers.
 */
typedef struct hal_serial_config {
  /**
   * @brief Bit rate.
   */
  uint32_t                  speed;
  /* End of the mandatory fields.*/
  /**
   * @brief Initialization value for the CTL0 register.
   */
  uint32_t                  ctl0;
  /**
   * @brief Initialization value for the CTL1 register.
   */
  uint32_t                  ctl1;
  /**
   * @brief Initialization value for the CTL2 register.
   */
  uint32_t                  ctl2;

  /**
    * @brief Set callback from irq
    */
  uartirq_t                 irq_cb;
  /**
    * @pointer to ctx
    */
  void                      *ctx;
} SerialConfig;

/**
 * @brief   @p SerialDriver specific data.
 */
#if !defined(SERIAL_ADVANCED_BUFFERING_SUPPORT) ||                          \
    (SERIAL_ADVANCED_BUFFERING_SUPPORT == FALSE) ||                         \
    defined(__DOXYGEN__)
#define _serial_driver_data                                                 \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdstate_t                 state;                                          \
  /* Input queue.*/                                                         \
  input_queue_t             iqueue;                                         \
  /* Output queue.*/                                                        \
  output_queue_t            oqueue;                                         \
  /* Input circular buffer.*/                                               \
  uint8_t                   ib[SERIAL_BUFFERS_SIZE];                        \
  /* Output circular buffer.*/                                              \
  uint8_t                   ob[SERIAL_BUFFERS_SIZE];                        \
  /* End of the mandatory fields.*/                                         \
  /* Base address of the USART/UART registers block.*/                      \
  uint32_t                  usart;                                          \
  /* Clock frequency for the associated USART/UART.*/                       \
  uint32_t                  clock;                                          \
  /* Mask to be applied on received frames.*/                               \
  uint8_t                   rxmask;                                         \
  /* Serial Config.*/                                                       \
  const SerialConfig        *config;
#else
#define _serial_driver_data                                                 \
  _base_asynchronous_channel_data                                           \
  /* Driver state.*/                                                        \
  sdstate_t                 state;                                          \
  /* Input queue.*/                                                         \
  input_queue_t             iqueue;                                         \
  /* Output queue.*/                                                        \
  output_queue_t            oqueue;                                         \
  /* End of the mandatory fields.*/                                         \
  /* Base address of the USART/UART registers block.*/                      \
  uint32_t                  usart;                                          \
  /* Clock frequency for the associated USART/UART.*/                       \
  uint32_t                  clock;                                          \
  /* Mask to be applied on received frames.*/                               \
  uint8_t                   rxmask;                                         \
  /* Serial Config.*/                                                       \
  const SerialConfig        *config;
#endif

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if GD32_SERIAL_USE_USART0 && !defined(__DOXYGEN__)
extern SerialDriver SD0;
#endif
#if GD32_SERIAL_USE_USART1 && !defined(__DOXYGEN__)
extern SerialDriver SD1;
#endif
#if GD32_SERIAL_USE_USART2 && !defined(__DOXYGEN__)
extern SerialDriver SD2;
#endif
#if GD32_SERIAL_USE_UART3 && !defined(__DOXYGEN__)
extern SerialDriver SD3;
#endif
#if GD32_SERIAL_USE_UART4 && !defined(__DOXYGEN__)
extern SerialDriver SD4;
#endif
#if GD32_SERIAL_USE_USART5 && !defined(__DOXYGEN__)
extern SerialDriver SD5;
#endif
#if GD32_SERIAL_USE_UART6 && !defined(__DOXYGEN__)
extern SerialDriver SD6;
#endif
#if GD32_SERIAL_USE_UART7 && !defined(__DOXYGEN__)
extern SerialDriver SD7;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void sd_lld_init(void);
  void sd_lld_start(SerialDriver *sdp, const SerialConfig *config);
  void sd_lld_stop(SerialDriver *sdp);
  void sd_lld_serve_interrupt(SerialDriver *sdp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_SERIAL */

#endif /* HAL_SERIAL_LLD_H */

/** @} */
