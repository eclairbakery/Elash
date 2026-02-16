#pragma once

#include <el-util/dynarena.h>

#include <el-lexer/tokbuf.h>
#include <el-lexer/token.h>

#include <el-pp/error.h>
#include <el-pp/vars.h>

typedef enum ElPpState {
    EL_PP_STATE_IDLE,
    EL_PP_STATE_EXECUTING_INCLUDE,
    EL_PP_STATE_EXPADING_MACRO,
    EL_PP_STATE_EVALUATING_EXPR,
} ElPpState;

typedef struct ElPreprocessor {
    ElPpVars vars;

    ElTokenBuf buf;
    usize buf_offset;

    ElDynArena arena;

    ElPpErrorDetails last_err_details;
} ElPreprocessor;

ElPpErrorCode el_pp_init(ElPreprocessor* pp);
void          el_pp_destroy(ElPreprocessor* pp);

ElPpErrorCode el_pp_reset(ElPreprocessor* pp);

ElPpErrorCode el_pp_preprocess(ElPreprocessor* pp, ElToken in_tok, ElToken* out_tok);

////// IMPLEMENTATION DETAILS //////////////////////
// note: those functions are implementation
// details and should not be called directly
// outside internal el-pp implementation.
ElPpErrorCode _el_pp_ret_error(ElPreprocessor* pp, ElPpErrorCode code);
ElPpErrorCode _el_pp_ret_success(ElPreprocessor* pp);
////////////////////////////////////////////////////

