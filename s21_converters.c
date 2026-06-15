#include "s21_helpers.h"
#include <math.h>
#include <stdio.h>
#include <limits.h>
int from_int_to_decimal(int src, s21_decimal *dst){
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

int from_decimal_to_int(s21_decimal source, int *dst){
    int error = 0;
    *dst = 0;
    unsigned long long reminder = 0;
    unsigned long long combine = 0;
    int scale = get_scale(source);
        return error;
}

int s21_from_float_to_decimal(float src, s21_decimal *dst){

    return 0;
}

