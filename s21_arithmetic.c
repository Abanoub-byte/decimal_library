#include "s21_decimal.h"
#include "s21_helpers.h"

int s21_sub(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){
    
    int error = 0;

    if(result == NULL) {
        error = 1;
    }

    int scale_value1 = s21_get_scale(value_1);
    int scale_value2 = s21_get_scale(value_2);

    if(scale_value1 != scale_value2){
        equalize_scales(&value_1, &value_2);
    }

    unsigned long long combined1 = value_1.bits[0] + value_1.bits[1];
    unsigned long long combined2 = value_2.bits[0] + value_2.bits[1];

    unsigned long long result1 = combined1 - combined2;
    unsigned long long result2 = value_1.bits[2] - value_2.bits[2];


 
    
}

int s21_add(s21_decimal value_1, s21_decimal value_2, s21_decimal *result){

    int error = 0;

    if(result == NULL) {
        error = 1;
    }

    int scale_value1 = s21_get_scale(value_1);
    int scale_value2 = s21_get_scale(value_2);

    if (scale_value1 != scale_value2){
        equalize_scales(&value_1, &value_2);
    }

    unsigned long long combined1 = (unsigned int)value_1.bits[0] + (unsigned int)value_2.bits[0]; // 1111 1111 
    unsigned long long carry1_bits0 = (combined1 << 32); // 1111 0000 

    
    result->bits[0] = combined1 & 0xFFFFFFFF; //1111

    unsigned long long combined2 = (unsigned int)value_1.bits[1] + (unsigned int)value_2.bits[1] + carry1_bits0;
    unsigned long long carry_bits1 = (combined2 << 32);

    result->bits[1] = combined2 & 0xFFFFFFFF; // 1111

    unsigned long long combined3 = (unsinged int)value_1.bits[2] + (unsigned int)value_2.bits[2];





}