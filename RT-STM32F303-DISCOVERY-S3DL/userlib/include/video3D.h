/*
 *   S3DL - video3D.h
 *   Written by Rocco Marco Guglielmi
 */

#ifndef _VIDEO3D_H_
#define _VIDEO3D_H_

#include <ports/STM32F3/video3d_lld.h>
#include "games_handler.h"
#include "s3dlconf.h"

#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)
/**
 * @brief   Configures pad in order to use 3dcube
 * @details This function is called by s3dlInit()
 */
#define  video3dInit() {                                               \
  s3dl_games_handler_init();                                           \
   video3d_init_lld();                                                 \
   s3dl_draw_init();                                                   \
   s3dl_do_a_game_init();                                              \
}

/**
 * @brief   Sent out data using SPI
 */
#define s3dl_refresh_display3D() {                                     \
  refresh_display3D_lld();                                             \
}



#endif /* S3DL_USE_VIDEO3D */
#endif /* _VIDEO3D_H_ */
