#pragma once

#include <elash/defs/sv.h>

#include <elash/lexer/tokbuf.h>
#include <elash/lexer/token.h>

#include <inttypes.h>
#include <stdbool.h>

typedef struct ElPpMacro {
    bool is_function_style;
    struct {
        ElStringView* arr;
        usize count;
    } args;
    ElTokenBuf replacement;
} ElPpMacro;
