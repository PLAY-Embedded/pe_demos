/*
 *   S3DL - games.h
 *   Written by Rocco Marco Guglielmi
 */

#ifndef _GAMES_H_
#define _GAMES_H_

#include "string.h"
#include "draw.h"

#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)
#define MAX_STRING_LEN      64
#define GAME_NAME_LEN       16
extern char ST_0[MAX_STRING_LEN];
extern char ST_1[MAX_STRING_LEN];
extern char ST_2[MAX_STRING_LEN];
extern char ST_3[MAX_STRING_LEN];

extern char game_name[GAME_NAME_LEN];
extern bool FLAG;

/**
 * @brief   Enumeration of available games
 */
typedef enum chibicube_games{
  NONE = 0,
  RAINFALL = 1,
  SINUSOID = 2,
  SPIN = 3,
  SPIRAL = 4,
  WAVE = 5,
  SPHEREPULSE = 6,
  WRITE = 7,
} games_t;

/**
 * @brief   Total number of games
 */
#define NUMB_OF_GAMES  7

/**
 * @brief   Enumeration of available write mode
 */
typedef enum string_mode{
  DRAG_AXIALLY = 0,
  SCROLL_PLANARLY = 1,
  SURROUND_DISPLAY = 2,
} write_mode_t;

/**
 * @brief   Total number of write mode
 */
#define NUMB_OF_WRITE_MODE  3

/**
 * @brief   Enumeration of available directions
 */
typedef enum direction{
  BACKWARD = 0,
  FORWARD = 1,
} direction_t;

/**
 * @brief   Structure of the Rainfall game
 */
typedef struct{
  uint16_t drop_numb;
  uint8_t speed_fall;
  uint32_t lifetime;
} rainfall_t;

/**
 * @brief   Structure of the Sinusoid game
 */
typedef struct{
  int16_t theta;
  uint8_t speed;
  uint32_t lifetime;
} sinusoid_t;

/**
 * @brief   Structure of the Spin game
 */
typedef struct{
  int16_t theta;
  uint8_t rotation_speed;
  uint32_t lifetime;
} spin_t;

/**
 * @brief   Structure of the Spiral game
 */
typedef struct{
  int16_t theta;
  int16_t phi;
  uint8_t speed_x;
  uint8_t speed_y;
  uint32_t lifetime;
} spiral_t;

/**
 * @brief   Structure of the Wave game
 */
typedef struct{
  int16_t theta;
  int16_t phi;
  uint8_t speed_x;
  uint8_t speed_y;
  uint32_t lifetime;
} wave_t;

/**
 * @brief   Structure of the Write game
 */
typedef struct{
  write_mode_t mode;
  plane_t plane;
  uint8_t index;
  direction_t direction;
  uint8_t speed;
  uint32_t lifetime;
  bool loop;
} write_t;

/**
 * @brief   Structure of the Sphere Pulse game
 */
typedef struct{
  uint8_t speed;
  uint32_t lifetime;
} sphere_pulse_t;

/**
 * @brief   Structure of the Trail game
 */
typedef struct{
  int16_t theta;
  uint8_t speed;
  uint32_t lifetime;
} trail_t;
/**
 * @brief   Build rainfall_t by different parameters.
 * @param[in] rf         pointer to the rainfall
 * @param[in] d          number of drop
 * @param[in] s          speed fall
 * @param[in] l          duration of animation
 */
#define BUILD_RAINFALL(rf, d, s, l){                                           \
  (rf)->drop_numb = d;                                                         \
  (rf)->speed_fall = s;                                                        \
  (rf)->lifetime = l;                                                          \
}

/**
 * @brief   Build sinusoid_t by different parameters.
 * @param[in] sins       pointer to the sinusoid
 * @param[in] t          initial angle
 * @param[in] s          speed of animation
 * @param[in] l          duration of animation
 */
#define BUILD_SINUSOID(sins, t, s, l){                                         \
  (sins)->theta = t;                                                           \
  (sins)->speed = s;                                                           \
  (sins)->lifetime = l;                                                        \
}

/**
 * @brief   Build spin_t by different parameters.
 * @param[in] sp         pointer to the spin
 * @param[in] t          initial angle
 * @param[in] rs         rotation speed
 * @param[in] l          duration of animation
 */
#define BUILD_SPIN(sp, t, rs, l){                                              \
  (sp)->theta = t;                                                             \
  (sp)->rotation_speed = rs;                                                   \
  (sp)->lifetime = l;                                                          \
}

/**
 * @brief   Build spiral_t by different parameters.
 * @param[in] spir       pointer to the spiral
 * @param[in] th         initial x angle
 * @param[in] ph        initial y angle
 * @param[in] sx         speed along x
 * @param[in] sy         speed along y
 * @param[in] l          duration of animation
 */
#define BUILD_SPIRAL(spir, th, ph, sx , sy, l){                                \
  (spir)->theta = th;                                                          \
  (spir)->phi = ph;                                                            \
  (spir)->speed_x = sx;                                                        \
  (spir)->speed_y = sy;                                                        \
  (spir)->lifetime = l;                                                        \
}

/**
 * @brief   Build wave_t by different parameters.
 * @param[in] wv         pointer to the wave
 * @param[in] th         initial x angle
 * @param[in] phi        initial y angle
 * @param[in] sx         speed along x
 * @param[in] sy         speed along y
 * @param[in] l          duration of animation
 */
#define BUILD_WAVE(wv, th, ph, sx , sy, l){                                    \
  (wv)->theta = th;                                                            \
  (wv)->phi = ph;                                                              \
  (wv)->speed_x = sx;                                                          \
  (wv)->speed_y = sy;                                                          \
  (wv)->lifetime = l;                                                          \
}

/**
 * @brief   Build write_t by different parameters.
 * @param[in] wr         pointer to the write
 * @param[in] md         mode of animation
 * @param[in] pl         plane on which to perform the animation
 * @param[in] in         index for SCROLL_PLANARLY mode
 * @param[in] dir        directon of animation
 * @param[in] sp         speed of animation
 * @param[in] l          duration of animation
 */
#define BUILD_WRITE(wr, md, pl, in, dir, sp, l, lp){                           \
    (wr)->mode = md;                                                           \
    (wr)->plane = pl;                                                          \
    (wr)->index = in;                                                          \
    (wr)->direction = dir;                                                     \
    (wr)->speed = sp;                                                          \
    (wr)->lifetime = l;                                                        \
    (wr)->loop = lp;                                                           \
}

/**
 * @brief   Build sphere_pulse_t by different parameters.
 * @param[in] sp_pu      pointer to the write
 * @param[in] sp         speed of animation
 * @param[in] l          duration of animation
 */
#define BUILD_SPHERE_PULSE(sp_pu, sp, l){                                      \
    (sp_pu)->speed = sp;                                                       \
    (sp_pu)->lifetime = l;                                                     \
}

/**
 * @brief   Build trail_t by different parameters.
 * @param[in] tr       pointer to the trail
 * @param[in] t          initial angle
 * @param[in] s          speed of animation
 * @param[in] l          duration of animation
 */
#define BUILD_TRAIL(tr, t, s, l){                                              \
  (tr)->theta = t;                                                             \
  (tr)->speed = s;                                                             \
  (tr)->lifetime = l;                                                          \
}
#ifdef __cplusplus
extern "C" {
#endif
  void s3dl_do_a_game_init(void);
  void s3dl_do_test(void);
  void s3dl_do_rainfall(rainfall_t *rf);
  void s3dl_do_sinusoid(sinusoid_t*sns);
  void s3dl_do_spin(spin_t *sp);
  void s3dl_do_spiral(spiral_t *spir);
  void s3dl_do_wave(wave_t *wv);
  void s3dl_do_write(write_t *wr, char string[MAX_STRING_LEN]);
  void s3dl_do_sphere_pulse(sphere_pulse_t *sp_pu);
  void s3dl_do_trail(trail_t*tr);
#ifdef __cplusplus
}
#endif
#endif /* S3DL_USE_VIDEO3D */
#endif /* _GAMES_H_ */

