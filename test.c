#include <stdio.h>

#include "s21_helpers.h"
#include "s21_decimal.h"

// void multiply(s21_decimal *decimal, int multiply){
//     unsigned long long temp =  (unsigned long long)decimal->bits[0] * multiply;
//     decimal->bits[0] = temp & 0xFFFFFFFF;
//     unsigned long long temp1 = temp >> 32;

//     unsigned long long temp2 = (unsigned long long)decimal->bits[1] * multiply;
//     unsigned long long temp3  = temp2 >> 32;
//     decimal->bits[1] = temp2 &  0xFFFFFFFF;

//     unsigned long long temp4 = (unsigned long long)decimal->bits[2] * multiply;
//     int temp5 = temp4 >> 32;
//     decimal->bits[2] = temp5 &  0xFFFFFFFF;

// }

void multiply(s21_decimal *decimal, unsigned int mulitplyer);
int add_digit(s21_decimal *decimal, unsigned int digit);

int main(){
    unsigned int number = 3000000000;
    multiply(number, 10);
    
    return 0;
}

int add_digit(s21_decimal *decimal, unsigned int digit){

    int error = 0;
    unsigned long long value1 = (unsigned long long)decimal->bits[0] + digit;
    decimal->bits[0] = value1 & 0XFFFFFFFF;

    unsigned long long value2 = (unsigned long long)decimal->bits[1] +(value1 >> 32);
    decimal->bits[1] =  value2 & 0xFFFFFFFF;
    
    unsigned long long value3 = (unsigned long long)decimal->bits[2] + (value2 >> 32);
    if ((value3 >> 32) != 0){
        error = 1;
    }else {
        decimal->bits[2] = value3 & 0xFFFFFFFF;
    }
    return error;
}

void multiply(s21_decimal *decimal, unsigned int multiplyer){

    unsigned long long num1 = (unsigned long long) decimal->bits[0] * multiplyer; 
    unsigned long long num2 = (unsigned long long) decimal->bits[1] * multiplyer;
    unsigned long long num3 = (unsigned long long) decimal->bits[2] * multiplyer;

    decimal->bits[0] = num1 & 0xFFFFFFFF;
    decimal->bits[1] = (num2 & 0xFFFFFFFF) + (num1 >> 32);

    unsigned long long value3 = (unsigned long long)(num3 & 0XFFFFFFFF) + (num2 >> 32);

    if (value3 >> 32 != 0){
        printf("number is too big");
    }else{
        printf("succeed");
        decimal->bits[2] = value3;

}
}

void