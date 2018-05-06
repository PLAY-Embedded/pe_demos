/*
 *   S3DL - controller_lld.c
 *   Written by Rocco Marco Guglielmi
 */

#include <ports/SPC560D/CONTROLLER/controller_lld.h>

#if S3DL_USE_CONTROLLER || defined(__DOXYGEN__)

/**
 * @brief   Init all the enabled controller
 */
void controller_init_lld(void){
#if CONTROLLER_USE_CHIBIOS_SHELL || defined(__DOXYGEN__)
s3dl_shell_init();
#endif /* CONTROLLER_USE_CHIBIOS_SHELL */
}

#endif /* S3DL_USE_CONTROLLER */
