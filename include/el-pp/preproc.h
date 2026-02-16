#pragma once

#include <el-util/dynarena.h>

#include <el-lexer/tokbuf.h>
#include <el-lexer/token.h>

#include <el-pp/error.h>
#include <el-pp/vars.h>

typedef struct ElPreprocessor {
    ElPpVars vars;

    ElTokenBuf buf;
    usize buf_offset;

    ElDynArena arena;

    ElPpErrorDetails last_err_details;
} ElPreprocessor;

ElPpErrorCode el_pp_init(ElPreprocessor* pp);
ElPpErrorCode el_pp_destroy(ElPreprocessor* pp);

ElPpErrorCode el_pp_reset(ElPreprocessor* pp);

ElPpErrorCode el_pp_preprocess(ElPreprocessor* pp, ElToken in_tok, ElToken* out_tok);

