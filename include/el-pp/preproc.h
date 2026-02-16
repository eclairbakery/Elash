#pragma once

#include <el-util/dynarena.h>

#include <el-lexer/tokbuf.h>
#include <el-lexer/tokque.h>
#include <el-lexer/token.h>

#include <el-pp/error.h>
#include <el-pp/instr.h>
#include <el-pp/vars.h>

#include <stdbool.h>

typedef struct ElPreprocessor {
    ElToken input;
    ElTokenQueue pending;

    ElDynArena arena;

    ElPpInstr currently_parsing;
    ElPpVars vars;

    ElPpErrorDetails last_err_details;
} ElPreprocessor;

ElPpErrorCode el_pp_init(ElPreprocessor* pp);
void          el_pp_destroy(ElPreprocessor* pp);

ElPpErrorCode el_pp_reset(ElPreprocessor* pp);

bool          el_pp_want_next_token(ElPreprocessor* pp);
void          el_pp_pass_token(ElPreprocessor* pp, ElToken tok);

ElPpErrorCode el_pp_preprocess(ElPreprocessor* pp, ElToken* out_tok);

////// IMPLEMENTATION DETAILS //////////////////////
// note: those functions are implementation
// details and should not be called directly
// outside internal el-pp implementation.
ElPpErrorCode _el_pp_ret_error(ElPreprocessor* pp, ElPpErrorCode code);
ElPpErrorCode _el_pp_ret_success(ElPreprocessor* pp);
////////////////////////////////////////////////////

