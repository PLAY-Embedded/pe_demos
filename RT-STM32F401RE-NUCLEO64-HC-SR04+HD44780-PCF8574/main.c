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
#include "lcd.h"

#define LINE_ARD_D14                PAL_LINE(GPIOB, 9U)
#define LINE_ARD_D15                PAL_LINE(GPIOB, 8U)

#define LINE_TRIGGER                PAL_LINE(GPIOB, 10U)
#define LINE_ECHO                   PAL_LINE(GPIOA, 8U)

/*===========================================================================*/
/* LCD configuration                                                         */
/*===========================================================================*/

static const I2CConfig i2ccfg = {
  OPMODE_I2C,
  100000,
  STD_DUTY_CYCLE,
};

static const LCDConfig lcdcfg = {
  &I2CD1,                   /* I2C driver */
  &i2ccfg,                  /* I2C configuration */
  0x3F,                     /* Slave address */
  LCD_CURSOR_OFF,           /* Cursor disabled */
  LCD_BLINKING_OFF,         /* Blinking disabled */
  LCD_SET_FONT_5X10,        /* Font 5x10 */
  LCD_SET_2LINES,           /* 2 lines */
  LCD_BL_ON                 /* backlight initial status */
};

/*===========================================================================*/
/* ICU related code                                                          */
/*===========================================================================*/

#define ICU_TIM_FREQ                1000000
#define M_TO_CM                     100.0f
#define SPEED_OF_SOUND              343.2f

static float lastdistance = 0.0;

static void icuwidthcb(ICUDriver *icup) {

  icucnt_t width = icuGetWidthX(icup);
  lastdistance = (SPEED_OF_SOUND * width * M_TO_CM) / (ICU_TIM_FREQ * 2);
}

static ICUConfig icucfg = {
  ICU_INPUT_ACTIVE_HIGH,
  ICU_TIM_FREQ,                                /* 1MHz ICU clock frequency.   */
  icuwidthcb,
  NULL,
  NULL,
  ICU_CHANNEL_1,
  0
};

/*===========================================================================*/
/* Generic code                                                              */
/*===========================================================================*/

static char string[40];
/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("LED blinker");
  while (true) {
    palClearPad(GPIOA, GPIOA_LED_GREEN);
    chThdSleepMilliseconds(500);
    palSetPad(GPIOA, GPIOA_LED_GREEN);
    chThdSleepMilliseconds(500);
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

  lcdInit();

  /*
   * Creates the blinker thread.
   */
  chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

  /* Initializes the ICU driver 1. The ICU input is PA8. */
  palSetLineMode(LINE_ECHO, PAL_MODE_ALTERNATE(1));
  icuStart(&ICUD1, &icucfg);
  icuStartCapture(&ICUD1);
  icuEnableNotifications(&ICUD1);

  palSetLineMode(LINE_TRIGGER, PAL_MODE_OUTPUT_PUSHPULL);

  /* Configuring I2C related PINs */
  palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |
                 PAL_STM32_PUPDR_PULLUP);
  palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |
                 PAL_STM32_PUPDR_PULLUP);

  lcdStart(&LCDD1, &lcdcfg);
  lcdWriteString(&LCDD1, "Distance", 0);

  while (true) {
    /* Triggering */
    palWriteLine(LINE_TRIGGER, PAL_HIGH);
    chThdSleepMicroseconds(10);
    palWriteLine(LINE_TRIGGER, PAL_LOW);

    chsnprintf(string, sizeof(string), "%.2f cm       ", lastdistance);
    lcdWriteString(&LCDD1, string, 40);
    chThdSleepMilliseconds(200);
  }
  icuStopCapture(&ICUD1);
  icuStop(&ICUD1);
}
