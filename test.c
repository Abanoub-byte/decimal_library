#include <stdio.h>
#include "s21_helpers.h"

int main(){
      // Test 1: same number, same scale → equal
    s21_decimal a = {0};
    s21_decimal b = {0};
    a.bits[0] = 314;
    b.bits[0] = 314;
    set_scale(&a, 2);
    set_scale(&b, 2);
    printf("Test 1: 3.14 == 3.14 → %d (expected 1)\n", s21_is_equal(a, b));

    // Test 2: different number → not equal
    s21_decimal c = {0};
    s21_decimal d = {0};
    c.bits[0] = 314;
    d.bits[0] = 315;
    set_scale(&c, 2);
    set_scale(&d, 2);
    printf("Test 2: 3.14 == 3.15 → %d (expected 0)\n", s21_is_equal(c, d));

    // Test 3: same number, different sign → not equal
    s21_decimal e = {0};
    s21_decimal f = {0};
    e.bits[0] = 42;
    f.bits[0] = 42;
    s21_set_sign(&f, 1);
    printf("Test 3: 42 == -42 → %d (expected 0)\n", s21_is_equal(e, f));

    // Test 4: +0 and -0 → equal
    s21_decimal g = {0};
    s21_decimal h = {0};
    s21_set_sign(&h, 1);
    printf("Test 4: +0 == -0 → %d (expected 1)\n", s21_is_equal(g, h));

    // Test 5: number in bits[1] → equal
    s21_decimal i = {0};
    s21_decimal j = {0};
    i.bits[0] = 705032704;
    i.bits[1] = 1;
    j.bits[0] = 705032704;
    j.bits[1] = 1;
    printf("Test 5: big == big → %d (expected 1)\n", s21_is_equal(i, j));

    // Test 6: bits[1] different → not equal
    s21_decimal k = {0};
    s21_decimal l = {0};
    k.bits[0] = 705032704;
    k.bits[1] = 1;
    l.bits[0] = 705032704;
    l.bits[1] = 2;
    printf("Test 6: big != bigger → %d (expected 0)\n", s21_is_equal(k, l));

    return 0;
}//202504