/*
 *   S3DL - geometry.h
 *   Written by Rocco Marco Guglielmi
 */

#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

#include "../../userlib/include/opt_math.h"


typedef uint32_t                 color_t;

typedef enum plane {
  XY = 0,
  YZ = 1,
  ZX = 2
} plane_t;

typedef struct{
  uint8_t x_position;
  uint8_t y_position;
  uint8_t z_position;
  color_t color;
} point_t;

typedef struct{
  /*The color is fixed by first point*/
  point_t point_1;
  point_t point_2;
} line_t;


typedef struct{
  /*The color is fixed by first point*/
  decimal_t center_x;
  decimal_t center_y;
  decimal_t center_z;
  decimal_t radius;
  bool IS_FILLED;

} sphere_t;


/**********************  Bit definition for color   ***************************/
#define c_I                                  ((uint32_t)0xFF000000)                          /*!< Monocromatic intensity            */
#define c_R                                  ((uint32_t)0x00FF0000)                          /*!< Red intensity                     */
#define c_G                                  ((uint32_t)0x0000FF00)                          /*!< Green intensity                   */
#define c_B                                  ((uint32_t)0x000000FF)                          /*!< Blue intensity                    */

#define GET_INTENSITY(col)                   (((uint32_t)col & c_I) >> 24)                  /*!< Get monocromatic intensity        */
#define GET_RED(col)                         (((uint32_t)col & c_R) >> 16)                  /*!< Get monocromatic intensity        */
#define GET_GREEN(col)                       (((uint32_t)col & c_G) >>  8)                  /*!< Get monocromatic intensity        */
#define GET_BLUE(col)                        (((uint32_t)col & c_B)      )                  /*!< Get monocromatic intensity        */

#define BUILD_COLOR(RED, GREEN, BLUE, INT)   ((INT << 24) | (RED << 16) | (GREEN << 8) | (BLUE))     /*!< Build a color_t by its components */


/**
 * @brief   Build point with coordinates and color.
 * @param[in] point      pointer to the point
 * @param[in] x          x position
 * @param[in] y          y position
 * @param[in] z          z position
 * @param[in] c          color
 */

#define BUILD_POINT(point, x, y, z, c) {                                \
  (point)->x_position = x;                                              \
  (point)->y_position = y;                                              \
  (point)->z_position = z;                                              \
  (point)->color = c;                                                   \
}

/**
 * @brief   Copy point2 in point1
 * @param[in] point1     pointer to the destination point
 * @param[in] point2     pointer to the source point
 */

#define COPY_POINT(point1, point2) {                                   \
  (point1)->x_position = (point2)->x_position;                         \
  (point1)->y_position = (point2)->y_position;                         \
  (point1)->z_position = (point2)->z_position;                         \
  (point1)->color = (point2)->color;                                   \
}

/**
 * @brief   Build point with coordinates and color.
 * @param[in] point      pointer to the point
 * @param[in] x          x position
 * @param[in] y          y position
 * @param[in] z          z position
 * @param[in] c          color
 */

#define BUILD_SPHERE(sphere, x, y, z, r, fil) {                      \
  (sphere)->center_x = x;                                            \
  (sphere)->center_y = y;                                            \
  (sphere)->center_z = z;                                            \
  (sphere)->radius = r;                                              \
  (sphere)->IS_FILLED = fil;                                         \
}
#endif /* _GEOMETRY_H_ */
