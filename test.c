#include <stdio.h>
#include "s21_helpers.h"

int main(){
    char str[] = "34.0000";
    int scale  = get_scale_from_string(str);

    printf("scale is %d \n", scale);
    return 0;
}