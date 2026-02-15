#pragma once

#include <el-defs/sv.h>

#include <el-lexer/token.h>

#include <inttypes.h>
#include <stdbool.h>

typedef struct ElPpMacro {
    bool is_function_style;
    struct {
        ElStringView* arr;
        usize count;
    } args;
    struct {
        ElToken* toks;
        usize count;
    } replacement;
} ElPpMacro;
