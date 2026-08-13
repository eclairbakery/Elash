#include "preproc-internals.h"

bool _el_pp_preprocess_directive(ElPreproc* pp, ElToken* out_tok) {
    ElToken directive = pp->input.next(&pp->input, pp->diag);

    // optimization i guess
    switch (directive.lexeme.data[0]) {
    case 'i':
        if (el_sv_eql(directive.lexeme, EL_SV("include")))
            return _el_pp_handle_include(pp, out_tok);
        break;

    case 'e':
        if (el_sv_eql(directive.lexeme, EL_SV("embed")))
            return _el_pp_handle_embed(pp, out_tok);
        break;

    default:
        // make the compiler happy
        break;
    }

    el_diag_report(
        pp->diag, EL_DIAG_ERROR, "pp.unknown-dir",
        directive.span, "unknown directive: #${name}",
        EL_DIAG_STRING("name", directive.lexeme),
    );
    return false;
}
