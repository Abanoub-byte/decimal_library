#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <stdio.h>
#include <math.h>
#include "s21_decimal.h"

typedef struct {
    unsigned int bits[7];  // enough room for overflow during add/mul
} s21_big_decimal;

typedef struct {
    int index;
    int localPosition;
} bitPosition;

// Float/double helpers
unsigned long long s21_bank_round_double(double value);
void s21_set_mantissa_from_ull(s21_decimal *decimal,
                                unsigned long long mantissa);
void s21_remove_ull_trailing_zeros(unsigned long long *mantissa, int *scale);

int get_float_sign(float num);
int get_double_sign(double num);

// Bit helpers
bitPosition get_bit_position(int index);
int get_bit_value(s21_decimal decimal, int bit);
void set_bit(s21_decimal *decimal, int bit, unsigned int value);

// Sign / scale helpers
int s21_get_sign(s21_decimal decimal);
void s21_set_sign(s21_decimal *decimal, int sign);

void s21_set_scale(s21_decimal *decimal, int scale);
int s21_get_scale(s21_decimal decimal);

// Mantissa arithmetic helpers
int multiply(s21_decimal *decimal, unsigned int multiplier);
int divide(s21_decimal *decimal, unsigned int divider);

#endif