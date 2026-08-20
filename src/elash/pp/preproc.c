#include "preproc-internals.h"

#include <elash/util/dynarena.h>

bool el_pp_init(
    ElPreproc* pp, ElTokenStream input, const ElSourceDocument* root_doc,
    ElDynArena* arena, const ElPpIncMap* imap
) {
    pp->frame = NULL;
    pp->has_lookahead = false;

    if (!el_tkque_init(&pp->pending)) {
        return false;
    }

    pp->imap  = imap;
    pp->arena = arena;

    _el_pp_push_frame(pp, input, root_doc);

    pp->current_scope = NULL;
    pp->builtin_scope = _el_pp_push_scope(pp);
    pp->global_scope  = _el_pp_push_scope(pp);

    return true;
}

void el_pp_free(ElPreproc* pp) {
    EL_ASSERT(pp->current_scope == pp->global_scope, "nested scopes were not popped before destruction");
    el_pp_scope_free(pp->global_scope);
    el_pp_scope_free(pp->builtin_scope);

    el_tkque_destroy(&pp->pending);
}

ElPpScope* _el_pp_push_scope(ElPreproc* pp) {
    ElPpScope* scope = el_pp_scope_new(pp->current_scope);
    pp->current_scope = scope;
    return scope;
}

ElPpScope* _el_pp_pop_scope(ElPreproc* pp) {
    ElPpScope* parent = pp->current_scope->parent;
    el_pp_scope_free(pp->current_scope);
    return pp->current_scope = parent;
}

void _el_pp_push_frame(ElPreproc* pp, ElTokenStream stream, const ElSourceDocument* doc) {
    pp->include_depth++;
    pp->frame = EL_DYNARENA_NEW_STRUCT(pp->arena, ElPpFrame, {
        .stream = stream,
        .doc    = doc,
        .parent = pp->frame,
    });

    _el_pp_push_scope(pp);
}

static void el_pp_pop_frame(ElPreproc* pp) {
    pp->frame = pp->frame->parent;
    pp->include_depth--;
    _el_pp_pop_scope(pp);
}

static void frame_unread(ElPreproc* pp, ElToken tok) {
    pp->frame->pushback = tok;
    pp->frame->has_pushback = true;
}

static bool read_from_active_frame(ElPreproc* pp, ElToken* out_tok) {
    if (pp->frame == NULL) {
        return false;
    }

    if (pp->frame->has_pushback) {
        *out_tok = pp->frame->pushback;
        pp->frame->has_pushback = false;
        return true;
    }

    *out_tok = pp->frame->stream.next(&pp->frame->stream, pp->diag);
    return out_tok->type != EL_TT_EOF;
}

bool _el_pp_read(ElPreproc* pp, ElToken* out_tok) {
    while (read_from_active_frame(pp, out_tok)) {
        switch (out_tok->type) {
        case EL_TT_WHITESPACE:
        case EL_TT_LINE_COMMENT:
        case EL_TT_BLOCK_COMMENT:
            continue;

        case EL_TT_NEWLINE:
        //case EL_TT_HASH:
            frame_unread(pp, *out_tok);
            return false;

        default:
            return true;
        }
    }

    return false;
}

bool _el_pp_peek(ElPreproc* pp, ElToken* out_tok) {
    if (!_el_pp_read(pp, out_tok)) {
        return false;
    }
    frame_unread(pp, *out_tok);
    return true;
}

bool _el_pp_next_internal(ElPreproc* pp, ElToken* out_tok, bool handle_directives) {
    while (true) {
        ElToken input_tok;

        if (pp->pending.len != 0) {
            el_tkque_pop(&pp->pending, &input_tok);
        } else if (pp->has_lookahead) {
            input_tok = pp->lookahead;
            pp->has_lookahead = false;
        } else if (pp->frame == NULL) {
            return false;
        } else if (!read_from_active_frame(pp, &input_tok)) {
            el_pp_pop_frame(pp);
            continue;
        }

        switch (input_tok.type) {
        case EL_TT_NEWLINE:
        case EL_TT_WHITESPACE:
        case EL_TT_LINE_COMMENT:
        case EL_TT_BLOCK_COMMENT:
            // skip whitespace, new lines and comments
            continue;

        case EL_TT_HASH:
            if (handle_directives) {
                return _el_pp_preprocess_directive(pp, input_tok, out_tok);
            } else {
                // maybe fallthrough would be a better approach BUT
                // -Werror=implicit-fallthrough won't let me do that
                *out_tok = input_tok;
                return true;
            }

        default:
            *out_tok = input_tok;
            return true;
        }
    }
}

bool _el_pp_next(ElPreproc* pp, ElToken* out_tok) {
    return _el_pp_next_internal(pp, out_tok, false);
}
bool _el_pp_next_d(ElPreproc* pp, ElToken* out_tok) {
    return _el_pp_next_internal(pp, out_tok, true);
}

bool el_pp_next(ElPreproc* pp, ElToken* out_tok, ElDiagEngine* diag) {
    pp->diag = diag;
    return _el_pp_next_internal(pp, out_tok, true);
}

ElToken _el_pp_advance(ElPreproc* pp) {
    ElToken tok;
    if (!_el_pp_read(pp, &tok)) {
        return (ElToken) { .type = EL_TT_EOF };
    }
    return tok;
}

bool _el_pp_match(ElPreproc* pp, ElTokenType type) {
    ElToken tok;
    if (!_el_pp_peek(pp, &tok) || tok.type != type) {
        return false;

    }
    _el_pp_advance(pp);
    return true;
}

bool _el_pp_expect(ElPreproc* pp, ElTokenType type, ElSourceSpan span, ElStringView what) {
    ElToken tok = _el_pp_advance(pp);
    if (tok.type == type) {
        return true;
    }

    return el_diag_report(
        pp->diag, EL_DIAG_ERROR, "pp.unexpected-token",
        span, "expected ${what}, found ${tok}",
        EL_DIAG_STRING("what", what),
        EL_DIAG_TOKEN("tok", tok),
    );
}

static ElToken _el_pp_token_stream_next(ElTokenStream* self, ElDiagEngine* diag) {
    ElPreproc* pp = self->ctx;
    ElToken tok;
    if (!el_pp_next(pp, &tok, diag)) {
        return (ElToken){ .type = EL_TT_EOF };
    }
    return tok;
}

ElTokenStream el_pp_as_token_stream(ElPreproc* pp) {
    return (ElTokenStream) {
        .next = _el_pp_token_stream_next,
        .ctx = pp,
    };
}
