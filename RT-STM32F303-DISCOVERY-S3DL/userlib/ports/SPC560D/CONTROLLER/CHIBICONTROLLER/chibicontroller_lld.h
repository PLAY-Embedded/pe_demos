/*
 *   S3DL - shell_lld.h
 *   Written by Rocco Marco Guglielmi
 */
#include <ports/SPC560D/CONTROLLER/serial_driver_config.h>
#include "halconf.h"
#include "chibios_shell.h"
#include "chibios_chprintf.h"

#ifndef _SHELL_LLD_H_
#define _SHELL_LLD_H_

#if CONTROLLER_USE_CHIBIOS_SHELL || defined(__DOXYGEN__)


#if (SHELL_USE_SD) && ((!HAL_USE_SERIAL) || (!SPC5_SERIAL_USE_LINFLEX0))
#error "Shell error!"
#error "SHELL_USE_SD requires HAL_USE_SERIAL and SPC5_SERIAL_USE_LINFLEX0"
#endif

#if (SHELL_USE_SOU)
#error "Shell error!"
#error "SPC560D doesn't use Serial Over USB"
#endif

#define SHELL_WA_SIZE   THD_WA_SIZE(2048)
extern Thread *shelltp;
extern const chShellCommand commands[];
extern BaseSequentialStream* chp;
static const chShellConfig shell_cfg = {
  #if SHELL_USE_SD || defined(__DOXYGEN__)
  (BaseSequentialStream *) &SD1,
  #endif /* SHELL_USE_SD */
  commands
};


#ifdef __cplusplus
extern "C" {
#endif
void s3dl_shell_init(void);
void s3dl_shell_spawner_lld(void);
#ifdef __cplusplus
}
#endif
#endif /* CONTROLLER_USE_CHIBIOS_SHELL */
#endif /* _SHELL_LLD_H_ */
/** @} */
