/*
 *   S3DL - opt_math.h
 *   Written by Rocco Marco Guglielmi
 */

#ifndef _OPT_MATH_H_
#define _OPT_MATH_H_

#include "chtypes.h"

#define right_angle           90
#define straight_angle       180
#define round_angle          360

/**
 * @brief   Uses integers to simulate the decimal numbers
 */
typedef   int32_t               decimal_t;

/**
  * @brief   Precision of decimal numbers is fixed to 4 digits after
  *          the decimal point
  */
#define   decimal_prec           10000

/**
 * @brief   int32_t to decimal_t
 * @details Converts from signed integer to signed decimal.
 *
 * @param[in] n         a signed integer number
 * @return              signed decimal number
 */
#define INT2DEC(n) \
          (decimal_t) (n) * decimal_prec

/**
 * @brief   decimal_t to int32_t
 * @details Converts from signed decimal to signed integer.
 *
 * @param[in] n         a signed decimal number
 * @return              signed integer number
 */
#define DEC2INT(n) \
          (int32_t) (n) / decimal_prec

/**
 * @brief   product between 2 decimal_t
 * @details makes the product of two decimals preserving the meaning of
 *          the position of the digits
 *
 * @param[in] a         first decimal_t
 * @param[in] b         second decimal_t
 * @return              correct product
 */
#define decimal_product(a, b) \
          (decimal_t) (a) * (b) / decimal_prec


/*
 * SINE TABLE
 */
extern decimal_t sin_table[360];


#ifdef __cplusplus
extern "C" {
#endif
uint32_t sqrt2(uint32_t x);
decimal_t sine_map(decimal_t in, int32_t inMin, int32_t inMax, int32_t outMin, int32_t outMax);
uint8_t my_round(decimal_t in);
int32_t int_abs2(int32_t x);
uint16_t sine_adapt(int32_t x);
#ifdef __cplusplus
}
#endif

#endif /* _OPT_MATH_H_ */
