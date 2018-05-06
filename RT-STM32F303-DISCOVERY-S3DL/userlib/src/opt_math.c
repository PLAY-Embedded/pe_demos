/*
 *   S3DL - opt_math.c
 *   Written by Rocco Marco Guglielmi
 */

#include "../../userlib/include/opt_math.h"

/*===========================================================================*/
/* External declaration                                                      */
/*===========================================================================*/

/*
 * SINE TABLE
 */
decimal_t sin_table[360] = {   0,   175,   349,   523,   698,   872,  1045,
                            1219,  1392,  1564,  1736,  1908,  2079,  2250,
                            2419,  2588,  2756,  2924,  3090,  3256,  3420,
                            3584,  3746,  3907,  4067,  4226,  4384,  4540,
                            4695,  4848,  5000,  5150,  5299,  5446,  5592,
                            5736,  5878,  6018,  6157,  6293,  6428,  6561,
                            6691,  6820,  6947,  7071,  7193,  7314,  7431,
                            7547,  7660,  7771,  7880,  7986,  8090,  8192,
                            8290,  8387,  8480,  8572,  8660,  8746,  8829,
                            8910,  8988,  9063,  9135,  9205,  9272,  9336,
                            9397,  9455,  9511,  9563,  9613,  9659,  9703,
                            9744,  9781,  9816,  9848,  9877,  9903,  9925,
                            9945,  9962,  9976,  9986,  9994,  9998, 10000,
                            9998,  9994,  9986,  9976,  9962,  9945,  9925,
                            9903,  9877,  9848,  9816,  9781,  9744,  9703,
                            9659,  9613,  9563,  9511,  9455,  9397,  9336,
                            9272,  9205,  9135,  9063,  8988,  8910,  8829,
                            8746,  8660,  8572,  8480,  8387,  8290,  8192,
                            8090,  7986,  7880,  7771,  7660,  7547,  7431,
                            7314,  7193,  7071,  6947,  6820,  6691,  6561,
                            6428,  6293,  6157,  6018,  5878,  5736,  5592,
                            5446,  5299,  5150,  5000,  4848,  4695,  4540,
                            4384,  4226,  4067,  3907,  3746,  3584,  3420,
                            3256,  3090,  2924,  2756,  2588,  2419,  2250,
                            2079,  1908,  1736,  1564,  1392,  1219,  1045,
                             872,   698,   523,   349,   175,     0,  -175,
                            -349,  -523,  -698,  -872, -1045, -1219, -1392,
                           -1564, -1736, -1908, -2079, -2250, -2419, -2588,
                           -2756, -2924, -3090, -3256, -3420, -3584, -3746,
                           -3907, -4067, -4226, -4384, -4540, -4695, -4848,
                           -5000, -5150, -5299, -5446, -5592, -5736, -5878,
                           -6018, -6157, -6293, -6428, -6561, -6691, -6820,
                           -6947, -7071, -7193, -7314, -7431, -7547, -7660,
                           -7771, -7880, -7986, -8090, -8192, -8290, -8387,
                           -8480, -8572, -8660, -8746, -8829, -8910, -8988,
                           -9063, -9135, -9205, -9272, -9336, -9397, -9455,
                           -9511, -9563, -9613, -9659, -9703, -9744, -9781,
                           -9816, -9848, -9877, -9903, -9925, -9945, -9962,
                           -9976, -9986, -9994, -9998,-10000, -9998, -9994,
                           -9986, -9976, -9962, -9945, -9925, -9903, -9877,
                           -9848, -9816, -9781, -9744, -9703, -9659, -9613,
                           -9563, -9511, -9455, -9397, -9336, -9272, -9205,
                           -9135, -9063, -8988, -8910, -8829, -8746, -8660,
                           -8572, -8480, -8387, -8290, -8192, -8090, -7986,
                           -7880, -7771, -7660, -7547, -7431, -7314, -7193,
                           -7071, -6947, -6820, -6691, -6561, -6428, -6293,
                           -6157, -6018, -5878, -5736, -5592, -5446, -5299,
                           -5150, -5000, -4848, -4695, -4540, -4384, -4226,
                           -4067, -3907, -3746, -3584, -3420, -3256, -3090,
                           -2924, -2756, -2588, -2419, -2250, -2079, -1908,
                           -1736, -1564, -1392, -1219, -1045,  -872,  -698,
                            -523,  -349,  -175};

/*===========================================================================*/
/* Functions.                                                                */
/*===========================================================================*/



/**
 * @brief   Fast integer inverse square root
 *
 * @param[in]   value value by which find the square root
 * @return      the square root
 */
uint32_t sqrt2(uint32_t x){
  int root = 0, r = 0, dp1, L = 0, g[10], j, y, yn, xm = x;
  while(xm > 0){
    g[L++] = xm % 100;
    xm = xm / 100;
  }
  for(j = L - 1; j >= 0; j--){
    r = r * 100 + g[j];
    y=0;
    for(dp1 = 1; dp1 < 10; dp1++){
      yn = dp1 * (20 * root + dp1);
      if(yn<=r){
        y=yn;
      }
      else{
        break;
      }
    }
    root = root * 10 + dp1 - 1;
    r = r - y;
  }
  return(root);
}



/**
 * @brief   Receives a number within the range (inMin, inMax) and returns a
 * proportional output within the range (outMin, outMax)
 *
 * @param[in] in             parameter to map
 * @param[in] inMin          input lowest bound
 * @param[in] inMax          input highest bound
 * @param[in] outMin         output lowest bound
 * @param[in] outMax         output highest bound
 * @return                   mapped output
 */
decimal_t sine_map(decimal_t in, int32_t inMin, int32_t inMax, int32_t outMin, int32_t outMax){
  return (decimal_t) ((in - (inMin * decimal_prec)) * (outMax - outMin) / (inMax - inMin)  + outMin * decimal_prec);
}
/**
 * @brief   Receives a decimal_t and round it as an unsigned
 *
 * @param[in] in             value to round
 * @return                   rounded value
 */
uint8_t my_round(decimal_t in){
  decimal_t decimal_part = in % decimal_prec;
  if (decimal_part >= (decimal_prec / 2)){
    return (uint8_t) ((in / decimal_prec) + 1);
  }
  else{
    return (uint8_t) (in / decimal_prec);
  }
}


/**
 * @brief   Absolute
 *
 * @param[in]   x value by which find the abs
 * @return      the abs
 */
int32_t int_abs2(int32_t x){
  if (x > 0){
    return x;
  }
  else
    return -x;
}

/**
 * @brief       Receive an angle and return equivalent angle between 0 and 359
 *              degrees
 *
 * @param[in]   x   angle to adapt for sine or cosine table
 * @return      the adapted angle
 */
uint16_t sine_adapt(int32_t x){
  while (x < 0){
    x = x + 360;
  }
  return x % 360;
}
