#include <criterion/criterion.h>
#include <elash/util/int128.h>
#include <stdbool.h>
#include <stdint.h>

TestSuite(el_int128);

// Macro helper for equality assertion in Criterion
#define cr_assert_i128_eq(A, B) \
    cr_assert(el_i128_eq((A), (B)), "Expected 0x%016llx%016llx, got 0x%016llx%016llx", \
              (unsigned long long)el_i128_hi(B), (unsigned long long)el_i128_lo(B), \
              (unsigned long long)el_i128_hi(A), (unsigned long long)el_i128_lo(A))

Test(el_int128, constructors_and_accessors) {
    ElInt128 zero = EL_INT128(0);
    ElInt128 one = EL_INT128(1);
    ElInt128 neg_one = EL_INT128(-1);
    ElInt128 max_val = INT128_MAX;
    ElInt128 min_val = INT128_MIN;
    ElInt128 custom = EL_INT128_H(0x123456789ABCDEF0ULL, 0x7FEDCBA987654321ULL);

    cr_assert_eq(el_i128_lo(zero), 0ULL);
    cr_assert_eq(el_i128_hi(zero), 0ULL);

    cr_assert_eq(el_i128_lo(one), 1ULL);
    cr_assert_eq(el_i128_hi(one), 0ULL);

    cr_assert_eq(el_i128_lo(neg_one), 0xFFFFFFFFFFFFFFFFULL);
    cr_assert_eq(el_i128_hi(neg_one), 0xFFFFFFFFFFFFFFFFULL);

    cr_assert_eq(el_i128_lo(max_val), 0xFFFFFFFFFFFFFFFFULL);
    cr_assert_eq(el_i128_hi(max_val), 0x7FFFFFFFFFFFFFFFULL);

    cr_assert_eq(el_i128_lo(min_val), 0ULL);
    cr_assert_eq(el_i128_hi(min_val), 0x8000000000000000ULL);

    cr_assert_eq(el_i128_lo(custom), 0x123456789ABCDEF0ULL);
    cr_assert_eq(el_i128_hi(custom), 0x7FEDCBA987654321ULL);
}

Test(el_int128, equality_and_inequality) {
    ElInt128 zero = EL_INT128(0);
    ElInt128 one = EL_INT128(1);
    ElInt128 neg_one = EL_INT128(-1);
    ElInt128 max_val = INT128_MAX;
    ElInt128 min_val = INT128_MIN;

    cr_assert(el_i128_eq(zero, EL_INT128(0)));
    cr_assert(el_i128_eq(neg_one, EL_INT128(-1)));
    cr_assert(el_i128_eq(max_val, INT128_MAX));
    cr_assert(el_i128_eq(min_val, INT128_MIN));

    cr_assert(!el_i128_eq(zero, one));
    cr_assert(!el_i128_eq(min_val, max_val));

    cr_assert(el_i128_ne(zero, one));
    cr_assert(el_i128_ne(min_val, max_val));
    cr_assert(!el_i128_ne(zero, zero));
}

Test(el_int128, comparison) {
    ElInt128 min_val = INT128_MIN;
    ElInt128 neg_two = EL_INT128(-2);
    ElInt128 neg_one = EL_INT128(-1);
    ElInt128 zero = EL_INT128(0);
    ElInt128 one = EL_INT128(1);
    ElInt128 max_val = INT128_MAX;

    // Less than (lt)
    cr_assert(el_i128_lt(min_val, neg_two));
    cr_assert(el_i128_lt(neg_two, neg_one));
    cr_assert(el_i128_lt(neg_one, zero));
    cr_assert(el_i128_lt(zero, one));
    cr_assert(el_i128_lt(one, max_val));
    cr_assert(el_i128_lt(min_val, max_val));

    cr_assert(!el_i128_lt(zero, zero));
    cr_assert(!el_i128_lt(zero, neg_one));
    cr_assert(!el_i128_lt(max_val, min_val));

    // Greater than (gt)
    cr_assert(el_i128_gt(max_val, one));
    cr_assert(el_i128_gt(one, zero));
    cr_assert(el_i128_gt(zero, neg_one));
    cr_assert(el_i128_gt(neg_one, neg_two));
    cr_assert(el_i128_gt(neg_two, min_val));

    cr_assert(!el_i128_gt(zero, zero));
    cr_assert(!el_i128_gt(neg_one, zero));

    // Less than or equal (le)
    cr_assert(el_i128_le(zero, zero));
    cr_assert(el_i128_le(min_val, min_val));
    cr_assert(el_i128_le(max_val, max_val));
    cr_assert(el_i128_le(neg_one, zero));
    cr_assert(el_i128_le(min_val, max_val));
    cr_assert(!el_i128_le(one, zero));

    // Greater than or equal (ge)
    cr_assert(el_i128_ge(zero, zero));
    cr_assert(el_i128_ge(min_val, min_val));
    cr_assert(el_i128_ge(max_val, max_val));
    cr_assert(el_i128_ge(zero, neg_one));
    cr_assert(el_i128_ge(max_val, min_val));
    cr_assert(!el_i128_ge(zero, one));
}

Test(el_int128, addition) {
    ElInt128 zero = EL_INT128(0);
    ElInt128 one = EL_INT128(1);
    ElInt128 neg_one = EL_INT128(-1);
    ElInt128 max_val = INT128_MAX;
    ElInt128 min_val = INT128_MIN;

    cr_assert_i128_eq(el_i128_add(zero, zero), zero);
    cr_assert_i128_eq(el_i128_add(one, neg_one), zero);
    cr_assert_i128_eq(el_i128_add(EL_INT128(100), EL_INT128(250)), EL_INT128(350));
    cr_assert_i128_eq(el_i128_add(EL_INT128(-100), EL_INT128(-250)), EL_INT128(-350));

    // 64-bit boundary carry
    ElInt128 hi_boundary = EL_INT128_H(0xFFFFFFFFFFFFFFFFULL, 0);
    ElInt128 carried = el_i128_add(hi_boundary, one);
    cr_assert_i128_eq(carried, EL_INT128_H(0, 1));

    // Overflow wrap-around (INT128_MAX + 1 == INT128_MIN)
    cr_assert_i128_eq(el_i128_add(max_val, one), min_val);

    // INT128_MAX + INT128_MAX == -2
    cr_assert_i128_eq(el_i128_add(max_val, max_val), EL_INT128(-2));
}

Test(el_int128, subtraction) {
    ElInt128 zero = EL_INT128(0);
    ElInt128 one = EL_INT128(1);
    ElInt128 neg_one = EL_INT128(-1);
    ElInt128 max_val = INT128_MAX;
    ElInt128 min_val = INT128_MIN;

    cr_assert_i128_eq(el_i128_sub(zero, zero), zero);
    cr_assert_i128_eq(el_i128_sub(one, one), zero);
    cr_assert_i128_eq(el_i128_sub(zero, one), neg_one);
    cr_assert_i128_eq(el_i128_sub(EL_INT128(500), EL_INT128(200)), EL_INT128(300));
    cr_assert_i128_eq(el_i128_sub(EL_INT128(-100), EL_INT128(200)), EL_INT128(-300));

    // 64-bit boundary borrow
    ElInt128 hi_one = EL_INT128_H(0, 1);
    cr_assert_i128_eq(el_i128_sub(hi_one, one), EL_INT128_H(0xFFFFFFFFFFFFFFFFULL, 0));

    // Underflow wrap-around (INT128_MIN - 1 == INT128_MAX)
    cr_assert_i128_eq(el_i128_sub(min_val, one), max_val);
}

Test(el_int128, negation) {
    ElInt128 zero = EL_INT128(0);
    ElInt128 one = EL_INT128(1);
    ElInt128 neg_one = EL_INT128(-1);
    ElInt128 max_val = INT128_MAX;
    ElInt128 min_val = INT128_MIN;

    cr_assert_i128_eq(el_i128_neg(zero), zero);
    cr_assert_i128_eq(el_i128_neg(one), neg_one);
    cr_assert_i128_eq(el_i128_neg(neg_one), one);
    cr_assert_i128_eq(el_i128_neg(max_val), el_i128_add(min_val, one));

    // 2's complement wrap for INT128_MIN: -INT128_MIN == INT128_MIN
    cr_assert_i128_eq(el_i128_neg(min_val), min_val);
}

Test(el_int128, bitwise_logical) {
    ElInt128 zero = EL_INT128(0);
    ElInt128 neg_one = EL_INT128(-1);
    ElInt128 a = EL_INT128_H(0xFF00FF00FF00FF00ULL, 0x00FF00FF00FF00FFULL);
    ElInt128 b = EL_INT128_H(0x00FF00FF00FF00FFULL, 0xFF00FF00FF00FF00ULL);

    cr_assert_i128_eq(el_i128_and(a, b), zero);
    cr_assert_i128_eq(el_i128_or(a, b), neg_one);
    cr_assert_i128_eq(el_i128_xor(a, b), neg_one);
    cr_assert_i128_eq(el_i128_not(zero), neg_one);
    cr_assert_i128_eq(el_i128_not(neg_one), zero);
    cr_assert_i128_eq(el_i128_not(a), b);
}

Test(el_int128, shifts) {
    ElInt128 one = EL_INT128(1);
    ElInt128 neg_one = EL_INT128(-1);

    // Shift left 0, 1, 63, 64, 65, 127
    cr_assert_i128_eq(el_i128_shl(one, 0), one);
    cr_assert_i128_eq(el_i128_shl(one, 1), EL_INT128(2));
    cr_assert_i128_eq(el_i128_shl(one, 63), EL_INT128_H(0x8000000000000000ULL, 0));
    cr_assert_i128_eq(el_i128_shl(one, 64), EL_INT128_H(0, 1));
    cr_assert_i128_eq(el_i128_shl(one, 65), EL_INT128_H(0, 2));
    cr_assert_i128_eq(el_i128_shl(one, 127), INT128_MIN);

    // Shift right positive numbers (logical shift behavior)
    ElInt128 shl_127 = el_i128_shl(one, 64);
    cr_assert_i128_eq(el_i128_shr(shl_127, 64), one);

    // Arithmetic shift right (sign propagation)
    cr_assert_i128_eq(el_i128_shr(neg_one, 0), neg_one);
    cr_assert_i128_eq(el_i128_shr(neg_one, 1), neg_one);
    cr_assert_i128_eq(el_i128_shr(neg_one, 64), neg_one);
    cr_assert_i128_eq(el_i128_shr(neg_one, 100), neg_one);

    ElInt128 min_val = INT128_MIN;
    cr_assert_i128_eq(el_i128_shr(min_val, 1), EL_INT128_H(0, 0xC000000000000000ULL));
    cr_assert_i128_eq(el_i128_shr(min_val, 64), EL_INT128_H(0x8000000000000000ULL, 0xFFFFFFFFFFFFFFFFULL));
}

Test(el_int128, multiplication) {
    ElInt128 zero = EL_INT128(0);
    ElInt128 one = EL_INT128(1);
    ElInt128 neg_one = EL_INT128(-1);
    ElInt128 max_val = INT128_MAX;
    ElInt128 min_val = INT128_MIN;
    ElInt128 custom = EL_INT128_H(0x123456789ABCDEF0ULL, 0x0FEDCBA987654321ULL);

    cr_assert_i128_eq(el_i128_mul(zero, max_val), zero);
    cr_assert_i128_eq(el_i128_mul(max_val, zero), zero);
    cr_assert_i128_eq(el_i128_mul(one, custom), custom);
    cr_assert_i128_eq(el_i128_mul(neg_one, custom), el_i128_neg(custom));
    cr_assert_i128_eq(el_i128_mul(neg_one, neg_one), one);
    cr_assert_i128_eq(el_i128_mul(EL_INT128(5), EL_INT128(-4)), EL_INT128(-20));
    cr_assert_i128_eq(el_i128_mul(EL_INT128(-7), EL_INT128(-8)), EL_INT128(56));

    // 64-bit overflow into hi part
    ElInt128 a64 = EL_INT128(0x100000000ULL);
    cr_assert_i128_eq(el_i128_mul(a64, a64), EL_INT128_H(0, 1));

    // Edge multiplication with MIN / MAX
    cr_assert_i128_eq(el_i128_mul(min_val, one), min_val);
    cr_assert_i128_eq(el_i128_mul(min_val, zero), zero);
}

Test(el_int128, division) {
    ElInt128 zero = EL_INT128(0);
    ElInt128 one = EL_INT128(1);
    ElInt128 neg_one = EL_INT128(-1);
    ElInt128 min_val = INT128_MIN;
    ElInt128 max_val = INT128_MAX;

    // Basic positive & negative division
    cr_assert_i128_eq(el_i128_div(EL_INT128(20), EL_INT128(5)), EL_INT128(4));
    cr_assert_i128_eq(el_i128_div(EL_INT128(-20), EL_INT128(3)), EL_INT128(-6));
    cr_assert_i128_eq(el_i128_div(EL_INT128(20), EL_INT128(-3)), EL_INT128(-6));
    cr_assert_i128_eq(el_i128_div(EL_INT128(-20), EL_INT128(-3)), EL_INT128(6));

    // Division by 1 and -1
    cr_assert_i128_eq(el_i128_div(max_val, one), max_val);
    cr_assert_i128_eq(el_i128_div(max_val, neg_one), el_i128_neg(max_val));

    // Smaller numerator than denominator
    cr_assert_i128_eq(el_i128_div(EL_INT128(5), EL_INT128(10)), zero);
    cr_assert_i128_eq(el_i128_div(EL_INT128(-5), EL_INT128(10)), zero);

    // INT128_MIN edge cases
    cr_assert_i128_eq(el_i128_div(min_val, min_val), one);
    cr_assert_i128_eq(el_i128_div(min_val, one), min_val);
    cr_assert_i128_eq(el_i128_div(min_val, max_val), EL_INT128(-1));
}

Test(el_int128, modulo) {
    ElInt128 zero = EL_INT128(0);
    ElInt128 one = EL_INT128(1);
    ElInt128 min_val = INT128_MIN;
    ElInt128 max_val = INT128_MAX;

    cr_assert_i128_eq(el_i128_mod(EL_INT128(20), EL_INT128(5)), zero);
    cr_assert_i128_eq(el_i128_mod(EL_INT128(20), EL_INT128(3)), EL_INT128(2));
    cr_assert_i128_eq(el_i128_mod(EL_INT128(-20), EL_INT128(3)), EL_INT128(-2));
    cr_assert_i128_eq(el_i128_mod(EL_INT128(20), EL_INT128(-3)), EL_INT128(2));
    cr_assert_i128_eq(el_i128_mod(EL_INT128(-20), EL_INT128(-3)), EL_INT128(-2));

    // INT128_MIN edge cases
    cr_assert_i128_eq(el_i128_mod(min_val, one), zero);
    cr_assert_i128_eq(el_i128_mod(min_val, min_val), zero);
    cr_assert_i128_eq(el_i128_mod(min_val, max_val), EL_INT128(-1));
}

// NOLINTBEGIN(readability-magic-numbers): Never gonna give you up
Test(el_int128, to_string) {
    char buf[128];

    ElUint128 foo = EL_UINT128(12345);
    ElStringView sfoo = el_u128_to_string(foo, 10, buf);
    cr_assert(el_sv_eql(sfoo, EL_SV("12345")));

    ElStringView hfoo = el_u128_to_string(foo, 16, buf);
    cr_assert(el_sv_eql(hfoo, EL_SV("3039")));

    ElInt128 bar = EL_INT128(12345);
    ElStringView sbar = el_i128_to_string(bar, 10, buf);
    cr_assert(el_sv_eql(sbar, EL_SV("12345")));

    ElInt128 baz = EL_INT128(-12345);
    ElStringView sbaz = el_i128_to_string(baz, 10, buf);
    cr_assert(el_sv_eql(sbaz, EL_SV("-12345")));

    ElStringView hbaz = el_i128_to_string(baz, 36, buf);
    cr_assert(el_sv_eql(hbaz, EL_SV("-9ix")));

    ElUint128 qux = EL_UINT128(0);
    ElStringView squx = el_u128_to_string(qux, 14, buf);
    cr_assert(el_sv_eql(squx, EL_SV("0")));
}
// NOLINTEND(readability-magic-numbers)
