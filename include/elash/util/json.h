#pragma once

#include <elash/defs/sv.h>

#include <stdio.h>

#define EL_JSON_MIN_PRINTABLE 0x20

static inline void el_json_write_string(FILE* out, ElStringView sv) {
    fputc('"', out);
    for (usize i = 0; i < sv.len; ++i) {
        char c = sv.data[i];
        switch (c) {
        case '"':  fputs("\\\"", out); break;
        case '\\': fputs("\\\\", out); break;
        case '\b': fputs("\\b", out);  break;
        case '\f': fputs("\\f", out);  break;
        case '\n': fputs("\\n", out);  break;
        case '\r': fputs("\\r", out);  break;
        case '\t': fputs("\\t", out);  break;
        default:
            if ((unsigned char)c < EL_JSON_MIN_PRINTABLE) {
                fprintf(out, "\\u%04x", c);
            } else {
                fputc(c, out);
            }
            break;
        }
    }
    fputc('"', out);
}
