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

/*
 * This file has been automatically generated from board.chcfg by
 * tools/gd32/boardgen.py, do not edit it manually. Edit the .chcfg
 * description and regenerate instead.
 */

#include "hal.h"
#include "gd32_gpio.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of GD32 GPIO port setup.
 */
typedef struct {
  uint32_t              ctl;
  uint32_t              omode;
  uint32_t              ospd;
  uint32_t              pud;
  uint32_t              octl;
  uint32_t              afsel0;
  uint32_t              afsel1;
} gpio_setup_t;

/**
 * @brief   Type of GD32 GPIO initialization data.
 */
typedef struct {
#if GD32_HAS_GPIOA || defined(__DOXYGEN__)
  gpio_setup_t          PAData;
#endif
#if GD32_HAS_GPIOB || defined(__DOXYGEN__)
  gpio_setup_t          PBData;
#endif
#if GD32_HAS_GPIOC || defined(__DOXYGEN__)
  gpio_setup_t          PCData;
#endif
#if GD32_HAS_GPIOD || defined(__DOXYGEN__)
  gpio_setup_t          PDData;
#endif
#if GD32_HAS_GPIOE || defined(__DOXYGEN__)
  gpio_setup_t          PEData;
#endif
#if GD32_HAS_GPIOF || defined(__DOXYGEN__)
  gpio_setup_t          PFData;
#endif
#if GD32_HAS_GPIOG || defined(__DOXYGEN__)
  gpio_setup_t          PGData;
#endif
#if GD32_HAS_GPIOH || defined(__DOXYGEN__)
  gpio_setup_t          PHData;
#endif
#if GD32_HAS_GPIOJ || defined(__DOXYGEN__)
  gpio_setup_t          PJData;
#endif
#if GD32_HAS_GPIOK || defined(__DOXYGEN__)
  gpio_setup_t          PKData;
#endif
} gpio_config_t;

/**
 * @brief   GD32 GPIO static initialization data.
 */
static const gpio_config_t gpio_default_config = {
#if GD32_HAS_GPIOA
  {VAL_GPIOA_CTL, VAL_GPIOA_OMODE, VAL_GPIOA_OSPD, VAL_GPIOA_PUD,
   VAL_GPIOA_OCTL, VAL_GPIOA_AFSEL0, VAL_GPIOA_AFSEL1},
#endif
#if GD32_HAS_GPIOB
  {VAL_GPIOB_CTL, VAL_GPIOB_OMODE, VAL_GPIOB_OSPD, VAL_GPIOB_PUD,
   VAL_GPIOB_OCTL, VAL_GPIOB_AFSEL0, VAL_GPIOB_AFSEL1},
#endif
#if GD32_HAS_GPIOC
  {VAL_GPIOC_CTL, VAL_GPIOC_OMODE, VAL_GPIOC_OSPD, VAL_GPIOC_PUD,
   VAL_GPIOC_OCTL, VAL_GPIOC_AFSEL0, VAL_GPIOC_AFSEL1},
#endif
#if GD32_HAS_GPIOD
  {VAL_GPIOD_CTL, VAL_GPIOD_OMODE, VAL_GPIOD_OSPD, VAL_GPIOD_PUD,
   VAL_GPIOD_OCTL, VAL_GPIOD_AFSEL0, VAL_GPIOD_AFSEL1},
#endif
#if GD32_HAS_GPIOE
  {VAL_GPIOE_CTL, VAL_GPIOE_OMODE, VAL_GPIOE_OSPD, VAL_GPIOE_PUD,
   VAL_GPIOE_OCTL, VAL_GPIOE_AFSEL0, VAL_GPIOE_AFSEL1},
#endif
#if GD32_HAS_GPIOF
  {VAL_GPIOF_CTL, VAL_GPIOF_OMODE, VAL_GPIOF_OSPD, VAL_GPIOF_PUD,
   VAL_GPIOF_OCTL, VAL_GPIOF_AFSEL0, VAL_GPIOF_AFSEL1},
#endif
#if GD32_HAS_GPIOG
  {VAL_GPIOG_CTL, VAL_GPIOG_OMODE, VAL_GPIOG_OSPD, VAL_GPIOG_PUD,
   VAL_GPIOG_OCTL, VAL_GPIOG_AFSEL0, VAL_GPIOG_AFSEL1},
#endif
#if GD32_HAS_GPIOH
  {VAL_GPIOH_CTL, VAL_GPIOH_OMODE, VAL_GPIOH_OSPD, VAL_GPIOH_PUD,
   VAL_GPIOH_OCTL, VAL_GPIOH_AFSEL0, VAL_GPIOH_AFSEL1},
#endif
#if GD32_HAS_GPIOJ
  {VAL_GPIOJ_CTL, VAL_GPIOJ_OMODE, VAL_GPIOJ_OSPD, VAL_GPIOJ_PUD,
   VAL_GPIOJ_OCTL, VAL_GPIOJ_AFSEL0, VAL_GPIOJ_AFSEL1},
#endif
#if GD32_HAS_GPIOK
  {VAL_GPIOK_CTL, VAL_GPIOK_OMODE, VAL_GPIOK_OSPD, VAL_GPIOK_PUD,
   VAL_GPIOK_OCTL, VAL_GPIOK_AFSEL0, VAL_GPIOK_AFSEL1}
#endif
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void gpio_init(ioportid_t gpiop, const gpio_setup_t *config) {

  GPIO_OMODE(gpiop)  = config->omode;
  GPIO_OSPD(gpiop)   = config->ospd;
  GPIO_PUD(gpiop)    = config->pud;
  GPIO_OCTL(gpiop)   = config->octl;
  GPIO_AFSEL0(gpiop) = config->afsel0;
  GPIO_AFSEL1(gpiop) = config->afsel1;
  GPIO_CTL(gpiop)    = config->ctl;
}

static void gd32_gpio_init(void) {

  /* Enabling GPIO-related clocks and forcing a reset on all ports, one
     at a time (no combined GD32_GPIO_EN_MASK exists yet).*/
#if GD32_HAS_GPIOA
  rcuResetGPIOA();
  rcuEnableGPIOA(true);
#endif
#if GD32_HAS_GPIOB
  rcuResetGPIOB();
  rcuEnableGPIOB(true);
#endif
#if GD32_HAS_GPIOC
  rcuResetGPIOC();
  rcuEnableGPIOC(true);
#endif
#if GD32_HAS_GPIOD
  rcuResetGPIOD();
  rcuEnableGPIOD(true);
#endif
#if GD32_HAS_GPIOE
  rcuResetGPIOE();
  rcuEnableGPIOE(true);
#endif
#if GD32_HAS_GPIOF
  rcuResetGPIOF();
  rcuEnableGPIOF(true);
#endif
#if GD32_HAS_GPIOG
  rcuResetGPIOG();
  rcuEnableGPIOG(true);
#endif
#if GD32_HAS_GPIOH
  rcuResetGPIOH();
  rcuEnableGPIOH(true);
#endif
#if GD32_HAS_GPIOJ
  rcuResetGPIOJ();
  rcuEnableGPIOJ(true);
#endif
#if GD32_HAS_GPIOK
  rcuResetGPIOK();
  rcuEnableGPIOK(true);
#endif

  /* Initializing all the defined GPIO ports.*/
#if GD32_HAS_GPIOA
  gpio_init(GPIOA, &gpio_default_config.PAData);
#endif
#if GD32_HAS_GPIOB
  gpio_init(GPIOB, &gpio_default_config.PBData);
#endif
#if GD32_HAS_GPIOC
  gpio_init(GPIOC, &gpio_default_config.PCData);
#endif
#if GD32_HAS_GPIOD
  gpio_init(GPIOD, &gpio_default_config.PDData);
#endif
#if GD32_HAS_GPIOE
  gpio_init(GPIOE, &gpio_default_config.PEData);
#endif
#if GD32_HAS_GPIOF
  gpio_init(GPIOF, &gpio_default_config.PFData);
#endif
#if GD32_HAS_GPIOG
  gpio_init(GPIOG, &gpio_default_config.PGData);
#endif
#if GD32_HAS_GPIOH
  gpio_init(GPIOH, &gpio_default_config.PHData);
#endif
#if GD32_HAS_GPIOJ
  gpio_init(GPIOJ, &gpio_default_config.PJData);
#endif
#if GD32_HAS_GPIOK
  gpio_init(GPIOK, &gpio_default_config.PKData);
#endif
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Early initialization code.
 * @details GPIO ports are initialized before everything else. The main
 *          clock tree is brought up later, from hal_lld_init() (see
 *          GD32H73x_75x/hal_lld.c gd32_clock_init()), unlike some other
 *          ChibiOS ports where clock init also happens here.
 */
void __early_init(void) {
  gd32_lld_early_init();

  gd32_gpio_init();
}

/**
 * @brief   Board-specific initialization code.
 * @note    This function is empty in most cases.
 */
void boardInit(void) {
}

#if HAL_USE_SDC || defined(__DOXYGEN__)
/**
 * @brief   SDC card detection.
 * @note    Weak default, override with a strong symbol in board-specific
 *          code to implement real hardware detection.
 */
__attribute__((weak))
bool sdc_lld_is_card_inserted(SDCDriver *sdcp) {

  (void)sdcp;
  return true;
}

/**
 * @brief   SDC card write protection detection.
 */
__attribute__((weak))
bool sdc_lld_is_write_protected(SDCDriver *sdcp) {

  (void)sdcp;
  return false;
}
#endif /* HAL_USE_SDC */
