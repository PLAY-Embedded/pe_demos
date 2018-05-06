/*
 *   S3DL - games_handler.h
 *   Written by Rocco Marco Guglielmi
 */

#ifndef _GAMES_HANDLER_H_
#define _GAMES_HANDLER_H_

#include <stdarg.h>

#include "games.h"

#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)

/**
 * @brief   Establishes the number of games before a write
 */
#if !defined(GAMES_BEFORE_A_WRITE) || defined(__DOXYGEN__)
#define GAMES_BEFORE_A_WRITE       2
#endif

#define MAX_SENTENCES      10
#define DEF_SENTENCES      6
#define MAX_GAMES_NUMB     50
#define DEF_GAMES          6
/**
 * @brief   Modalities of play game of 3D cube command
 */
typedef enum cube_mode{
  CUBE_DEFAULT_MODE = 0,
  CUBE_SINGLEGAME_MODE = 1,
  CUBE_RANDOM_MODE = 2,
  CUBE_GAMESLIST_MODE = 3,
} cube_mode_t;



extern cube_mode_t GAME_HANDLER_MODE;
extern bool CUBE_IS_ON;
extern char sentence[MAX_SENTENCES][MAX_STRING_LEN];
extern uint8_t NUMB_OF_SENTENCE;
extern games_t gameslist[MAX_GAMES_NUMB];
extern uint8_t GAMES_NUMB;
extern games_t SINGLEGAME_SELECTION;

extern sphere_pulse_t spu_single;
extern sinusoid_t sns_single;
extern spiral_t spir_single;
extern wave_t wv_single;
extern rainfall_t rf_single;
extern spin_t sp_single;
extern write_t write_single;
extern char string_sm [MAX_STRING_LEN];

#ifdef __cplusplus
extern "C" {
#endif

void s3dl_games_handler_init(void);
void s3dl_games_handler(void);
void s3dl_sentences_init(uint8_t number_of_sentences, const char *fmt, ...);
void s3dl_gameslist_init(uint8_t number_of_games, games_t games, ...);
#ifdef __cplusplus
}
#endif
#endif /* S3DL_USE_VIDEO3D */
#endif /* _GAMES_HANDLER_H_ */
