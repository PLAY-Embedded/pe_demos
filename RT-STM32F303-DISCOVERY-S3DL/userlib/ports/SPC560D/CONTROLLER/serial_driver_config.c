/*
 *   S3DL - serial_driver_config.c
 *   Written by Rocco Marco Guglielmi
 */

#include <ports/SPC560D/CONTROLLER/serial_driver_config.h>

#if SHELL_USE_SD || CHIBICONTROLLER_USE_SD || defined(__DOXYGEN__)

static const SerialConfig serconf = {
  S3DL_SERIAL_BITRATE,
  S3DL_SERIAL_BUFFERS_SIZE
};

/**
 * @brief   Init the serial USB
 */
void serial_init(void){
  sdStart(&SD1, NULL);
}
#endif /* SHELL_USE_SD || CHIBICONTROLLER_USE_SD */
