#include <criterion/criterion.h>
#include <elash/util/int128.h>
#include <stdbool.h>
#include <stdint.h>

TestSuite(el_uint128);

#define cr_assert_u128_eq(A, B) \
    cr_assert(el_u128_eq((A), (B)), "Expected 0x%016llx%016llx, got 0x%016llx%016llx", \
              (unsigned long long)el_u128_hi(B), (unsigned long long)el_u128_lo(B), \
              (unsigned long long)el_u128_hi(A), (unsigned long long)el_u128_lo(A))

Test(el_uint128, constructors_and_accessors) {
    ElUint128 zero = EL_UINT128(0);
    ElUint128 one = EL_UINT128(1);
    ElUint128 max_val = UINT128_MAX;
    ElUint128 custom = EL_UINT128_H(0x123456789ABCDEF0ULL, 0x0FEDCBA987654321ULL);

    cr_assert_eq(el_u128_lo(zero), 0ULL);
    cr_assert_eq(el_u128_hi(zero), 0ULL);

    cr_assert_eq(el_u128_lo(one), 1ULL);
    cr_assert_eq(el_u128_hi(one), 0ULL);

    cr_assert_eq(el_u128_lo(max_val), 0xFFFFFFFFFFFFFFFFULL);
    cr_assert_eq(el_u128_hi(max_val), 0xFFFFFFFFFFFFFFFFULL);

    cr_assert_eq(el_u128_lo(custom), 0x123456789ABCDEF0ULL);
    cr_assert_eq(el_u128_hi(custom), 0x0FEDCBA987654321ULL);
}

Test(el_uint128, equality_and_inequality) {
    ElUint128 zero = EL_UINT128(0);
    ElUint128 one = EL_UINT128(1);
    ElUint128 max_val = UINT128_MAX;

    cr_assert(el_u128_eq(zero, EL_UINT128(0)));
    cr_assert(el_u128_eq(max_val, UINT128_MAX));

    cr_assert(!el_u128_eq(zero, one));
    cr_assert(!el_u128_eq(one, max_val));

    cr_assert(el_u128_ne(zero, one));
    cr_assert(el_u128_ne(one, max_val));
    cr_assert(!el_u128_ne(zero, zero));
}

Test(el_uint128, comparison) {
    ElUint128 zero = EL_UINT128(0);
    ElUint128 one = EL_UINT128(1);
    ElUint128 hi_one = EL_UINT128_H(0, 1);
    ElUint128 max_val = UINT128_MAX;

    // Less than (lt)
    cr_assert(el_u128_lt(zero, one));
    cr_assert(el_u128_lt(one, hi_one));
    cr_assert(el_u128_lt(hi_one, max_val));
    cr_assert(el_u128_lt(zero, max_val));

    cr_assert(!el_u128_lt(zero, zero));
    cr_assert(!el_u128_lt(one, zero));
    cr_assert(!el_u128_lt(max_val, hi_one));

    // Greater than (gt)
    cr_assert(el_u128_gt(one, zero));
    cr_assert(el_u128_gt(hi_one, one));
    cr_assert(el_u128_gt(max_val, hi_one));
    cr_assert(el_u128_gt(max_val, zero));

    cr_assert(!el_u128_gt(zero, zero));
    cr_assert(!el_u128_gt(zero, one));

    // Less than or equal (le)
    cr_assert(el_u128_le(zero, zero));
    cr_assert(el_u128_le(max_val, max_val));
    cr_assert(el_u128_le(zero, one));
    cr_assert(el_u128_le(one, hi_one));
    cr_assert(!el_u128_le(one, zero));

    // Greater than or equal (ge)
    cr_assert(el_u128_ge(zero, zero));
    cr_assert(el_u128_ge(max_val, max_val));
    cr_assert(el_u128_ge(one, zero));
    cr_assert(el_u128_ge(hi_one, one));
    cr_assert(!el_u128_ge(zero, one));
}

Test(el_uint128, addition) {
    ElUint128 zero = EL_UINT128(0);
    ElUint128 one = EL_UINT128(1);
    ElUint128 max_val = UINT128_MAX;

    cr_assert_u128_eq(el_u128_add(zero, zero), zero);
    cr_assert_u128_eq(el_u128_add(one, zero), one);
    cr_assert_u128_eq(el_u128_add(EL_UINT128(100), EL_UINT128(250)), EL_UINT128(350));

    // Carry across 64-bit boundary
    ElUint128 hi_boundary = EL_UINT128_H(0xFFFFFFFFFFFFFFFFULL, 0);
    cr_assert_u128_eq(el_u128_add(hi_boundary, one), EL_UINT128_H(0, 1));

    // Overflow wrap-around (UINT128_MAX + 1 == 0)
    cr_assert_u128_eq(el_u128_add(max_val, one), zero);
    cr_assert_u128_eq(el_u128_add(max_val, max_val), EL_UINT128_H(0xFFFFFFFFFFFFFFFEULL, 0xFFFFFFFFFFFFFFFFULL));
}

Test(el_uint128, subtraction) {
    ElUint128 zero = EL_UINT128(0);
    ElUint128 one = EL_UINT128(1);
    ElUint128 max_val = UINT128_MAX;

    cr_assert_u128_eq(el_u128_sub(zero, zero), zero);
    cr_assert_u128_eq(el_u128_sub(one, one), zero);
    cr_assert_u128_eq(el_u128_sub(EL_UINT128(500), EL_UINT128(200)), EL_UINT128(300));

    // Borrow across 64-bit boundary
    ElUint128 hi_one = EL_UINT128_H(0, 1);
    cr_assert_u128_eq(el_u128_sub(hi_one, one), EL_UINT128_H(0xFFFFFFFFFFFFFFFFULL, 0));

    // Underflow wrap-around (0 - 1 == UINT128_MAX)
    cr_assert_u128_eq(el_u128_sub(zero, one), max_val);
}

Test(el_uint128, negation) {
    ElUint128 zero = EL_UINT128(0);
    ElUint128 one = EL_UINT128(1);
    ElUint128 max_val = UINT128_MAX;

    // 2's complement negation for unsigned 128
    cr_assert_u128_eq(el_u128_neg(zero), zero);
    cr_assert_u128_eq(el_u128_neg(one), max_val);
    cr_assert_u128_eq(el_u128_neg(max_val), one);
}

Test(el_uint128, bitwise_logical) {
    ElUint128 zero = EL_UINT128(0);
    ElUint128 max_val = UINT128_MAX;
    ElUint128 a = EL_UINT128_H(0xFF00FF00FF00FF00ULL, 0x00FF00FF00FF00FFULL);
    ElUint128 b = EL_UINT128_H(0x00FF00FF00FF00FFULL, 0xFF00FF00FF00FF00ULL);

    cr_assert_u128_eq(el_u128_and(a, b), zero);
    cr_assert_u128_eq(el_u128_or(a, b), max_val);
    cr_assert_u128_eq(el_u128_xor(a, b), max_val);
    cr_assert_u128_eq(el_u128_not(zero), max_val);
    cr_assert_u128_eq(el_u128_not(max_val), zero);
    cr_assert_u128_eq(el_u128_not(a), b);
}

Test(el_uint128, shifts) {
    ElUint128 one = EL_UINT128(1);
    ElUint128 max_val = UINT128_MAX;

    // Shift left 0, 1, 63, 64, 65, 127
    cr_assert_u128_eq(el_u128_shl(one, 0), one);
    cr_assert_u128_eq(el_u128_shl(one, 1), EL_UINT128(2));
    cr_assert_u128_eq(el_u128_shl(one, 63), EL_UINT128_H(0x8000000000000000ULL, 0));
    cr_assert_u128_eq(el_u128_shl(one, 64), EL_UINT128_H(0, 1));
    cr_assert_u128_eq(el_u128_shl(one, 65), EL_UINT128_H(0, 2));
    cr_assert_u128_eq(el_u128_shl(one, 127), EL_UINT128_H(0, 0x8000000000000000ULL));

    // Logical shift right
    ElUint128 shl_64 = el_u128_shl(one, 64);
    cr_assert_u128_eq(el_u128_shr(shl_64, 64), one);
    cr_assert_u128_eq(el_u128_shr(max_val, 64), EL_UINT128(0xFFFFFFFFFFFFFFFFULL));
    cr_assert_u128_eq(el_u128_shr(max_val, 127), one);
}

Test(el_uint128, multiplication) {
    ElUint128 zero = EL_UINT128(0);
    ElUint128 one = EL_UINT128(1);
    ElUint128 max_val = UINT128_MAX;
    ElUint128 custom = EL_UINT128_H(0x123456789ABCDEF0ULL, 0x0FEDCBA987654321ULL);

    cr_assert_u128_eq(el_u128_mul(zero, max_val), zero);
    cr_assert_u128_eq(el_u128_mul(max_val, zero), zero);
    cr_assert_u128_eq(el_u128_mul(one, custom), custom);
    cr_assert_u128_eq(el_u128_mul(EL_UINT128(12345), EL_UINT128(67890)), EL_UINT128(838102050ULL));

    // 64-bit boundary product
    ElUint128 a64 = EL_UINT128(0x100000000ULL);
    cr_assert_u128_eq(el_u128_mul(a64, a64), EL_UINT128_H(0, 1));
}

Test(el_uint128, division) {
    ElUint128 zero = EL_UINT128(0);
    ElUint128 one = EL_UINT128(1);
    ElUint128 max_val = UINT128_MAX;
    ElUint128 custom = EL_UINT128_H(0x123456789ABCDEF0ULL, 0x0FEDCBA987654321ULL);

    cr_assert_u128_eq(el_u128_div(EL_UINT128(100), EL_UINT128(7)), EL_UINT128(14));
    cr_assert_u128_eq(el_u128_div(custom, custom), one);
    cr_assert_u128_eq(el_u128_div(one, custom), zero);
    cr_assert_u128_eq(el_u128_div(max_val, one), max_val);
    cr_assert_u128_eq(el_u128_div(max_val, max_val), one);
}

Test(el_uint128, modulo) {
    ElUint128 zero = EL_UINT128(0);
    ElUint128 one = EL_UINT128(1);
    ElUint128 max_val = UINT128_MAX;
    ElUint128 custom = EL_UINT128_H(0x123456789ABCDEF0ULL, 0x0FEDCBA987654321ULL);

    cr_assert_u128_eq(el_u128_mod(EL_UINT128(100), EL_UINT128(7)), EL_UINT128(2));
    cr_assert_u128_eq(el_u128_mod(custom, custom), zero);
    cr_assert_u128_eq(el_u128_mod(one, custom), one);
    cr_assert_u128_eq(el_u128_mod(max_val, one), zero);
    cr_assert_u128_eq(el_u128_mod(max_val, max_val), zero);
}
