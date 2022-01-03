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

#include "rf.h"
#include "string.h"

#define  TRANSMITTER                          TRUE

#define  NRF24L01_LINE_CE                     LINE_ARD_D8
#define  NRF24L01_LINE_IRQ                    LINE_ARD_D9
#define  NRF24L01_SPI_CS                      LINE_ARD_D10
#define  NRF24L01_SPI_SCK                     LINE_ARD_D13
#define  NRF24L01_SPI_MISO                    LINE_ARD_D12
#define  NRF24L01_SPI_MOSI                    LINE_ARD_D11

#define  FRAME_LEN                            5

static const SPIConfig spicfg = {
  .circular         = false,
  .slave            = false,
  .data_cb          = NULL,
  .error_cb         = NULL,
  .ssline           = NRF24L01_SPI_CS,
  .cr1              = SPI_CR1_BR_1 | SPI_CR1_BR_0,
  .cr2              = 0
};

static RFConfig nrf24l01_cfg = {
  .line_ce          = NRF24L01_LINE_CE,
  .line_irq         = NRF24L01_LINE_IRQ,
  .spip             = &SPID1,
  .spicfg           = &spicfg,
  .auto_retr_count  = NRF24L01_ARC_15_times,
  .auto_retr_delay  = NRF24L01_ARD_4000us,
  .address_width    = NRF24L01_AW_5_bytes,
  .channel_freq     = 120,
  .data_rate        = NRF24L01_ADR_2Mbps,
  .out_pwr          = NRF24L01_PWR_0dBm,
  .lna              = NRF24L01_LNA_disabled,
  .en_dpl           = NRF24L01_DPL_enabled,
  .en_ack_pay       = NRF24L01_ACK_PAY_disabled,
  .en_dyn_ack       = NRF24L01_DYN_ACK_disabled
};

/*===========================================================================*/
/* Generic code.                                                             */
/*===========================================================================*/

static char string[RF_MAX_STRLEN + 1];
static uint32_t strl;

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
  palSetLineMode(NRF24L01_SPI_SCK, PAL_MODE_ALTERNATE(5) |
                                   PAL_STM32_OSPEED_HIGHEST);
  palSetLineMode(NRF24L01_SPI_MISO, PAL_MODE_ALTERNATE(5) |
                                    PAL_STM32_OSPEED_HIGHEST);
  palSetLineMode(NRF24L01_SPI_MOSI, PAL_MODE_ALTERNATE(5) |
                                    PAL_STM32_OSPEED_HIGHEST);
  palSetLineMode(NRF24L01_SPI_CS, PAL_MODE_OUTPUT_PUSHPULL |
                                  PAL_STM32_OSPEED_HIGHEST);
  /*
   * CE and IRQ pins setup.
   */
  palSetLineMode(NRF24L01_LINE_CE, PAL_MODE_OUTPUT_PUSHPULL |
                                   PAL_STM32_OSPEED_HIGHEST);
  palSetLineMode(NRF24L01_LINE_IRQ, PAL_MODE_INPUT |
                                    PAL_STM32_OSPEED_HIGHEST);

  /* Starting Serial Driver 2 with default configurations. */
  sdStart(&SD2, NULL);

  /* RF Driver Object constructor. */
  rfInit();

  /* Starting RF driver. */
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
      msg = rfTransmitString(&RFD1, string, "RXadd", TIME_MS2I(75));
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
    rfReceiveString(&RFD1, string, "RXadd", TIME_MS2I(2));
    strl = strlen(string);
    if(strl){
      chnWrite(&SD2, (const uint8_t *)string, strl);
      chnWrite(&SD2, (const uint8_t *)"\n\r", 2);
    }
#endif
  }
  rfStop(&RFD1);
}
