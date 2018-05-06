/*
 *   S3DL - draw.h
 *   Written by Rocco Marco Guglielmi
 */

#ifndef _DRAW_H_
#define _DRAW_H_



#include <ports/STM32F3/draw_lld.h>
#include "alphabet.h"
#include "opt_math.h"
#include "stdlib.h"


#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)

#if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)

/**
 * @brief   This variable is used to set how color is generated
 */
extern uint8_t COLOR_MODE;

/**
 * @brief   Color fluctuates over time
 */
#define        COLOR_DEFAULT_MODE              0

/**
 * @brief   Color is generated random point by point
 */
#define        COLOR_RANDOM_MODE               1

/**
 * @brief   Color is generated according to position. The center is more bright
 *
 * @details It is possible radially translating the scale of brightness
 *          increasing the variable ENHANCE_CENTER_INDEX
 */
#define        COLOR_ENHANCE_CENTER_MODE       2

/**
 * @brief   Color change increasing with x position
 *
 * @details It is possible translating the scale of brightness decreasing the
 *          variable INCREASE_DECREASE_INDEX
 */
#define        COLOR_INCREASE_WITH_X_MODE      3

/**
 * @brief   Color change decreasing with x position
 *
 * @details It is possible translating the scale of brightness decreasing the
 *          variable INCREASE_DECREASE_INDEX
 */
#define        COLOR_DECREASE_WITH_X_MODE      4

/**
 * @brief   Color change increasing with y position
 *
 * @details It is possible translating the scale of brightness decreasing the
 *          variable INCREASE_DECREASE_INDEX
 */
#define        COLOR_INCREASE_WITH_Y_MODE      5

/**
 * @brief   Color change decreasing with y position
 *
 * @details It is possible translating the scale of brightness decreasing the
 *          variable INCREASE_DECREASE_INDEX
 */
#define        COLOR_DECREASE_WITH_Y_MODE      6

/**
 * @brief   Color change increasing with z position
 *
 * @details It is possible translating the scale of brightness decreasing the
 *          variable INCREASE_DECREASE_INDEX
 */
#define        COLOR_INCREASE_WITH_Z_MODE      7

/**
 * @brief   Color change decreasing with z position
 *
 * @details It is possible translating the scale of brightness decreasing the
 *          variable INCREASE_DECREASE_INDEX
 */
#define        COLOR_DECREASE_WITH_Z_MODE      8

/**
 * @brief   Color doesn't change
 */
#define        COLOR_CONSTANT_MODE             9


/**
 * @brief   Set COLOR_MODE as n
 *
 */
#define SET_COLOR_MODE(n){                                                     \
COLOR_MODE = n;                                                                \
}

/**
 * @brief   This variable can be used to change the threshold of brightness in
 *          COLOR_ENHANCE_CENTER_MODE
 *
 * @details Default value is 1,
 *          in order to see the effects of this variable must be set COLOR_MODE
 *          as COLOR_ENHANCE_CENTER_MODE
 */
extern uint8_t ENHANCE_CENTER_INDEX;

/**
 * @brief   This variable can be used to change the threshold of brightness in
 *          COLOR_DECREASE/INCREASE_WITH_X/Y/Z_MODE
 *
 * @details Default value is 254
 */
extern uint8_t INCREASE_DECREASE_INDEX;

/**
 * @brief   This color is used in COLOR_CONSTANT_MODE
 *
 * @details Default value is 254
 */
extern color_t CONST_COLOR;

#endif /*DISPLAY3D_USE_PWM*/

#if S3DL_USE_FLOAT || defined(__DOXYGEN__)
extern float center_x, center_y, center_z;
#else
extern decimal_t center_x, center_y, center_z;
#endif /* S3DL_USE_FLOAT */
/**
 * @brief   Draws a point on the write buffer
 *
 * @param[in] p     point to print
 *
 */
#define s3dl_draw_point(p) {                                                   \
  draw_point_lld(p);                                                           \
}


/**
 * @brief   Retrieves information about the color of a point of
 *          which are known the coordinates
 *
 * @param[out] p     pointer to a point of which are known the coordinates
 *
 */
#define s3dl_get_point(p, x, y, z) {                                           \
  get_point_lld(p, x, y, z);                                                   \
}

#ifdef __cplusplus
extern "C" {
#endif
void s3dl_draw_init(void);
void s3dl_draw_clean_plane(plane_t planes, uint8_t index);
void s3dl_draw_set_plane(plane_t planes, uint8_t index);
void s3dl_draw_clean_display3D(void);
void s3dl_draw_sphere(sphere_t figure);
void s3dl_draw_ascii(char character, int16_t center_plane_1,
                     int16_t center_plane_2, uint8_t plane_index, plane_t planes,
                     bool is_reflected, bool is_upsidedown);
void s3dl_draw_rainfall(uint16_t drop_numb);
void s3dl_draw_sinusoid(int16_t theta);
void s3dl_draw_spin(int16_t theta, int32_t distortion);
void s3dl_draw_spiral(int16_t theta, int16_t phi);
void s3dl_draw_wave(int16_t theta, int16_t phi);
void s3dl_draw_trail(int16_t theta);
#ifdef __cplusplus
}
#endif

#endif /* S3DL_USE_VIDEO3D */
#endif /* DRAW_H_ */
