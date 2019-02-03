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
 
#include "ch.h"
#include "hal.h"

#include "string.h"
#include "s3dl.h"

char string[128];

static THD_WORKING_AREA(waGames_handler, 1024);
static THD_FUNCTION(Games_handler, arg) {
  (void) arg;
  chRegSetThreadName("Games_handler");
  while(TRUE){
    s3dl_games_handler();
  }
}

static THD_WORKING_AREA(waDisplay_refresh, 1024);
static THD_FUNCTION(Display_refresh, arg) {
  (void)arg;
  chRegSetThreadName("Display3D refresh");
  while (TRUE){
    s3dl_refresh_display3D();
    palTogglePad(GPIOE, GPIOE_LED4_BLUE);
  }
}

/*
 * Application entry point.
 */
int main(void){
  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();
  s3dlInit();

  s3dl_sentences_init(4, "PLAY Embedded", "chibios.org", "ChibiCube",
                         "playembedded.org");
  /*
   * Activates the thread
   */
  chThdCreateStatic(waGames_handler, sizeof(waGames_handler), NORMALPRIO + 1,
                    Games_handler, NULL);
  chThdCreateStatic(waDisplay_refresh, sizeof(waDisplay_refresh),
                    NORMALPRIO + 2, Display_refresh, NULL);

  chRegSetThreadName("main");
  while (TRUE) {
    palTogglePad(GPIOE, GPIOE_LED9_BLUE);
    chThdSleepMilliseconds(500);
  }
}







