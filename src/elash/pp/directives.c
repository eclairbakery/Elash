#include "preproc-internals.h"

bool _el_pp_preprocess_directive(ElPreproc* pp, ElToken hash, ElToken* out_tok) {
    ElToken dir;
    if (!_el_pp_read(pp, &dir)) {
        return false;
    }

    ElSourceSpan dspan =
        el_srcspan_merge(hash.span, dir.span);

    if (el_sv_eql(dir.lexeme, EL_SV("include")))
        return _el_pp_handle_include(pp, dspan) && _el_pp_next_d(pp, out_tok);
    if (el_sv_eql(dir.lexeme, EL_SV("embed")))
        return _el_pp_handle_embed(pp, dspan) && _el_pp_next_d(pp, out_tok);

    if (el_sv_eql(dir.lexeme, EL_SV("var")))
        return _el_pp_handle_var(pp, dspan) && _el_pp_next_d(pp, out_tok);
    if (el_sv_eql(dir.lexeme, EL_SV("set")))
        return _el_pp_handle_set(pp, dspan) && _el_pp_next_d(pp, out_tok);
    if (el_sv_eql(dir.lexeme, EL_SV("inc")))
        return _el_pp_handle_inc(pp, dspan) && _el_pp_next_d(pp, out_tok);
    if (el_sv_eql(dir.lexeme, EL_SV("dec")))
        return _el_pp_handle_dec(pp, dspan) && _el_pp_next_d(pp, out_tok);

    if (el_sv_eql(dir.lexeme, EL_SV("error")))
        return _el_pp_handle_diag(pp, EL_DIAG_ERROR, dspan) && _el_pp_next_d(pp, out_tok);
    if (el_sv_eql(dir.lexeme, EL_SV("note")))
        return _el_pp_handle_diag(pp, EL_DIAG_NOTE, dspan) && _el_pp_next_d(pp, out_tok);
    if (el_sv_eql(dir.lexeme, EL_SV("warn")))
        return _el_pp_handle_diag(pp, EL_DIAG_WARN, dspan) && _el_pp_next_d(pp, out_tok);

    el_diag_report(
        pp->diag, EL_DIAG_ERROR, "pp.unknown-dir",
        dspan, "unknown dir: #${name}",
        EL_DIAG_STRING("name", dir.lexeme),
    );
    return false;
}
