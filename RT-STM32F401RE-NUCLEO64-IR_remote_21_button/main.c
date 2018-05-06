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

static BaseSequentialStream* chp = (BaseSequentialStream*) &SD2;
static event_listener_t el;
static event_source_t IR_receiver;
/*===========================================================================*/
/* IR remote related code.                                                   */
/*===========================================================================*/

#define DELTA          5
#define START_PULSE    45
#define END_PULSE      395
#define RPT_CMD_PULSE  959
#define ZERO_PULSE     6
#define ONE_PULSE      17
#define COMMA_PULSE    23


/**
 * @brief  Button commands
 */
typedef enum {
  BTN_CH_DOWN = 0x00FFA25D,
  BTN_CH  = 0x00FF629D,
  BTN_CH_UP = 0x00FFE21D,
  BTN_PREV = 0x00FF22DD,
  BTN_NEXT = 0x00FF02FD,
  BTN_PLAY_PAUSE = 0x00FFC23D,
  BTN_VOL_DOWN = 0x00FFE01F,
  BTN_VOL_UP = 0x00FFA857,
  BTN_EQ  = 0x00FF906F,
  BTN_0 = 0x00FF6897,
  BTN_100 = 0x00FF9867,
  BTN_200 = 0x00FFB04F,
  BTN_1 = 0x00FF30CF,
  BTN_2 = 0x00FF18E7,
  BTN_3 = 0x00FF7A85,
  BTN_4 = 0x00FF10EF,
  BTN_5 = 0x00FF38C7,
  BTN_6 = 0x00FF5AA5,
  BTN_7 = 0x00FF42BD,
  BTN_8 = 0x00FF4AB5,
  BTN_9 = 0x00FF52AD
}IR_CMD_t;

/*===========================================================================*/
/* ICU related code.                                                         */
/*===========================================================================*/
static int32_t index = -1;
static bool START_OCCURED = FALSE, REPEAT_FLAG = FALSE;
static IR_CMD_t tmp, command;
static void icuwidthcb(ICUDriver *icup) {

  icucnt_t cnt = icuGetWidthX(icup);
  if((cnt > (START_PULSE - DELTA)) && (cnt < (START_PULSE + DELTA))){
    index = 0;
    START_OCCURED = TRUE;
  }
  else if((cnt > (ONE_PULSE - DELTA)) && (cnt < (ONE_PULSE + DELTA))){
    tmp |= 1 << (31 - index);
    index++;
  }
  else if((cnt > (ZERO_PULSE - DELTA)) && (cnt < (ZERO_PULSE + DELTA))){
    tmp &= ~(1 << (31 - index));
    index++;
  }
  else if((cnt > (END_PULSE - DELTA)) && (cnt < (END_PULSE + DELTA))){
    if((START_OCCURED) && (index == 32))
      command = tmp;
    else{
      command = 0;
    }
    REPEAT_FLAG = FALSE;
    START_OCCURED = FALSE;
    index = -1;
  }
  else if((cnt > (RPT_CMD_PULSE - DELTA)) && (cnt < (RPT_CMD_PULSE + DELTA))){
    REPEAT_FLAG = TRUE;
  }
  else if((cnt > (COMMA_PULSE - DELTA)) && (cnt < (COMMA_PULSE + DELTA))){
    chEvtBroadcastFlags(&IR_receiver, 0);
  }
  else{
    /* Long dead pulse nothing to do */
  }
}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_HIGH,
  10000,            /* 10kHz ICU clock frequency. */
  icuwidthcb,
  NULL,
  NULL,
  ICU_CHANNEL_1,
  0
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

/*
 * Application entry point.
 */
int main(void) {

  /*
 * System initializations.
 * - HAL initialization, this also initializes the configured device drivers
 * and performs the board-specific initializations.
 * - Kernel initialization, the main() function becomes a thread and the
 * RTOS is active.
 */
  halInit();
  chSysInit();

  chEvtObjectInit(&IR_receiver);
  chEvtRegister(&IR_receiver, &el, 0);
/*
 * Initializes the ICU driver 3.
 * GPIOC6 is the ICU input.
 * The two pins have to be externally connected together.
 */
  sdStart(&SD2, NULL);
  icuStart(&ICUD3, &icucfg);
  palSetPadMode(GPIOC, 6, PAL_MODE_ALTERNATE(2) | PAL_STM32_PUPDR_PULLUP);
  icuStartCapture(&ICUD3);
  icuEnableNotifications(&ICUD3);
  chThdSleepMilliseconds(2000);



  /*
 * Normal main() thread activity, in this demo it does nothing.
 */
  while (true) {
    chEvtWaitAny(ALL_EVENTS);
    switch(command){
    case BTN_CH_DOWN:
      chprintf(chp, "CH-");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_CH:
      chprintf(chp, "CH");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_CH_UP:
      chprintf(chp, "CH+");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_PREV:
      chprintf(chp, "PREV");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_NEXT:
      chprintf(chp, "NEXT");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_PLAY_PAUSE:
      chprintf(chp, "PLAY/PAUSE");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_VOL_DOWN:
      chprintf(chp, "VOL-");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_VOL_UP:
      chprintf(chp, "VOL+");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_EQ:
      chprintf(chp, "EQ");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_0:
      chprintf(chp, "0");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_100:
      chprintf(chp, "100+");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_200:
      chprintf(chp, "200+");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_1:
      chprintf(chp, "1");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_2:
      chprintf(chp, "2");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_3:
      chprintf(chp, "3");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_4:
      chprintf(chp, "4");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_5:
      chprintf(chp, "5");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_6:
      chprintf(chp, "6");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_7:
      chprintf(chp, "7");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_8:
      chprintf(chp, "8");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    case BTN_9:
      chprintf(chp, "9");
      if(REPEAT_FLAG)
        chprintf(chp, " RPT");
      break;
    default:
      chprintf(chp, "Unknown");
      break;
    }
    chprintf(chp,"\n\r");
  }
  return 0;
}
