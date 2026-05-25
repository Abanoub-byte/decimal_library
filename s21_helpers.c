#include "s21_helpers.h"

void float_to_string(float num, char str[], int size){
    snprintf(str, size, "%.7g", num);
}

void double_to_string(double num, char str[], int size){
    snprintf(str, size, "%.15g", num);
}

int get_scale_from_string(char str[]){
    int i = 0;
    int scale = 0;
    int position = -1;

    while (str[i] != '\0'){
      if (str[i] == '.') position = i;
      i++;
    }
    
    scale = i - position -1;
    if(position == -1 ) scale = 0;
    return scale;
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

void s21_set_sign(s21_decimal *num, int sign){
   if(sign){
        num -> bits[3] = num -> bits[3] | (1u << 31);
   }else{
        num -> bits[3] = num -> bits[3] &~ (1u <<31);
   }
}

bitPosition get_bit_position(int index){
    bitPosition result;
    result.index = index / 32;
    result.localPosition = index % 32;

    return result;
}

int get_bit_value(s21_decimal *decimal, int bit){
    bitPosition position  = get_bit_position(bit);
    int value = 0;
    value = (decimal->bits[position.index] >> position.localPosition) & 1;
    return value;
}

int s21_get_sign(s21_decimal *decimal){
    return get_bit_value(decimal, 127);
}

void s21_set_sign(s21_decimal *decimal, int sign){
   set_bit(decimal, 127 ,sign);
}
