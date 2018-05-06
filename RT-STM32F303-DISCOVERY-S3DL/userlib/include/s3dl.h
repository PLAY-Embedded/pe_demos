/*
 *   S3DL - s3dl.h
 *   Written by Rocco Marco Guglielmi
 */

/**
 * @file    s3dl.h
 * @brief   S3DL subsystem header.
 *
 * @addtogroup S3DL
 * @{
 */

#ifndef _S3DL_H_
#define _S3DL_H_


#include "s3dlconf.h"
/*
 * Subsystem inclusion
 */


#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)
#include "video3D.h"
#endif

/**
 * @brief   S3DL identification macro.
 */
#define _S3DL_

/**
 * @brief   Library version string.
 */
#define S3DL_LIBRARY_VERSION       "1.2.0"

/**
 * @name    Library version
 * @{
 */
/**
 * @brief   Library version major number.
 */
#define S3DL_MAJOR         1

/**
 * @brief   Library version minor number.
 */
#define S3DL_MINOR         2

/**
 * @brief   Library version patch number.
 */
#define S3DL_PATCH         0

/** @} */

/**
 * @brief   Full library name string.
 */
#define S3DL_FULL_NAME      "S3DL v1.2.0"

#ifdef __cplusplus
extern "C" {
#endif
  void s3dlInit(void);
#ifdef __cplusplus
}
#endif

#endif /* _S3DL_H_ */

/** @} */
