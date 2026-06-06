#include "s21_helpers.h"

int s21_is_equal(s21_decimal decimal1, s21_decimal decimal2){
    int error = 0;

    unsigned int scale = get_scale(decimal1) - get_scale(decimal2);
    unsigned int sign = s21_get_sign(decimal1) - s21_get_sign(decimal2);
    
    unsigned int bits0 = decimal1.bits[0] - decimal2.bits[0];
    unsigned int bits1 = decimal1.bits[1] - decimal2.bits[1];
    unsigned int bits2 = decimal1.bits[2] - decimal2.bits[2];

    error = scale + sign + bits0 + bits1 + bits2;
    if(error != 0){
        error = 1;
    }

    return error;
}