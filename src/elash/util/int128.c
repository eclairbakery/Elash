#define EL_INT128_EMULATE
#define _EL_I128_API
#include <elash/util/int128.h>

static const char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";

static ElStringView to_string_internal(ElUint128 x, uint base, char* buf) {
    if (el_u128_eq(x, EL_UINT128(0))) {
        buf[0] = '0';
        return el_sv_from_char(buf);
    }

    usize i = 0;
    while (el_u128_ne(x, EL_UINT128(0))) {
        ElUint128 rem = el_u128_mod(x, EL_UINT128(base));
        buf[i++] = digits[rem.lo];
        x = el_u128_div(x, EL_UINT128(base));
    }

    for (usize j = 0; j < i / 2; ++j) {
        char tmp = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = tmp;
    }
    return el_sv_from_data_and_len(buf, i);
}

ElStringView el_u128_to_string(ElUint128 x, uint base, char* buf) {
    return to_string_internal(x, base, buf);
}

ElStringView el_i128_to_string(ElInt128 x, uint base, char* buf) {
    if (el_i128_lt(x, EL_INT128(0))) {
        buf[0] = '-';
        ElStringView result = to_string_internal(el_i128_abs_u128(x), base, buf + 1);
        result.data--;
        result.len++;
        return result;
    } else {
        return to_string_internal(el_i128_bitcast_u128(x), base, buf);
    }
}

ElUint128 el_i128_abs_u128(ElInt128 v) {
    if (el_i128_lt(v, EL_INT128(0))) {
        return el_i128_bitcast_u128(el_i128_neg(v));
    }
    return el_i128_bitcast_u128(v);
}

