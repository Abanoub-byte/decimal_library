#include "s21_helpers.h"
#include <math.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>

int s21_from_int_to_decimal(int src, s21_decimal *dst){
    int error = 0;
    *dst = (s21_decimal){0};
    if(0>src){
        s21_set_sign(dst, 1);
        dst->bits[0] = -(unsigned int)(src);
    }else{
        dst->bits[0] = src;
    }
    return error;
}

int s21_from_decimal_to_int(s21_decimal decimal, int *dst){
  
  int err = 0;
  int scale = s21_get_scale(decimal);

  for (int i = 0; i < scale; i++){
    divide(&decimal, 10);
  }
  int fits = (decimal.bits[1] == 0 && decimal.bits[2] == 0);
  int sign = s21_get_sign(decimal);
  int in_range = (sign && (unsigned int)decimal.bits[0] <= (unsigned int)INT_MAX +1) || (unsigned int) decimal.bits[0] <=(unsigned long long)INT_MAX;

  if(fits && in_range){
    if(sign){
      *dst = (int)-(unsigned long long)decimal.bits[0];
    }else{
      *dst = decimal.bits[0];
    }
      }else{
        err = 1;
      }
  return err;
    }

// int s21_from_float_to_decimal(float src, s21_decimal *dst){

//   int sign = 0;
//   char buf[64];

//   float_to_string(src, buf, 50);
//   int scale = s21_get_scale_from_string(buf);
//   if(0>src){
//     set_bit(dst, 127, sign);
//   }
//   set_scale(dst, scale);
  
//   return 0;

// }
/*
      ffff   jjjj   
      ffffh
      


      */
