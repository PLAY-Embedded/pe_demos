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
#include "max7219.h"

static systime_t time, hours, minutes, seconds, millisec;
static BaseSequentialStream* chp = (BaseSequentialStream*) &SD2;
/*===========================================================================*/
/* Numeric display related code.                                             */
/*===========================================================================*/

#define DOT                                  1<<7

/*===========================================================================*/
/* SPI related code.                                                         */
/*===========================================================================*/

#define  GPIOB_SPID1_CS                 6
#define  GPIOA_SPID1_SCK                5
#define  GPIOA_SPID1_MISO               6
#define  GPIOA_SPID1_MOSI               7

static const SPIConfig spicfg = {
  NULL,
  GPIOB,                                          /*   port of CS   */
  GPIOB_SPID1_CS,                                 /*   pin of CS    */
  SPI_CR1_BR | SPI_CR1_DFF,                       /*   CR1 register */
  0                                               /*   CR2 register */
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
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  /*
   * SPID1 I/O pins setup.(It bypasses board.h configurations)
   */
  palSetPadMode(GPIOA, GPIOA_SPID1_SCK,
                PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);   /* New SCK */
  palSetPadMode(GPIOA, GPIOA_SPID1_MISO,
                PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);   /* New MISO*/
  palSetPadMode(GPIOA, GPIOA_SPID1_MOSI,
                PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);   /* New MOSI*/
  palSetPadMode(GPIOB, GPIOB_SPID1_CS,
                PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);/* New CS  */
  sdStart(&SD2, NULL);
  spiStart(&SPID1, &spicfg);
  max7219WriteRegister(&SPID1, MAX7219_AD_DISPLAY_TEST, FALSE);
  max7219WriteRegister(&SPID1, MAX7219_AD_SHUTDOWN, MAX7219_OM_Normal);
  max7219WriteRegister(&SPID1, MAX7219_AD_SCAN_LIMIT, MAX7219_SL_7);
  max7219WriteRegister(&SPID1, MAX7219_AD_DECODE_MODE,
                       MAX7219_DM_CodeB_7);
  max7219WriteRegister(&SPID1, MAX7219_AD_INTENSITY,
                       MAX7219_IM_31_32);
  while (TRUE) {

    time = chVTGetSystemTime();
    hours = time / 36000000;
    minutes = (time / 600000) % 60;
    seconds = (time / 10000) % 60;
    millisec = (time / 10) % 1000;
    chprintf(chp, "%d:%2d:%2d.%3d\n\r", hours, minutes, seconds, millisec);
    max7219WriteRegister(&SPID1, MAX7219_AD_DIGIT_7, (millisec % 10));
    max7219WriteRegister(&SPID1, MAX7219_AD_DIGIT_6, ((millisec / 10) % 10));
    max7219WriteRegister(&SPID1, MAX7219_AD_DIGIT_5, (millisec / 100));
    max7219WriteRegister(&SPID1, MAX7219_AD_DIGIT_4, (seconds % 10) | DOT);
    max7219WriteRegister(&SPID1, MAX7219_AD_DIGIT_3, (seconds / 10));
    max7219WriteRegister(&SPID1, MAX7219_AD_DIGIT_2, (minutes % 10) | DOT);
    max7219WriteRegister(&SPID1, MAX7219_AD_DIGIT_1, (minutes / 10));
    max7219WriteRegister(&SPID1, MAX7219_AD_DIGIT_0, hours | DOT);
    chThdSleepMilliseconds(1);
  }
  spiStop(&SPID1);
}

