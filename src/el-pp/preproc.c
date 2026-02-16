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
    if (!el_tkbuf_init(&pp->buf)) {
        return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
    }

    pp->buf_offset = 0;
    pp->last_err_details = EL_PP_RESULT_OK;
    return _el_pp_ret_success(pp);
}

void el_pp_destroy(ElPreprocessor* pp) {
    el_dynarena_free(&pp->arena);
    el_tkbuf_destroy(&pp->buf);
}

ElPpErrorCode el_pp_reset(ElPreprocessor* pp) {
    el_dynarena_reset(&pp->arena);
    if (!el_tkbuf_clear(&pp->buf)) {
        return _el_pp_ret_error(pp, EL_PPERR_ALLOC_FAILED);
    }
    return _el_pp_ret_success(pp);
}

ElPpErrorCode el_pp_preprocess(ElPreprocessor* pp, ElToken in_tok, ElToken* out_tok) {
    // STUB
    *out_tok = in_tok;
    return _el_pp_ret_success(pp);
}

