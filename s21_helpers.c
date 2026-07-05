#include "s21_helpers.h"
#include <math.h>


int get_double_scale(double src) {
  int scale = 0;
  double value = fabs(src);

  if (value != 0.0) {
    int exponent = (int)floor(log10(value));
    scale = 15 - 1 - exponent;

    if (scale < 0) {
      scale = 0;
    }

    if (scale > 28) {
      scale = 28;
    }
  }

  return scale;
}

int get_float_scale(float src) {
  int scale = 0;
  double value = fabs((double)src);

  if (value != 0.0) {
    int exponent = (int)floor(log10(value));
    scale = 7 - 1 - exponent;

    if (scale < 0) {
      scale = 0;
    }

    if (scale > 28) {
      scale = 28;
    }
  }

  return scale;
}

unsigned long long s21_bank_round_double(double value) {
  double int_part = floor(value);
  double frac = value - int_part;
  unsigned long long result = (unsigned long long)int_part;

  if (frac > 0.5) {
    result++;
  } else if (frac == 0.5 && result % 2 != 0) {
    result++;
  }

  return result;
}

void s21_set_mantissa_from_ull(s21_decimal *decimal,
                               unsigned long long mantissa) {
  decimal->bits[0] = (unsigned int)(mantissa & 0xFFFFFFFFULL);
  decimal->bits[1] = (unsigned int)((mantissa >> 32) & 0xFFFFFFFFULL);
  decimal->bits[2] = 0;
}


void s21_remove_ull_trailing_zeros(unsigned long long *mantissa, int *scale) {
  while (*scale > 0 && *mantissa != 0 && *mantissa % 10 == 0) {
    *mantissa /= 10;
    (*scale)--;
  }
}

int get_float_sign(float num){
    int sign = 0;
    if(signbit(num)){
        sign = 1;
    }
    return sign;
}

int get_double_sign(double num){
    int sign = 0;
    if(signbit(num)){
        sign = 1;
    }
    return sign;
}

bitPosition get_bit_position(int index){
    bitPosition result;
    result.index = index / 32;
    result.localPosition = index % 32;

    return result;
}

int get_bit_value(s21_decimal decimal, int bit){
    bitPosition position  = get_bit_position(bit);
    int value = 0;
    value = (decimal.bits[position.index] >> position.localPosition) & 1;
    return value;
}

int s21_get_sign(s21_decimal decimal){
    return get_bit_value(decimal, 127);
}

void s21_set_sign(s21_decimal *decimal, int sign){
   set_bit(decimal, 127 ,sign);
}

void s21_set_scale(s21_decimal *decimal, int scale){
    decimal->bits[3] = decimal->bits[3] & ~(0xFFu << 16 );
    decimal->bits[3] = decimal ->bits[3] | ((unsigned int)scale << 16);
}

int s21_get_scale(s21_decimal decimal){
    int scale = decimal.bits[3] & (0xFFu <<16);
    scale = (scale >> 16);
    return scale;
}

void set_bit(s21_decimal *decimal, int bit,unsigned int value){
    bitPosition position = get_bit_position(bit);
    if(value){
        decimal->bits[position.index] = decimal->bits[position.index] | (1u << position.localPosition);
    }else{
        decimal->bits[position.index] = decimal->bits[position.index] &~ (1u << position.localPosition);
    }
}

int s21_negate(s21_decimal decimal, s21_decimal *result){

  int error = 0;
  if(result == NULL) error = 1;
  if(!error){
    *result = decimal; //copy decimal into result
    int value = get_bit_value(*result, 127);
    if(value){
        set_bit(result,127, 0 );
    }else{
        set_bit(result, 127, 1);
    }
  }
        return error;
    }

int multiply(s21_decimal *decimal, unsigned int multiplier) {
  int error = 0;
  unsigned long long num1 = 0;
  unsigned long long num2 = 0;
  unsigned long long num3 = 0;
  unsigned long long bits0 = 0;
  unsigned long long bits1 = 0;
  unsigned long long bits2 = 0;

  if (decimal == NULL) {
    error = 1;
  }

  if (!error) {
    num1 = (unsigned long long)(unsigned int)decimal->bits[0] * multiplier;
    num2 = (unsigned long long)(unsigned int)decimal->bits[1] * multiplier;
    num3 = (unsigned long long)(unsigned int)decimal->bits[2] * multiplier;

    bits0 = num1 & 0xFFFFFFFFULL;
    bits1 = (num2 & 0xFFFFFFFFULL) + (num1 >> 32);
    bits2 = (num3 & 0xFFFFFFFFULL) + (num2 >> 32) + (bits1 >> 32);

    if ((num3 >> 32) != 0 || (bits2 >> 32) != 0) {
      error = 1;
    }
  }

  if (!error) {
    decimal->bits[0] = (unsigned int)bits0;
    decimal->bits[1] = (unsigned int)(bits1 & 0xFFFFFFFFULL);
    decimal->bits[2] = (unsigned int)(bits2 & 0xFFFFFFFFULL);
  }

  return error;
}

int divide(s21_decimal *decimal, unsigned int divider ){
    int err = 0;
    

    if (divider != 0 && decimal != NULL){
        unsigned long long combined1 = decimal->bits[2];
        unsigned long long carry1= combined1 % divider;
        decimal->bits[2] = combined1 / divider;
    
        unsigned long long combined2 = decimal->bits[1] | carry1 << 32;
        decimal->bits[1] = combined2 /divider;
        
        unsigned long long carry2 = combined2 % divider;
        decimal->bits[0] = (decimal->bits[0] | (carry2 << 32)) / divider;
    }else{
        err = 1;
    }
    return err;
}