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
 * @file    GPIOv1/hal_pal_lld.h
 * @brief   GD32 PAL low level driver header.
 *
 * @addtogroup PAL
 * @{
 */

#ifndef HAL_PAL_LLD_H
#define HAL_PAL_LLD_H

#include "gd32_gpio.h"

#if (HAL_USE_PAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

/* Specifies palInit() without parameter, required until all platforms will
   be updated to the new style.*/
#define PAL_NEW_INIT

#undef PAL_MODE_RESET
#undef PAL_MODE_UNCONNECTED
#undef PAL_MODE_INPUT
#undef PAL_MODE_INPUT_PULLUP
#undef PAL_MODE_INPUT_PULLDOWN
#undef PAL_MODE_INPUT_ANALOG
#undef PAL_MODE_OUTPUT_PUSHPULL
#undef PAL_MODE_OUTPUT_OPENDRAIN

/**
 * @name    GD32-specific I/O mode flags
 * @{
 */
#define PAL_GD32_MODE_MASK               (3U << 0U)
#define PAL_GD32_MODE_INPUT              (0U << 0U)
#define PAL_GD32_MODE_OUTPUT             (1U << 0U)
#define PAL_GD32_MODE_ALTERNATE          (2U << 0U)
#define PAL_GD32_MODE_ANALOG             (3U << 0U)

#define PAL_GD32_OTYPE_MASK              (1U << 2U)
#define PAL_GD32_OTYPE_PUSHPULL          (0U << 2U)
#define PAL_GD32_OTYPE_OPENDRAIN         (1U << 2U)

#define PAL_GD32_OSPEED_MASK             (3U << 3U)
#define PAL_GD32_OSPEED_LOWEST           (0U << 3U)
#define PAL_GD32_OSPEED_MID1             (1U << 3U)
#define PAL_GD32_OSPEED_MID2             (2U << 3U)
#define PAL_GD32_OSPEED_HIGHEST          (3U << 3U)

#define PAL_GD32_PUPDR_MASK              (3U << 5U)
#define PAL_GD32_PUPDR_FLOATING          (0U << 5U)
#define PAL_GD32_PUPDR_PULLUP            (1U << 5U)
#define PAL_GD32_PUPDR_PULLDOWN          (2U << 5U)

#define PAL_GD32_ALTERNATE_MASK          (15U << 7U)
#define PAL_GD32_ALTERNATE(n)            ((n) << 7U)

/**
 * @brief   Alternate function.
 *
 * @param[in] n         alternate function selector
 */
#define PAL_MODE_ALTERNATE(n)            (PAL_GD32_MODE_ALTERNATE |         \
                                          PAL_GD32_ALTERNATE(n))
/** @} */

/**
 * @name    Standard I/O mode flags
 * @{
 */
/**
 * @brief   Implemented as input.
 */
#define PAL_MODE_RESET                   PAL_GD32_MODE_INPUT

/**
 * @brief   Implemented as input with pull-up.
 */
#define PAL_MODE_UNCONNECTED             PAL_MODE_INPUT_PULLUP

/**
 * @brief   Regular input high-Z pad.
 */
#define PAL_MODE_INPUT                   PAL_GD32_MODE_INPUT

/**
 * @brief   Input pad with weak pull up resistor.
 */
#define PAL_MODE_INPUT_PULLUP            (PAL_GD32_MODE_INPUT |             \
                                          PAL_GD32_PUPDR_PULLUP)

/**
 * @brief   Input pad with weak pull down resistor.
 */
#define PAL_MODE_INPUT_PULLDOWN          (PAL_GD32_MODE_INPUT |             \
                                          PAL_GD32_PUPDR_PULLDOWN)

/**
 * @brief   Analog input mode.
 */
#define PAL_MODE_INPUT_ANALOG            PAL_GD32_MODE_ANALOG

/**
 * @brief   Push-pull output pad.
 */
#define PAL_MODE_OUTPUT_PUSHPULL         (PAL_GD32_MODE_OUTPUT |            \
                                          PAL_GD32_OTYPE_PUSHPULL)

/**
 * @brief   Open-drain output pad.
 */
#define PAL_MODE_OUTPUT_OPENDRAIN        (PAL_GD32_MODE_OUTPUT |            \
                                          PAL_GD32_OTYPE_OPENDRAIN)
/** @} */

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @name    Port related definitions
 * @{
 */
/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH           16U

/**
 * @brief   Whole port mask.
 * @details This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT              ((ioportmask_t)0xFFFFU)
/** @} */

/**
 * @name    Line handling macros
 * @{
 */
/**
 * @brief   Forms a line identifier.
 * @details A port/pad pair are encoded into an @p ioline_t type. The encoding
 *          of this type is platform-dependent.
 */
#define PAL_LINE(port, pad)                                                 \
  ((ioline_t)((uint32_t)(port)) | ((uint32_t)(pad)))

/**
 * @brief   Decodes a port identifier from a line identifier.
 */
#define PAL_PORT(line)                                                      \
  ((ioportid_t)(((uint32_t)(line)) & 0xFFFFFFF0U))

/**
 * @brief   Decodes a pad identifier from a line identifier.
 */
#define PAL_PAD(line)                                                       \
  ((uint32_t)((uint32_t)(line) & 0x0000000FU))

/**
 * @brief   Value identifying an invalid line.
 */
#define PAL_NOLINE                      0U
/** @} */

/**
 * @brief   Type of digital I/O port sized unsigned integer.
 */
typedef uint32_t ioportmask_t;

/**
 * @brief   Type of digital I/O modes.
 */
typedef uint32_t iomode_t;

/**
 * @brief   Type of an I/O line.
 */
typedef uint32_t ioline_t;

/**
 * @brief   Type of an event mode.
 */
typedef uint32_t ioeventmode_t;

/**
 * @brief   Type of a port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type. On GD32 it is a plain address, matching
 *          the GD32 vendor firmware library's GPIOA..GPIOK definitions.
 */
typedef uint32_t ioportid_t;

/**
 * @brief   Type of an pad identifier.
 */
typedef uint32_t iopadid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/* The low level driver wraps the definitions already present in the GD32    */
/* firmware library.                                                         */
/*===========================================================================*/

/**
 * @brief   GPIO port A identifier.
 */
#if GD32_HAS_GPIOA || defined(__DOXYGEN__)
#define IOPORT1         GPIOA
#endif

/**
 * @brief   GPIO port B identifier.
 */
#if GD32_HAS_GPIOB || defined(__DOXYGEN__)
#define IOPORT2         GPIOB
#endif

/**
 * @brief   GPIO port C identifier.
 */
#if GD32_HAS_GPIOC || defined(__DOXYGEN__)
#define IOPORT3         GPIOC
#endif

/**
 * @brief   GPIO port D identifier.
 */
#if GD32_HAS_GPIOD || defined(__DOXYGEN__)
#define IOPORT4         GPIOD
#endif

/**
 * @brief   GPIO port E identifier.
 */
#if GD32_HAS_GPIOE || defined(__DOXYGEN__)
#define IOPORT5         GPIOE
#endif

/**
 * @brief   GPIO port F identifier.
 */
#if GD32_HAS_GPIOF || defined(__DOXYGEN__)
#define IOPORT6         GPIOF
#endif

/**
 * @brief   GPIO port G identifier.
 */
#if GD32_HAS_GPIOG || defined(__DOXYGEN__)
#define IOPORT7         GPIOG
#endif

/**
 * @brief   GPIO port H identifier.
 */
#if GD32_HAS_GPIOH || defined(__DOXYGEN__)
#define IOPORT8         GPIOH
#endif

/**
 * @brief   GPIO port J identifier.
 */
#if GD32_HAS_GPIOJ || defined(__DOXYGEN__)
#define IOPORT9         GPIOJ
#endif

/**
 * @brief   GPIO port K identifier.
 */
#if GD32_HAS_GPIOK || defined(__DOXYGEN__)
#define IOPORT10        GPIOK
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   Low level PAL subsystem initialization.
 *
 * @notapi
 */
#define pal_lld_init() _pal_lld_init()

/**
 * @brief   Reads an I/O port.
 * @details This function is implemented by reading the GPIO ISTAT
 *          register, the implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 *
 * @notapi
 */
#define pal_lld_readport(port) ((ioportmask_t)GPIO_ISTAT(port))

/**
 * @brief   Reads the output latch.
 * @details The purpose of this function is to read back the latched output
 *          value, implemented by reading the GPIO OCTL register.
 *
 * @param[in] port      port identifier
 * @return              The latched logical states.
 *
 * @notapi
 */
#define pal_lld_readlatch(port) ((ioportmask_t)GPIO_OCTL(port))

/**
 * @brief   Writes a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO OCTL register.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @notapi
 */
#define pal_lld_writeport(port, bits) (GPIO_OCTL(port) = (uint32_t)(bits))

/**
 * @brief   Sets a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BOP register,
 *          the lower 16 bits of which set pins (upper 16 bits, left at 0
 *          here, clear pins - same encoding as STM32's BSRR).
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be ORed on the specified port
 *
 * @notapi
 */
#define pal_lld_setport(port, bits) (GPIO_BOP(port) = (uint32_t)(bits))

/**
 * @brief   Clears a bits mask on a I/O port.
 * @details This function is implemented by writing the GD32-specific
 *          dedicated GPIO_BC (bit-clear) register.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be cleared on the specified port
 *
 * @notapi
 */
#define pal_lld_clearport(port, bits) (GPIO_BC(port) = (uint32_t)(bits))

/**
 * @brief   Toggles a bits mask on a I/O port.
 * @details This function is implemented by writing the GD32-specific
 *          dedicated GPIO_TG (bit-toggle) register.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be XORed on the specified port
 *
 * @notapi
 */
#define pal_lld_toggleport(port, bits) (GPIO_TG(port) = (uint32_t)(bits))

/**
 * @brief   Reads a group of bits.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @return              The group logical states.
 *
 * @notapi
 */
#define pal_lld_readgroup(port, mask, offset)                               \
  ((pal_lld_readport(port) >> (offset)) & (mask))

/**
 * @brief   Writes a group of bits.
 * @details This function is implemented by writing the GPIO BOP register.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] bits      bits to be written. Values exceeding the group width
 *                      are masked.
 *
 * @notapi
 */
#define pal_lld_writegroup(port, mask, offset, bits) {                       \
  uint32_t w = ((~(uint32_t)(bits) & (uint32_t)(mask)) << (16U + (offset))) | \
               ((uint32_t)(bits) & (uint32_t)(mask)) << (offset);            \
  GPIO_BOP(port) = w;                                                        \
}

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] mode      group mode
 *
 * @notapi
 */
#define pal_lld_setgroupmode(port, mask, offset, mode)                      \
  _pal_lld_setgroupmode(port, mask << offset, mode)

/**
 * @brief   Reads a logical state from an I/O pad.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @return              The logical state.
 * @retval PAL_LOW      low logical state.
 * @retval PAL_HIGH     high logical state.
 *
 * @notapi
 */
#define pal_lld_readpad(port, pad)                                          \
  (((GPIO_ISTAT(port) >> (pad)) & 1U) != 0U ? PAL_HIGH : PAL_LOW)

/**
 * @brief   Writes a logical state on an output pad.
 * @note    This function is not meant to be invoked directly by the
 *          application  code.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] bit       logical value, the value must be @p PAL_LOW or
 *                      @p PAL_HIGH
 *
 * @notapi
 */
#define pal_lld_writepad(port, pad, bit) pal_lld_writegroup(port, 1, pad, bit)

/**
 * @brief   Sets a pad logical state to @p PAL_HIGH.
 * @details This function is implemented by writing the GPIO BOP register.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_setpad(port, pad) (GPIO_BOP(port) = (uint32_t)(1U << (pad)))

/**
 * @brief   Clears a pad logical state to @p PAL_LOW.
 * @details This function is implemented by writing the GD32-specific
 *          dedicated GPIO_BC (bit-clear) register.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_clearpad(port, pad) (GPIO_BC(port) = (uint32_t)(1U << (pad)))

/**
 * @brief   Toggles a pad logical state.
 * @details This function is implemented by writing the GD32-specific
 *          dedicated GPIO_TG (bit-toggle) register.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_togglepad(port, pad) (GPIO_TG(port) = (uint32_t)(1U << (pad)))

/**
 * @brief   Pad mode setup.
 * @details This function programs a pad with the specified mode.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] mode      pad mode
 *
 * @notapi
 */
#define pal_lld_setpadmode(port, pad, mode)                                 \
  _pal_lld_setgroupmode(port, 1U << (pad), mode)

/**
 * @brief   Pad event enable.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] mode      pad event mode
 *
 * @notapi
 */
#define pal_lld_enablepadevent(port, pad, mode)                             \
  _pal_lld_enablepadevent(port, pad, mode)

/**
 * @brief   Pad event disable.
 * @details This function disables previously programmed event callbacks.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_disablepadevent(port, pad)                                  \
  _pal_lld_disablepadevent(port, pad)

/**
 * @brief   Returns a PAL event structure associated to a pad.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
#define pal_lld_get_pad_event(port, pad)                                    \
  &_pal_events[pad]; (void)(port)

/**
 * @brief   Returns a PAL event structure associated to a line.
 *
 * @param[in] line      line identifier
 *
 * @notapi
 */
#define pal_lld_get_line_event(line)                                        \
  &_pal_events[PAL_PAD(line)]

/**
 * @brief   Pad event enable check.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @return              Pad event status.
 * @retval false        if the pad event is disabled.
 * @retval true         if the pad event is enabled.
 *
 * @notapi
 */
#define pal_lld_ispadeventenabled(port, pad)                                \
  (bool)((EXTI_INTEN0 & (1U << (uint32_t)pad)) != 0U)

/**
 * @brief   Selects the GPIO port as EXTI line source.
 * @details Programs the SYSCFG_EXTISSx register, 4 lines per register,
 *          4 bits per line. The port index follows the GPIO A=0..K=10
 *          encoding. This is the equivalent of the SYSCFG EXTI
 *          configuration, without involving the vendor library.
 *
 * @param[in] port      port identifier (GPIOA..GPIOK)
 * @param[in] pad       pad number within the port (0..15)
 *
 * @notapi
 */
#define pal_lld_set_exti_source(port, pad) {                                \
  uint32_t portidx, ssidx, ssoff, ssmask;                                  \
  portidx = (((uint32_t)(port) - (uint32_t)GPIOA) >> 10U) & 0xFU;          \
  ssidx   = (uint32_t)(pad) >> 2U;                                         \
  ssoff   = ((uint32_t)(pad) & 3U) * 4U;                                   \
  ssmask  = ~(0xFU << ssoff);                                             \
  switch (ssidx) {                                                         \
  case 0:                                                                  \
    SYSCFG_EXTISS0 = (SYSCFG_EXTISS0 & ssmask) | (portidx << ssoff);       \
    break;                                                                 \
  case 1:                                                                  \
    SYSCFG_EXTISS1 = (SYSCFG_EXTISS1 & ssmask) | (portidx << ssoff);       \
    break;                                                                 \
  case 2:                                                                  \
    SYSCFG_EXTISS2 = (SYSCFG_EXTISS2 & ssmask) | (portidx << ssoff);       \
    break;                                                                 \
  default:                                                                 \
    SYSCFG_EXTISS3 = (SYSCFG_EXTISS3 & ssmask) | (portidx << ssoff);       \
    break;                                                                 \
  }                                                                        \
}

#if !defined(__DOXYGEN__)
#if (PAL_USE_WAIT == TRUE) || (PAL_USE_CALLBACKS == TRUE)
extern palevent_t _pal_events[16];
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void _pal_lld_init(void);
  void _pal_lld_setgroupmode(ioportid_t port,
                             ioportmask_t mask,
                             iomode_t mode);
  void _pal_lld_enablepadevent(ioportid_t port,
                               iopadid_t pad,
                               ioeventmode_t mode);
  void _pal_lld_disablepadevent(ioportid_t port, iopadid_t pad);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PAL == TRUE */

#endif /* HAL_PAL_LLD_H */

/** @} */
