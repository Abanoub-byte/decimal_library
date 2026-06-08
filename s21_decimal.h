#ifndef S21_DECIMAL_H
#define S21_DECIMAL_H

typedef struct {
    unsigned int bits[4];
} s21_decimal;

int from_float_to_decimal(float src, s21_decimal *dst);
int s21_from_decimal_to_float(s21_decimal src, float *dst);
int s21_from_int_to_decimal(int src, s21_decimal *dst);
int s21_from_decimal_to_int(s21_decimal src, int *dst);
int s21_negate(s21_decimal decimal, s21_decimal *result);
int s21_is_equal(s21_decimal decimal1, s21_decimal decimal2);

#endif