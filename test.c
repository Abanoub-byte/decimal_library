#include "s21_helpers.h"
#include <stdio.h>
#include "s21_decimal.h"
#include <math.h>


int from_decimal_to_int(s21_decimal decimal, int *dst);

int main(){
  s21_decimal decimal = {0};
  int dst;

  decimal.bits[0] = 500;
  
  set_scale(&decimal, 2 );
  printf("the set scale is %d", get_scale(decimal));
  
  int num = from_decimal_to_int(decimal, &dst);

  printf("%d\n",num);

  return 0;
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
}