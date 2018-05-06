/*
 *   S3DL - shell_lld.c
 *   Written by Rocco Marco Guglielmi
 */

#include <ports/SPC560D/CONTROLLER/SHELL/shell_lld.h>
#include "chibios_shell.h"

#if CONTROLLER_USE_CHIBIOS_SHELL || defined(__DOXYGEN__)

Thread *shelltp = NULL;

BaseSequentialStream* chp = (BaseSequentialStream*)&SD1;

/**
 * @brief   Configures Serial in order to use ChibiOS shell
 */
void  s3dl_shell_init(void) {
  serial_init();
  chshellInit();
}

/**
 * @brief   Makes the shell respawn on the Serial Driver
 */
void s3dl_shell_spawner_lld(void){
  if (!shelltp){
    /* If shelltp == NULL
     * then create shell thread
     */
    shelltp = chshellCreate(&shell_cfg, SHELL_WA_SIZE, NORMALPRIO - 1);
  }
  else if (chThdTerminated(shelltp)) {
      chThdRelease(shelltp);    /* Recovers memory of the previous shell.   */
      shelltp = NULL;           /* Triggers spawning of a new shell.        */
    }
    chThdSleepMilliseconds(1000);
}


#endif /* CONTROLLER_USE_CHIBIOS_SHELL */
/** @} */
