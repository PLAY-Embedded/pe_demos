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

#define MAX_VALUE            1000
#define HALF_ADC             2047

BaseSequentialStream * chp = (BaseSequentialStream *) &SD2;
static bool flag = FALSE;
static int32_t x_raw, y_raw, x_offset, y_offset, x_scaled, y_scaled;
/*===========================================================================*/
/* ADC related code                                                          */
/*===========================================================================*/
/*
 * In this demo we want to use a single channel to sample voltage across
 * the potentiometer.
 */
#define MY_NUM_CH                                              2
#define MY_SAMPLING_NUMBER                                    10

static adcsample_t sample_buff[MY_SAMPLING_NUMBER] [MY_NUM_CH];

/*
 * ADC conversion group.
 * Mode:        Linear buffer, SW triggered.
 * Channels:    IN0 IN1.
 */
static const ADCConversionGroup my_conversion_group = {
  FALSE,                            /*NOT CIRCULAR*/
  MY_NUM_CH,                        /*NUMB OF CH*/
  NULL,                             /*NO ADC CALLBACK*/
  NULL,                             /*NO ADC ERROR CALLBACK*/
  0,                                /* CR1 */
  ADC_CR2_SWSTART,                  /* CR2 */
  0,                                /* SMPR1 */
  ADC_SMPR2_SMP_AN0(ADC_SAMPLE_144) |
  ADC_SMPR2_SMP_AN1(ADC_SAMPLE_144),/* SMPR2 */
  0,                                /* HTR */
  0,                                /* LTR */
  ADC_SQR1_NUM_CH(MY_NUM_CH),       /* SQR1 */
  0,                                /* SQR2 */
  ADC_SQR3_SQ1_N(ADC_CHANNEL_IN0) |
  ADC_SQR3_SQ2_N (ADC_CHANNEL_IN1)  /* SQR3 */
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

static THD_WORKING_AREA(waThd1, 256);
static THD_FUNCTION(Thd1, arg) {

  (void) arg;
  chRegSetThreadName("LED blinker");
  while(TRUE) {
    palTogglePad(GPIOA, GPIOA_LED_GREEN);
    chThdSleepMilliseconds(250);
  }
}

static THD_WORKING_AREA(waThd2, 512);
static THD_FUNCTION(Thd2, arg) {
  unsigned ii;
  (void) arg;
  chRegSetThreadName("Joystick sampler");
  /*
   * Setting up VRx and VRy pins.
   */
  palSetLineMode(LINE_ADC1_IN0, PAL_MODE_INPUT_ANALOG);
  palSetLineMode(LINE_ADC1_IN1, PAL_MODE_INPUT_ANALOG);
  
  /*
   * Activates the ADC1 driver.
   */
  adcStart(&ADCD1, NULL);

  /* Sampling data for offset computing */
  adcConvert(&ADCD1, &my_conversion_group, (adcsample_t*) sample_buff,
             MY_SAMPLING_NUMBER);
  /* Computing mean removing noise */
  x_raw = 0;
  y_raw = 0;
  for(ii = 0; ii < MY_SAMPLING_NUMBER; ii++){
    x_raw += sample_buff[ii][0];
    y_raw += sample_buff[ii][1];
  }
  x_raw /= MY_SAMPLING_NUMBER;
  y_raw /= MY_SAMPLING_NUMBER;
  /* Computing offset */
  x_offset = x_raw - HALF_ADC;
  y_offset = y_raw - HALF_ADC;
  while(TRUE) {
	/* Sampling data for demo purpose */
    adcConvert(&ADCD1, &my_conversion_group, (adcsample_t*) sample_buff,
               MY_SAMPLING_NUMBER);
	/* Computing mean removing noise */
    x_raw = 0;
    y_raw = 0;
    for(ii = 0; ii < MY_SAMPLING_NUMBER; ii++){
      x_raw += sample_buff[ii][0];
      y_raw += sample_buff[ii][1];
    }
    x_raw /= MY_SAMPLING_NUMBER;
    y_raw /= MY_SAMPLING_NUMBER;
	/* Removing offset and centring range */
    x_raw -= (HALF_ADC + x_offset);
    y_raw -= (HALF_ADC + y_offset);

	/* Resizing value properly */
    if(x_raw < 0){
      x_scaled = x_raw * MAX_VALUE / (HALF_ADC + x_offset);
    }
    else{
      x_scaled = x_raw * MAX_VALUE / (HALF_ADC - x_offset);
    }
    if(y_raw < 0){
      y_scaled = y_raw * MAX_VALUE / (HALF_ADC + y_offset);
    }
    else{
      y_scaled = y_raw * MAX_VALUE / (HALF_ADC - y_offset);
    }
	/* Data ready */
    flag = TRUE;
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
   * Activates the serial driver 2 using the driver default configuration.
   */
  sdStart(&SD2, NULL);

  chThdCreateStatic(waThd1, sizeof(waThd1), NORMALPRIO + 1, Thd1, NULL);
  chThdCreateStatic(waThd2, sizeof(waThd2), NORMALPRIO + 1, Thd2, NULL);
  
  /*
   * Setting up SW pin.
   */
  palSetLineMode(LINE_ARD_A2, PAL_MODE_INPUT_PULLUP);
  
  /*
   * Normal main() thread activity, in this demo it checks flag status. If flag
   * is true, last value is printed and then flag is lowered. 
   */
  while (TRUE) {

    if (flag) {
      chprintf(chp, "PLAY Embedded + ChibiOS\\RT: Slider demo \r\n");
      chprintf(chp, "X:%4d, Y:%4d ", x_scaled, y_scaled);
      if(palReadPad(GPIOA, 4) == PAL_LOW){
        chprintf(chp, "BUTTON PRESSED");
      }
      chprintf(chp, "\r\n");
      flag = FALSE;
      chThdSleepMilliseconds(150);
      chprintf(chp, "\033[2J\033[1;1H");
    }
    chThdSleepMilliseconds(1);
  }
}
