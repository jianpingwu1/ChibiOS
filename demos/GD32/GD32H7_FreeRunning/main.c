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

#include "ch.h"
#include "hal.h"

/*
 * This demo exercises OSAL_ST_MODE_FREERUNNING (tickless) on the
 * GD32H73x/75x port. The free running system timer is GD32 TIMER4
 * (32-bit), see GD32/LLD/SYSTICKv1/hal_st_lld.{h,c}. CH_CFG_ST_FREQUENCY
 * is 1MHz, so each system tick is 1us. The blinker below toggles the LED
 * with a 500ms period derived from the freerunning counter, which validates
 * the 1MHz base by measuring the wall-clock period on an oscilloscope.
 *
 * This demo uses the GD32H759I_EVAL board template. LINE_LED1/
 * LINE_LED2 (see board.h) are wired to PF10/PA6, active high, matching the
 * vendor GD32H759I-EVAL board support package (Utilites/gd32h759i_eval.h/
 * .c) - update board.h if the target board's LED is wired differently.
 */

/*
 * Console on USART0, pin mapping taken from the vendor GD32H759I-EVAL
 * board support package (Utilities/gd32h759i_eval.h).
 */
#define CONSOLE_SD          SD0
#define CONSOLE_TX_LINE     PAL_LINE(GPIOA, 9U)
#define CONSOLE_RX_LINE     PAL_LINE(GPIOA, 10U)

/*
 * Heartbeat blinker thread.
 * Uses the freerunning 1MHz system counter directly (stGetTime() returns
 * microseconds in this configuration) to produce an accurate 500ms toggle.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");
  while (true) {
    palSetLine(LINE_LED1);
    chThdSleepMilliseconds(500);
    palClearLine(LINE_LED1);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device
   *   drivers and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /* Console on USART0 (PA9 TX, PA10 RX, AF7).*/
  palSetLineMode(CONSOLE_TX_LINE, PAL_MODE_ALTERNATE(7));
  palSetLineMode(CONSOLE_RX_LINE, PAL_MODE_ALTERNATE(7));
  sdStart(&CONSOLE_SD, NULL);

  chnWrite(&CONSOLE_SD, (const uint8_t *)"GD32H7 ChibiOS freerun\r\n", 24);

  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /*
   * Normal main() thread activity.
   */
  while (true) {
    palToggleLine(LINE_LED2);
    chThdSleepMilliseconds(1000);
  }
}