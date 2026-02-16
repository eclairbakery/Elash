#include <el-pp/preproc.h>

#include <el-util/dynarena.h>

ElPpErrorCode _el_pp_ret_error(ElPreprocessor* pp, ElPpErrorCode code) {
    pp->last_err_details.code = code;
    return code;
}

ElPpErrorCode _el_pp_ret_success(ElPreprocessor* pp) {
    pp->last_err_details.code = EL_PPERR_SUCCESS;
    return EL_PPERR_SUCCESS;
}


ElPpErrorCode el_pp_init(ElPreprocessor* pp) {
    if (!el_dynarena_init(&pp->arena)) {
        return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
    }
    if (!el_tkque_init(&pp->pending)) {
        return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
    }

    pp->last_err_details = EL_PP_RESULT_OK;
    return _el_pp_ret_success(pp);
}

void el_pp_destroy(ElPreprocessor* pp) {
    el_dynarena_free(&pp->arena);
    el_tkque_destroy(&pp->pending);
}

ElPpErrorCode el_pp_reset(ElPreprocessor* pp) {
    el_dynarena_reset(&pp->arena);
    if (!el_tkque_clear(&pp->pending)) {
        return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
    }
    return _el_pp_ret_success(pp);
}

bool el_pp_want_next_token(ElPreprocessor* pp) {
    return pp->pending.len == 0;
}

void el_pp_pass_token(ElPreprocessor* pp, ElToken tok) {
    if (!el_pp_want_next_token(pp)) {
        return;
    }
    pp->input = tok;
}

// FAKE IMPLEMENTATION for development.
ElPpErrorCode el_pp_preprocess(ElPreprocessor* pp, ElToken* out_tok) {
    if (pp->pending.len != 0) {
        if (!el_tkque_pop(&pp->pending, out_tok))
            return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
        return _el_pp_ret_success(pp);
    }
    
    if (pp->input.type == EL_TT_PP_NOTE) {
        el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_INT_LITERAL, .lexeme = EL_SV("123") });
        el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_PLUS });
        el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_INT_LITERAL, .lexeme = EL_SV("321") });
        *out_tok = (ElToken) { .type = EL_TT_IDENT, .lexeme = EL_SV("hello") };
        return _el_pp_ret_success(pp);
    }
    *out_tok = pp->input;
    return _el_pp_ret_success(pp);
}

