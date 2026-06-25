#include "s21_helpers.h"
#include <math.h>
#include <stdio.h>
#include <limits.h>
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

int from_decimal_to_int(s21_decimal decimal, int *dst){
  int err = 0;

  if(decimal.bits[1] == 0 && decimal.bits[2] == 0 && decimal.bits[0] < INT_MAX){
      
    int scale = get_scale(decimal);
    int sign = s21_get_sign(decimal);
    
      *dst = decimal.bits[0];
    
      for (int i = 0; i < scale; i++){
        *dst  = *dst / 10;
      }
      if(sign){
        *dst = *dst * -1;
      }
      }else{
        err = 1;
      }

  return err;
/*
      ffff   jjjj   
      ffff
      


      */
