#include "s21_helpers.h"


void equalize_scales(s21_decimal *decimal1, s21_decimal *decimal2){
    int scale1 = get_scale(*decimal1);
    int scale2 = get_scale(*decimal2);
    
    while(scale1>scale2){
        multiply(decimal2, 10);
        scale2++;
    }
    
    while(scale2>scale1){
        multiply(decimal1, 10);
        scale1++;
    }
    
}


int is_less_mentissa(s21_decimal decimal1, s21_decimal decimal2){
    int result = 0;

    equalize_scales(&decimal1, &decimal2);
    
    if(decimal2.bits[2] != decimal1.bits[2]){
        result = decimal1.bits[2] < decimal2.bits[2];
    }else if(decimal2.bits[1] != decimal1.bits[1]){
        result = decimal1.bits[1] <decimal2.bits[1];
    }else{
        result = decimal1.bits[0] < decimal2.bits[0];
    }
    return result;
}

int s21_is_equal(s21_decimal decimal1, s21_decimal decimal2){
   int sign1 = s21_get_sign(decimal1);
   int sign2 = s21_get_sign(decimal2);

   int error = 0;
    if (decimal1.bits[0] == 0 && decimal1.bits[1] == 0 && decimal1.bits[2] == 0 && decimal2.bits[0] == 0 && decimal2.bits[1] == 0 && decimal2.bits[2] == 0 ){
        error = 1;
   }else{
        equalize_scales(&decimal1, &decimal2);
        if(decimal1.bits[0] == decimal2.bits[0] && decimal1.bits[1] == decimal2.bits[1] && decimal1.bits[2] == decimal2.bits[2] && sign1 == sign2){
            error = 1;
        }
   }

    return error;
}

int is_not_equal(s21_decimal decimal1, s21_decimal decimal2){
    return !s21_is_equal(decimal1, decimal2);
}



int s21_is_less(s21_decimal decimal1, s21_decimal decimal2){
    int sign1 = s21_get_sign(decimal1);
    int sign2 = s21_get_sign(decimal2);
    int result = 0;

    if(is_not_equal(decimal1, decimal2)){
        if(sign1 && !sign2){
    result = 1;
   }else if(sign1 && !sign2){
    result = 0;
   }else if(sign1 && sign2){
    result = !is_less_mentissa(decimal1, decimal2);
   }else{
    result = is_less_mentissa(decimal1, decimal2);
   }
    }
   
    return result;
}