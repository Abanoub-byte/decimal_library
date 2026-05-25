#include <stdio.h>

#include "s21_helpers.h"


void bit_print(int num);
void print_moving_bit(int num);
void shift_print(int num, int shift, char direction);

void set_scale(s21_decimal *decimal, int scale){
    for(int i = 31; i>=0; i--){
        for(int k = 23; k >= 16;k--){
            if((scale >> i) & 1){
                set_bit(decimal, k, 1u);
            }else{
                set_bit(decimal, k, 0);
            }
        }
    }

}

int main(){
    int value = bit_value(123456);
    return 0;
}

void print_moving_bit(int num){
    for(int i = 5; i>=0; i--){
        num >>= 1;
        for(int k =31; k>=0; k--){
            if((num>>k) & 1){
                printf("1");
            }else{
                printf("0");
            }
        }    
        printf("\n");
    }
    printf("\n");
}

void bit_print(int num){
    
    for(int i =31; i>=0; i--){
        if((num>>i) & 1){
            printf("1");
        }else{
            printf("0");
        }
    }
}

void set_decimal_sign(s21_decimal *num, int sign){
if (sign) num->bits[3] = num->bits[3] | (1u << 31);
}



void shift_print(int num, int shift, char direction){
    switch (direction)
    {
    case 'r':
        num  = num >> shift;
        break;
    
    default:
         num  = num << shift;
        break;
    }
    for(int i = 31; i>= 0; i-- ){
        if((num >> i) & 1){
            printf("1");
        }else{
            printf("0");
        }
    }
}