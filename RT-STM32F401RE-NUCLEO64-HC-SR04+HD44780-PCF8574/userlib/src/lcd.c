/*
    PLAY Embedded - Copyright (C) 2006..2015 Rocco Marco Guglielmi

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

/*
    Special thanks to Giovanni Di Sirio for teachings, his moral support and
    friendship. Note that some or every piece of this file could be part of
    the ChibiOS project that is intellectual property of Giovanni Di Sirio.
    Please refer to ChibiOS/RT license before use this file.
	
	For suggestion or Bug report - guglielmir@playembedded.org
 */

/**
 * @file    lcd.c
 * @brief   LCD complex driver code.
 *
 * @addtogroup LCD
 * @{
 */

#include "lcd.h"

#if USERLIB_USE_LCD || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* LCD BITS */
#define LCD_RS                          1 << 0
#define LCD_RW                          1 << 1
#define LCD_E                           1 << 2
#define LCD_K                           1 << 3

#define LCD_D(n)                        1 << n
#define LCD_D_HIGHER(n)                 (n & 0xF0)
#define LCD_D_LOWER(n)                  ((n & 0x0F) << 4)

/* LCD REGISTERS */
#define LCD_INSTRUCTION_R               0
#define LCD_DATA_R                      LCD_RS


/* LCD_INSTRUCTIONS */
#define LCD_CLEAR_DISPLAY               0x01

#define LCD_RETURN_HOME                 0x02

#define LCD_EMS                         0x04
#define LCD_EMS_S                       0x01
#define LCD_EMS_ID                      0x02

#define LCD_DC                          0x08
#define LCD_DC_B                        0x01
#define LCD_DC_C                        0x02
#define LCD_DC_D                        0x04

#define LCD_CDS                         0x10
#define LCD_CDS_RL                      0x04
#define LCD_CDS_SC                      0x08

#define LCD_FS                          0x20
#define LCD_FS_F                        0x04
#define LCD_FS_N                        0x08
#define LCD_FS_DL                       0x10

#define LCD_SET_CGRAM_ADDRESS           0x40
#define LCD_SET_CGRAM_ADDRESS_MASK      0X3F

#define LCD_SET_DDRAM_ADDRESS           0x80
#define LCD_SET_DDRAM_ADDRESS_MASK      0X7F

#define LCD_BUSY_FLAG                   0X80

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   LCDD1 driver identifier.
 */
LCDDriver LCDD1;

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Write a data into a register for the lcd
 *
 * @param[in] lcdp          LCD driver
 * @param[in] reg           Register id
 * @param[in] value         Writing value
 *
 * @notapi
 */
static void hd44780WriteRegister(LCDDriver *lcdp, uint8_t reg, uint8_t value) {
  uint8_t txbuf[4];
  osalThreadSleepMilliseconds(2);

  txbuf[0] = reg | LCD_D_HIGHER(value) | LCD_E;
  if(lcdp->backlight)
    txbuf[0] |= LCD_K;
  txbuf[1] = reg | LCD_D_HIGHER(value);
  if(lcdp->backlight)
    txbuf[1] |= LCD_K;
  txbuf[2] = reg | LCD_D_LOWER(value) | LCD_E;
  if(lcdp->backlight)
    txbuf[2] |= LCD_K;
  txbuf[3] = reg | LCD_D_LOWER(value);
  if(lcdp->backlight)
    txbuf[3] |= LCD_K;

  i2cMasterTransmitTimeout(lcdp->config->i2cp, lcdp->config->slaveaddress,
                           txbuf, 4, NULL, 0, TIME_INFINITE);
}

/**
 * @brief   Perform a initialization by instruction as explained in HD44780
 *          datasheet.
 * @note    This reset is required after a mis-configuration or if there aren't
 *          condition to enable internal reset circuit.
 *
 * @param[in] lcdp          LCD driver
 *
 * @notapi
 */
static void hd44780InitByIstructions(LCDDriver *lcdp) {
  uint8_t txbuf[2];
  osalThreadSleepMilliseconds(50);

  txbuf[0] = LCD_D(4) | LCD_D(5) | LCD_E;
  txbuf[1] = LCD_D(4) | LCD_D(5);
  i2cMasterTransmitTimeout(lcdp->config->i2cp, lcdp->config->slaveaddress,
                           txbuf, 2, NULL, 0, TIME_INFINITE);
  osalThreadSleepMilliseconds(5);

  txbuf[0] = LCD_D(4) | LCD_D(5) | LCD_E;
  txbuf[1] = LCD_D(4) | LCD_D(5);
  i2cMasterTransmitTimeout(lcdp->config->i2cp, lcdp->config->slaveaddress,
                           txbuf, 2, NULL, 0, TIME_INFINITE);
  osalThreadSleepMilliseconds(1);

  txbuf[0] = LCD_D(4) | LCD_D(5) | LCD_E;
  txbuf[1] = LCD_D(4) | LCD_D(5);
  i2cMasterTransmitTimeout(lcdp->config->i2cp, lcdp->config->slaveaddress,
                           txbuf, 2, NULL, 0, TIME_INFINITE);

  txbuf[0] = LCD_D(5) | LCD_E;
  txbuf[1] = LCD_D(5);
  i2cMasterTransmitTimeout(lcdp->config->i2cp, lcdp->config->slaveaddress,
                           txbuf, 2, NULL, 0, TIME_INFINITE);

  /* Configuring data interface */
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_FS | LCD_DATA_LENGHT |
                       lcdp->config->font | lcdp->config->lines);

  /* Turning off display and clearing */
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_DC);
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_CLEAR_DISPLAY);

  /* Setting display control turning on display */
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_DC | LCD_DC_D |
                       lcdp->config->cursor | lcdp->config->blinking);

  /* Setting Entry Mode */
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_EMS | LCD_EMS_ID);
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   LCD Complex Driver initialization.
 *
 * @init
 */
void lcdInit(void) {

  /* Object initialization */
  lcdObjectInit(&LCDD1);
}

/**
 * @brief   Initializes an instance.
 *
 * @param[out] lcdp         pointer to the @p LCDDriver object
 *
 * @init
 */
void lcdObjectInit(LCDDriver *lcdp){

  lcdp->state  = LCD_STOP;
  lcdp->config = NULL;
  lcdp->backlight = 0;
}

/**
 * @brief   Configures and activates the LCD Complex Driver  peripheral.
 *
 * @param[in] lcd   pointer to the @p LCDDriver object
 * @param[in] config    pointer to the @p LCDConfig object
 *
 * @api
 */
void lcdStart(LCDDriver *lcdp, const LCDConfig *config) {

  osalDbgCheck((lcdp != NULL) && (config != NULL));

  osalDbgAssert((lcdp->state == LCD_STOP) || (lcdp->state == LCD_ACTIVE),
              "lcdStart(), invalid state");

  lcdp->config = config;
  lcdp->backlight = lcdp->config->backlight;

  i2cStart(lcdp->config->i2cp, lcdp->config->i2ccfg);
  /* Initializing HD44780 by instructions. */
  hd44780InitByIstructions(lcdp);

  lcdp->state = LCD_ACTIVE;
}

/**
 * @brief   Deactivates the LCD Complex Driver  peripheral.
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 *
 * @api
 */
void lcdStop(LCDDriver *lcdp) {

  osalDbgCheck(lcdp != NULL);

  osalDbgAssert((lcdp->state == LCD_STOP) || (lcdp->state == LCD_ACTIVE),
              "lcdStop(), invalid state");

  lcdp->backlight = FALSE;
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_DC);
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_CLEAR_DISPLAY);
  i2cStop(lcdp->config->i2cp);
  lcdp->state = LCD_STOP;
}

/**
 * @brief   Turn on back-light.
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 *
 * @api
 */
void lcdBacklightOn(LCDDriver *lcdp) {

  osalDbgCheck(lcdp != NULL);
  osalDbgAssert((lcdp->state == LCD_ACTIVE), "lcdBacklightOn(), invalid state");

  lcdp->backlight = TRUE;
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, 0);
}

/**
 * @brief   Turn off back-light.
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 *
 * @api
 */
void lcdBacklightOff(LCDDriver *lcdp) {

  osalDbgCheck(lcdp != NULL);
  osalDbgAssert((lcdp->state == LCD_ACTIVE), "lcdBacklightOff(), invalid state");

  lcdp->backlight = FALSE;
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, 0);
}

/**
 * @brief   Clears display and return cursor in the first position.
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 *
 * @api
 */
void lcdClearDisplay(LCDDriver *lcdp){

  osalDbgCheck(lcdp != NULL);
  osalDbgAssert((lcdp->state == LCD_ACTIVE), "lcdClearDisplay(), invalid state");
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_CLEAR_DISPLAY);
}

/**
 * @brief   Return cursor in the first position.
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 *
 * @api
 */
void lcdReturnHome(LCDDriver *lcdp){

  osalDbgCheck(lcdp != NULL);
  osalDbgAssert((lcdp->state == LCD_ACTIVE), "lcdReturnHome(), invalid state");
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_RETURN_HOME);
}

/**
 * @brief   Set DDRAM address position leaving data unchanged.
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 * @param[in] add       DDRAM address (from 0 to LCD_DDRAM_MAX_ADDRESS)
 *
 * @api
 */
void lcdSetAddress(LCDDriver *lcdp, uint8_t add){

  osalDbgCheck(lcdp != NULL);
  osalDbgAssert((lcdp->state == LCD_ACTIVE),
                "lcdSetAddress(), invalid state");
  if(add > LCD_SET_DDRAM_ADDRESS_MASK)
    return;
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_SET_DDRAM_ADDRESS | add);
}

/**
 * @brief   Writes string starting from a certain position.
 *
 * @detail  If string lenght exceeds, then is cutted
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 * @param[in] string    string to write
 * @param[in] pos       position for cursor (from 0 to LCD_DDRAM_MAX_ADDRESS)
 *
 * @api
 */
void lcdWriteString(LCDDriver *lcdp, char* string, uint8_t pos){
  int32_t iteration;
  osalDbgCheck((lcdp != NULL) && (string != NULL));
  osalDbgAssert((lcdp->state == LCD_ACTIVE),
                "lcdWriteString(), invalid state");

  iteration = LCD_SET_DDRAM_ADDRESS_MASK - pos + 1;
  if(iteration > 0){
    lcdSetAddress(lcdp, pos);
    while((*string != '\0') && (iteration > 0)){
      hd44780WriteRegister(lcdp, LCD_DATA_R, *string);
      string++;
      iteration--;
    }
  }
  else
    return;
}

/**
 * @brief   Makes a shift according to an arbitrary direction
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 * @param[in] dir       direction (LCD_RIGHT or LCD_LEFT)
 *
 * @api
 */
void lcdDoDisplayShift(LCDDriver *lcdp, uint8_t dir){

  osalDbgCheck(lcdp != NULL);
  osalDbgAssert((lcdp->state == LCD_ACTIVE),
                "lcdDoDisplayShift(), invalid state");
  hd44780WriteRegister(lcdp, LCD_INSTRUCTION_R, LCD_CDS | LCD_CDS_SC | dir);
}

#if LCD_USE_DIMMABLE_BACKLIGHT
/**
 * @brief   Set back-light percentage.
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 * @param[in] perc      back-light percentage (from 0 to 100)
 *
 * @api
 */
void lcdSetBacklight(LCDDriver *lcdp, uint32_t perc){

  osalDbgCheck(lcdp != NULL);
  osalDbgAssert((lcdp->state == LCD_ACTIVE), "lcdSetBacklight(), invalid state");
  pwmEnableChannel(lcdp->config->pwmp, lcdp->config->channelid,
                   PWM_PERCENTAGE_TO_WIDTH(lcdp->config->pwmp, perc * 100));
  lcdp->backlight = perc;
}

/**
 * @brief   Shift back-light from current value to 0.
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 *
 * @api
 */
void lcdBacklightFadeOut(LCDDriver *lcdp){
  uint32_t curr = lcdp->backlight;
  osalDbgCheck(lcdp != NULL);
  osalDbgAssert((lcdp->state == LCD_ACTIVE),
                "lcdBacklightFadeOut(), invalid state");
  while(curr != 0){
    curr--;
    lcdSetBacklight(lcdp, curr);
    osalThreadSleepMilliseconds(10);
  }
}

/**
 * @brief   Shift back-light from current value to 100.
 *
 * @param[in] lcdp      pointer to the @p LCDDriver object
 *
 * @api
 */
void lcdBacklightFadeIn(LCDDriver *lcdp){
  uint32_t curr = lcdp->backlight;
  osalDbgCheck(lcdp != NULL);
  osalDbgAssert((lcdp->state == LCD_ACTIVE),
                "lcdBacklightFadeIn(), invalid state");
  while(curr != 100){
    curr++;
    lcdSetBacklight(lcdp, curr);
    osalThreadSleepMilliseconds(10);
  }
}
#endif

/** @} */
#endif /* USERLIB_USE_LCD */
