/*
 *   S3DL - video3d_lld.h
 *   Written by Rocco Marco Guglielmi
 */

#ifndef _VIDEO3D_LLD_H_
#define _VIDEO3D_LLD_H_

#include "hal.h"
#include "ch.h"
#include "halconf.h"
#include "s3dlconf.h"

#if (DISPLAY3D_USE_COLOR) || defined(__DOXYGEN__)
#warning "Video3D warning!"
#warning "ChibiCube is monochromatic! It is useless to set DISPLAY3D_USE_COLOR as TRUE"
#endif

#if ((S3DL_USE_VIDEO3D) && ((!HAL_USE_SPI) || (!STM32_SPI_USE_SPI2))) || defined(__DOXYGEN__)
#error "Video3D error!"
#error "S3DL_USE_VIDEO3D requires HAL_USE_SPI and STM32_SPI_USE_SPI2"
#endif

#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)

/**
 * @brief   ChibiCube v2 identification macro.
 */
#define _CHIBICUBE_v2

/*============================================================================*/
/* CONNECTIONS                                                                */
/*============================================================================*/
/*  SPI used : SPI DRIVER 2
 *  SCK           -->  PB13
 *  MISO          -->  PB14
 *  MOSI          -->  PB15
 *  CS            -->  PB12
 *
 *  DECODER 3 TO 8:
 *  DEC_BIT0      -->  PB2
 *  DEC_BIT1      -->  PB3
 *  DEC_BIT2      -->  PB4
 *  DEC_EN        -->  PB5
 */


#define  GPIOB_SPI2_SCK              13
#define  GPIOB_SPI2_MISO             14
#define  GPIOB_SPI2_MOSI             15
#define  GPIOB_SPI2_CS               12

#define  GPIOB_DEC_BIT_0              2
#define  GPIOB_DEC_BIT_1              3
#define  GPIOB_DEC_BIT_2              4
#define  GPIOB_DEC_EN                 5

#define  PLANE_MASK_FOR_LATCH       ((uint16_t)0xFFC3)
/*============================================================================*/
/* SPI configuration                                                          */
/*============================================================================*/
/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Clock frequency (36 MHz / 256), Data size 8bit
 */
static const SPIConfig spiconf_slowest = {
 NULL,
 GPIOB,                                                   /*port of CS*/
 GPIOB_PIN12,                                             /*pin of CS*/
 SPI_CR1_BR,                                              /*CR1 register*/
 SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0,              /*CR2 register*/
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Clock frequency (36 MHz / 128), Data size 8bit
 */
static const SPIConfig spiconf_slow = {
 NULL,
 GPIOB,                                                   /*port of CS*/
 GPIOB_PIN12,                                             /*pin of CS*/
 SPI_CR1_BR_2 | SPI_CR1_BR_1,                             /*CR1 register*/
 SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0,              /*CR2 register*/
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Clock frequency (36 MHz / 64), Data size 8bit
 */
static const SPIConfig spiconf_mid_slow = {
 NULL,
 GPIOB,                                                   /*port of CS*/
 GPIOB_PIN12,                                             /*pin of CS*/
 SPI_CR1_BR_2 | SPI_CR1_BR_0,                             /*CR1 register*/
 SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0,              /*CR2 register*/
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Clock frequency (36 MHz / 32), Data size 8bit
 */
static const SPIConfig spiconf_mid_1 = {
 NULL,
 GPIOB,                                                   /*port of CS*/
 GPIOB_PIN12,                                             /*pin of CS*/
 SPI_CR1_BR_2,                                            /*CR1 register*/
 SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0,              /*CR2 register*/
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Clock frequency (36 MHz / 16), Data size 8bit
 */
static const SPIConfig spiconf_mid_2 = {
 NULL,
 GPIOB,                                                   /*port of CS*/
 GPIOB_PIN12,                                             /*pin of CS*/
 SPI_CR1_BR_1 | SPI_CR1_BR_0,                             /*CR1 register*/
 SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0,              /*CR2 register*/
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Clock frequency (36 MHz / 8), Data size 8bit
 */
static const SPIConfig spiconf_mid_fast = {
 NULL,
 GPIOB,                                                   /*port of CS*/
 GPIOB_PIN12,                                             /*pin of CS*/
 SPI_CR1_BR_1,                                            /*CR1 register*/
 SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0,              /*CR2 register*/
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 * (CPOL = 0, CPHA = 0)
 * Clock frequency (36 MHz / 4), Data size 8bit
 */
static const SPIConfig spiconf_fast = {
 NULL,
 GPIOB,                                                   /*port of CS*/
 GPIOB_PIN12,                                             /*pin of CS*/
 SPI_CR1_BR_0,                                            /*CR1 register*/
 SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0,              /*CR2 register*/
};

/*
 * Default level clock low (CPOL = 0),
 * Data propagated on rising edge and captured on falling edge
 *(CPOL = 0, CPHA = 0)
 * Clock frequency (36 MHz / 2), Data size 8bit
 */
static const SPIConfig spiconf_max = {
  NULL,
  GPIOB,                                                   /*port of CS*/
  GPIOB_PIN12,                                             /*pin of CS*/
  0,                                                       /*CR1 register*/
  SPI_CR2_DS_2 | SPI_CR2_DS_1 | SPI_CR2_DS_0,              /*CR2 register*/
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
    spiSend(&SPID2, 8, frame_buffer[int_deepth][plane]);                       \
}



/**
 * @brief   Latch out the previous data
 */
#define latch_out() {                                                          \
  spiUnselect(&SPID2);                                                         \
  spiSelect(&SPID2);                                                           \
}

/**
 * @brief   Refresh a plane using PWM
 */
#define plane_pwm(plane){                                                      \
  palWritePort(GPIOB, (palReadLatch(GPIOB) & PLANE_MASK_FOR_LATCH)             \
               | (plane << 2));                                                \
  spiStart(&SPID2, &spiconf_max);                                              \
  plane_send(0, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID2, &spiconf_max);                                              \
  plane_send(1, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID2, &spiconf_fast);                                             \
  plane_send(2, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID2, &spiconf_mid_fast);                                         \
  plane_send(3, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID2, &spiconf_mid_2);                                            \
  plane_send(4, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID2, &spiconf_mid_1);                                            \
  plane_send(5, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID2, &spiconf_mid_slow);                                         \
  plane_send(6, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID2, &spiconf_slow);                                             \
  plane_send(7, plane);                                                        \
  latch_out();                                                                 \
                                                                               \
  spiStart(&SPID2, &spiconf_slowest);                                          \
  spiSend(&SPID2, 8, empty_buffer);                                            \
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
