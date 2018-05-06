/*
 *   S3DL - games.c
 *   Written by Rocco Marco Guglielmi
 */

#include "games.h"


#if S3DL_USE_VIDEO3D || defined(__DOXYGEN__)


/**
 * @brief     Special strings
 *
 */
char ST_0[MAX_STRING_LEN] = {1, 2,'\0'};
char ST_1[MAX_STRING_LEN] = {1, 2, ' ', ' ', 'l', 'i', 'f','e', '.',
                             'a', 'u', 'g', 'm', 'e', 'n', 't', 'e', 'd','\0'};
char ST_2[MAX_STRING_LEN] = {3,'\0'};
char ST_3[MAX_STRING_LEN] = {3, ' ', ' ', 'l', 'i', 'f','e', '.',
                             'a', 'u', 'g', 'm', 'e', 'n', 't', 'e', 'd','\0'};

char game_name[GAME_NAME_LEN];
bool FLAG = TRUE;

#define MAX_SPEED        255
#define DELAY_END_WRITE  500

#define SPEED_ADAPTER    7


static uint32_t t0;
static int16_t DISPLAY_X_LEN, DISPLAY_Y_LEN, DISPLAY_Z_LEN;
static int16_t cnt_1 = 0, cnt_2 = 0, cnt_3 = 0, cnt_4 = 0, cnt_5 = 0, cnt_6 = 0, cent_plane_1, cent_plane_2,
               th, phi, dir, dis;
static uint16_t write_lenght, str_index_1 = 0, str_index_2 = 1, str_index_3 = 0, str_index_4 = 1, str_index_5 = 0, str_index_6 = 1;
/**
 * @brief     Init some common variable used in gameplay
 *
 */

void s3dl_do_a_game_init(void){
  DISPLAY_X_LEN = DISPLAY3D_X_END - DISPLAY3D_X_START + 1,
  DISPLAY_Y_LEN = DISPLAY3D_Y_END - DISPLAY3D_Y_START + 1,
  DISPLAY_Z_LEN = DISPLAY3D_Z_END - DISPLAY3D_Z_START + 1;
  cent_plane_1 =  ASCII_DIM / 2;
  cent_plane_2 =  ASCII_DIM / 2;
}

/**
 * @brief     Do a test of every led
 *
 * @details   This function is used during the construction of the cube
 *
 */

void s3dl_do_test(void){
  unsigned x_ind, y_ind, z_ind;
  point_t pnt;
  color_t full_int = 0xFFFFFFFF;

  strcpy(game_name, "test");
  s3dl_draw_clean_display3D();
  for(z_ind = DISPLAY3D_Z_START; z_ind < DISPLAY3D_Z_END; z_ind++){
    for(y_ind = DISPLAY3D_Y_START; y_ind < DISPLAY3D_Y_END; y_ind++){
      for(x_ind = DISPLAY3D_X_START; x_ind < DISPLAY3D_X_START; x_ind++){
        BUILD_POINT(&pnt, x_ind, y_ind, z_ind, full_int);
        s3dl_draw_point (pnt);
        chThdSleepMilliseconds(500);
      }
    }
    s3dl_draw_clean_plane(XY, z_ind);
  }
  chThdSleepMilliseconds(1000);
  for(z_ind = DISPLAY3D_Z_START; z_ind < DISPLAY3D_Z_END; z_ind++){
    s3dl_draw_set_plane(XY, z_ind);
    chThdSleepMilliseconds(1000);
    s3dl_draw_clean_plane(XY, z_ind);
  }
  chThdSleepMilliseconds(1000);
}
/**
 * @brief     Do the rain fall animation for a lifetime
 *
 * @param[in] rf     rain fall defined by a lifetime and others parameters
 *
 */
void s3dl_do_rainfall(rainfall_t *rf){
  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  SET_COLOR_MODE(COLOR_RANDOM_MODE);
  #endif /* DISPLAY3D_USE_PWM */

  strcpy(game_name, "rainfall");
  t0 = chVTGetSystemTime() + (rf)->lifetime * CH_CFG_ST_FREQUENCY;

  while((chVTGetSystemTime() <= t0) && FLAG){
    s3dl_draw_rainfall((rf)->drop_numb);
    while (((chVTGetSystemTime() <= t0) && FLAG) && ((rf)->speed_fall == 0)){
      chThdSleepMilliseconds(50);
    }
    chThdSleepMilliseconds(25 * MAX_SPEED / (rf)->speed_fall);
  }
}

/**
 * @brief     Do a 3D sinusoid animation for a lifetime
 *
 * @param[in] sns     sinusoid defined by a lifetime and a speed
 *
 */
void s3dl_do_sinusoid(sinusoid_t *sns){
  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  SET_COLOR_MODE(COLOR_DEFAULT_MODE);
  #endif /* DISPLAY3D_USE_PWM */

  strcpy(game_name, "sinusoid");
  th = (sns)->theta;
  t0 = chVTGetSystemTime() + (sns)->lifetime * CH_CFG_ST_FREQUENCY;

  while((chVTGetSystemTime() <= t0) && FLAG){
    s3dl_draw_sinusoid(th);
    th = th + (sns)->speed / SPEED_ADAPTER;
    chThdSleepMilliseconds(25);
  }
}

/**
 * @brief     Do the spin animation for a lifetime
 *
 * @param[in] sp     spin defined by a lifetime and others parameters
 *
 */
void s3dl_do_spin(spin_t *sp){
  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  SET_COLOR_MODE(COLOR_DEFAULT_MODE);
  #endif /* DISPLAY3D_USE_PWM */

  strcpy(game_name, "spin");
  dis = 0;
  dir = 1;
  t0 = chVTGetSystemTime() + (sp)->lifetime * CH_CFG_ST_FREQUENCY;

  while((chVTGetSystemTime() <= t0) && FLAG){
    s3dl_draw_spin(th, dis);
    dis = dis + (100 * dir);
    if(dis < -12000 || dis > 12000){
      dir = dir * (-1);
    }
    th = th + (sp)->rotation_speed / SPEED_ADAPTER ;
    chThdSleepMilliseconds(25);
  }
}
/**
 * @brief     Do a 3D spiral animation for a lifetime
 *
 * @param[in] spir    spiral defined by a lifetime and others parameters
 *
 */
void s3dl_do_spiral(spiral_t *spir){
  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  CONST_COLOR = 0xFFFFFFFF;
  SET_COLOR_MODE(COLOR_CONSTANT_MODE);
  #endif /* DISPLAY3D_USE_PWM */

  strcpy(game_name, "spiral");
  th = (spir)->theta;
  phi = (spir)->phi;
  t0 = chVTGetSystemTime() + ((spir)->lifetime) * CH_CFG_ST_FREQUENCY;

  while((chVTGetSystemTime() <= t0) && FLAG){
    s3dl_draw_spiral(th, phi);
    th = th + (spir)->speed_x / SPEED_ADAPTER;
    phi = phi + (spir)->speed_y / SPEED_ADAPTER;
    chThdSleepMilliseconds(25);
  }
}

/**
 * @brief     Do a 3D wawe animation for a lifetime
 *
 * @param[in] wv      wawe defined by a lifetime and others parameters
 *
 */
void s3dl_do_wave(wave_t *wv){
  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  CONST_COLOR = 0xFFFFFFFF;
  SET_COLOR_MODE(COLOR_CONSTANT_MODE);
  #endif /* DISPLAY3D_USE_PWM */

  strcpy(game_name, "wave");
  th = (wv)->theta;
  phi = (wv)->phi;
  t0 = chVTGetSystemTime() + (wv)->lifetime * CH_CFG_ST_FREQUENCY;

  while((chVTGetSystemTime() <= t0) && FLAG){
    s3dl_draw_wave(th, phi);
    th = th + (wv)->speed_x / SPEED_ADAPTER;
    phi = phi + (wv)->speed_y / SPEED_ADAPTER;
    chThdSleepMilliseconds(25);
  }
}

/**
 * @brief     Do a the write animation for a lifetime
 *
 * @param[in] wr       wr defined by a lifetime, direction, mode, speed and loop
 * @param[in] string   the string to write
 *
 */
void s3dl_do_write(write_t *wr, char string[MAX_STRING_LEN]){
  char temporary_string[MAX_STRING_LEN + 2];

  strcpy(game_name, "write");
  strcpy(temporary_string, string);
  strcat(temporary_string, "  ");
  str_index_1 = 0;
  str_index_2 = 1;
  write_lenght = strlen(temporary_string);
  if((wr)->mode == DRAG_AXIALLY){
    if((wr)->plane == XY){
      cnt_1 = DISPLAY3D_Z_END - 1;
    }
    else if((wr)->plane == YZ){
      cnt_1 = DISPLAY3D_X_END - 1;
    }
    else{ /*(wr)->plane == ZX*/
      cnt_1 = DISPLAY3D_Y_END - 1;
    }
  }
  if((wr)->mode == SCROLL_PLANARLY){
    if((wr)->plane == XY){
      cnt_1 = DISPLAY3D_Z_START - 1 - (DISPLAY_Z_LEN / 2) ;
      cnt_2 = cnt_1 - (DISPLAY_Z_LEN);
    }
    else if((wr)->plane == YZ){
      cnt_1 = DISPLAY3D_X_START - 1 - (DISPLAY_X_LEN / 2) ;
      cnt_2 = cnt_1 - (DISPLAY_X_LEN);
    }
    else{ /*(wr)->plane == ZX*/
      cnt_1 = DISPLAY3D_Y_START - 1 - (DISPLAY_Y_LEN / 2) ;
      cnt_2 = cnt_1 - (DISPLAY_Y_LEN);
    }
  }
  if(((wr)->mode == SURROUND_DISPLAY) && ((wr)->direction == BACKWARD)){
    cnt_1 = DISPLAY3D_Y_START - 1 - (DISPLAY_Y_LEN / 2) ;
    cnt_2 = cnt_1 - (DISPLAY_Y_LEN);
    cnt_3 = cnt_1 - (DISPLAY_Y_LEN) + 1;
    cnt_4 = cnt_2 - (DISPLAY_Y_LEN) + 1;
    cnt_5 = cnt_3 - (DISPLAY_X_LEN) + 1;
    cnt_6 = cnt_4 - (DISPLAY_X_LEN) + 1;
    str_index_1 = 0;
    str_index_2 = 1;
    str_index_3 = 0;
    str_index_4 = 1;
    str_index_5 = 0;
    str_index_6 = 1;
  }
  if(((wr)->mode == SURROUND_DISPLAY) && ((wr)->direction == FORWARD)){
    cnt_1 = DISPLAY3D_Y_START - 1 - (DISPLAY_Y_LEN / 2) ;
    cnt_2 = cnt_1 - (DISPLAY_Y_LEN);
    cnt_3 = cnt_1 - (DISPLAY_Y_LEN) + 2;
    cnt_4 = cnt_2 - (DISPLAY_Y_LEN) + 2;
    cnt_5 = cnt_3 - (DISPLAY_X_LEN) + 1;
    cnt_6 = cnt_4 - (DISPLAY_X_LEN) + 1;
    str_index_1 = 0;
    str_index_2 = 1;
    str_index_3 = 0;
    str_index_4 = 1;
    str_index_5 = 0;
    str_index_6 = 1;
  }
  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  CONST_COLOR = 0xFFFFFFFF;
  SET_COLOR_MODE(COLOR_CONSTANT_MODE);
  #endif /* DISPLAY3D_USE_PWM */
  s3dl_draw_clean_display3D();
  t0 = chVTGetSystemTime() + (wr)->lifetime * CH_CFG_ST_FREQUENCY;

  while((chVTGetSystemTime() <= t0) && FLAG){
    switch((wr)->mode){
    default:
    case DRAG_AXIALLY:
      if((wr)->plane == XY && (wr)->direction == BACKWARD){
        s3dl_draw_clean_plane(XY, DISPLAY3D_Z_END - cnt_1);
        cnt_1 = (cnt_1 + 1) % DISPLAY3D_Z_END;
        s3dl_draw_ascii(temporary_string[str_index_1] , cent_plane_1, cent_plane_2,
                        DISPLAY3D_Z_END - cnt_1,
                        XY, FALSE, TRUE);
        if((cnt_1 == DISPLAY3D_Z_END - 1)){
          str_index_1 = (str_index_1 + 1) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
        }
      }
      else if((wr)->plane == XY && (wr)->direction == FORWARD){
        s3dl_draw_clean_plane(XY, DISPLAY3D_Z_START + cnt_1);
        cnt_1 = (cnt_1 + 1) % DISPLAY3D_Z_END;
        s3dl_draw_ascii(temporary_string[str_index_1] , cent_plane_1, cent_plane_2,
                        DISPLAY3D_Z_START + cnt_1,
                        XY, FALSE, TRUE);
        if((cnt_1 == DISPLAY3D_Z_END - 1)){
          str_index_1 = (str_index_1 + 1) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
        }
      }
      else if((wr)->plane == YZ && (wr)->direction == BACKWARD){
        s3dl_draw_clean_plane(YZ, DISPLAY3D_X_END - cnt_1);
        cnt_1 = (cnt_1 + 1) % DISPLAY3D_X_END;
        s3dl_draw_ascii(temporary_string[str_index_1] , cent_plane_1, cent_plane_2,
                        DISPLAY3D_X_END - cnt_1,
                        YZ, TRUE, FALSE);
        if((cnt_1 == DISPLAY3D_X_END - 1)){
          str_index_1 = (str_index_1 + 1) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
        }
      }
      else if((wr)->plane == YZ && (wr)->direction == FORWARD){
        s3dl_draw_clean_plane(YZ, DISPLAY3D_X_START + cnt_1);
        cnt_1 = (cnt_1 + 1) % DISPLAY3D_X_END;
        s3dl_draw_ascii(temporary_string[str_index_1] , cent_plane_1, cent_plane_2,
                        DISPLAY3D_X_START + cnt_1,
                        YZ, TRUE, FALSE);
        if((cnt_1 == DISPLAY3D_X_END - 1)){
          str_index_1 = (str_index_1 + 1) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
        }
      }
      else if((wr)->plane == ZX && (wr)->direction == BACKWARD){
        s3dl_draw_clean_plane(ZX, DISPLAY3D_Y_END - cnt_1);
        cnt_1 = (cnt_1 + 1) % DISPLAY3D_Y_END;
        s3dl_draw_ascii(temporary_string[str_index_1] , cent_plane_1, cent_plane_2,
                        DISPLAY3D_Y_END - cnt_1,
                        ZX, FALSE, FALSE);
        if((cnt_1 == DISPLAY3D_Y_END - 1)){
          str_index_1 = (str_index_1 + 1) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
        }
      }
      else{ /*(wr)->plane == ZX && (wr)->direction == FORWARD*/
        s3dl_draw_clean_plane(ZX, DISPLAY3D_Y_START + cnt_1);
        cnt_1 = (cnt_1 + 1) % DISPLAY3D_Y_END;
        s3dl_draw_ascii(temporary_string[str_index_1] , cent_plane_1, cent_plane_2,
                        DISPLAY3D_Y_START + cnt_1,
                        ZX, FALSE, FALSE);
        if((cnt_1 == DISPLAY3D_Y_END - 1)){
          str_index_1 = (str_index_1 + 1) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
        }
      }
      while ((chVTGetSystemTime() <= t0) && FLAG && (wr)->speed == 0){
        chThdSleepMilliseconds(50);
      }
      chThdSleepMilliseconds(30 * MAX_SPEED / (wr)->speed);
      break;
    case SCROLL_PLANARLY:
      if((wr)->plane == XY && (wr)->direction == BACKWARD){
        s3dl_draw_ascii(temporary_string[str_index_1], cent_plane_1,
                        DISPLAY3D_Z_END - cnt_1, (wr)->index, XY, FALSE, TRUE);
        s3dl_draw_ascii(temporary_string[str_index_2], cent_plane_1,
                        DISPLAY3D_Z_END - cnt_2, (wr)->index, XY, FALSE, TRUE);
        cnt_1++;
        cnt_2++;
        if(cnt_1 == DISPLAY3D_Z_END + DISPLAY_Z_LEN / 2){
          str_index_1 = (str_index_1 + 2) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_1 = DISPLAY3D_Z_START - 1 - DISPLAY_Z_LEN / 2;
        }
        if(cnt_2 == DISPLAY3D_Z_END + DISPLAY_Z_LEN / 2){
          str_index_2 = (str_index_2 + 2) % write_lenght;
          if((str_index_2 <= 1) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_2 = DISPLAY3D_Z_START - 1 - DISPLAY_Z_LEN / 2;
        }
      }
      else if((wr)->plane == XY && (wr)->direction == FORWARD){
        s3dl_draw_ascii(temporary_string[str_index_1], cent_plane_1,
                        cnt_1, (wr)->index, XY, FALSE, TRUE);
        s3dl_draw_ascii(temporary_string[str_index_2], cent_plane_1,
                        cnt_2, (wr)->index, XY, FALSE, TRUE);
        cnt_1++;
        cnt_2++;
        if(cnt_1 == DISPLAY3D_Z_END + DISPLAY_Z_LEN / 2){
          str_index_1 = (str_index_1 + 2) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_1 = DISPLAY3D_Z_START - 1 - DISPLAY_Z_LEN / 2;
        }
        if(cnt_2 == DISPLAY3D_Z_END + DISPLAY_Z_LEN / 2){
          str_index_2 = (str_index_2 + 2) % write_lenght;
          if((str_index_2 <= 1) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_2 = DISPLAY3D_Z_START - 1 - DISPLAY_Z_LEN / 2;
        }
      }
      else if((wr)->plane == YZ && (wr)->direction == BACKWARD){
        s3dl_draw_ascii(temporary_string[str_index_1] ,DISPLAY3D_X_END - cnt_1 ,
                        cent_plane_2 , (wr)->index, YZ, TRUE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_2] ,DISPLAY3D_X_END - cnt_2 ,
                        cent_plane_2 , (wr)->index, YZ, TRUE, FALSE);
        cnt_1++;
        cnt_2++;
        if(cnt_1 == DISPLAY3D_X_END + DISPLAY_X_LEN / 2){
          str_index_1 = (str_index_1 + 2) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_1 = DISPLAY3D_X_START - 1 - DISPLAY_X_LEN / 2;
        }
        if(cnt_2 == DISPLAY3D_X_END + DISPLAY_X_LEN / 2){
          str_index_2 = (str_index_2 + 2) % write_lenght;
          if((str_index_2 <= 1) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_2 = DISPLAY3D_X_START - 1 - DISPLAY_X_LEN / 2;
        }
      }
      else if((wr)->plane == YZ && (wr)->direction == FORWARD){
        s3dl_draw_ascii(temporary_string[str_index_1] ,cnt_1 ,
                        cent_plane_2 , (wr)->index, YZ, TRUE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_2] ,cnt_2 ,
                        cent_plane_2 , (wr)->index, YZ, TRUE, FALSE);
        cnt_1++;
        cnt_2++;
        if(cnt_1 == DISPLAY3D_X_END + DISPLAY_X_LEN / 2){
          str_index_1 = (str_index_1 + 2) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_1 = DISPLAY3D_X_START - 1 - DISPLAY_X_LEN / 2;
        }
        if(cnt_2 == DISPLAY3D_X_END + DISPLAY_X_LEN / 2){
          str_index_2 = (str_index_2 + 2) % write_lenght;
          if((str_index_2 <= 1) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_2 = DISPLAY3D_X_START - 1 - DISPLAY_X_LEN / 2;
        }
      }
      else if((wr)->plane == ZX && (wr)->direction == BACKWARD){
        s3dl_draw_ascii(temporary_string[str_index_1], cent_plane_1,
                        DISPLAY3D_Y_END - cnt_1, (wr)->index, ZX, FALSE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_2], cent_plane_1,
                        DISPLAY3D_Y_END - cnt_2, (wr)->index, ZX, FALSE, FALSE);
        cnt_1++;
        cnt_2++;
        if(cnt_1 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_1 = (str_index_1 + 2) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_1 = DISPLAY3D_Y_START - 1 - DISPLAY_Y_LEN / 2;
        }
        if(cnt_2 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_2 = (str_index_2 + 2) % write_lenght;
          if((str_index_2 <= 1) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_2 = DISPLAY3D_Y_START - 1 - DISPLAY_Y_LEN / 2;
        }
      }
      else{ /*(wr)->plane == ZX && (wr)->direction == FORWARD*/
        s3dl_draw_ascii(temporary_string[str_index_1], cent_plane_1,
                        cnt_1, (wr)->index, ZX, FALSE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_2], cent_plane_1,
                        cnt_2, (wr)->index, ZX, FALSE, FALSE);
        cnt_1++;
        cnt_2++;
        if(cnt_1 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_1 = (str_index_1 + 2) % write_lenght;
          if((str_index_1 == 0) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_1 = DISPLAY3D_Y_START - 1 - DISPLAY_Y_LEN / 2;
        }
        if(cnt_2 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_2 = (str_index_2 + 2) % write_lenght;
          if((str_index_2 <= 1) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_2 = DISPLAY3D_Y_START - 1 - DISPLAY_Y_LEN / 2;
        }
      }
      while (((chVTGetSystemTime() <= t0) && FLAG) && ((wr)->speed == 0)){
        chThdSleepMilliseconds(50);
      }
      chThdSleepMilliseconds(30 * MAX_SPEED / (wr)->speed);
      break;
    case SURROUND_DISPLAY:
      if((wr)->direction == BACKWARD){
        s3dl_draw_ascii(temporary_string[str_index_1], cent_plane_1,
                        cnt_1, DISPLAY3D_Y_END, ZX, TRUE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_2], cent_plane_1,
                        cnt_2, DISPLAY3D_Y_END, ZX, TRUE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_3] ,DISPLAY3D_X_END - cnt_3 ,
                        cent_plane_2 , DISPLAY3D_X_END, YZ, TRUE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_4] ,DISPLAY3D_X_END - cnt_4 ,
                        cent_plane_2 , DISPLAY3D_X_END, YZ, TRUE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_5], cent_plane_1,
                        DISPLAY3D_Y_END - cnt_5, DISPLAY3D_Y_START, ZX, FALSE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_6], cent_plane_1,
                        DISPLAY3D_Y_END - cnt_6, DISPLAY3D_Y_START, ZX, FALSE, FALSE);
        cnt_1++;
        cnt_2++;
        cnt_3++;
        cnt_4++;
        cnt_5++;
        cnt_6++;
        if(cnt_1 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_1 = (str_index_1 + 2) % write_lenght;
          if(!((str_index_1 <= 1) && !((wr)->loop))){
            cnt_1 = DISPLAY3D_Y_START - 1 - DISPLAY_Y_LEN / 2;
          }
        }
        if(cnt_2 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_2 = (str_index_2 + 2) % write_lenght;
          if(!((str_index_2 <= 1) && !((wr)->loop))){
            cnt_2 = DISPLAY3D_Y_START - 1 - DISPLAY_Y_LEN / 2;
          }
        }
        if(cnt_3 == DISPLAY3D_X_END + DISPLAY_X_LEN / 2){
          str_index_3 = str_index_1;
          if(!((str_index_3 <= 1) && !((wr)->loop))){
            cnt_3 = cnt_1 - (DISPLAY_Y_LEN) + 1;
          }
        }
        if(cnt_4 == DISPLAY3D_X_END + DISPLAY_X_LEN / 2){
          str_index_4 = str_index_2;
          if(!((str_index_4 <= 1) && !((wr)->loop))){
            cnt_4 = cnt_2 - (DISPLAY_Y_LEN) + 1;
          }
        }
        if(cnt_5 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_5 = str_index_3;
          if((str_index_5 <= 1) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_5 = cnt_3 - (DISPLAY_X_LEN) + 1;
        }
        if(cnt_6 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_6 = str_index_4;
          if((str_index_6 <= 1) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_6 = cnt_4 - (DISPLAY_X_LEN) + 1;
        }
      }
      else{
        s3dl_draw_ascii(temporary_string[str_index_1], cent_plane_1,
                        DISPLAY3D_Y_START + cnt_1, DISPLAY3D_Y_START, ZX, FALSE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_2], cent_plane_1,
                        DISPLAY3D_Y_START + cnt_2, DISPLAY3D_Y_START, ZX, FALSE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_3] ,cnt_3 ,
                        cent_plane_2 , DISPLAY3D_X_END, YZ, TRUE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_4] ,cnt_4 ,
                        cent_plane_2 , DISPLAY3D_X_END, YZ, TRUE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_5], cent_plane_1,
                        DISPLAY3D_Y_END - cnt_5, DISPLAY3D_Y_END, ZX, TRUE, FALSE);
        s3dl_draw_ascii(temporary_string[str_index_6], cent_plane_1,
                        DISPLAY3D_Y_END - cnt_6, DISPLAY3D_Y_END, ZX, TRUE, FALSE);
        cnt_1++;
        cnt_2++;
        cnt_3++;
        cnt_4++;
        cnt_5++;
        cnt_6++;
        if(cnt_1 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_1 = (str_index_1 + 2) % write_lenght;
          if(!((str_index_1 <= 1) && !((wr)->loop))){
            cnt_1 = DISPLAY3D_Y_START - 1 - DISPLAY_Y_LEN / 2;
          }
        }
        if(cnt_2 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_2 = (str_index_2 + 2) % write_lenght;
          if(!((str_index_2 <= 1) && !((wr)->loop))){
            cnt_2 = DISPLAY3D_Y_START - 1 - DISPLAY_Y_LEN / 2;
          }
        }
        if(cnt_3 == DISPLAY3D_X_END + DISPLAY_X_LEN / 2){
          str_index_3 = str_index_1;
          if(!((str_index_3 <= 1) && !((wr)->loop))){
            cnt_3 = cnt_1 - (DISPLAY_Y_LEN) + 2;
          }
        }
        if(cnt_4 == DISPLAY3D_X_END + DISPLAY_X_LEN / 2){
          str_index_4 = str_index_2;
          if(!((str_index_4 <= 1) && !((wr)->loop))){
            cnt_4 = cnt_2 - (DISPLAY_Y_LEN) + 2;
          }
        }
        if(cnt_5 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_5 = str_index_3;
          if((str_index_5 <= 1) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_5 = cnt_3 - (DISPLAY_X_LEN) + 1;
        }
        if(cnt_6 == DISPLAY3D_Y_END + DISPLAY_Y_LEN / 2){
          str_index_6 = str_index_4;
          if((str_index_6 <= 1) && !((wr)->loop)){
            chThdSleepMilliseconds(DELAY_END_WRITE);
            return;
          }
          cnt_6 = cnt_4 - (DISPLAY_X_LEN) + 1;
        }
      }
      while (((chVTGetSystemTime() <= t0) && FLAG) && ((wr)->speed == 0)){
        chThdSleepMilliseconds(50);
      }
      chThdSleepMilliseconds(30 * MAX_SPEED / (wr)->speed);
      break;
    }
    chThdSleepMilliseconds(1);
  }
}

void s3dl_do_sphere_pulse(sphere_pulse_t *sp_pu){
  sphere_t sph;
  decimal_t radius = 40000, center = 45000;

  strcpy(game_name, "sphere");
  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  ENHANCE_CENTER_INDEX = 1;
  #else
  decimal_t min_dim = 15000, max_dim = 45000;
  #endif /* !DISPLAY3D_USE_PWM */

  dir = 1;
  t0 = chVTGetSystemTime() + (sp_pu)->lifetime * CH_CFG_ST_FREQUENCY;

  while((chVTGetSystemTime() <= t0) && FLAG){
    #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
    SET_COLOR_MODE(COLOR_ENHANCE_CENTER_MODE);
    BUILD_SPHERE(&sph, center, center, center, radius, TRUE);
    s3dl_draw_sphere(sph);
    ENHANCE_CENTER_INDEX = ENHANCE_CENTER_INDEX + dir;
    if(ENHANCE_CENTER_INDEX > 7 || ENHANCE_CENTER_INDEX < 2){
      dir = dir * (-1);
    }
    chThdSleepMilliseconds(260 - (sp_pu)->speed);
    #else
    BUILD_SPHERE(&sph, center, center, center, radius, FALSE);
    radius = radius + INT2DEC((sp_pu)->speed) / 1000 * dir;
    if(radius >= max_dim || radius <= min_dim){
      dir = dir * (-1);
    }
    s3dl_draw_sphere(sph);
    chThdSleepMilliseconds(25);
    #endif /* S3DL_USE_PWM */

  }
}

/**
 * @brief     Do a the trail animation for a lifetime
 *
 * @param[in] tr     trail defined by a lifetime and a speed
 *
 */
void s3dl_do_trail(trail_t *tr){
  #if DISPLAY3D_USE_PWM || defined(__DOXYGEN__)
  CONST_COLOR = 0xFFFFFFFF;
  SET_COLOR_MODE(COLOR_CONSTANT_MODE);
  #endif /* DISPLAY3D_USE_PWM */

  strcpy(game_name, "trail");
  th = (tr)->theta;
  t0 = chVTGetSystemTime() + (tr)->lifetime * CH_CFG_ST_FREQUENCY;

  while((chVTGetSystemTime() <= t0) && FLAG){
    s3dl_draw_trail(th);
    th = th + (tr)->speed / SPEED_ADAPTER;
    chThdSleepMilliseconds(25);
  }
}
#endif /* S3DL_USE_VIDEO3D */
