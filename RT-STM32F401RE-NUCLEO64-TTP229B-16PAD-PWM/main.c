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

#define GPIOB_SCL               8
#define GPIOB_SDO               9

static event_source_t TTP229;
static event_listener_t myel;
static unsigned ii, jj;
static uint16_t matrixlatch;
BaseSequentialStream* chp = (BaseSequentialStream*) &SD2;
/*===========================================================================*/
/* EXT related code                                                          */
/*===========================================================================*/

static void ExtCallBack(EXTDriver *extp, expchannel_t channel) {

  (void) extp;
  (void) channel;
  chSysLockFromISR();
  chEvtBroadcastFlagsI(&TTP229, 0);
  chSysUnlockFromISR();
}

static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_FALLING_EDGE |
     EXT_CH_MODE_AUTOSTART |
     EXT_MODE_GPIOB, ExtCallBack},            /* IRQ line connected to PB9 */
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

/*===========================================================================*/
/* PWM related code                                                          */
/*===========================================================================*/

static void pwmpcb(PWMDriver *pwmp) {

  (void)pwmp;
}

static void pwmc3cb(PWMDriver *pwmp) {

  (void)pwmp;
  if(!palReadPad(GPIOB, GPIOB_SDO))
    matrixlatch |= (1 << ii);
  ii++;
  if (ii == 16)
    chEvtBroadcastFlags(&TTP229, 0);
}

static PWMConfig pwmcfg = {
  1000000,                                    /* 1MHz PWM clock frequency.   */
  10,                                         /* Initial PWM frequency 100kHz.       */
  pwmpcb,
  {
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_DISABLED, NULL},
   {PWM_OUTPUT_ACTIVE_LOW, pwmc3cb},
   {PWM_OUTPUT_DISABLED, NULL}
  },
  0,
  0
};

/*===========================================================================*/
/* Generic code                                                              */
/*===========================================================================*/

static THD_WORKING_AREA(waThd1, 128);
static THD_FUNCTION(Thd1, arg) {

  (void) arg;
  chRegSetThreadName("Led handler");

  while(TRUE) {
    palTogglePad(GPIOA, GPIOA_LED_GREEN);
    chThdSleepMilliseconds(500);
  }
}

static THD_WORKING_AREA(waThd2, 128);
static THD_FUNCTION(Thd2, arg) {

  (void) arg;
  chRegSetThreadName("Matrix handler");
  chEvtObjectInit(&TTP229);
  chEvtRegister(&TTP229, &myel, 0);
  palSetPadMode(GPIOB, GPIOB_SCL, PAL_MODE_ALTERNATE(2));
  palSetPadMode(GPIOB, GPIOB_SDO, PAL_MODE_INPUT | PAL_STM32_OSPEED_HIGHEST);
  while(TRUE) {

    extStart(&EXTD1, &extcfg);
    chEvtWaitAny(ALL_EVENTS);
    extStop(&EXTD1);
    ii = 0;
    matrixlatch = 0;
    pwmStart(&PWMD4, &pwmcfg);
    pwmEnableChannelNotification(&PWMD4, 2);
    pwmEnableChannel(&PWMD4, 2, PWM_PERCENTAGE_TO_WIDTH(&PWMD4, 5000));
    chEvtWaitAny(ALL_EVENTS);
    pwmStop(&PWMD4);
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

  /*
   * Here we are creating two threads.
   */
  chThdCreateStatic(waThd1, sizeof(waThd1), NORMALPRIO + 1, Thd1, NULL);
  chThdCreateStatic(waThd2, sizeof(waThd2), NORMALPRIO + 2, Thd2, NULL);
  sdStart(&SD2, NULL);

  /*
   * Normal main() thread activity: nothing.
   */
  while (TRUE) {
    for(jj = 0; jj < 16; jj++)
      if(matrixlatch & (1 << jj))
        chprintf(chp,"Button %d pressed\n\r", jj + 1);

    matrixlatch = 0;
    chThdSleepMilliseconds(200);
  }
}
