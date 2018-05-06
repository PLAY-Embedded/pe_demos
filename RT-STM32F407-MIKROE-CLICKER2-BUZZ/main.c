/*
    PLAY Embedded demos - Copyright (C) 2014-2018 Rocco Marco Guglielmi

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
#include "usbcfg.h"

#define C5 1911
#define D5 1703
#define E5 1517
#define F5 1432
#define G5 1275
#define A6 1136
#define B6 1012

static BaseAsynchronousChannel * chnp = (BaseAsynchronousChannel*) &SDU1;
static uint8_t note, rsize;
static msg_t msg;
static PWMConfig pwmcfg = {
  1000000,                                  /* 1MHz PWM clock frequency. */
  100,                                      /* Initial PWM period 1ms.   */
  NULL,
  {
   {PWM_OUTPUT_ACTIVE_LOW, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};


/*
 * This is a periodic thread that does absolutely nothing except flashing
 * a LED.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    palTogglePad(GPIOE, GPIOE_LED1);
    chThdSleepMilliseconds(250);
  }
}

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

  /* Creates the example thread. */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO - 1, Thread1, NULL);

  /*
   * Initializes a serial-over-USB CDC driver.
   */
  sduObjectInit(&SDU1);
  sduStart(&SDU1, &serusbcfg);

  /*
   * Activates the USB driver and then the USB bus pull-up on D+.
   * Note, a delay is inserted in order to not have to disconnect the cable
   * after a reset.
   */
  usbDisconnectBus(serusbcfg.usbp);
  chThdSleepMilliseconds(1500);
  usbStart(serusbcfg.usbp, &usbcfg);
  usbConnectBus(serusbcfg.usbp);

  chThdSleepMilliseconds(2000);


  /*
   * Normal main() thread activity, in this demo it does nothing except
   * sleeping in a loop and check the button state.
   */
  pwmStart(&PWMD1, &pwmcfg);
  while (TRUE) {
    rsize = chnReadTimeout(chnp, &note, 1, TIME_MS2I(600));
    if( rsize == 1){
      msg = chnPutTimeout(chnp, note, TIME_MS2I(600));
      switch(note){
      case 'a':
        pwmChangePeriod(&PWMD1, C5);
        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
        break;
      case 's':
        pwmChangePeriod(&PWMD1, D5);
        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
        break;
      case 'd':
        pwmChangePeriod(&PWMD1, E5);
        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
        break;
      case 'f':
        pwmChangePeriod(&PWMD1, F5);
        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
        break;
      case 'g':
        pwmChangePeriod(&PWMD1, G5);
        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
        break;
      case 'h':
        pwmChangePeriod(&PWMD1, A6);
        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
        break;
      case 'j':
        pwmChangePeriod(&PWMD1, B6);
        pwmEnableChannel(&PWMD1, 0, PWM_PERCENTAGE_TO_WIDTH(&PWMD1, 5000));
        break;
	  default:
	    pwmDisableChannel(&PWMD1, 0);
		break;
      }
    }
    else{
      pwmDisableChannel(&PWMD1, 0);
    }
  }
}
