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

void set_scale(s21_decimal *decimal, int scale){
    decimal->bits[3] = decimal->bits[3] & ~(0xFFu << 16 );
    decimal->bits[3] = decimal ->bits[3] | ((unsigned int)scale << 16);
}

int get_scale(s21_decimal decimal){
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
        *result = decimal; //copy decimal into result
        int value = get_bit_value(*result, 127);
        if(value){
            set_bit(result,127, 0 );
        }else{
            set_bit(result, 127, 1);
        }
        return 0;
    }

    int multiply(s21_decimal *decimal, unsigned int multiplier) {
    
    int error = 0;
    unsigned long long num1 = (unsigned long long)decimal->bits[0] * multiplier;
    unsigned long long num2 = (unsigned long long)decimal->bits[1] * multiplier;
    unsigned long long num3 = (unsigned long long)decimal->bits[2] * multiplier;

    decimal->bits[0] = num1 & 0xFFFFFFFF;
    decimal->bits[1] = (num2 & 0xFFFFFFFF) + (num1 >> 32);

    unsigned long long value3 = (unsigned long long)(num3 & 0xFFFFFFFF) + (num2 >> 32);
    
    if (value3 >> 32 != 0) {
        error = 1;
    }else{
        decimal->bits[2] = value3;
    }
    return error;
}