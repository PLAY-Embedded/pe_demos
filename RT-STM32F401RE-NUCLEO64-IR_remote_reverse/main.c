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
#include "chprintf.h"

#define SIZE 64

typedef struct queue {
  int32_t data[SIZE];
  unsigned head;
  unsigned tail;
}queue_t;

static void qInit(queue_t* qp) {
  qp->head = 0;
  qp->tail = 0;
}

static void qEnque(queue_t* qp, int32_t value) {
  qp->data[qp->tail] = value;
  qp->tail = (qp->tail + 1) % SIZE;
}

static int32_t qDequeue(queue_t* qp) {
  int32_t ret = qp->data[qp->head];
  qp->head = (qp->head + 1) % SIZE;
  return ret;
}

static bool isNotFull(queue_t* qp) {
  return (((qp->tail + 1) % SIZE) != qp->head);
}

static bool isNotEmpty(queue_t* qp) {
  return (qp->head != qp->tail);
}

static queue_t widths;

static BaseSequentialStream* chp = (BaseSequentialStream*) &SD2;
static void icuwidthcb(ICUDriver *icup) {
  if(isNotFull(&widths))
  qEnque(&widths, icuGetWidthX(icup));
}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_HIGH,
  10000,                                    /* 10kHz ICU clock frequency.   */
  icuwidthcb,
  NULL,
  NULL,
  ICU_CHANNEL_1,
  0
};

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

  /* Initializing a queue to store widths. */
  qInit(&widths);

  /*
   * Initializes the ICU driver 3.
   * GPIOC6 is the ICU input.
   * The two pins have to be externally connected together.
   */
  sdStart(&SD2, NULL);
  icuStart(&ICUD3, &icucfg);
  palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(2));
  icuStartCapture(&ICUD3);
  icuEnableNotifications(&ICUD3);
  chThdSleepMilliseconds(2000);



  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (true) {
    if(isNotEmpty(&widths))
      chprintf(chp, "%d ", qDequeue(&widths));
    chThdSleepMilliseconds(10);
  }
  return 0;
}
