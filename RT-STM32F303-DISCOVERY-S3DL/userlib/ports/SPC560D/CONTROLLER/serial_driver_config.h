/*
 *   S3DL - serial_driver_config.h
 *   Written by Rocco Marco Guglielmi
 */

#include "../../../../userlib/s3dlconf.h"
#include "hal.h"
#include "ch.h"

#ifndef _SERIAL_DRIVER_CONFIG_H_
#define _SERIAL_DRIVER_CONFIG_H_

#if SHELL_USE_SD || CHIBICONTROLLER_USE_SD || defined(__DOXYGEN__)

#define S3DL_SERIAL_BITRATE            115200
#define S3DL_SERIAL_BUFFERS_SIZE           16


#ifdef __cplusplus
extern "C" {
#endif
void serial_init(void);
#ifdef __cplusplus
}
#endif
#endif /* SHELL_USE_SD || CHIBICONTROLLER_USE_SD */
#endif /* _SERIAL_DRIVER_CONFIG_H_ */
/** @} */
