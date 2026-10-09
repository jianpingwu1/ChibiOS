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
 * This demo uses the GD32H759I_EVAL board template. LINE_LED1/
 * LINE_LED2 (see board.h) are wired to PF10/PA6, active high, matching the
 * vendor GD32H759I-EVAL board support package (Utilites/gd32h759i_eval.h/
 * .c) - update board.h if the target board's LED is wired differently.
 */

/*
 * Console on USART0, pin mapping taken from the vendor GD32H759I-EVAL
 * board support package (Utilites/gd32h759i_eval.h).
 */
#define CONSOLE_SD          SD0
#define CONSOLE_TX_LINE     PAL_LINE(GPIOA, 9U)
#define CONSOLE_RX_LINE     PAL_LINE(GPIOA, 10U)

/*
 * Heartbeat blinker thread, times are in milliseconds.
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

  palSetLineMode(LINE_LED1, PAL_MODE_OUTPUT_PUSHPULL);

  /*
   * Console startup, the default configuration is used so the bit rate is
   * the SERIAL_DEFAULT_BITRATE set in halconf.h.
   */
  palSetLineMode(CONSOLE_TX_LINE, PAL_MODE_ALTERNATE(7) |
                                  PAL_GD32_OTYPE_PUSHPULL |
                                  PAL_GD32_OSPEED_HIGHEST |
                                  PAL_GD32_PUPDR_PULLUP);
  palSetLineMode(CONSOLE_RX_LINE, PAL_MODE_ALTERNATE(7) |
                                  PAL_GD32_PUPDR_PULLUP);
  sdStart(&CONSOLE_SD, NULL);
  chnWrite(&CONSOLE_SD, (const uint8_t *)"GD32H7 ChibiOS minimal\r\n", 24);

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO + 1, Thread1, NULL);

  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop.
   */
  while (true) {
    chThdSleepMilliseconds(500);
  }
}
