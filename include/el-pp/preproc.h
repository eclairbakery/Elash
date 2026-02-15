#pragma once

#include <el-lexer/tokbuf.h>
#include <el-pp/vars.h>

typedef struct ElPreprocessor {
    ElPpVars vars;
    ElTokenBuf buf;
} ElPreprocessor;

