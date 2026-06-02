#include <stdio.h>

void print_bits_32(const char *label, unsigned int num) {
    printf("  %s = ", label);
    for (int i = 31; i >= 0; i--) {
        printf("%u", (num >> i) & 1);
        if (i % 8 == 0 && i > 0) printf(" ");
    }
    printf("  (decimal: %u)\n", num);
}

void print_bits_64(const char *label, unsigned long long num) {
    printf("  %s = ", label);
    for (int i = 63; i >= 0; i--) {
        printf("%llu", (num >> i) & 1);
        if (i % 8 == 0 && i > 0) printf(" ");
    }
    printf("  (decimal: %llu)\n", num);
}

int main() {
    unsigned int bits0 = 500000000;
    unsigned int bits1 = 3;
    unsigned int bits2 = 0;
    unsigned int multiplier = 10;

    printf("============================================\n");
    printf("TRACING YOUR | (OR) VERSION\n");
    printf("============================================\n\n");

    printf("STARTING VALUES:\n");
    print_bits_32("bits[0]", bits0);
    print_bits_32("bits[1]", bits1);
    print_bits_32("bits[2]", bits2);
    printf("  multiplier = %u\n", multiplier);
    printf("  Full number = %llu\n", (unsigned long long)bits1 * 4294967296ULL + bits0);
    printf("  Expected after *10 = %llu\n", ((unsigned long long)bits1 * 4294967296ULL + bits0) * 10);

    // Compute all three multiplications first (like your code does)
    unsigned long long num1 = (unsigned long long)bits0 * multiplier;
    unsigned long long num2 = (unsigned long long)bits1 * multiplier;
    unsigned long long num3 = (unsigned long long)bits2 * multiplier;

    printf("\n============================================\n");
    printf("MULTIPLICATIONS:\n");
    printf("============================================\n");
    print_bits_64("num1 (bits[0]*10)", num1);
    print_bits_64("num2 (bits[1]*10)", num2);
    print_bits_64("num3 (bits[2]*10)", num3);

    // =============================================
    // YOUR LINE: bits[0] = bits[0] | (num1 & 0xFFFFFFFF)
    // =============================================
    printf("\n============================================\n");
    printf("YOUR LINE: bits[0] = bits[0] | (num1 & 0xFFFFFFFF)\n");
    printf("============================================\n\n");

    unsigned int num1_lower = (unsigned int)(num1 & 0xFFFFFFFF);
    print_bits_32("bits[0] (old)     ", bits0);
    printf("  |\n");
    print_bits_32("num1 & 0xFFFFFFFF ", num1_lower);
    printf("  =\n");

    printf("\n  Let's see bit by bit where | goes wrong:\n\n");
    printf("  bits[0] = ");
    for (int i = 31; i >= 0; i--) {
        printf("%u", (bits0 >> i) & 1);
        if (i % 8 == 0 && i > 0) printf(" ");
    }
    printf("\n");
    printf("  OR         ");
    for (int i = 31; i >= 0; i--) {
        printf("%u", (num1_lower >> i) & 1);
        if (i % 8 == 0 && i > 0) printf(" ");
    }
    printf("\n");
    printf("             ");
    for (int i = 31; i >= 0; i--) {
        unsigned int b0 = (bits0 >> i) & 1;
        unsigned int b1 = (num1_lower >> i) & 1;
        if (b0 == 1 && b1 == 1) {
            printf("^");  // both 1: OR gives 1, but ADD would give 10 (carry!)
        } else {
            printf("-");
        }
        if (i % 8 == 0 && i > 0) printf(" ");
    }
    printf("  (^ = both are 1, OR loses the carry here!)\n\n");

    unsigned int or_result = bits0 | num1_lower;
    unsigned int add_result = bits0 + num1_lower;
    print_bits_32("bits[0] | num1    ", or_result);
    print_bits_32("bits[0] + num1    ", add_result);
    printf("\n  OR  result = %u\n", or_result);
    printf("  ADD result = %u\n", add_result);
    printf("  CORRECT    = %u  (just assign num1 lower, no old bits[0])\n", num1_lower);
    printf("\n  OR lost information at every ^ position!\n");
    printf("  When both bits are 1: OR gives 1, but real addition carries.\n");

    bits0 = or_result;  // continue with your buggy value

    // =============================================
    // YOUR LINE: bits[1] = (bits[1] | (num2 & 0xFFF)) | (num1 >> 32)
    // =============================================
    printf("\n============================================\n");
    printf("YOUR LINE: bits[1] = (bits[1] | (num2 & 0xFFF)) | (num1>>32)\n");
    printf("============================================\n\n");

    unsigned int num2_lower = (unsigned int)(num2 & 0xFFFFFFFF);
    unsigned int carry1 = (unsigned int)(num1 >> 32);

    printf("  Three values being OR'd together:\n\n");
    print_bits_32("bits[1] (old)     ", bits1);
    print_bits_32("num2 & 0xFFFFFFFF ", num2_lower);
    print_bits_32("carry (num1 >> 32)", carry1);

    printf("\n  Step by step:\n\n");

    printf("  bits[1]    = ");
    for (int i = 31; i >= 0; i--) {
        printf("%u", (bits1 >> i) & 1);
        if (i % 8 == 0 && i > 0) printf(" ");
    }
    printf("  = 3  (binary: 11)\n");

    printf("  num2 lower = ");
    for (int i = 31; i >= 0; i--) {
        printf("%u", (num2_lower >> i) & 1);
        if (i % 8 == 0 && i > 0) printf(" ");
    }
    printf("  = 30 (binary: 11110)\n");

    printf("  carry      = ");
    for (int i = 31; i >= 0; i--) {
        printf("%u", (carry1 >> i) & 1);
        if (i % 8 == 0 && i > 0) printf(" ");
    }
    printf("  = 1  (binary: 1)\n");

    printf("\n  ZOOM IN on the last 6 bits:\n\n");
    printf("    bits[1]    = ...0 00011  (3)\n");
    printf("    num2 lower = ...0 11110  (30)\n");
    printf("    carry      = ...0 00001  (1)\n");
    printf("    -------------------------\n");

    unsigned int or_bits1 = (bits1 | num2_lower) | carry1;
    unsigned int add_bits1 = num2_lower + carry1;  // correct: no old bits[1]

    printf("    OR  all    = ...0 11111  = %u\n", or_bits1);
    printf("    CORRECT    = ...0 11111  = %u\n", add_bits1);
    printf("\n  LUCKY! They match here because no bits overlap.\n");
    printf("  But this is ONLY because 3 = 11 and 30 = 11110 share bit 1.\n");
    printf("  3 | 30 = 11 | 11110 = 11111 = 31, and 3 is inside 30 already.\n");

    printf("\n  Let's try with bits[1] = 5 instead of 3:\n\n");
    unsigned int test_bits1 = 5;
    unsigned int test_num2 = 50;  // 5 * 10
    unsigned int test_carry = 1;
    printf("    bits[1]  = 5  = ...000101\n");
    printf("    5 * 10   = 50 = ...110010\n");
    printf("    carry    = 1  = ...000001\n");
    printf("    -------------------------\n");
    printf("    OR  all  = %u | %u | %u = %u\n", test_bits1, test_num2, test_carry, test_bits1 | test_num2 | test_carry);
    printf("    ADD corr = %u + %u = %u\n", test_num2, test_carry, test_num2 + test_carry);
    printf("    ^ = %u vs %u  DIFFERENT! OR fails here.\n\n", test_bits1 | test_num2 | test_carry, test_num2 + test_carry);

    printf("    Why? Because 5 = 101 and 50 = 110010\n");
    printf("    bit 0: 5 has 1, 50 has 0, carry has 1 -> OR = 1, ADD = 10 (carry!)\n");
    printf("    OR can't carry, so it loses information.\n");

    // =============================================
    // FINAL COMPARISON
    // =============================================
    printf("\n============================================\n");
    printf("FINAL: WHY | FAILS\n");
    printf("============================================\n\n");
    printf("  | (OR) rule:  1 | 1 = 1   (merges, no carry)\n");
    printf("  + (ADD) rule: 1 + 1 = 10  (carries to next bit)\n\n");
    printf("  OR sometimes gives the right answer BY LUCK\n");
    printf("  (when no bits overlap). But it's not reliable.\n\n");
    printf("  Also: you're OR-ing with old bits[N] values,\n");
    printf("  but num1/num2/num3 already contain bits[N]*10.\n");
    printf("  So you're double-counting, same as the + version.\n\n");
    printf("  CORRECT approach: just ASSIGN, only + the carry:\n");
    printf("    bits[0] = num1 & 0xFFFFFFFF\n");
    printf("    bits[1] = (num2 & 0xFFFFFFFF) + (num1 >> 32)\n");
    printf("    bits[2] = (num3 & 0xFFFFFFFF) + (num2 >> 32)\n");

    return 0;
}