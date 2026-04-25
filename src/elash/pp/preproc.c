#include <elash/pp/preproc.h>

#include <elash/util/dynarena.h>

ElPpErrorCode _el_pp_ret_error(ElPreprocessor* pp, ElPpErrorCode code) {
    pp->last_err_details.code = code;
    return code;
}

ElPpErrorCode _el_pp_ret_skip(ElPreprocessor* pp) {
    pp->last_err_details.code = EL_PPERR_SKIPPED;
    return EL_PPERR_SKIPPED;
}

ElPpErrorCode _el_pp_ret_success(ElPreprocessor* pp) {
    pp->last_err_details.code = EL_PPERR_SUCCESS;
    return EL_PPERR_SUCCESS;
}

ElPpErrorCode el_pp_init(ElPreprocessor* pp, ElTokenStream input) {
    if (!el_dynarena_init(&pp->arena)) {
        return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
    }
    if (!el_tkque_init(&pp->pending)) {
        el_dynarena_free(&pp->arena);
        return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
    }

    pp->input = input;
    pp->last_err_details = EL_PP_RESULT_OK;
    return _el_pp_ret_success(pp);
}

void el_pp_destroy(ElPreprocessor* pp) {
    el_dynarena_free(&pp->arena);
    el_tkque_destroy(&pp->pending);
}

ElPpErrorCode el_pp_reset(ElPreprocessor* pp, ElTokenStream input) {
    el_dynarena_reset(&pp->arena);
    if (!el_tkque_clear(&pp->pending)) {
        return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
    }
    pp->input = input;
    return _el_pp_ret_success(pp);
}

ElPpErrorCode el_pp_next(ElPreprocessor* pp, ElToken* out_tok, ElDiagEngine* engine) {
    while (true) {
        if (pp->pending.len != 0) {
            if (!el_tkque_pop(&pp->pending, out_tok))
                return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
            return _el_pp_ret_success(pp);
        }

        ElToken input_tok = pp->input.next(&pp->input, engine);
        
        switch (input_tok.type) {
        case EL_TT_NEWLINE:
        case EL_TT_WHITESPACE:
        case EL_TT_LINE_COMMENT:
        case EL_TT_BLOCK_COMMENT:
            // skip whitespace, new lines and comments 
            continue;

        case EL_TT_PP_NOTE:
            if (!el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_INT_LITERAL, .lexeme = EL_SV("123"), .span = input_tok.span }))
                return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
            if (!el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_PLUS, .lexeme = EL_SV("+"), .span = input_tok.span }))
                return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
            if (!el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_INT_LITERAL, .lexeme = EL_SV("321"), .span = input_tok.span }))
                return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
            
            *out_tok = (ElToken) { .type = EL_TT_IDENT, .lexeme = EL_SV("hello"), .span = input_tok.span };
            return _el_pp_ret_success(pp);

        default:
            *out_tok = input_tok;
            return _el_pp_ret_success(pp);
        }
    }
}

static ElToken _el_pp_token_stream_next(ElTokenStream* self, ElDiagEngine* engine) {
    ElPreprocessor* pp = (ElPreprocessor*)self->ctx;
    ElToken tok;
    ElPpErrorCode err = el_pp_next(pp, &tok, engine);
    
    if (err != EL_PPERR_SUCCESS) {
        // TODO: map pp errors into diagnostics engine
        // for now, i think we can just return unknown token on error
        if (err != EL_PPERR_SKIPPED) {
            tok.type = EL_TT_UNKNOWN;
        }
    }
    
    return tok;
}

ElTokenStream el_pp_as_token_stream(ElPreprocessor* pp) {
    return (ElTokenStream) {
        .next = _el_pp_token_stream_next,
        .ctx = pp,
    };
}
