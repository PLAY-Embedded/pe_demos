/*
 *   S3DL - draw_lld.h
 *   Written by Rocco Marco Guglielmi
 */

#ifndef _DRAW_LLD_H_
#define _DRAW_LLD_H_

#include "video3d_lld.h"
#include "bitfields.h"
#include "geometry.h"

#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)
#ifdef __cplusplus
extern "C" {
#endif
  void draw_point_lld(point_t p);
  void get_point_lld(point_t* p, uint8_t x, uint8_t y, uint8_t z);
#ifdef __cplusplus
}
#endif

#endif /* S3DL_USE_VIDEO3D */
#endif /* _DRAW_LLD_H_ */
