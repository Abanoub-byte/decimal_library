#ifndef S21_HELPERS_H
#define S21_HELPERS_H

#include <stdio.h>
#include <math.h>
#include "s21_decimal.h"

typedef struct {
    int index;
    int localPosition;
}bitPosition;

int set_scale(s21_decimal *num, int scale);
int get_scale(s21_decimal num);
int s21_get_sign(s21_decimal num);
void s21_set_sign(s21_decimal *num, int sign);
int get_double_sign(double num);
int get_float_sign(float num);
int get_scale_from_string(char str[]);
void float_to_string(float num, char str[], int size);
void double_to_string(double num, char str[], int size);

void get_mantissa(char str[]);
void set_bit(s21_decimal *num, int bit,unsigned int value);
bitPosition get_bit_position(int index);
int get_bit_value(s21_decimal *decimal, bitPosition position);
#endif