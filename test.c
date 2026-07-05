#include <stdio.h>
#include "s21_decimal.h"
#include "s21_helpers.h"
#include <math.h> 
int main(void) {
  s21_decimal d = {{0xFFFFFFFFu, 0, 0, 0}};

  int err = multiply(&d, 2);

  printf("err = %d\n", err);
  printf("bits[0] signed   = %d\n", (int)d.bits[0]);
  printf("bits[0] unsigned = %u\n", (unsigned int)d.bits[0]);
  printf("bits[0] hex      = 0x%08X\n", (unsigned int)d.bits[0]);

  printf("bits[1] signed   = %d\n", (int)d.bits[1]);
  printf("bits[1] unsigned = %u\n", (unsigned int)d.bits[1]);
  printf("bits[1] hex      = 0x%08X\n", (unsigned int)d.bits[1]);

  return 0;
}