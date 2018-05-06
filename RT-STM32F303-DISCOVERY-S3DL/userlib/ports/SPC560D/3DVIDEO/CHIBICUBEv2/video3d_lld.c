/*
 *   S3DL - video3d_lld.c
 *   Written by Rocco Marco Guglielmi
 */

#include <ports/SPC560D/3DVIDEO/CHIBICUBEv2/video3d_lld.h>

#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)
/*===========================================================================*/
/* Local parameters                                                          */
/*===========================================================================*/
static uint8_t plane;

uint8_t order_invert[16] = {
   0x0, 0x8, 0x4, 0xC,
   0x2, 0xA, 0x6, 0xE,
   0x1, 0x9, 0x5, 0xD,
   0x3, 0xB, 0x7, 0xF };
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
uint8_t frame_buffer[Z_DIM][X_DIM] ={
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
		{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
};
#endif /*DISPLAY3D_USE_PWM*/



/*===========================================================================*/
/* FUNCTIONS                                                                 */
/*===========================================================================*/

/**
 * @brief   Configures pad in order to use 3dcube
 */
void video3d_init_lld(void){
  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  plane = 0;
  palWritePort(PORT_B, ((palReadLatch(PORT_B) & PLANE_MASK_FOR_LATCH) | (order_invert[plane] << 1)));
  #else
  plane = 7;
  spiStart(&SPID1, &spiconf_nopwm);
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
  palWritePort(PORT_B, ((palReadLatch(PORT_B) & PLANE_MASK_FOR_LATCH) | (order_invert[plane] << 1)));
  plane = (plane + 1) & 7;
  spiSelect(&SPID1);
  spiSend(&SPID1, 8, frame_buffer[plane]);
  spiUnselect(&SPID1);
  #endif /* DISPLAY3D_USE_PWM */
}

#endif /* S3DL_USE_VIDEO3D */
