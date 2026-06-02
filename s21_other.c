    #include "s21_decimal.h"
    #include "s21_helpers.h"

    int s21_negate(s21_decimal decimal, s21_decimal *result){
        *result = decimal; //copy decimal into result
        int value = get_bit_value(result, 127);
        if(value){
            set_bit(result,127, 0 );
        }else{
            set_bit(result, 127, 1);
        }
        return 0;
    }