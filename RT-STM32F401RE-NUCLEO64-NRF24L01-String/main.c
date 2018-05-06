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

#include "rf.h"
#include "string.h"

#define  TRANSMITTER                   FALSE

#define  GPIOA_RF_CE                       9
#define  GPIOC_RF_IRQ                      7
#define  GPIOB_RF_SPID1_CS                 6
#define  GPIOA_RF_SPID1_SCK                5
#define  GPIOA_RF_SPID1_MISO               6
#define  GPIOA_RF_SPID1_MOSI               7

#define  FRAME_LEN                         5
static const SPIConfig std_spi_cfg = {
  NULL,
  GPIOB,                                          /*   port of CS   */
  GPIOB_RF_SPID1_CS,                              /*   pin of CS    */
  SPI_CR1_BR_1 | SPI_CR1_BR_0,                    /*   CR1 register */
  0                                               /*   CR2 register */
};

static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_FALLING_EDGE |
     EXT_CH_MODE_AUTOSTART |
     EXT_MODE_GPIOC, rfExtCallBack},            /* IRQ line connected to PC7 */
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
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL}
  }
};

static RFConfig nrf24l01_cfg = {
  GPIOA,
  GPIOA_RF_CE,
  GPIOC,
  GPIOC_RF_IRQ,
  &SPID1,
  &std_spi_cfg,
  &EXTD1,
  &extcfg,
  NRF24L01_ARC_15_times,     /* auto_retr_count */
  NRF24L01_ARD_4000us,       /* auto_retr_delay */
  NRF24L01_AW_5_bytes,       /* address_width */
  120,                       /* channel_freq 2.4 + 0.12 GHz */
  NRF24L01_ADR_2Mbps,        /* data_rate */
  NRF24L01_PWR_0dBm,         /* out_pwr */
  NRF24L01_LNA_disabled,     /* lna */
  NRF24L01_DPL_enabled ,     /* en_dpl */
  NRF24L01_ACK_PAY_disabled, /* en_ack_pay */
  NRF24L01_DYN_ACK_disabled  /* en_dyn_ack */
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/


/*
 * Green LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread, 512);
static THD_FUNCTION(Thread, arg) {
  char string[RF_MAX_STRLEN + 1];
  uint32_t strl;
  (void)arg;
  chRegSetThreadName("RF thread");

  rfStart(&RFD1, &nrf24l01_cfg);

  while (TRUE) {
#if TRANSMITTER == TRUE
    char c;
    rf_msg_t msg;
    uint8_t counter = 0;
    string[0] = '\0';
    while(TRUE){
      c = chnGetTimeout((&SD2), TIME_INFINITE);
      if((c == '\r') || (counter == RF_MAX_STRLEN)){
        string[counter] = '\0';
        chnWrite(&SD2, (const uint8_t *)"\n\r", 2);
        break;
      }
      else if((c > 31) && (c < 127)){
        string[counter] = c;
        counter++;
        chnPutTimeout((&SD2), c, TIME_INFINITE);
      }
    }
    strl = strlen(string);
    if(strl){
      msg = rfTransmitString(&RFD1, string, "RXadd", MS2ST(75));
      if(msg == RF_OK){
        chnWrite(&SD2, (const uint8_t *)"Message sent\n\r", 14);
      }
      else if(msg == RF_ERROR){
        chnWrite(&SD2, (const uint8_t *)"Message not sent (MAX_RT)\n\r", 27);
      }
      else{
        chnWrite(&SD2, (const uint8_t *)"Message not sent (TIMEOUT)\n\r", 28);
      }
    }
    chThdSleepMilliseconds(50);
#else
    string[0] = '\0';
    rfReceiveString(&RFD1, string, "RXadd", MS2ST(2));
    strl = strlen(string);
    if(strl){
      chnWrite(&SD2, (const uint8_t *)string, strl);
      chnWrite(&SD2, (const uint8_t *)"\n\r", 2);
    }
#endif
  }
  rfStop(&RFD1);
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
   * Application library initialization.
   * - PLAY initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   */
  rfInit();

  /*
   * SPID1 I/O pins setup.(It bypasses board.h configurations)
   */
  palSetPadMode(GPIOA, GPIOA_RF_SPID1_SCK,
                 PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);   /* New SCK */
  palSetPadMode(GPIOA, GPIOA_RF_SPID1_MISO,
                 PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);   /* New MISO*/
  palSetPadMode(GPIOA, GPIOA_RF_SPID1_MOSI,
                 PAL_MODE_ALTERNATE(5) | PAL_STM32_OSPEED_HIGHEST);   /* New MOSI*/
  palSetPadMode(GPIOB, GPIOB_RF_SPID1_CS,
                 PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);/* New CS  */
  /*
   * CE and IRQ pins setup.
   */

  palSetPadMode(GPIOA, GPIOA_RF_CE,
                 PAL_MODE_OUTPUT_PUSHPULL | PAL_STM32_OSPEED_HIGHEST);/* New CE  */
  palSetPadMode(GPIOC, GPIOC_RF_IRQ,
                 PAL_MODE_INPUT | PAL_STM32_OSPEED_HIGHEST);          /* New IRQ  */

  sdStart(&SD2, NULL);

  chThdCreateStatic(waThread, sizeof(waThread), NORMALPRIO, Thread, NULL);
  while(TRUE){

    chThdSleepMilliseconds(500);
  }
}
