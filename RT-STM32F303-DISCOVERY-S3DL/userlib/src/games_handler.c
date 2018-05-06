/*
 *   S3DL - games_handler.c
 *   Written by Rocco Marco Guglielmi
 */

#include "games_handler.h"

#include "s3dl.h"

#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)




/**
 * @brief   Mode of the game handler
 *
 * @details This parameter could be modified by controller module
 */
cube_mode_t GAME_HANDLER_MODE;

/**
 * @brief   Status of the cube
 *
 * @details This parameter could be modified by controller module
 */
bool CUBE_IS_ON;

/**
 * @brief   Structure of the sentences used by write game
 *
 * @details This structure could be modified using s3dl_sentences_init()
 */
char sentence[MAX_SENTENCES][MAX_STRING_LEN];

/**
 * @brief   Number of sentences inserted into the structure
 *
 * @details As long this number is 0 a default set of senteces is used
 */
uint8_t NUMB_OF_SENTENCE = 0;

/**
 * @brief   Structure of games used in CUBE_PERSONAL_MODE
 *
 * @details This structure could be modified using s3dl_gamelist_init()
 */
games_t gameslist[MAX_GAMES_NUMB];

/**
 * @brief   Number of games inserted into the structure
 *
 * @details As long this number is 0 cube stay in CUBE_DEFAULT_MODE
 */
uint8_t GAMES_NUMB = 0;

/**
 * @brief   Selection of the singlegame mode
 *
 */
games_t SINGLEGAME_SELECTION = NONE;

/**
 * @brief   Rainfall
 *
 * @details This structure could be modified using s3dl_sentences_init()
 */
rainfall_t rainfall;


/**
 * @brief   Sinusoid
 *
 * @details This structure could be modified using s3dl_sentences_init()
 */
sinusoid_t sinusoid;

/**
 * @brief   Spin
 *
 * @details This structure could be modified using s3dl_sentences_init()
 */
spin_t spin;

/**
 * @brief   Spiral
 *
 * @details This structure could be modified using s3dl_sentences_init()
 */
spiral_t spiral;

/**
 * @brief   Wave
 *
 * @details This structure could be modified using s3dl_sentences_init()
 */
wave_t wave;

/**
 * @brief   Sphere Pulse
 *
 * @details This structure could be modified using s3dl_sentences_init()
 */
sphere_pulse_t sphere_pulse;

/**
 * @brief   Write
 *
 * @details This structure could be modified using s3dl_sentences_init()
 */
write_t write;

/**
 * @brief   String to print in singlemode
 *
 * @details This string is modified by a controller
 */
char string_sm [MAX_STRING_LEN];

/**
 * @brief   Default sentences
 */
static games_t def_gameslist[DEF_SENTENCES] = {
                                               RAINFALL,
                                               SINUSOID,
                                               SPIN,
                                               SPIRAL,
                                               WAVE,
                                               SPHEREPULSE
                                                          };
/**
 * @brief   Default sentences
 */
static char def_sent[DEF_SENTENCES][MAX_STRING_LEN] =
{
{"ChibiCube"   },
{"ChibiOS/RT"  },
{S3DL_FULL_NAME},
{"SPC5Studio"  },
{1, 2, ' ', ' ', 'l', 'i', 'f','e', '.','a', 'u', 'g', 'm', 'e', 'n', 't', 'e', 'd','\0'}
};
/*
 * GAME PARAMETERS
 */
static uint8_t spu_speed = 200;
static uint16_t rf_drop_numb = 10;
static uint32_t write_lifetime = 50;

static uint8_t std_speed = 120;
static int16_t std_angle = 0;
static uint32_t std_lifetime = 10;
static uint32_t a_very_long_lifetime = 600000;

static games_t precedent_game, actual_game;
static uint8_t sent_ind, games_ind, gameslist_ind, def_mode_ind;



/**
 * @brief   This function initializes all the structures of the games
 */
void s3dl_games_handler_init(void){
  actual_game = NONE;
  precedent_game = NONE;
  sent_ind = 0;
  games_ind = GAMES_BEFORE_A_WRITE;
  def_mode_ind = DEF_GAMES;
  gameslist_ind = GAMES_NUMB;

  CUBE_IS_ON = TRUE;
  SINGLEGAME_SELECTION = NONE;

  if(GAMES_NUMB){
   GAME_HANDLER_MODE = CUBE_GAMESLIST_MODE;
  }
  else{
    GAME_HANDLER_MODE = CUBE_DEFAULT_MODE;
  }

  BUILD_SPHERE_PULSE(&sphere_pulse, spu_speed, std_lifetime);
  BUILD_SINUSOID(&sinusoid ,std_angle ,std_speed ,std_lifetime);
  BUILD_SPIRAL(&spiral, std_angle, std_angle, std_speed, std_speed, std_lifetime);
  BUILD_WAVE(&wave, std_angle, std_angle, std_speed, std_speed, std_lifetime);
  BUILD_RAINFALL(&rainfall, rf_drop_numb, std_speed, std_lifetime);
  BUILD_SPIN(&spin, std_angle, std_speed, std_lifetime);
  BUILD_WRITE(&write, DRAG_AXIALLY, YZ, 8, BACKWARD, std_speed, write_lifetime, FALSE);
}


/**
 * @brief   This function handles the game according to shell incoming commands
 */
void s3dl_games_handler(void){
  if(CUBE_IS_ON){
    if(!FLAG){
      FLAG = TRUE;
      actual_game = NONE;
      precedent_game = NONE;
      sent_ind = 0;
      games_ind = GAMES_BEFORE_A_WRITE;
      def_mode_ind = DEF_GAMES;
      gameslist_ind = GAMES_NUMB;
    }

    switch(GAME_HANDLER_MODE){
    default:
    case CUBE_DEFAULT_MODE:
      while (FLAG){
        games_ind = (games_ind + 1) % (GAMES_BEFORE_A_WRITE + 1);
        if(games_ind){
          def_mode_ind = (def_mode_ind + 1) % DEF_GAMES;
          actual_game = def_gameslist[def_mode_ind];
        }
        else{
          write.mode = (write.mode + 1) % NUMB_OF_WRITE_MODE;
          actual_game = WRITE;
        }
        s3dl_draw_clean_display3D();
        switch (actual_game){
        default:
        case NONE:
          chThdSleepMilliseconds(1);
          break;
        case RAINFALL:
          s3dl_do_rainfall(&rainfall);
          break;
        case SINUSOID:
          s3dl_do_sinusoid(&sinusoid);
          break;
        case SPIN:
          s3dl_do_spin(&spin);
          break;
        case SPIRAL:
          s3dl_do_spiral(&spiral);
          break;
        case WAVE:
          s3dl_do_wave(&wave);
          break;
        case SPHEREPULSE:
          s3dl_do_sphere_pulse(&sphere_pulse);
          break;
        case WRITE:
          if (NUMB_OF_SENTENCE){
            s3dl_do_write(&write, sentence[sent_ind]);
            sent_ind = (sent_ind + 1) % NUMB_OF_SENTENCE;
          }
          else{
            s3dl_do_write(&write, def_sent[sent_ind]);
            sent_ind = (sent_ind + 1) % DEF_SENTENCES;
          }
          break;
        }
      }
      break;
    case CUBE_SINGLEGAME_MODE:
      s3dl_draw_clean_display3D();
      switch (SINGLEGAME_SELECTION){
      default:
      case NONE:
        while (FLAG){
          chThdSleepMilliseconds(50);
        }
        break;
      case RAINFALL:
        while (FLAG){
          rainfall.lifetime = a_very_long_lifetime;
          s3dl_do_rainfall(&rainfall);
        }
        break;
      case SINUSOID:
        while (FLAG){
          sinusoid.lifetime = a_very_long_lifetime;
          s3dl_do_sinusoid(&sinusoid);
        }
        break;
      case SPIN:
        while (FLAG){
          spin.lifetime = a_very_long_lifetime;
          s3dl_do_spin(&spin);
        }
        break;
      case SPIRAL:
        while (FLAG){
          spiral.lifetime = a_very_long_lifetime;
          s3dl_do_spiral(&spiral);
        }
        break;
      case WAVE:
        while (FLAG){
          wave.lifetime = a_very_long_lifetime;
          s3dl_do_wave(&wave);
        }
        break;
      case WRITE:
        while (FLAG){
          write.mode = SURROUND_DISPLAY;
          write.loop = TRUE;
          write.lifetime = a_very_long_lifetime;
          s3dl_do_write(&write, string_sm);
        }
        break;
      case SPHEREPULSE:
        while (FLAG){
          s3dl_do_sphere_pulse(&sphere_pulse);
        }
        break;
      }
      break;
    case CUBE_RANDOM_MODE:
      while (FLAG){
        games_ind = (games_ind + 1) % (GAMES_BEFORE_A_WRITE + 1);
        if(games_ind){
          do{
            srand(chVTGetSystemTime());
            actual_game = (rand() % (NUMB_OF_GAMES - 1)) + 1;
          }while (actual_game == precedent_game);
        }
        else{
          write.mode = (write.mode + 1) % NUMB_OF_WRITE_MODE;
          actual_game = WRITE;
        }
        s3dl_draw_clean_display3D();
        switch (actual_game){
        default:
        case NONE:
          chThdSleepMilliseconds(1);
          break;
        case RAINFALL:
          s3dl_do_rainfall(&rainfall);
          break;
        case SINUSOID:
          s3dl_do_sinusoid(&sinusoid);
          break;
        case SPIN:
          s3dl_do_spin(&spin);
          break;
        case SPIRAL:
          s3dl_do_spiral(&spiral);
          break;
        case WAVE:
          s3dl_do_wave(&wave);
          break;
        case SPHEREPULSE:
          s3dl_do_sphere_pulse(&sphere_pulse);
          break;
        case WRITE:
          if (NUMB_OF_SENTENCE){
            s3dl_do_write(&write, sentence[sent_ind]);
            sent_ind = (sent_ind + 1) % NUMB_OF_SENTENCE;
          }
          else{
            s3dl_do_write(&write, def_sent[sent_ind]);
            sent_ind = (sent_ind + 1) % DEF_SENTENCES;
          }
          break;
        }
        precedent_game = actual_game;
      }
      break;
    case CUBE_GAMESLIST_MODE:
       while (FLAG){
         games_ind = (games_ind + 1) % (GAMES_BEFORE_A_WRITE + 1);
         if(games_ind){
           gameslist_ind = (gameslist_ind + 1) % GAMES_NUMB;
           actual_game = gameslist[gameslist_ind];
         }
         else{
           write.mode = (write.mode + 1) % NUMB_OF_WRITE_MODE;
           actual_game = WRITE;
         }
         s3dl_draw_clean_display3D();
         switch (actual_game){
         default:
         case NONE:
           chThdSleepMilliseconds(1);
           break;
         case RAINFALL:
           s3dl_do_rainfall(&rainfall);
           break;
         case SINUSOID:
           s3dl_do_sinusoid(&sinusoid);
           break;
         case SPIN:
           s3dl_do_spin(&spin);
           break;
         case SPIRAL:
           s3dl_do_spiral(&spiral);
           break;
         case WAVE:
           s3dl_do_wave(&wave);
           break;
         case SPHEREPULSE:
           s3dl_do_sphere_pulse(&sphere_pulse);
           break;
         case WRITE:
           if (NUMB_OF_SENTENCE){
             s3dl_do_write(&write, sentence[sent_ind]);
             sent_ind = (sent_ind + 1) % NUMB_OF_SENTENCE;
           }
           else{
             s3dl_do_write(&write, def_sent[sent_ind]);
             sent_ind = (sent_ind + 1) % DEF_SENTENCES;
           }
           break;
         }
       }
       break;
    }
  }
  else{
    if(!FLAG){
      s3dl_draw_clean_display3D();
    }
    FLAG = TRUE;
    chThdSleepMilliseconds(200);
  }
}

/**
 * @brief   This function initializes the structure of strings
 *
 * @details If a string is longer than MAX_STRING_LEN it is replaced by
 *          "string too long"
 *          If the number of strings is greater than MAX_SENTENCES only the first
 *          MAX_SENTENCES strings are considered
 *          If this function is not used and NUMB_OF_SENTENCE is 0 a default set
 *          of string is used
 */
void s3dl_sentences_init(uint8_t number_of_sentences, const char *fmt, ...){
  uint8_t i;
  char* s = (char*)fmt;
  va_list ap;
  if(number_of_sentences <= MAX_SENTENCES){
    NUMB_OF_SENTENCE = number_of_sentences;
  }
  else{
    NUMB_OF_SENTENCE = MAX_SENTENCES;
  }
  va_start(ap, fmt);
  for(i = 0; i < NUMB_OF_SENTENCE; i++){
    if (strlen(s) < MAX_STRING_LEN){
     strcpy(sentence[i], s);
    }
    else{
      strcpy(sentence[i], "string too long");
    }
    s = va_arg(ap, char*);
  }
  va_end(ap);
}

void s3dl_gameslist_init(uint8_t number_of_games, games_t games, ...){
  uint8_t i;
  games_t g = games;
  va_list ap;
  if(number_of_games <= MAX_GAMES_NUMB){
    GAMES_NUMB = number_of_games;
  }
  else{
    GAMES_NUMB = MAX_GAMES_NUMB;
  }
  va_start(ap, games);
  for(i = 0; i < GAMES_NUMB; i++){
    gameslist[i] = g;
    g = va_arg(ap, int);
  }
  va_end(ap);
  GAME_HANDLER_MODE = CUBE_GAMESLIST_MODE;
}
#endif /* S3DL_USE_VIDEO3D */
