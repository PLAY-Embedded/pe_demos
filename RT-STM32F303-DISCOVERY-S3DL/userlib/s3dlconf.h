/*
 *   S3DL - sd3lconf.h
 *   Written by Rocco Marco Guglielmi
 */

/**
 * @file    s3dlconf.h
 * @brief   S3DL configuration header.
 * @details S3DL configuration file, this file allows to enable or disable the
 *          various subsystems supported by S3DL.
 *
 * @addtogroup S3DL_CONF
 * @{
 */

#ifndef _S3DLCONF_H_
#define _S3DLCONF_H_

#include "ch.h"

/*============================================================================
 *                           SUBSYSTEM SETTINGS
 *============================================================================*/
/**
 * @brief   Enables the 2D Video subsystem.
 */
#if !defined(S3DL_USE_VIDEO2D) || defined(__DOXYGEN__)
#define S3DL_USE_VIDEO2D            FALSE
#endif

/**
 * @brief   Enables the 3D Video subsystem.
 */
#if !defined(S3DL_USE_VIDEO3D) || defined(__DOXYGEN__)
#define S3DL_USE_VIDEO3D            TRUE
#endif

/**
 * @brief   Enables the Controller subsystem.
 */
#if !defined(S3DL_USE_CONTROLLER) || defined(__DOXYGEN__)
#define S3DL_USE_CONTROLLER         TRUE
#endif

/**
 * @brief   Enables the MEMS subsystem.
 */
#if !defined(S3DL_USE_MEMS) || defined(__DOXYGEN__)
#define S3DL_USE_MEMS               FALSE
#endif

/*============================================================================
 *                          3D DISPLAY SETTINGS
 *============================================================================*/
#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)
#define DISPLAY3D_USE_PWM                          TRUE
#define DISPLAY3D_USE_COLOR                        FALSE
#define DISPLAY3D_X_START                          1
#define DISPLAY3D_Y_START                          1
#define DISPLAY3D_Z_START                          1
#define DISPLAY3D_X_END                            8
#define DISPLAY3D_Y_END                            8
#define DISPLAY3D_Z_END                            8

/*
 * Error checks
 */
#if DISPLAY3D_USE_COLOR && !DISPLAY3D_USE_PWM
#error "DISPLAY3D_USE_COLOR requires DISPLAY3D_USE_PWM"
#endif

#if (DISPLAY3D_X_START + 1 >= DISPLAY3D_X_END) || (DISPLAY3D_Y_START + 1 >= DISPLAY3D_Y_END) || (DISPLAY3D_Z_START + 1 >= DISPLAY3D_Z_END)
#error "DISPLAY3D_*_START should be smaller than DISPLAY3D_*_END and their difference must be at least 2"
#endif
#endif /* S3DL_USE_VIDEO3D */

/*============================================================================
 *                          CONTROLLER SETTINGS
 *============================================================================*/
#if S3DL_USE_CONTROLLER || defined(__DOXYGEN__)
/**
 * @brief   Enables the ChibiOS Shell.
 */
#if !defined(CONTROLLER_USE_CHIBIOS_SHELL) || defined(__DOXYGEN__)
#define CONTROLLER_USE_CHIBIOS_SHELL          FALSE
#endif

/**
 * @brief   Enables the communication for ChibiController.
 */
#if !defined(CONTROLLER_USE_CHIBICONTROLLER) || defined(__DOXYGEN__)
#define CONTROLLER_USE_CHIBICONTROLLER          TRUE
#endif

#if CONTROLLER_USE_CHIBIOS_SHELL || defined(__DOXYGEN__)

/**
 * @brief   The shell use serial driver.
 */
#if !defined(SHELL_USE_SD) || defined(__DOXYGEN__)
#define SHELL_USE_SD          FALSE
#endif /* SHELL_USE_SD */

/**
 * @brief   The shell use serial over usb.
 */
#if !defined(SHELL_USE_SOU) || defined(__DOXYGEN__)
#define SHELL_USE_SOU         TRUE
#endif /* SHELL_USE_SOU */
#endif /* CONTROLLER_USE_CHIBIOS_SHELL */

#if CONTROLLER_USE_CHIBICONTROLLER || defined(__DOXYGEN__)

/**
 * @brief   The ChibiController use serial driver.
 */
#if !defined(CHIBICONTROLLER_USE_SD) || defined(__DOXYGEN__)
#define CHIBICONTROLLER_USE_SD          FALSE
#endif /* CHIBICONTROLLER_USE_SD */

/**
 * @brief   The ChibiController use serial over usb.
 */
#if !defined(CHIBICONTROLLER_USE_SOU) || defined(__DOXYGEN__)
#define CHIBICONTROLLER_USE_SOU         TRUE
#endif /* CHIBICONTROLLER_USE_SOU */
#endif /* CONTROLLER_USE_CHIBICONTROLLER */
#endif /* S3DL_USE_CONTROLLER */
/*============================================================================
 *                          MEMS SETTINGS
 *============================================================================*/
#if S3DL_USE_MEMS || defined(__DOXYGEN__)
/**
 * @brief   Enables the accelerometer.
 */
#if !defined(MEMS_USE_ACCELEROMETER) || defined(__DOXYGEN__)
#define MEMS_USE_ACCELEROMETER         FALSE
#endif

/**
 * @brief   Enables the gyroscope.
 */
#if !defined(MEMS_USE_GYROSCOPE) || defined(__DOXYGEN__)
#define MEMS_USE_GYROSCOPE             FALSE
#endif

/**
 * @brief   Enables the magnetometer.
 */
#if !defined(MEMS_USE_MAGNETOMETER) || defined(__DOXYGEN__)
#define MEMS_USE_MAGNETOMETER          FALSE
#endif
#endif /* S3DL_USE_MEMS */

#endif /* _S3DLCONF_H_ */

/** @} */
