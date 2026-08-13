#include "preproc-internals.h"

#include <elash/util/dynarena.h>

bool el_pp_init(ElPreproc* pp, ElTokenStream input, ElDynArena* arena) {
    pp->currently_streaming = false;
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

static bool preprocess_directive(ElPreproc* pp, ElToken* out_tok) {
    // TODO: placeholder
    ElToken dir = pp->input.next(&pp->input, pp->diag);

    if (el_sv_eql(dir.lexeme, EL_SV("test"))) {
        el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_INT_LITERAL, .lexeme = EL_SV("123"), .span = dir.span });
        el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_PLUS,        .lexeme = EL_SV("+"),   .span = dir.span });
        el_tkque_push(&pp->pending, (ElToken) { .type = EL_TT_INT_LITERAL, .lexeme = EL_SV("321"), .span = dir.span });
    }

    *out_tok = (ElToken) { .type = EL_TT_IDENT, .lexeme = EL_SV("hello"), .span = dir.span };
    return true;
}

static bool yield_pending_tokens(ElPreproc* pp, ElToken* out_tok) {
    if (pp->currently_streaming) {
        *out_tok = pp->stream.next(&pp->stream, pp->diag);
        if (out_tok->type != EL_TT_EOF)
            return true;

        pp->currently_streaming = false;
    }

    if (pp->pending.len != 0) {
        el_tkque_pop(&pp->pending, out_tok);
        return true;
    }
    return false;
}

bool el_pp_next(ElPreproc* pp, ElToken* out_tok, ElDiagEngine* diag) {
    pp->diag = diag;

    while (true) {
        if (yield_pending_tokens(pp, out_tok))
            return true;

        ElToken input_tok = pp->input.next(&pp->input, pp->diag);
        switch (input_tok.type) {
        case EL_TT_NEWLINE:
        case EL_TT_WHITESPACE:
        case EL_TT_LINE_COMMENT:
        case EL_TT_BLOCK_COMMENT:
            // skip whitespace, new lines and comments
            continue;

        case EL_TT_HASH:
            return preprocess_directive(pp, out_tok);

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
