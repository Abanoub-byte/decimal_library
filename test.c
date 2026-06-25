#include "s21_helpers.h"
#include <stdio.h>
#include "s21_decimal.h"


int main(){
  s21_decimal decimal = {0};
  decimal.bits[0] = 1234;
  set_scale(&decimal, 2 );
 
  int num = get_scale(decimal);
  double num2 = decimal.bits[0];

  for (int i = 0; i < num; i++){
   num2=  num2 /10;
  }
  printf("%f\n",num2);
  return 0;
}