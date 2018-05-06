/*
 *   S3DL - s3dl.h
 *   Written by Rocco Marco Guglielmi
 */

/**
 * @file    s3dl.c
 * @brief   S3DL subsystem code.
 *
 * @addtogroup S3DL
 * @{
 */

#include "s3dl.h"
/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/
/**
 * @brief   S3DL initialization.
 * @details This function invokes the low level initialization code then
 *          initializes all the subsystems enabled in the S3DL.
 *
 * @init
 */
void s3dlInit(void) {
  #if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)
  video3dInit();
  #endif
  #if S3DL_USE_VIDEO2D || defined(__DOXYGEN__)
  video2dInit();
  #endif
}

/** @} */
