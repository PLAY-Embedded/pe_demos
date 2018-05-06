/*
 *   S3DL - video3d_lld.h
 *   Written by Rocco Marco Guglielmi
 */

#ifndef _VIDEO3D_LLD_H_
#define _VIDEO3D_LLD_H_

#include "../../../../../userlib/s3dlconf.h"
#include "components.h"

#include "halconf.h"

#if (DISPLAY3D_USE_COLOR) || defined(__DOXYGEN__)
#warning "Video3D warning!"
#warning "ChibiCube is monochromatic! It is useless to set DISPLAY3D_USE_COLOR as TRUE"
#endif

#if (S3DL_USE_VIDEO3D) && ((!HAL_USE_SPI) || (!SPC5_SPI_USE_DSPI0)) || defined(__DOXYGEN__)
#error "Video3D error!"
#error "S3DL_USE_VIDEO3D requires HAL_USE_SPI and SPC5_SPI_USE_DSPI0"
#endif
#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)

/**
 * @brief   ChibiCube v2 identification macro.
 */
#define _CHIBICUBE_v2

/*============================================================================*/
/* CONNECTIONS                                                                */
/*============================================================================*/
/*  SPI used : DSPI_0
 *  SCK           -->  PA14
 *  MISO          -->  PA12
 *  MOSI          -->  PA13
 *  CS            -->  PA3
 *
 *  DECODER 3 TO 8:
 *  DEC_BIT0      -->  PB11
 *  DEC_BIT1      -->  PB12
 *  DEC_BIT2      -->  PB13
 *  DEC_EN        -->  PB14
 */

#define PLANE_MASK_FOR_LATCH       ((uint16_t)0xFFE1)
extern uint8_t order_invert[16];
/*============================================================================*/
/* SPI configuration                                                          */
/*============================================================================*/

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Clock frequency (187.5 kHz), Data size 8bit
 */
static const SPIConfig spiconf_nopwm = {
  NULL,
  PORT_A,
  3,
  0 | SPC5_CTAR_FMSZ(8) |
      SPC5_CTAR_PCSSCK_PRE7 | SPC5_CTAR_PASC_PRE3 |
      SPC5_CTAR_PDT_PRE3 | SPC5_CTAR_PBR_PRE2 |
      SPC5_CTAR_CSSCK_DIV128 | SPC5_CTAR_ASC_DIV32 |
      SPC5_CTAR_DT_DIV16 | SPC5_CTAR_BR_DIV128,
  0 | SPC5_PUSHR_CONT | SPC5_PUSHR_CTAS(0) | SPC5_PUSHR_PCS(0)
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Double baud rate, delay CS-SCK min, delay SCK-CS min
 * Clock frequency (187.50 kHz), Data size 8bit
 */
static const SPIConfig spiconf_slowest = {
  NULL,
  PORT_A,
  3,
  0 | SPC5_CTAR_FMSZ(8) | SPC5_CTAR_DBR |
      SPC5_CTAR_PCSSCK_PRE1 | SPC5_CTAR_PASC_PRE1 |
      SPC5_CTAR_PDT_PRE3 | SPC5_CTAR_PBR_PRE2 |
      SPC5_CTAR_CSSCK_DIV2 | SPC5_CTAR_ASC_DIV2 |
      SPC5_CTAR_DT_DIV16 | SPC5_CTAR_BR_DIV256,
  0 | SPC5_PUSHR_CONT | SPC5_PUSHR_CTAS(0) | SPC5_PUSHR_PCS(0)
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Double baud rate, delay CS-SCK min, delay SCK-CS min
 * Clock frequency (375 kHz), Data size 8bit
 */
static const SPIConfig spiconf_slow = {
  NULL,
  PORT_A,
  3,
  0 | SPC5_CTAR_FMSZ(8) | SPC5_CTAR_DBR |
      SPC5_CTAR_PCSSCK_PRE1 | SPC5_CTAR_PASC_PRE1 |
      SPC5_CTAR_PDT_PRE3 | SPC5_CTAR_PBR_PRE2 |
      SPC5_CTAR_CSSCK_DIV2 | SPC5_CTAR_ASC_DIV2 |
      SPC5_CTAR_DT_DIV16 | SPC5_CTAR_BR_DIV128,
  0 | SPC5_PUSHR_CONT | SPC5_PUSHR_CTAS(0) | SPC5_PUSHR_PCS(0)
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Double baud rate, delay CS-SCK min, delay SCK-CS min
 * Clock frequency (750 kHz), Data size 8bit
 */
static const SPIConfig spiconf_mid_slow = {
  NULL,
  PORT_A,
  3,
  0 | SPC5_CTAR_FMSZ(8) | SPC5_CTAR_DBR |
      SPC5_CTAR_PCSSCK_PRE1 | SPC5_CTAR_PASC_PRE1 |
      SPC5_CTAR_PDT_PRE3 | SPC5_CTAR_PBR_PRE2 |
      SPC5_CTAR_CSSCK_DIV2 | SPC5_CTAR_ASC_DIV2 |
      SPC5_CTAR_DT_DIV16 | SPC5_CTAR_BR_DIV64,
  0 | SPC5_PUSHR_CONT | SPC5_PUSHR_CTAS(0) | SPC5_PUSHR_PCS(0)
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Double baud rate, delay CS-SCK min, delay SCK-CS min
 * Clock frequency (1.5 MHz), Data size 8bit
 */
static const SPIConfig spiconf_mid_1 = {
  NULL,
  PORT_A,
  3,
  0 | SPC5_CTAR_FMSZ(8) | SPC5_CTAR_DBR |
      SPC5_CTAR_PCSSCK_PRE1 | SPC5_CTAR_PASC_PRE1 |
      SPC5_CTAR_PDT_PRE3 | SPC5_CTAR_PBR_PRE2 |
      SPC5_CTAR_CSSCK_DIV2 | SPC5_CTAR_ASC_DIV2 |
      SPC5_CTAR_DT_DIV16 | SPC5_CTAR_BR_DIV32,
  0 | SPC5_PUSHR_CONT | SPC5_PUSHR_CTAS(0) | SPC5_PUSHR_PCS(0)
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Double baud rate, delay CS-SCK min, delay SCK-CS min
 * Clock frequency (3 MHz), Data size 8bit
 */
static const SPIConfig spiconf_mid_2 = {
  NULL,
  PORT_A,
  3,
  0 | SPC5_CTAR_FMSZ(8) | SPC5_CTAR_DBR |
      SPC5_CTAR_PCSSCK_PRE1 | SPC5_CTAR_PASC_PRE1 |
      SPC5_CTAR_PDT_PRE3 | SPC5_CTAR_PBR_PRE2 |
      SPC5_CTAR_CSSCK_DIV2 | SPC5_CTAR_ASC_DIV2 |
      SPC5_CTAR_DT_DIV16 | SPC5_CTAR_BR_DIV16,
  0 | SPC5_PUSHR_CONT | SPC5_PUSHR_CTAS(0) | SPC5_PUSHR_PCS(0)
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Double baud rate, delay CS-SCK min, delay SCK-CS min
 * Clock frequency (6 MHz), Data size 8bit
 */
static const SPIConfig spiconf_mid_fast = {
  NULL,
  PORT_A,
  3,
  0 | SPC5_CTAR_FMSZ(8) | SPC5_CTAR_DBR |
      SPC5_CTAR_PCSSCK_PRE1 | SPC5_CTAR_PASC_PRE1 |
      SPC5_CTAR_PDT_PRE3 | SPC5_CTAR_PBR_PRE2 |
      SPC5_CTAR_CSSCK_DIV2 | SPC5_CTAR_ASC_DIV2 |
      SPC5_CTAR_DT_DIV16 | SPC5_CTAR_BR_DIV8,
  0 | SPC5_PUSHR_CONT | SPC5_PUSHR_CTAS(0) | SPC5_PUSHR_PCS(0)
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Double baud rate, delay CS-SCK min, delay SCK-CS min
 * Clock frequency (12 MHz), Data size 8bit
 */
static const SPIConfig spiconf_fast = {
  NULL,
  PORT_A,
  3,
  0 | SPC5_CTAR_FMSZ(8) | SPC5_CTAR_DBR |
      SPC5_CTAR_PCSSCK_PRE1 | SPC5_CTAR_PASC_PRE1 |
      SPC5_CTAR_PDT_PRE3 | SPC5_CTAR_PBR_PRE2 |
      SPC5_CTAR_CSSCK_DIV2 | SPC5_CTAR_ASC_DIV2 |
      SPC5_CTAR_DT_DIV16 | SPC5_CTAR_BR_DIV4,
  0 | SPC5_PUSHR_CONT | SPC5_PUSHR_CTAS(0) | SPC5_PUSHR_PCS(0)
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Double baud rate, delay CS-SCK min, delay SCK-CS min
 * Clock frequency (24 MHz), Data size 8bit
 */
static const SPIConfig spiconf_max = {
  NULL,
  PORT_A,
  3,
  0 | SPC5_CTAR_FMSZ(8) | SPC5_CTAR_DBR |
      SPC5_CTAR_PCSSCK_PRE1 | SPC5_CTAR_PASC_PRE1 |
      SPC5_CTAR_PDT_PRE3 | SPC5_CTAR_PBR_PRE2 |
      SPC5_CTAR_CSSCK_DIV2 | SPC5_CTAR_ASC_DIV2 |
      SPC5_CTAR_DT_DIV16 | SPC5_CTAR_BR_DIV2,
  0 | SPC5_PUSHR_CONT | SPC5_PUSHR_CTAS(0) | SPC5_PUSHR_PCS(0)
};





/*============================================================================*/
/* Cube dim e intensity depth                                                */
/*============================================================================*/

#define  Z_DIM                       8
#define  Y_DIM                       8
#define  X_DIM                       8


#define  MAX_INT_DEPTH               8
/*============================================================================*/
/* Configuration depending part                                               */
/*============================================================================*/

#if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
/*
 * Y dim information is hidden into uint8_t
 */
extern uint8_t frame_buffer[MAX_INT_DEPTH][Z_DIM][X_DIM];
extern uint8_t empty_buffer[X_DIM];
/**
 * @brief   Update data of the latch
 */
#define plane_send(int_deepth, plane){                                         \
    spiSend(&SPID1, 8, frame_buffer[int_deepth][plane]);                       \
}



/**
 * @brief   Latch out the previous data
 */
#define latch_out() {                                                          \
  spiUnselect(&SPID1);                                                         \
  spiSelect(&SPID1);                                                           \
}

/**
 * @brief   Refresh a plane using PWM
 */
#define plane_pwm(plane){                                                      \
  palWritePort(PORT_B, ((palReadLatch(PORT_B) & PLANE_MASK_FOR_LATCH)          \
                       | (order_invert[plane] << 1)));                         \
  spiStart(&SPID1, &spiconf_max);                                              \
  plane_send(0, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID1, &spiconf_max);                                              \
  plane_send(1, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID1, &spiconf_fast);                                             \
  plane_send(2, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID1, &spiconf_mid_fast);                                         \
  plane_send(3, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID1, &spiconf_mid_2);                                            \
  plane_send(4, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID1, &spiconf_mid_1);                                            \
  plane_send(5, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID1, &spiconf_mid_slow);                                         \
  plane_send(6, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID1, &spiconf_slow);                                             \
  plane_send(7, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID1, &spiconf_slowest);                                          \
  spiSend(&SPID1, 8, empty_buffer);                                            \
  latch_out();                                                                 \
}
#else

/*
 * X dim information is hidden into uint8_t
 */
extern uint8_t frame_buffer[Z_DIM][X_DIM];


#endif /*DISPLAY3D_USE_PWM*/









/*===========================================================================*/
/* Function external declarations.                                           */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void video3d_init_lld(void);
  void refresh_display3D_lld(void);
#ifdef __cplusplus
}
#endif

#endif /* S3DL_USE_VIDEO3D */
#endif /* _VIDEO3D_LLD_H_ */
