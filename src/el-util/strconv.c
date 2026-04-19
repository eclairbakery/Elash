#include <el-util/strconv.h>

#include <stdint.h>

bool el_string_to_u64(ElStringView str, uint64_t* out) {
    if (str.len == 0) return false;

    uint64_t res = 0;
    bool has_digits = false;
    for (usize i = 0; i < str.len; i++) {
        char c = str.data[i];
        if (c >= '0' && c <= '9') {
            uint64_t digit = (uint64_t) (c - '0');
            if (res > (UINT64_MAX - digit) / 10) {
                return false;
            }
            res = res * 10 + digit;
            has_digits = true;
        } else if (c == '_' || c == '\'') {
            // digit separators must be between digits
            if (!has_digits || i + 1 == str.len) return false;
            continue;
        } else {
            return false;
        }
    }

    if (!has_digits) return false;

    if (out) *out = res;
    return true;
}

bool el_string_to_i64(ElStringView str, int64_t* out) {
    if (str.len == 0) return false;

    bool negative = false;
    usize start = 0;

    if (str.data[0] == '-') {
        negative = true;
        start = 1;
    } else if (str.data[0] == '+') {
        start = 1;
    }

    if (start >= str.len) return false;

    ElStringView absolute = { .data = str.data + start, .len = str.len - start };
    uint64_t ures = 0;
    if (!el_string_to_u64(absolute, &ures)) return false;

    if (negative) {
        if (ures > (uint64_t) INT64_MAX + 1) return false;
        if (out) {
            if (ures == (uint64_t) INT64_MAX + 1) {
                *out = INT64_MIN;
            } else {
                *out = -(int64_t) ures;
            }
        }
    } else {
        if (ures > (uint64_t) INT64_MAX) return false;
        if (out) *out = (int64_t) ures;
    }

    return true;
}

static inline bool is_digit(char c) { return c >= '0' && c <= '9'; }
static inline bool is_sep(char c) { return c == '_' || c == '\''; }

static bool parse_int_part(ElStringView str, usize* i, long double* res, bool* has_digits) {
    bool prev_digit = false;
    while (*i < str.len) {
        char c = str.data[*i];
        if (is_digit(c)) {
            *res = *res * 10.0L + (c - '0');
            *has_digits = true;
            prev_digit = true;
            (*i)++;
        } else if (is_sep(c)) {
            if (!prev_digit || *i + 1 >= str.len || !is_digit(str.data[*i + 1])) return false;
            prev_digit = false;
            (*i)++;
        } else break;
    }
    return true;
}

static bool parse_frac_part(ElStringView str, usize* i, long double* res, bool* has_digits) {
    if (*i >= str.len || str.data[*i] != '.') return true;
    (*i)++;
    long double frac = 0.0L;
    long double div = 1.0L;
    bool prev_digit = false;
    while (*i < str.len) {
        char c = str.data[*i];
        if (is_digit(c)) {
            frac = frac * 10.0L + (c - '0');
            div *= 10.0L;
            *has_digits = true;
            prev_digit = true;
            (*i)++;
        } else if (is_sep(c)) {
            if (!prev_digit || *i + 1 >= str.len || !is_digit(str.data[*i + 1])) return false;
            prev_digit = false;
            (*i)++;
        } else break;
    }
    *res += frac / div;
    return true;
}

static bool parse_exp_part(ElStringView str, usize* i, long double* res) {
    if (*i >= str.len || (str.data[*i] != 'e' && str.data[*i] != 'E')) return true;
    (*i)++;
    bool neg = false;
    if (*i < str.len && (str.data[*i] == '-' || str.data[*i] == '+')) {
        if (str.data[*i] == '-') neg = true;
        (*i)++;
    }
    if (*i >= str.len || !is_digit(str.data[*i])) return false;
    
    long long exp = 0;
    bool prev_digit = false;
    while (*i < str.len) {
        char c = str.data[*i];
        if (is_digit(c)) {
            exp = exp * 10 + (c - '0');
            prev_digit = true;
            (*i)++;
        } else if (is_sep(c)) {
            if (!prev_digit || *i + 1 >= str.len || !is_digit(str.data[*i + 1])) return false;
            prev_digit = false;
            (*i)++;
        } else break;
    }
    
    long double mult = 1.0L;
    long double base = 10.0L;
    long long e = exp;
    while (e > 0) {
        if (e % 2 == 1) mult *= base;
        base *= base;
        e /= 2;
    }
    if (neg) *res /= mult;
    else *res *= mult;
    return true;
}

bool el_string_to_long_double(ElStringView str, long double* out) {
    if (str.len == 0) return false;
    usize i = 0;
    bool neg = false;
    if (str.data[i] == '-') { neg = true; i++; }
    else if (str.data[i] == '+') i++;
    
    if (i >= str.len) return false;
    
    long double res = 0.0L;
    bool has_digits = false;
    if (!parse_int_part(str, &i, &res, &has_digits)) return false;
    if (!parse_frac_part(str, &i, &res, &has_digits)) return false;
    if (!parse_exp_part(str, &i, &res)) return false;
    
    if (i != str.len || !has_digits) return false;
    if (out) *out = neg ? -res : res;
    return true;
}
