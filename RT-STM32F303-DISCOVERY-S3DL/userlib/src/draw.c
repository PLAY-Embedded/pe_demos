/*
 *   S3DL - draw.c
 *   Written by Rocco Marco Guglielmi
 */

#include "draw.h"


#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)

#if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
uint8_t COLOR_MODE = COLOR_DEFAULT_MODE;
uint8_t ENHANCE_CENTER_INDEX = 1;
uint8_t INCREASE_DECREASE_INDEX = 254;
color_t CONST_COLOR = 0xFF000000;
#else
static color_t on = 0xFF000000;
#endif /* DISPLAY3D_USE_PWM */

static decimal_t dc_map_x, dc_map_y, dc_map_z, dc_x, dc_y, dc_z;
decimal_t dc_center_x, dc_center_y, dc_center_z;

#define dist_divider       10000
static point_t dpnt;
static uint8_t x_ind, y_ind, z_ind, X, Y, Z;
static int32_t temp_1;
static color_t off = 0;
/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/


/**
 * @brief   shift all plane along z
 *
 */
static void fall(void){
  point_t p1, p2;
  for(z_ind = DISPLAY3D_Z_START + 1; z_ind <= DISPLAY3D_Z_END; z_ind++){
    for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
      for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
        BUILD_POINT(&p2, x_ind, y_ind, z_ind, 0);
        s3dl_get_point(&p1, x_ind, y_ind, z_ind);
        BUILD_POINT(&p1, p1.x_position, p1.y_position, p1.z_position - 1, p1.color);
        s3dl_draw_point(p1);
        s3dl_draw_point(p2);
      }
    }
  }
}

#if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)

/**
 * @brief    Return a color according to generator mode
 *
 * @param[in] curr_x           current x position
 * @param[in] curr_y           current y position
 * @param[in] curr_z           current z position
 * @return   Color
 */
static color_t color_generator(uint8_t curr_x, uint8_t curr_y, uint8_t curr_z){
  color_t tmp;
  uint8_t i;
  decimal_t dc_tmp_1;
  uint32_t tmpx, tmpy, tmpz;
  uint32_t cubexdim, cubeydim, cubezdim;

  cubexdim = DISPLAY3D_X_END - DISPLAY3D_X_START + 1;
  cubeydim = DISPLAY3D_Y_END - DISPLAY3D_Y_START + 1;
  cubezdim = DISPLAY3D_Z_END - DISPLAY3D_Z_START + 1;
  switch(COLOR_MODE){
  default:
  case (COLOR_DEFAULT_MODE):
    (void) curr_x;
    (void) curr_y;
    (void) curr_z;
    tmp = ((chVTGetSystemTime() / 10) % 510) + 1;
    if(tmp < 256){
      return tmp << 24;
    }
    else{
     return (511 - tmp) << 24;
    }
    break;
  case (COLOR_RANDOM_MODE):
    (void) curr_x;
    (void) curr_y;
    (void) curr_z;
    srand(chVTGetSystemTime());
    dc_tmp_1 = sin_table[rand() % 181];
    return ((DEC2INT((rand() % 255) *  dc_tmp_1) + 1) << 24);
    break;
  case (COLOR_ENHANCE_CENTER_MODE):
    if(curr_x <= (cubexdim / 2)){
      tmpx = curr_x;
    }
    else{
      tmpx = cubexdim - curr_x + 1;
    }
    if(curr_y <= (cubeydim / 2)){
      tmpy = curr_y;
    }
    else{
      tmpy = cubeydim - curr_y + 1;
    }
    if(curr_z <= (cubezdim / 2)){
      tmpz = curr_z;
    }
    else{
      tmpz = cubezdim - curr_z + 1;
    }
    dc_tmp_1 = sine_map(INT2DEC(tmpx + tmpy + tmpz), 1, (cubexdim / 2) + (cubeydim / 2) + (cubezdim / 2), 0, right_angle);
    dc_tmp_1 = sin_table[my_round(dc_tmp_1)];
    if (ENHANCE_CENTER_INDEX < 1){
      ENHANCE_CENTER_INDEX = 1;
    }
    else{
      for(i = 0; i <= ENHANCE_CENTER_INDEX; i++){
        dc_tmp_1 = decimal_product(dc_tmp_1, dc_tmp_1);
      }
    }
    tmp = (255 * DEC2INT(dc_tmp_1));
    return (tmp << 24);
    break;
  case (COLOR_INCREASE_WITH_X_MODE):
    (void) curr_y;
    (void) curr_z;
    tmp = (curr_x * INCREASE_DECREASE_INDEX / cubexdim) + 1;
    return (tmp << 24);
    break;
  case (COLOR_DECREASE_WITH_X_MODE):
   (void) curr_y;
   (void) curr_z;
    tmp = ((cubexdim - (curr_x - 1)) * INCREASE_DECREASE_INDEX / cubexdim) + 1;
    return (tmp << 24);
    break;
  case (COLOR_INCREASE_WITH_Y_MODE):
   (void) curr_x;
   (void) curr_z;
   tmp = (curr_y * INCREASE_DECREASE_INDEX / cubeydim) + 1;
   return (tmp << 24);
    break;
  case (COLOR_DECREASE_WITH_Y_MODE):
    (void) curr_x;
    (void) curr_z;
    tmp = ((cubeydim - (curr_y - 1)) * INCREASE_DECREASE_INDEX / cubeydim) + 1;
    return (tmp << 24);
    break;
  case (COLOR_INCREASE_WITH_Z_MODE):
    (void) curr_x;
    (void) curr_y;
    tmp = (curr_z * INCREASE_DECREASE_INDEX / cubezdim) + 1;
    return (tmp << 24);
    break;
  case (COLOR_DECREASE_WITH_Z_MODE):
    (void) curr_x;
    (void) curr_y;
    tmp = ((cubezdim - (curr_z - 1)) * INCREASE_DECREASE_INDEX / cubezdim) + 1;
    return (tmp << 24);
    break;
  case (COLOR_CONSTANT_MODE):
    return (CONST_COLOR);
    break;
  }
}
#endif /* DISPLAY3D_USE_PWM */
/*===========================================================================*/
/* Exported functions.                                                       */
/*===========================================================================*/

/**
 * @brief   Calculates the value of some common variable like center of cube
 */
void s3dl_draw_init(void){
dc_center_x = INT2DEC(DISPLAY3D_X_END + DISPLAY3D_X_START) / 2;
dc_center_y = INT2DEC(DISPLAY3D_Z_END + DISPLAY3D_Z_START) / 2;
dc_center_z = INT2DEC(DISPLAY3D_Z_END + DISPLAY3D_Z_START) / 2;

}
/**
 * @brief   Turn off a plane XY or YZ or ZX of the display 3d
 */
void s3dl_draw_clean_plane(plane_t planes, uint8_t index){
  switch (planes){
  default:
  case(XY):
    if(!(index < DISPLAY3D_Z_START) && !(index > DISPLAY3D_Z_END)){
      z_ind = index;
      for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
        for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, 0);
          s3dl_draw_point (dpnt);
        }
      }
    }
    break;
  case(YZ):
    if(!(index < DISPLAY3D_X_START) && !(index > DISPLAY3D_X_END)){
      x_ind = index;
      for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
        for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, 0);
          s3dl_draw_point (dpnt);
        }
      }
    }
    break;
  case(ZX):
    if(!(index < DISPLAY3D_Y_START) && !(index > DISPLAY3D_Y_END)){
      y_ind = index;
      for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
        for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, 0);
          s3dl_draw_point (dpnt);
        }
      }
    }
    break;
  }
}

/**
 * @brief   Turn on a plane XY or YZ or ZX of the display 3d
 */
void s3dl_draw_set_plane(plane_t planes, uint8_t index){
  switch (planes){
  default:
  case(XY):
    if(!(index < DISPLAY3D_Z_START) && !(index > DISPLAY3D_Z_END)){
      z_ind = index;
      for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
        for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
          #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
          #else
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, on);
          #endif
          s3dl_draw_point(dpnt);
        }
      }
    }
    break;
  case(YZ):
    if(!(index < DISPLAY3D_X_START) && !(index > DISPLAY3D_X_END)){
      x_ind = index;
      for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
        for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
          #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
          #else
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, on);
          #endif
          s3dl_draw_point(dpnt);
        }
      }
    }
    break;
  case(ZX):
    if(!(index < DISPLAY3D_Y_START) && !(index > DISPLAY3D_Y_END)){
      y_ind = index;
      for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
        for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
          #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
          #else
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, on);
          #endif
          s3dl_draw_point(dpnt);
        }
      }
    }
    break;
  }
}

/**
 * @brief   Turn off all the display 3d
 */
void s3dl_draw_clean_display3D(void){
  point_t dpnt;
  for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
    for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
      for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
        BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, 0);
        s3dl_draw_point(dpnt);
      }
    }
  }
}

/**
 * @brief   Prints a Globe according to parameters radius, center and filling
 *
 * @param[in] figure        globe to draw
 */
void s3dl_draw_sphere(sphere_t figure){
  decimal_t ext_radius_squared = decimal_product(figure.radius, figure.radius);
  decimal_t int_radius_squared = decimal_product(figure.radius - 10000, figure.radius - 10000);
  for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
    dc_x = decimal_product(INT2DEC(x_ind) - figure.center_x, INT2DEC(x_ind) - figure.center_x);
    for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
      dc_y = decimal_product(INT2DEC(y_ind) - figure.center_y, INT2DEC(y_ind) - figure.center_y);
      for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
        dc_z = decimal_product(INT2DEC(z_ind) - figure.center_z, INT2DEC(z_ind) - figure.center_z);
        if(((figure.IS_FILLED) && (dc_x + dc_y + dc_z <= ext_radius_squared)) ||
            ((!figure.IS_FILLED) && (dc_x + dc_y + dc_z <= ext_radius_squared) &&
             (dc_x + dc_y + dc_z >= int_radius_squared))){
          #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
          #else
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, on);
          #endif /* DISPLAY3D_USE_PWM */
          s3dl_draw_point(dpnt);
        }
        else{
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, off);
          s3dl_draw_point(dpnt);
        }
      }
    }
  }
}

/**
 * @brief   Prints an ascii on a plane in different ways
 *
 * @param[in] character              ascii table char position
 * @param[in] center_plane_1         first plane's center position
 * @param[in] center_plane_2         second plane's center position
 * @param[in] plane_index            index of plane
 * @param[in] planes                 plane on witch print the chartype
 * @param[in] is_reflected           the char is reflected or not
 * @param[in] is_upsidedown          the char is upsidedown or not
 */
void s3dl_draw_ascii(char character, int16_t center_plane_1,
                     int16_t center_plane_2, uint8_t plane_index, plane_t planes,
                     bool is_reflected, bool is_upsidedown){
  uint8_t x_start_pos, y_start_pos, z_start_pos;
  uint8_t x_offset = (DISPLAY3D_X_END - DISPLAY3D_X_START + 1) / 2,
          y_offset = (DISPLAY3D_Y_END - DISPLAY3D_Y_START + 1) / 2,
          z_offset = (DISPLAY3D_Z_END - DISPLAY3D_Z_START + 1) / 2;
  if(!(character >= 128)){
    switch (planes){
    default:
    case(XY):
      if(!(plane_index < DISPLAY3D_Z_START) && !(plane_index > DISPLAY3D_Z_END)){
        z_ind = plane_index;
        if((!is_reflected) && (!is_upsidedown)){
          x_start_pos = (uint8_t) center_plane_1 + 1 - x_offset;
          y_start_pos = (uint8_t) center_plane_2 + y_offset;
          for(x_ind = 0; x_ind < 8; x_ind++){
            for(y_ind = 0; y_ind < 8; y_ind++){
              if((ascii_8x8_table[character % 128][x_ind]) & (BIT_(y_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_start_pos - y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_start_pos - y_ind, z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_start_pos - y_ind, z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
        else if((is_reflected) && (!is_upsidedown)){
          x_start_pos = (uint8_t) center_plane_1 + x_offset;
          y_start_pos = (uint8_t) center_plane_2 + y_offset;
          for(x_ind = 0; x_ind < 8; x_ind++){
            for(y_ind = 0; y_ind < 8; y_ind++){
              if((ascii_8x8_table[character % 128][x_ind]) & (BIT_(y_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_start_pos - y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_start_pos - y_ind, z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_start_pos - y_ind, z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
        else if((!is_reflected) && (is_upsidedown)){
          x_start_pos = (uint8_t) center_plane_1 + 1 - x_offset;
          y_start_pos = (uint8_t) center_plane_2 + 1 - y_offset;
          for(x_ind = 0; x_ind < 8; x_ind++){
            for(y_ind = 0; y_ind < 8; y_ind++){
              if((ascii_8x8_table[character % 128][x_ind]) & (BIT_(y_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_start_pos + y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_start_pos + y_ind, z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_start_pos + y_ind, z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
        else{
          x_start_pos = (uint8_t) center_plane_1 + x_offset;
          y_start_pos = (uint8_t) center_plane_2 + 1 - y_offset;
          for(x_ind = 0; x_ind < 8; x_ind++){
            for(y_ind = 0; y_ind < 8; y_ind++){
              if((ascii_8x8_table[character % 128][x_ind]) & (BIT_(y_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_start_pos + y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_start_pos + y_ind, z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_start_pos + y_ind, z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
      }
      break;
    case(YZ):
      if(!(plane_index < DISPLAY3D_X_START) && !(plane_index > DISPLAY3D_X_END)){
        x_ind = plane_index;
        if((!is_reflected) && (!is_upsidedown)){
          y_start_pos = (uint8_t) center_plane_1 + y_offset;
          z_start_pos = (uint8_t) center_plane_2 + z_offset;
          for(y_ind = 0; y_ind < 8; y_ind++){
            for(z_ind = 0; z_ind < 8; z_ind++){
              if((ascii_8x8_table[character % 128][z_ind]) & (BIT_(y_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_ind , y_start_pos - y_ind, z_start_pos - z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_ind , y_start_pos - y_ind, z_start_pos - z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_ind , y_start_pos - y_ind, z_start_pos - z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
        else if((is_reflected) && (!is_upsidedown)){
          y_start_pos = (uint8_t) center_plane_1 + 1 - y_offset;
          z_start_pos = (uint8_t) center_plane_2 + z_offset;
          for(y_ind = 0; y_ind < 8; y_ind++){
            for(z_ind = 0; z_ind < 8; z_ind++){
              if((ascii_8x8_table[character % 128][z_ind]) & (BIT_(y_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_ind , y_start_pos + y_ind, z_start_pos - z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_ind , y_start_pos + y_ind, z_start_pos - z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_ind , y_start_pos + y_ind, z_start_pos - z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
        else if((!is_reflected) && (is_upsidedown)){
          y_start_pos = (uint8_t) center_plane_1 + y_offset;
          z_start_pos = (uint8_t) center_plane_2 + 1 - z_offset;
          for(y_ind = 0; y_ind < 8; y_ind++){
            for(z_ind = 0; z_ind < 8; z_ind++){
              if((ascii_8x8_table[character % 128][z_ind]) & (BIT_(y_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_ind , y_start_pos - y_ind, z_start_pos + z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_ind , y_start_pos - y_ind, z_start_pos + z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_ind , y_start_pos - y_ind, z_start_pos + z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
        else{
          y_start_pos = (uint8_t) center_plane_1 + 1 - y_offset;
          z_start_pos = (uint8_t) center_plane_2 + 1 - z_offset;
          for(y_ind = 0; y_ind < 8; y_ind++){
            for(z_ind = 0; z_ind < 8; z_ind++){
              if((ascii_8x8_table[character % 128][z_ind]) & (BIT_(y_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_ind , y_start_pos + y_ind, z_start_pos + z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_ind , y_start_pos + y_ind, z_start_pos + z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_ind , y_start_pos + y_ind, z_start_pos + z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
      }
      break;
    case(ZX):
      if(!(plane_index < DISPLAY3D_Y_START) && !(plane_index > DISPLAY3D_Y_END)){
        y_ind = plane_index;
        if((!is_reflected) && (!is_upsidedown)){
          x_start_pos = (uint8_t) center_plane_2 + 1 - x_offset;
          z_start_pos = (uint8_t) center_plane_1 + z_offset;
          for(x_ind = 0; x_ind < 8; x_ind++){
            for(z_ind = 0; z_ind < 8; z_ind++){
              if((ascii_8x8_table[character % 128][z_ind]) & (BIT_(x_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_ind , z_start_pos - z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_ind , z_start_pos - z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_ind , z_start_pos - z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
        else if((is_reflected) && (!is_upsidedown)){
          x_start_pos = (uint8_t) center_plane_2 + x_offset;
          z_start_pos = (uint8_t) center_plane_1 + z_offset;
          for(x_ind = 0; x_ind < 8; x_ind++){
            for(z_ind = 0; z_ind < 8; z_ind++){
              if((ascii_8x8_table[character % 128][z_ind]) & (BIT_(x_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_ind, z_start_pos - z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_ind, z_start_pos - z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_ind, z_start_pos - z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
        else if((!is_reflected) && (is_upsidedown)){
          x_start_pos = (uint8_t) center_plane_2 + 1 - x_offset;
          z_start_pos = (uint8_t) center_plane_1 + 1 - z_offset;
          for(x_ind = 0; x_ind < 8; x_ind++){
            for(z_ind = 0; z_ind < 8; z_ind++){
              if((ascii_8x8_table[character % 128][z_ind]) & (BIT_(x_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_ind, z_start_pos + z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_ind, z_start_pos + z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
              }
              else{
                BUILD_POINT(&dpnt, x_start_pos + x_ind, y_ind, z_start_pos + z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
        else{
          for(x_ind = 0; x_ind < 8; x_ind++){
            for(z_ind = 0; z_ind < 8; z_ind++){
              x_start_pos = (uint8_t) center_plane_2 + x_offset;
              z_start_pos = (uint8_t) center_plane_1 + 1 - z_offset;
              if((ascii_8x8_table[character % 128][z_ind]) & (BIT_(x_ind))){
                #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_ind, z_start_pos + z_ind, color_generator(x_ind, y_ind, z_ind));
                #else
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_ind, z_start_pos + z_ind, on);
                #endif /*DISPLAY3D_USE_PWM*/
                }
              else{
                BUILD_POINT(&dpnt, x_start_pos - x_ind, y_ind, z_start_pos + z_ind, off);
              }
              s3dl_draw_point(dpnt);
            }
          }
        }
      }
      break;
    }
  }
}

/**
 * @brief   Prints rain drops on the highest plane and makes falling down
 *          the others
 *
 * @param[in] theta_x            phase of wave on x
 * @param[in] theta_y            phase of wave on y
 *
 */
void s3dl_draw_rainfall(uint16_t drop_numb){
  unsigned i;
  uint8_t x,y;
  point_t p;
  srand(chVTGetSystemTime());
  fall();
  if(!drop_numb){
    return;
  }
  for (i = 0; i < drop_numb; i++){
    x = (rand() % (2 * DISPLAY3D_X_END)) + 1;
    y = (rand() % (2 * DISPLAY3D_Y_END)) + 1;
      #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
      BUILD_POINT(&p, x, y, DISPLAY3D_Z_END, color_generator(x_ind, y_ind, z_ind));
      #else
      BUILD_POINT(&p, x, y, DISPLAY3D_Z_END, on);
      #endif /* DISPLAY3D_USE_PWM */
      s3dl_draw_point(p);
  }
}

/**
 * @brief   Prints a 3D sinusoid according to theta
 *
 * @param[in] theta            phase of sinusoid
 *
 */
void s3dl_draw_sinusoid(int16_t theta){
  for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
    for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
      dc_map_x = sine_map(INT2DEC(x_ind), DISPLAY3D_X_START, DISPLAY3D_X_END, -straight_angle, straight_angle);
      dc_map_y = sine_map(INT2DEC(y_ind), DISPLAY3D_Y_START, DISPLAY3D_Y_END, -straight_angle, straight_angle);
      temp_1 = sine_adapt(theta + sqrt2((DEC2INT(dc_map_x) * DEC2INT(dc_map_x) + DEC2INT(dc_map_y) * DEC2INT(dc_map_y))));
      Z = my_round(sine_map(sin_table[temp_1], -1, 1, DISPLAY3D_Z_START, DISPLAY3D_Z_END));
      for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
        if (z_ind == Z){
          #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
          #else
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, on);
          #endif /* DISPLAY3D_USE_PWM */
          s3dl_draw_point (dpnt);
        }
        else{
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, off);
          s3dl_draw_point (dpnt);
        }
      }
    }
  }
}

/**
 * @brief   Prints a plane with a little distortion to the extremes
 *
 * @param[in] theta            phase of the plane
 * @param[in] distortion       distortion of the extremes
 *
 */
void s3dl_draw_spin(int16_t theta, int32_t distortion){
  for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
    for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
      dc_map_y = sine_map(INT2DEC(y_ind) , DISPLAY3D_Y_START, DISPLAY3D_Y_END, theta, theta + (distortion * right_angle / dist_divider));
      dc_x = decimal_product((INT2DEC(x_ind) - dc_center_x), sin_table[sine_adapt(DEC2INT(dc_map_y))]);
      for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
      #if S3DL_USE_FLOAT || defined(__DOXYGEN__)
        fl_z = ((float)z_ind - fl_center_z) * sin_table[sine_adapt(fl_map_y + right_angle)];
        if(abs2(fl_x - fl_z) < 0.7){
      #else
        dc_z = decimal_product((INT2DEC(z_ind) - dc_center_z), sin_table[sine_adapt(DEC2INT(dc_map_y) + right_angle)]);
        if(int_abs2(dc_x - dc_z) < 7000){
      #endif /* S3DL_USE_FLOAT */
          #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
          #else
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, on);
          #endif /* DISPLAY3D_USE_PWM */

          s3dl_draw_point (dpnt);
        }
        else{
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, off);
          s3dl_draw_point (dpnt);
        }
      }
    }
  }
}




/**
 * @brief   Prints a 3D spiral according to parameters theta and phi
 *
 * @param[in] theta            phase of spiral on x
 * @param[in] phi            phase of spiral on y
 *
 */
void s3dl_draw_spiral(int16_t theta, int16_t phi){
  for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
    dc_map_z = sine_map(INT2DEC(z_ind), DISPLAY3D_Z_START, DISPLAY3D_Z_END, -straight_angle + theta, straight_angle + theta);
    dc_x = sin_table[sine_adapt(DEC2INT(dc_map_z))];
    X = my_round(sine_map(dc_x, -1, 1, DISPLAY3D_X_START, DISPLAY3D_X_END));
    dc_y = sin_table[sine_adapt(phi - theta + DEC2INT(dc_map_z) + right_angle)];
    Y = my_round(sine_map(dc_y, -1, 1, DISPLAY3D_Y_START, DISPLAY3D_Y_END));
    for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
      for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
        if (((x_ind == X) || (x_ind == X - 1)) && (y_ind == Y)){
          #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
          #else
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, on);
          #endif /* DISPLAY3D_USE_PWM */
          s3dl_draw_point (dpnt);
        }
        else{
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, off);
          s3dl_draw_point (dpnt);
        }
      }
    }
  }
}


/**
 * @brief   Prints a 3D wave according to parameters theta and phi
 *
 * @param[in] theta            phase of wave on x
 * @param[in] phi              phase of wave on y
 *
 */
void s3dl_draw_wave(int16_t theta, int16_t phi){
  for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
    for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
      dc_map_x = sine_map(INT2DEC(x_ind), DISPLAY3D_X_START, DISPLAY3D_X_END, -straight_angle + theta, straight_angle + theta);
      dc_map_y = sine_map(INT2DEC(y_ind), DISPLAY3D_Y_START, DISPLAY3D_Y_END, -straight_angle + phi, straight_angle + phi);
      dc_z = sin_table[sine_adapt(DEC2INT(dc_map_x))] + sin_table[sine_adapt(DEC2INT(dc_map_y) + right_angle)];
      Z = my_round(sine_map(dc_z, -1, 1, DISPLAY3D_Z_START, DISPLAY3D_Z_END));
      for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
        if (z_ind == Z){
          #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
          #else
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, on);
          #endif /* DISPLAY3D_USE_PWM */
          s3dl_draw_point (dpnt);
        }
        else{
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, off);
          s3dl_draw_point (dpnt);
        }
      }
    }
  }
}

/**
 * @brief   Prints a trail according to theta
 *
 * @param[in] theta            phase of the trail
 *
 */
void s3dl_draw_trail(int16_t theta){
  for(x_ind = DISPLAY3D_X_START; x_ind <= DISPLAY3D_X_END; x_ind++){
    dc_map_x = sine_map(INT2DEC(x_ind), DISPLAY3D_X_START, DISPLAY3D_X_END, -straight_angle, straight_angle);
    for(y_ind = DISPLAY3D_Y_START; y_ind <= DISPLAY3D_Y_END; y_ind++){
      temp_1 = sine_adapt(theta + DEC2INT(dc_map_x));
      Z = my_round(sine_map(sin_table[temp_1], -1, 1, DISPLAY3D_Z_START, DISPLAY3D_Z_END));
      for(z_ind = DISPLAY3D_Z_START; z_ind <= DISPLAY3D_Z_END; z_ind++){
        if (z_ind == Z){
          #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, color_generator(x_ind, y_ind, z_ind));
          #else
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, on);
          #endif /* DISPLAY3D_USE_PWM */
          s3dl_draw_point (dpnt);
        }
        else{
          BUILD_POINT(&dpnt, x_ind, y_ind, z_ind, off);
          s3dl_draw_point (dpnt);
        }
      }
    }
  }
}
#endif /* S3DL_USE_VIDEO3D */
