/*
    PLAY Embedded demos - Copyright (C) 2014...2019 Rocco Marco Guglielmi

    This file is part of PLAY Embedded demos.

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

#include "hal.h"
#include "ch.h"

/* Green LED blinker thread, times are in milliseconds. */
THD_WORKING_AREA(waThread1, 256);
THD_FUNCTION(Thread1, arg) {

  (void)arg;

  while (true) {
    palToggleLine(LINE_LED_GREEN);
    chThdSleepMilliseconds(500);
  }
}

/* Hello world thread. */
THD_WORKING_AREA(waThread2, 256);
THD_FUNCTION(Thread2, arg) {

  (void)arg;

  /* Start Serial Driver 2 with default configuration */
  sdStart(&SD2, NULL);

  while (true) {
    chnWrite(&SD2, (const uint8_t *)"Hello World!\r\n", 14);
    chThdSleepMilliseconds(500);
  }
}

/*
 * Threads static table, one entry per thread. The number of entries must
 * match CH_CFG_MAX_THREADS.
 */
THD_TABLE_BEGIN
  THD_TABLE_THREAD(0, "LED blinker", waThread1, Thread1, NULL)
  THD_TABLE_THREAD(1, "Hello world", waThread2, Thread2, NULL)
THD_TABLE_END

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /* This is now the idle thread loop, you may perform here a low priority
     task but sleeps are not allowed. */
  while (true) {
  }
}
