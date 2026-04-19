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
