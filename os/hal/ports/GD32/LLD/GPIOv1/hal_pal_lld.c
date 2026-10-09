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
 * @file    GPIOv1/hal_pal_lld.c
 * @brief   GD32 PAL low level driver source.
 *
 * @addtogroup PAL
 * @{
 */

#include "hal.h"

#if (HAL_USE_PAL == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

#if (PAL_USE_WAIT == TRUE) || (PAL_USE_CALLBACKS == TRUE) || defined(__DOXYGEN__)
/**
 * @brief   Event records for the 16 GPIO EXTI channels.
 */
palevent_t _pal_events[16];
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   GD32 I/O ports configuration.
 *
 * @notapi
 */
void _pal_lld_init(void) {

#if PAL_USE_CALLBACKS || PAL_USE_WAIT || defined(__DOXYGEN__)
  unsigned i;

  for (i = 0; i < 16; i++) {
    _pal_init_event(i);
  }
#endif
}

/**
 * @brief   Pads mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 * @note    @p PAL_MODE_UNCONNECTED is implemented as push pull at minimum
 *          speed.
 *
 * @param[in] port      the port identifier
 * @param[in] mask      the group mask
 * @param[in] mode      the mode
 *
 * @notapi
 */
void _pal_lld_setgroupmode(ioportid_t port,
                           ioportmask_t mask,
                           iomode_t mode) {

  uint32_t ctl    = (mode & PAL_GD32_MODE_MASK) >> 0;
  uint32_t omode  = (mode & PAL_GD32_OTYPE_MASK) >> 2;
  uint32_t ospd   = (mode & PAL_GD32_OSPEED_MASK) >> 3;
  uint32_t pud    = (mode & PAL_GD32_PUPDR_MASK) >> 5;
  uint32_t altr   = (mode & PAL_GD32_ALTERNATE_MASK) >> 7;
  uint32_t bit    = 0;
  while (true) {
    if ((mask & 1) != 0) {
      uint32_t altrmask, m1, m2, m4;

      altrmask = altr << ((bit & 7) * 4);
      m1 = 1 << bit;
      m2 = 3 << (bit * 2);
      m4 = 15 << ((bit & 7) * 4);
      GPIO_OMODE(port) = (GPIO_OMODE(port) & ~m1) | omode;
      GPIO_OSPD(port)  = (GPIO_OSPD(port) & ~m2) | ospd;
      GPIO_PUD(port)   = (GPIO_PUD(port) & ~m2) | pud;
      if ((mode & PAL_GD32_MODE_MASK) == PAL_GD32_MODE_ALTERNATE) {
        /* If going in alternate mode then the alternate number is set
           before switching mode in order to avoid glitches.*/
        if (bit < 8)
          GPIO_AFSEL0(port) = (GPIO_AFSEL0(port) & ~m4) | altrmask;
        else
          GPIO_AFSEL1(port) = (GPIO_AFSEL1(port) & ~m4) | altrmask;
        GPIO_CTL(port)   = (GPIO_CTL(port) & ~m2) | ctl;
      }
      else {
        /* If going into a non-alternate mode then the mode is switched
           before setting the alternate mode in order to avoid glitches.*/
        GPIO_CTL(port)   = (GPIO_CTL(port) & ~m2) | ctl;
        if (bit < 8)
          GPIO_AFSEL0(port) = (GPIO_AFSEL0(port) & ~m4) | altrmask;
        else
          GPIO_AFSEL1(port) = (GPIO_AFSEL1(port) & ~m4) | altrmask;
      }
    }
    mask >>= 1;
    if (!mask)
      return;
    omode <<= 1;
    ospd <<= 2;
    pud <<= 2;
    ctl <<= 2;
    bit++;
  }
}

#if PAL_USE_CALLBACKS || PAL_USE_WAIT || defined(__DOXYGEN__)
/**
 * @brief   Pad event enable.
 * @note    Programming an unknown or unsupported mode is silently ignored.
 * @note    GD32 EXTI lines 0..15 are entirely contained in the "bank 0"
 *          register set (EXTI_INTEN0/EXTI_EVEN0/EXTI_RTEN0/EXTI_FTEN0/
 *          EXTI_PD0), see gd32h73x_75x_exti.h. Bank 1 (EXTI_xxx1) handles
 *          lines 32..63 and is not used by GPIO pad events.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] mode      pad event mode
 *
 * @notapi
 */
void _pal_lld_enablepadevent(ioportid_t port,
                             iopadid_t pad,
                             ioeventmode_t mode) {

  uint32_t padmask, ssidx, ssoff, ssmask, portidx;

  /* Mask of the pad.*/
  padmask = 1U << (uint32_t)pad;

  /* Multiple channel setting of the same channel not allowed, first disable
     it. This is done because on GD32, like on STM32, the same channel
     cannot be mapped on multiple ports.*/
  osalDbgAssert(((EXTI_RTEN0 & padmask) == 0U) &&
                ((EXTI_FTEN0 & padmask) == 0U), "channel already in use");

  /* Port index is obtained assuming that GPIO ports are placed at regular
     0x400 intervals in memory space, with the (non-existent, on this
     family) GPIOI slot left as a gap. This matches the GD32 vendor
     EXTI_SOURCE_GPIOx constants (gd32h73x_75x_syscfg.h) exactly: A=0,
     B=1, ..., H=7, J=9, K=10.*/
  portidx = (((uint32_t)port - (uint32_t)GPIOA) >> 10U) & 0xFU;

  /* Index and nibble offset of the SYSCFG_EXTISSx register to be used,
     4 bits per line, 4 lines per register.*/
  ssidx  = (uint32_t)pad >> 2U;
  ssoff  = ((uint32_t)pad & 3U) * 4U;
  ssmask = ~(0xFU << ssoff);

  switch (ssidx) {
  case 0:
    SYSCFG_EXTISS0 = (SYSCFG_EXTISS0 & ssmask) | (portidx << ssoff);
    break;
  case 1:
    SYSCFG_EXTISS1 = (SYSCFG_EXTISS1 & ssmask) | (portidx << ssoff);
    break;
  case 2:
    SYSCFG_EXTISS2 = (SYSCFG_EXTISS2 & ssmask) | (portidx << ssoff);
    break;
  default:
    SYSCFG_EXTISS3 = (SYSCFG_EXTISS3 & ssmask) | (portidx << ssoff);
    break;
  }

  /* Programming edge registers.*/
  if (mode & PAL_EVENT_MODE_RISING_EDGE)
    EXTI_RTEN0 |= padmask;
  else
    EXTI_RTEN0 &= ~padmask;
  if (mode & PAL_EVENT_MODE_FALLING_EDGE)
    EXTI_FTEN0 |= padmask;
  else
    EXTI_FTEN0 &= ~padmask;

  /* Programming interrupt and event registers.*/
  EXTI_INTEN0 |= padmask;
  EXTI_EVEN0  &= ~padmask;
}

/**
 * @brief   Pad event disable.
 * @details This function disables previously programmed event callbacks.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 *
 * @notapi
 */
void _pal_lld_disablepadevent(ioportid_t port, iopadid_t pad) {
  uint32_t padmask, rten0, ften0;

  rten0 = EXTI_RTEN0;
  ften0 = EXTI_FTEN0;

  /* Mask of the pad.*/
  padmask = 1U << (uint32_t)pad;

  /* If either RTEN0 or FTEN0 is enabled then the channel is in use.*/
  if (((rten0 | ften0) & padmask) != 0U) {
    uint32_t ssidx, ssoff, ssport, portidx;

    /* Port index is obtained assuming that GPIO ports are placed at
       regular 0x400 intervals in memory space (see _pal_lld_enablepadevent
       for the rationale).*/
    portidx = (((uint32_t)port - (uint32_t)GPIOA) >> 10U) & 0xFU;

    /* Index and nibble offset of the SYSCFG_EXTISSx register to be used.*/
    ssidx  = (uint32_t)pad >> 2U;
    ssoff  = ((uint32_t)pad & 3U) * 4U;

    switch (ssidx) {
    case 0:
      ssport = (SYSCFG_EXTISS0 >> ssoff) & 0xFU;
      break;
    case 1:
      ssport = (SYSCFG_EXTISS1 >> ssoff) & 0xFU;
      break;
    case 2:
      ssport = (SYSCFG_EXTISS2 >> ssoff) & 0xFU;
      break;
    default:
      ssport = (SYSCFG_EXTISS3 >> ssoff) & 0xFU;
      break;
    }

    osalDbgAssert(ssport == portidx, "channel mapped on different port");

    /* Disabling channel.*/
    EXTI_INTEN0 &= ~padmask;
    EXTI_EVEN0  &= ~padmask;
    EXTI_RTEN0   = rten0 & ~padmask;
    EXTI_FTEN0   = ften0 & ~padmask;
    EXTI_PD0     = padmask;

#if PAL_USE_CALLBACKS || PAL_USE_WAIT
    /* Callback cleared and/or thread reset.*/
    _pal_clear_event(pad);
#endif
  }
}
#endif /* PAL_USE_CALLBACKS || PAL_USE_WAIT */

#endif /* HAL_USE_PAL == TRUE */

/** @} */
