#pragma once

#include <el-defs/sv.h>

#include <el-lexer/tokbuf.h>
#include <el-lexer/token.h>

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
