#include "preproc-internals.h"

#include <elash/util/dynarena.h>

bool el_pp_init(ElPreproc* pp, ElTokenStream input, ElDynArena* arena) {
    if (!el_tkque_init(&pp->pending)) {
        return false;
    }

    pp->arena = arena;
    pp->input = input;
    return true;
}

void el_pp_destroy(ElPreproc* pp) {
    el_tkque_destroy(&pp->pending);
}

bool el_pp_next(ElPreproc* pp, ElToken* out_tok, ElDiagEngine* diag) {
    pp->diag = diag;

    while (true) {
        if (pp->pending.len != 0) {
            el_tkque_pop(&pp->pending, out_tok);
            return true;
        }

        ElToken input_tok = pp->input.next(&pp->input, pp->diag);

        switch (input_tok.type) {
        case EL_TT_NEWLINE:
        case EL_TT_WHITESPACE:
        case EL_TT_LINE_COMMENT:
        case EL_TT_BLOCK_COMMENT:
            // skip whitespace, new lines and comments
            continue;

        case EL_TT_HASH: {
            // TODO: placeholder
            ElToken dir = pp->input.next(&pp->input, pp->diag);

            if (el_sv_eql(dir.lexeme, EL_SV("test"))) {
                el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_INT_LITERAL, .lexeme = EL_SV("123"), .span = input_tok.span });
                el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_PLUS,        .lexeme = EL_SV("+"),   .span = input_tok.span });
                el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_INT_LITERAL, .lexeme = EL_SV("321"), .span = input_tok.span });
            }

            *out_tok = (ElToken) { .type = EL_TT_IDENT, .lexeme = EL_SV("hello"), .span = input_tok.span };
            return true;
        }

        default:
            *out_tok = input_tok;
            return true;
        }
    }
}

static ElToken _el_pp_token_stream_next(ElTokenStream* self, ElDiagEngine* diag) {
    ElPreproc* pp = (ElPreproc*)self->ctx;
    ElToken tok;
    (void) el_pp_next(pp, &tok, diag);
    return tok;
}

ElTokenStream el_pp_as_token_stream(ElPreproc* pp) {
    return (ElTokenStream) {
        .next = _el_pp_token_stream_next,
        .ctx = pp,
    };
}
