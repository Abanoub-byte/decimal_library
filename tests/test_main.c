#include <check.h>
#include <limits.h>
#include "../s21_decimal.h"
#include "../s21_helpers.h"

// ==========================================
// HELPER FUNCTION TESTS
// ==========================================

// --- set_bit / get_bit_value ---

START_TEST(test_set_bit_0_to_1) {
    s21_decimal d = {0};
    set_bit(&d, 0, 1);
    ck_assert_int_eq(get_bit_value(d, 0), 1);
}
END_TEST

START_TEST(test_set_bit_31_to_1) {
    s21_decimal d = {0};
    set_bit(&d, 31, 1);
    ck_assert_int_eq(get_bit_value(d, 31), 1);
}
END_TEST

START_TEST(test_set_bit_32_to_1) {
    s21_decimal d = {0};
    set_bit(&d, 32, 1);
    ck_assert_int_eq(get_bit_value(d, 32), 1);
    ck_assert_int_eq(d.bits[1], 1);
}
END_TEST

START_TEST(test_set_bit_95_to_1) {
    s21_decimal d = {0};
    set_bit(&d, 95, 1);
    ck_assert_int_eq(get_bit_value(d, 95), 1);
}
END_TEST

START_TEST(test_set_bit_clear) {
    s21_decimal d = {0};
    set_bit(&d, 5, 1);
    ck_assert_int_eq(get_bit_value(d, 5), 1);
    set_bit(&d, 5, 0);
    ck_assert_int_eq(get_bit_value(d, 5), 0);
}
END_TEST

START_TEST(test_set_bit_doesnt_affect_others) {
    s21_decimal d = {0};
    d.bits[0] = 13;  // binary 1101
    set_bit(&d, 4, 1);
    ck_assert_int_eq(d.bits[0], 29);  // binary 11101
}
END_TEST

// --- get_bit_position ---

START_TEST(test_get_bit_position_0) {
    bitPosition pos = get_bit_position(0);
    ck_assert_int_eq(pos.index, 0);
    ck_assert_int_eq(pos.localPosition, 0);
}
END_TEST

START_TEST(test_get_bit_position_47) {
    bitPosition pos = get_bit_position(47);
    ck_assert_int_eq(pos.index, 1);
    ck_assert_int_eq(pos.localPosition, 15);
}
END_TEST

START_TEST(test_get_bit_position_95) {
    bitPosition pos = get_bit_position(95);
    ck_assert_int_eq(pos.index, 2);
    ck_assert_int_eq(pos.localPosition, 31);
}
END_TEST

// --- set_sign / get_sign ---

START_TEST(test_set_sign_positive) {
    s21_decimal d = {0};
    s21_set_sign(&d, 0);
    ck_assert_int_eq(s21_get_sign(d), 0);
}
END_TEST

START_TEST(test_set_sign_negative) {
    s21_decimal d = {0};
    s21_set_sign(&d, 1);
    ck_assert_int_eq(s21_get_sign(d), 1);
}
END_TEST

START_TEST(test_set_sign_toggle) {
    s21_decimal d = {0};
    s21_set_sign(&d, 1);
    ck_assert_int_eq(s21_get_sign(d), 1);
    s21_set_sign(&d, 0);
    ck_assert_int_eq(s21_get_sign(d), 0);
}
END_TEST

START_TEST(test_set_sign_doesnt_affect_scale) {
    s21_decimal d = {0};
    set_scale(&d, 5);
    s21_set_sign(&d, 1);
    ck_assert_int_eq(get_scale(d), 5);
    ck_assert_int_eq(s21_get_sign(d), 1);
}
END_TEST

// --- set_scale / get_scale ---

START_TEST(test_set_scale_0) {
    s21_decimal d = {0};
    set_scale(&d, 0);
    ck_assert_int_eq(get_scale(d), 0);
}
END_TEST

START_TEST(test_set_scale_5) {
    s21_decimal d = {0};
    set_scale(&d, 5);
    ck_assert_int_eq(get_scale(d), 5);
}
END_TEST

START_TEST(test_set_scale_28) {
    s21_decimal d = {0};
    set_scale(&d, 28);
    ck_assert_int_eq(get_scale(d), 28);
}
END_TEST

START_TEST(test_set_scale_doesnt_affect_sign) {
    s21_decimal d = {0};
    s21_set_sign(&d, 1);
    set_scale(&d, 10);
    ck_assert_int_eq(s21_get_sign(d), 1);
    ck_assert_int_eq(get_scale(d), 10);
}
END_TEST

START_TEST(test_set_scale_replace) {
    s21_decimal d = {0};
    set_scale(&d, 5);
    set_scale(&d, 10);
    ck_assert_int_eq(get_scale(d), 10);
}
END_TEST

// --- multiply ---

START_TEST(test_multiply_simple) {
    s21_decimal d = {0};
    d.bits[0] = 5;
    int err = multiply(&d, 10);
    ck_assert_int_eq(err, 0);
    ck_assert_uint_eq(d.bits[0], 50);
}
END_TEST

START_TEST(test_multiply_overflow_to_bits1) {
    s21_decimal d = {0};
    d.bits[0] = 500000000;
    int err = multiply(&d, 10);
    ck_assert_int_eq(err, 0);
    ck_assert_uint_eq(d.bits[0], 705032704);
    ck_assert_uint_eq(d.bits[1], 1);
}
END_TEST

START_TEST(test_multiply_with_bits1) {
    s21_decimal d = {0};
    d.bits[0] = 500000000;
    d.bits[1] = 3;
    int err = multiply(&d, 10);
    ck_assert_int_eq(err, 0);
    // 3 * 2^32 + 500000000 = 13384901888
    // * 10 = 133849018880
    // bits[1] = 133849018880 / 2^32 = 31
    // bits[0] = 133849018880 % 2^32 = 755032704
    ck_assert_uint_eq(d.bits[1], 31);
}
END_TEST

START_TEST(test_multiply_by_1) {
    s21_decimal d = {0};
    d.bits[0] = 42;
    multiply(&d, 1);
    ck_assert_uint_eq(d.bits[0], 42);
}
END_TEST

START_TEST(test_multiply_zero) {
    s21_decimal d = {0};
    d.bits[0] = 42;
    multiply(&d, 0);
    ck_assert_uint_eq(d.bits[0], 0);
}
END_TEST

// ==========================================
// NEGATE TESTS
// ==========================================

START_TEST(test_negate_positive_to_negative) {
    s21_decimal a = {0};
    s21_decimal result = {0};
    a.bits[0] = 42;
    s21_negate(a, &result);
    ck_assert_int_eq(s21_get_sign(result), 1);
    ck_assert_uint_eq(result.bits[0], 42);
}
END_TEST

START_TEST(test_negate_negative_to_positive) {
    s21_decimal a = {0};
    s21_decimal result = {0};
    a.bits[0] = 42;
    s21_set_sign(&a, 1);
    s21_negate(a, &result);
    ck_assert_int_eq(s21_get_sign(result), 0);
    ck_assert_uint_eq(result.bits[0], 42);
}
END_TEST

START_TEST(test_negate_zero) {
    s21_decimal a = {0};
    s21_decimal result = {0};
    s21_negate(a, &result);
    ck_assert_int_eq(s21_get_sign(result), 1);
}
END_TEST

START_TEST(test_negate_preserves_scale) {
    s21_decimal a = {0};
    s21_decimal result = {0};
    a.bits[0] = 314;
    set_scale(&a, 2);
    s21_negate(a, &result);
    ck_assert_int_eq(get_scale(result), 2);
    ck_assert_uint_eq(result.bits[0], 314);
}
END_TEST

START_TEST(test_negate_return_code) {
    s21_decimal a = {0};
    s21_decimal result = {0};
    int err = s21_negate(a, &result);
    ck_assert_int_eq(err, 0);
}
END_TEST

// ==========================================
// IS_EQUAL TESTS
// ==========================================

START_TEST(test_equal_same_number) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 314;
    b.bits[0] = 314;
    set_scale(&a, 2);
    set_scale(&b, 2);
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_equal_different_number) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 314;
    b.bits[0] = 315;
    set_scale(&a, 2);
    set_scale(&b, 2);
    ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_equal_different_sign) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 42;
    b.bits[0] = 42;
    s21_set_sign(&b, 1);
    ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

START_TEST(test_equal_positive_zero_negative_zero) {
    s21_decimal a = {0}, b = {0};
    s21_set_sign(&b, 1);
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_equal_different_scale_same_value) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 314;
    b.bits[0] = 3140;
    set_scale(&a, 2);
    set_scale(&b, 3);
    // 314/100 = 3.14, 3140/1000 = 3.14
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_equal_big_numbers) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 705032704;
    a.bits[1] = 1;
    b.bits[0] = 705032704;
    b.bits[1] = 1;
    ck_assert_int_eq(s21_is_equal(a, b), 1);
}
END_TEST

START_TEST(test_equal_big_numbers_different) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 705032704;
    a.bits[1] = 1;
    b.bits[0] = 705032704;
    b.bits[1] = 2;
    ck_assert_int_eq(s21_is_equal(a, b), 0);
}
END_TEST

// ==========================================
// IS_NOT_EQUAL TESTS
// ==========================================

START_TEST(test_not_equal_same) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 42;
    b.bits[0] = 42;
    ck_assert_int_eq(s21_is_not_equal(a, b), 0);
}
END_TEST

START_TEST(test_not_equal_different) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 42;
    b.bits[0] = 43;
    ck_assert_int_eq(s21_is_not_equal(a, b), 1);
}
END_TEST

// ==========================================
// IS_LESS TESTS
// ==========================================

START_TEST(test_less_positive) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 3;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_less(a, b), 1);
}
END_TEST

START_TEST(test_less_positive_reverse) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 5;
    b.bits[0] = 3;
    ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_less_equal_numbers) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 5;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_less_negative_vs_positive) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 5;
    s21_set_sign(&a, 1);  // -5
    b.bits[0] = 3;        // +3
    ck_assert_int_eq(s21_is_less(a, b), 1);
}
END_TEST

START_TEST(test_less_positive_vs_negative) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 3;        // +3
    b.bits[0] = 5;
    s21_set_sign(&b, 1);  // -5
    ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_less_both_negative) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 100;
    s21_set_sign(&a, 1);  // -100
    b.bits[0] = 5;
    s21_set_sign(&b, 1);  // -5
    // -100 < -5 = true
    ck_assert_int_eq(s21_is_less(a, b), 1);
}
END_TEST

START_TEST(test_less_both_negative_reverse) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 5;
    s21_set_sign(&a, 1);  // -5
    b.bits[0] = 100;
    s21_set_sign(&b, 1);  // -100
    // -5 < -100 = false
    ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_less_zero_not_less_than_zero) {
    s21_decimal a = {0}, b = {0};
    ck_assert_int_eq(s21_is_less(a, b), 0);
}
END_TEST

START_TEST(test_less_with_scale) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 314;
    set_scale(&a, 2);  // 3.14
    b.bits[0] = 315;
    set_scale(&b, 2);  // 3.15
    ck_assert_int_eq(s21_is_less(a, b), 1);
}
END_TEST

// ==========================================
// IS_LESS_OR_EQUAL TESTS
// ==========================================

START_TEST(test_less_or_equal_less) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 3;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_less_or_equal_equal) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 5;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_less_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_less_or_equal_greater) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 7;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_less_or_equal(a, b), 0);
}
END_TEST

// ==========================================
// IS_GREATER TESTS
// ==========================================

START_TEST(test_greater_true) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 7;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_greater(a, b), 1);
}
END_TEST

START_TEST(test_greater_false) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 3;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

START_TEST(test_greater_equal) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 5;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_greater(a, b), 0);
}
END_TEST

// ==========================================
// IS_GREATER_OR_EQUAL TESTS
// ==========================================

START_TEST(test_greater_or_equal_greater) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 7;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_equal) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 5;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_greater_or_equal(a, b), 1);
}
END_TEST

START_TEST(test_greater_or_equal_less) {
    s21_decimal a = {0}, b = {0};
    a.bits[0] = 3;
    b.bits[0] = 5;
    ck_assert_int_eq(s21_is_greater_or_equal(a, b), 0);
}
END_TEST

// ==========================================
// FROM_INT_TO_DECIMAL TESTS
// ==========================================

START_TEST(test_int_to_decimal_positive) {
    s21_decimal d;
    int err = s21_from_int_to_decimal(42, &d);
    ck_assert_int_eq(err, 0);
    ck_assert_uint_eq(d.bits[0], 42);
    ck_assert_int_eq(s21_get_sign(d), 0);
}
END_TEST

START_TEST(test_int_to_decimal_negative) {
    s21_decimal d;
    int err = s21_from_int_to_decimal(-42, &d);
    ck_assert_int_eq(err, 0);
    ck_assert_uint_eq(d.bits[0], 42);
    ck_assert_int_eq(s21_get_sign(d), 1);
}
END_TEST

START_TEST(test_int_to_decimal_zero) {
    s21_decimal d;
    s21_from_int_to_decimal(0, &d);
    ck_assert_uint_eq(d.bits[0], 0);
    ck_assert_int_eq(s21_get_sign(d), 0);
}
END_TEST

START_TEST(test_int_to_decimal_max) {
    s21_decimal d;
    s21_from_int_to_decimal(INT_MAX, &d);
    ck_assert_uint_eq(d.bits[0], (unsigned int)INT_MAX);
    ck_assert_int_eq(s21_get_sign(d), 0);
}
END_TEST

START_TEST(test_int_to_decimal_min) {
    s21_decimal d;
    s21_from_int_to_decimal(INT_MIN, &d);
    ck_assert_uint_eq(d.bits[0], (unsigned int)2147483648u);
    ck_assert_int_eq(s21_get_sign(d), 1);
}
END_TEST

START_TEST(test_int_to_decimal_scale_is_zero) {
    s21_decimal d;
    s21_from_int_to_decimal(123, &d);
    ck_assert_int_eq(get_scale(d), 0);
}
END_TEST


START_TEST(test_set_bit_boundary_64) {
    // bit 64 = first bit of bits[2]
    s21_decimal d = {0};
    set_bit(&d, 64, 1);
    ck_assert_uint_eq(d.bits[2], 1);
    ck_assert_uint_eq(d.bits[1], 0);  // shouldn't touch bits[1]
}
END_TEST

START_TEST(test_set_bit_multiple_without_overwrite) {
    // setting bit 3 shouldn't erase bit 0
    s21_decimal d = {0};
    set_bit(&d, 0, 1);
    set_bit(&d, 3, 1);
    ck_assert_int_eq(get_bit_value(d, 0), 1);  // still there?
    ck_assert_int_eq(get_bit_value(d, 3), 1);
}
END_TEST

START_TEST(test_clear_bit_doesnt_affect_neighbors) {
    s21_decimal d = {0};
    d.bits[0] = 7;  // binary: 111
    set_bit(&d, 1, 0);  // clear middle bit
    ck_assert_uint_eq(d.bits[0], 5);  // binary: 101
}
END_TEST

// --- set_sign tricky cases ---

START_TEST(test_sign_doesnt_affect_mantissa) {
    s21_decimal d = {0};
    d.bits[0] = 12345;
    d.bits[1] = 67890;
    s21_set_sign(&d, 1);
    ck_assert_uint_eq(d.bits[0], 12345);
    ck_assert_uint_eq(d.bits[1], 67890);
}
END_TEST

START_TEST(test_sign_doesnt_affect_scale) {
    s21_decimal d = {0};
    set_scale(&d, 15);
    s21_set_sign(&d, 1);
    ck_assert_int_eq(get_scale(d), 15);
    s21_set_sign(&d, 0);
    ck_assert_int_eq(get_scale(d), 15);
}
END_TEST

// --- set_scale tricky cases ---

START_TEST(test_scale_doesnt_affect_mantissa) {
    s21_decimal d = {0};
    d.bits[0] = 999999;
    d.bits[1] = 888888;
    set_scale(&d, 20);
    ck_assert_uint_eq(d.bits[0], 999999);
    ck_assert_uint_eq(d.bits[1], 888888);
}
END_TEST

START_TEST(test_scale_overwrite_old_value) {
    // make sure old scale is fully erased
    s21_decimal d = {0};
    set_scale(&d, 28);  // 28 = 00011100
    set_scale(&d, 1);   // 1  = 00000001
    ck_assert_int_eq(get_scale(d), 1);  // not 29 or something weird
}
END_TEST

START_TEST(test_scale_and_sign_together) {
    s21_decimal d = {0};
    s21_set_sign(&d, 1);
    set_scale(&d, 14);
    ck_assert_int_eq(s21_get_sign(d), 1);
    ck_assert_int_eq(get_scale(d), 14);
    // now change both
    s21_set_sign(&d, 0);
    set_scale(&d, 7);
    ck_assert_int_eq(s21_get_sign(d), 0);
    ck_assert_int_eq(get_scale(d), 7);
}
END_TEST

// --- multiply tricky cases ---

START_TEST(test_multiply_max_bits0) {
    // multiply max uint32 by 2, should overflow to bits[1]
    s21_decimal d = {0};
    d.bits[0] = 4294967295u;  // 0xFFFFFFFF
    multiply(&d, 2);
    ck_assert_uint_eq(d.bits[0], 4294967294u);  // 0xFFFFFFFE
    ck_assert_uint_eq(d.bits[1], 1);
}
END_TEST

START_TEST(test_multiply_chain_overflow) {
    // number that overflows from bits[0] to bits[1] to bits[2]
    s21_decimal d = {0};
    d.bits[0] = 4294967295u;
    d.bits[1] = 4294967295u;
    int err = multiply(&d, 2);
    ck_assert_int_eq(err, 0);
    ck_assert_uint_eq(d.bits[2], 1);
}
END_TEST

START_TEST(test_multiply_overflow_error) {
    // bits[2] is max, multiplying should overflow
    s21_decimal d = {0};
    d.bits[2] = 4294967295u;
    int err = multiply(&d, 2);
    ck_assert_int_eq(err, 1);  // should return error
}
END_TEST

// --- get_scale_from_string ---

START_TEST(test_scale_from_string_no_dot) {
    ck_assert_int_eq(get_scale_from_string("12345"), 0);
}
END_TEST

START_TEST(test_scale_from_string_one_decimal) {
    ck_assert_int_eq(get_scale_from_string("3.5"), 1);
}
END_TEST

START_TEST(test_scale_from_string_many_decimals) {
    ck_assert_int_eq(get_scale_from_string("3.14159"), 5);
}
END_TEST

START_TEST(test_scale_from_string_just_dot) {
    ck_assert_int_eq(get_scale_from_string("0.0"), 1);
}
END_TEST

// --- get_float_sign / get_double_sign ---

START_TEST(test_float_sign_positive) {
    ck_assert_int_eq(get_float_sign(3.14f), 0);
}
END_TEST

START_TEST(test_float_sign_negative) {
    ck_assert_int_eq(get_float_sign(-3.14f), 1);
}
END_TEST

START_TEST(test_float_sign_zero) {
    ck_assert_int_eq(get_float_sign(0.0f), 0);
}
END_TEST

START_TEST(test_float_sign_negative_zero) {
    ck_assert_int_eq(get_float_sign(-0.0f), 1);
}
END_TEST

START_TEST(test_double_sign_positive) {
    ck_assert_int_eq(get_double_sign(2.71), 0);
}
END_TEST

START_TEST(test_double_sign_negative) {
    ck_assert_int_eq(get_double_sign(-2.71), 1);
}
END_TEST

// --- float_to_string ---

START_TEST(test_float_to_string_simple) {
    char str[50];
    float_to_string(3.14f, str, 50);
    ck_assert_str_eq(str, "3.14");
}
END_TEST

START_TEST(test_float_to_string_integer) {
    char str[50];
    float_to_string(100.0f, str, 50);
    ck_assert_str_eq(str, "100");
}
END_TEST

START_TEST(test_float_to_string_negative) {
    char str[50];
    float_to_string(-5.5f, str, 50);
    ck_assert_str_eq(str, "-5.5");
}
END_TEST

// --- negate tricky ---

START_TEST(test_negate_double_negate) {
    s21_decimal a = {0}, r1 = {0}, r2 = {0};
    a.bits[0] = 42;
    s21_negate(a, &r1);   // +42 → -42
    s21_negate(r1, &r2);  // -42 → +42
    ck_assert_int_eq(s21_get_sign(r2), 0);
    ck_assert_uint_eq(r2.bits[0], 42);
}
END_TEST

START_TEST(test_negate_doesnt_modify_original) {
    s21_decimal a = {0}, result = {0};
    a.bits[0] = 100;
    s21_negate(a, &result);
    // original should be unchanged
    ck_assert_int_eq(s21_get_sign(a), 0);
    ck_assert_uint_eq(a.bits[0], 100);
}
END_TEST

// ==========================================
// SUITE SETUP
// ==========================================

Suite *helpers_suite(void) {
    Suite *s = suite_create("Helpers");

    TCase *tc_bit = tcase_create("set_bit/get_bit");
    tcase_add_test(tc_bit, test_set_bit_0_to_1);
    tcase_add_test(tc_bit, test_set_bit_31_to_1);
    tcase_add_test(tc_bit, test_set_bit_32_to_1);
    tcase_add_test(tc_bit, test_set_bit_95_to_1);
    tcase_add_test(tc_bit, test_set_bit_clear);
    tcase_add_test(tc_bit, test_set_bit_doesnt_affect_others);
    suite_add_tcase(s, tc_bit);

    TCase *tc_pos = tcase_create("get_bit_position");
    tcase_add_test(tc_pos, test_get_bit_position_0);
    tcase_add_test(tc_pos, test_get_bit_position_47);
    tcase_add_test(tc_pos, test_get_bit_position_95);
    suite_add_tcase(s, tc_pos);

    TCase *tc_sign = tcase_create("sign");
    tcase_add_test(tc_sign, test_set_sign_positive);
    tcase_add_test(tc_sign, test_set_sign_negative);
    tcase_add_test(tc_sign, test_set_sign_toggle);
    tcase_add_test(tc_sign, test_set_sign_doesnt_affect_scale);
    suite_add_tcase(s, tc_sign);

    TCase *tc_scale = tcase_create("scale");
    tcase_add_test(tc_scale, test_set_scale_0);
    tcase_add_test(tc_scale, test_set_scale_5);
    tcase_add_test(tc_scale, test_set_scale_28);
    tcase_add_test(tc_scale, test_set_scale_doesnt_affect_sign);
    tcase_add_test(tc_scale, test_set_scale_replace);
    suite_add_tcase(s, tc_scale);

    TCase *tc_mul = tcase_create("multiply");
    tcase_add_test(tc_mul, test_multiply_simple);
    tcase_add_test(tc_mul, test_multiply_overflow_to_bits1);
    tcase_add_test(tc_mul, test_multiply_with_bits1);
    tcase_add_test(tc_mul, test_multiply_by_1);
    tcase_add_test(tc_mul, test_multiply_zero);
    suite_add_tcase(s, tc_mul);

    return s;
}

Suite *negate_suite(void) {
    Suite *s = suite_create("Negate");

    TCase *tc = tcase_create("negate");
    tcase_add_test(tc, test_negate_positive_to_negative);
    tcase_add_test(tc, test_negate_negative_to_positive);
    tcase_add_test(tc, test_negate_zero);
    tcase_add_test(tc, test_negate_preserves_scale);
    tcase_add_test(tc, test_negate_return_code);
    suite_add_tcase(s, tc);

    return s;
}

Suite *comparison_suite(void) {
    Suite *s = suite_create("Comparison");

    TCase *tc_eq = tcase_create("is_equal");
    tcase_add_test(tc_eq, test_equal_same_number);
    tcase_add_test(tc_eq, test_equal_different_number);
    tcase_add_test(tc_eq, test_equal_different_sign);
    tcase_add_test(tc_eq, test_equal_positive_zero_negative_zero);
    tcase_add_test(tc_eq, test_equal_different_scale_same_value);
    tcase_add_test(tc_eq, test_equal_big_numbers);
    tcase_add_test(tc_eq, test_equal_big_numbers_different);
    suite_add_tcase(s, tc_eq);

    TCase *tc_neq = tcase_create("is_not_equal");
    tcase_add_test(tc_neq, test_not_equal_same);
    tcase_add_test(tc_neq, test_not_equal_different);
    suite_add_tcase(s, tc_neq);

    TCase *tc_less = tcase_create("is_less");
    tcase_add_test(tc_less, test_less_positive);
    tcase_add_test(tc_less, test_less_positive_reverse);
    tcase_add_test(tc_less, test_less_equal_numbers);
    tcase_add_test(tc_less, test_less_negative_vs_positive);
    tcase_add_test(tc_less, test_less_positive_vs_negative);
    tcase_add_test(tc_less, test_less_both_negative);
    tcase_add_test(tc_less, test_less_both_negative_reverse);
    tcase_add_test(tc_less, test_less_zero_not_less_than_zero);
    tcase_add_test(tc_less, test_less_with_scale);
    suite_add_tcase(s, tc_less);

    TCase *tc_leq = tcase_create("is_less_or_equal");
    tcase_add_test(tc_leq, test_less_or_equal_less);
    tcase_add_test(tc_leq, test_less_or_equal_equal);
    tcase_add_test(tc_leq, test_less_or_equal_greater);
    suite_add_tcase(s, tc_leq);

    TCase *tc_gt = tcase_create("is_greater");
    tcase_add_test(tc_gt, test_greater_true);
    tcase_add_test(tc_gt, test_greater_false);
    tcase_add_test(tc_gt, test_greater_equal);
    suite_add_tcase(s, tc_gt);

    TCase *tc_geq = tcase_create("is_greater_or_equal");
    tcase_add_test(tc_geq, test_greater_or_equal_greater);
    tcase_add_test(tc_geq, test_greater_or_equal_equal);
    tcase_add_test(tc_geq, test_greater_or_equal_less);
    suite_add_tcase(s, tc_geq);

    return s;
}

Suite *converters_suite(void) {
    Suite *s = suite_create("Converters");

    TCase *tc_itd = tcase_create("int_to_decimal");
    tcase_add_test(tc_itd, test_int_to_decimal_positive);
    tcase_add_test(tc_itd, test_int_to_decimal_negative);
    tcase_add_test(tc_itd, test_int_to_decimal_zero);
    tcase_add_test(tc_itd, test_int_to_decimal_max);
    tcase_add_test(tc_itd, test_int_to_decimal_min);
    tcase_add_test(tc_itd, test_int_to_decimal_scale_is_zero);
    suite_add_tcase(s, tc_itd);

    return s;
}
Suite *helpers_tricky_suite(void) {
    Suite *s = suite_create("Helpers Tricky");
    
    TCase *tc = tcase_create("edge_cases");
    tcase_add_test(tc, test_set_bit_boundary_64);
    tcase_add_test(tc, test_set_bit_multiple_without_overwrite);
    tcase_add_test(tc, test_clear_bit_doesnt_affect_neighbors);
    tcase_add_test(tc, test_sign_doesnt_affect_mantissa);
    tcase_add_test(tc, test_sign_doesnt_affect_scale);
    tcase_add_test(tc, test_scale_doesnt_affect_mantissa);
    tcase_add_test(tc, test_scale_overwrite_old_value);
    tcase_add_test(tc, test_scale_and_sign_together);
    tcase_add_test(tc, test_multiply_max_bits0);
    tcase_add_test(tc, test_multiply_chain_overflow);
    tcase_add_test(tc, test_multiply_overflow_error);
    tcase_add_test(tc, test_scale_from_string_no_dot);
    tcase_add_test(tc, test_scale_from_string_one_decimal);
    tcase_add_test(tc, test_scale_from_string_many_decimals);
    tcase_add_test(tc, test_scale_from_string_just_dot);
    tcase_add_test(tc, test_float_sign_positive);
    tcase_add_test(tc, test_float_sign_negative);
    tcase_add_test(tc, test_float_sign_zero);
    tcase_add_test(tc, test_float_sign_negative_zero);
    tcase_add_test(tc, test_double_sign_positive);
    tcase_add_test(tc, test_double_sign_negative);
    tcase_add_test(tc, test_float_to_string_simple);
    tcase_add_test(tc, test_float_to_string_integer);
    tcase_add_test(tc, test_float_to_string_negative);
    tcase_add_test(tc, test_negate_double_negate);
    tcase_add_test(tc, test_negate_doesnt_modify_original);
    suite_add_tcase(s, tc);
    
    return s;
}
int main(void) {
    int number_failed = 0;
    SRunner *sr;

    sr = srunner_create(helpers_suite());
    srunner_add_suite(sr, negate_suite());
    srunner_add_suite(sr, comparison_suite());
    srunner_add_suite(sr, converters_suite());
    srunner_add_suite(sr, helpers_tricky_suite());
    srunner_run_all(sr, CK_VERBOSE);
    number_failed = srunner_ntests_failed(sr);
    srunner_free(sr);

    return (number_failed == 0) ? 0 : 1;
}