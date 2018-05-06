/*
 *   S3DL - draw_lld.c
 *   Written by Rocco Marco Guglielmi
 */

#include "draw_lld.h"

#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)
#define START_POS          1
/**
 * @brief   Draws a point on the write buffer
 *
 * @param[in] p     point to print
 *
 */
void draw_point_lld(point_t p){
  if (!(p.x_position < DISPLAY3D_X_START || p.x_position > DISPLAY3D_X_END)
      && !(p.y_position < DISPLAY3D_Y_START || p.y_position > DISPLAY3D_Y_END)
      && !(p.z_position < DISPLAY3D_Z_START || p.z_position > DISPLAY3D_Z_END)){
    if(!(p.x_position < START_POS || p.x_position > X_DIM)
        && !(p.y_position < START_POS || p.y_position > Y_DIM)
        && !(p.z_position < START_POS || p.z_position > Z_DIM)){
      #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
      unsigned i;
      unsigned intensity = GET_INTENSITY (p.color);
      for (i = 0; i < MAX_INT_DEPTH; i++){
        if(intensity  & BIT_(i)){
          BIT_SET (frame_buffer[i][p.z_position - 1][p.x_position - 1], BIT_(p.y_position - 1));
        }
        else{
          BIT_CLEAR (frame_buffer[i][p.z_position - 1][p.x_position - 1], BIT_(p.y_position - 1));
        }
      }
      #else
      if(p.color != 0){
        BIT_SET (frame_buffer[p.z_position - 1][p.x_position - 1], BIT_(p.y_position - 1));
      }
      else{
        BIT_CLEAR (frame_buffer[p.z_position - 1][p.x_position - 1], BIT_(p.y_position - 1));
      }
    #endif /* DISPLAY3D_USE_PWM */
    }
  }
  else{
    return;
  }
}

/**
 * @brief   Retrieves information about the color of a point of
 *          which are known the coordinates
 *
 * @param[out] p     pointer to a point of which are known the coordinates
 *
 */
void get_point_lld(point_t* p, uint8_t x, uint8_t y, uint8_t z){
  p->x_position = x;
  p->y_position = y;
  p->z_position = z;
  if (!(p->x_position < DISPLAY3D_X_START || p->x_position > DISPLAY3D_X_END)
      && !(p->y_position < DISPLAY3D_Y_START || p->y_position > DISPLAY3D_Y_END)
      && !(p->z_position < DISPLAY3D_Z_START || p->z_position > DISPLAY3D_Z_END)){
    if(!(p->x_position < START_POS || p->x_position > X_DIM)
        && !(p->y_position < START_POS || p->y_position > Y_DIM)
        && !(p->z_position < START_POS || p->z_position > Z_DIM)){
      #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
      uint8_t i, intensity = 0;
      for (i = 0; i < MAX_INT_DEPTH; i++){
        if(BIT_(p->y_position - 1)  &  frame_buffer[i][p->z_position - 1][p->x_position - 1]){
          intensity = intensity | BIT_(i);
        }
      }
      p->color = BUILD_COLOR(0, 0, 0, intensity);
      #else
      if(BIT_(p->y_position - 1)  &  frame_buffer[p->z_position - 1][p->x_position - 1]){
        p->color = 1;
      }
      else{
        p->color = 0;
      }
      #endif /* DISPLAY3D_USE_PWM */
    }
  }
  else{
    return;
  }
}

#endif /* S3DL_USE_VIDEO3D */
