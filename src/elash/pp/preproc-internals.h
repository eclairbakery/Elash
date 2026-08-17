#pragma once
#include <elash/pp/preproc.h> // IWYU pragma: export

#define INCLUDE_DEPTH_LIMIT 220

typedef struct ElPpFrame {
    ElTokenStream           stream;
    const ElSourceDocument* doc;
    struct ElPpFrame*       parent;
    ElToken                 pushback;
    bool                    has_pushback;
} ElPpFrame;

bool _el_pp_preprocess_directive(ElPreproc* pp, ElToken* out_tok);

bool _el_pp_handle_include(ElPreproc* pp, ElToken* out_tok);
bool _el_pp_handle_embed  (ElPreproc* pp, ElToken* out_tok);

bool _el_pp_peek(ElPreproc* pp, ElToken* out_tok, ElDiagEngine* diag);

void _el_pp_push_frame(ElPreproc* pp, ElTokenStream stream, const ElSourceDocument* doc);
bool _el_pp_read_directive_token(ElPreproc* pp, ElToken* out_tok);
bool _el_pp_peek_directive_token(ElPreproc* pp, ElToken* out_tok);
