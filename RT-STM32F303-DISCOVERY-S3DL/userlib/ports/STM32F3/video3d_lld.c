/*
 *   S3DL - video3d_lld.c
 *   Written by Rocco Marco Guglielmi
 */

#include "video3d_lld.h"

#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)
/*===========================================================================*/
/* Local parameters                                                          */
/*===========================================================================*/
static uint8_t plane;

/*===========================================================================*/
/* Configuration depending part                                              */
/*===========================================================================*/

#if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
/*
 * Data structure for cube that allows double buffering and the intensity
 * regulation.
 * X dim information is hidden into uint8_t
 * MAX_BIT_POS It's a dimension that allows 256 level of intensity
 */
 uint8_t frame_buffer[MAX_INT_DEPTH][Z_DIM][X_DIM];
 uint8_t empty_buffer[X_DIM] = {0, 0, 0, 0, 0, 0, 0, 0} ;
#else
/*
 * Data structure for cube that allows double buffering but not the intensity
 * regulation.
 * X dim information is hidden into uint8_t
 */
uint8_t frame_buffer[Z_DIM][X_DIM];
#endif /*DISPLAY3D_USE_PWM*/



/*===========================================================================*/
/* FUNCTIONS                                                                 */
/*===========================================================================*/

/**
 * @brief   Configures pad in order to use 3dcube
 */
void video3d_init_lld(void){

  palSetPadMode(GPIOB, GPIOB_SPI2_SCK, PAL_MODE_ALTERNATE(5)
                | PAL_STM32_OSPEED_HIGHEST);                  /*  SPI D2 SCK  */
  palSetPadMode(GPIOB, GPIOB_SPI2_MISO, PAL_MODE_ALTERNATE(5)
                | PAL_STM32_OSPEED_HIGHEST);                  /*  SPI D2 MISO */
  palSetPadMode(GPIOB, GPIOB_SPI2_MOSI, PAL_MODE_ALTERNATE(5)
                | PAL_STM32_OSPEED_HIGHEST);                  /*  SPI D2 MOSI */
  palSetPadMode(GPIOB, GPIOB_SPI2_CS, PAL_MODE_OUTPUT_PUSHPULL
                | PAL_STM32_OSPEED_HIGHEST);                  /*  SPI D2 CS   */
  palSetPadMode(GPIOB, GPIOB_DEC_BIT_0, PAL_MODE_OUTPUT_PUSHPULL
                | PAL_STM32_OSPEED_HIGHEST);                  /*  DEC BIT 0   */
  palSetPadMode(GPIOB, GPIOB_DEC_BIT_1, PAL_MODE_OUTPUT_PUSHPULL
                | PAL_STM32_OSPEED_HIGHEST);                  /*  DEC BIT 1   */
  palSetPadMode(GPIOB, GPIOB_DEC_BIT_2, PAL_MODE_OUTPUT_PUSHPULL
                | PAL_STM32_OSPEED_HIGHEST);                  /*  DEC BIT 2   */
  palSetPadMode(GPIOB, GPIOB_DEC_EN, PAL_MODE_OUTPUT_PUSHPULL
                | PAL_STM32_OSPEED_HIGHEST);                  /*  DEC ENABLE  */

  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  plane = 0;
  palWritePort(GPIOB, (palReadLatch(GPIOB) & PLANE_MASK_FOR_LATCH) | (plane << 2));
  #else
  plane = 7;
  #endif /*DISPLAY3D_USE_PWM*/
}

/**
 * @brief   Sent out data using SPI
 */

void refresh_display3D_lld(void){
  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  plane_pwm(plane);
  plane = (plane + 1) & 7;
  #else
  palWritePort(GPIOB, (palReadLatch(GPIOB) & PLANE_MASK_FOR_LATCH) | (plane << 2));
  plane = (plane + 1) & 7;
  spiStart(&SPID2, &spiconf_slowest);
  spiSelect(&SPID2);
  spiSend(&SPID2, 8, frame_buffer[plane]);
  spiUnselect(&SPID2);
  #endif /* DISPLAY3D_USE_PWM */
}

#endif /* S3DL_USE_VIDEO3D */
