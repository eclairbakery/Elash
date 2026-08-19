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

bool _el_pp_handle_diag(ElPreproc* pp, ElDiagSeverity sev);

ElPpValue* _el_pp_eval(ElPreproc* pp);

bool _el_pp_next(ElPreproc* pp, ElToken* out_tok);
bool _el_pp_peek(ElPreproc* pp, ElToken* out_tok);
bool _el_pp_read(ElPreproc* pp, ElToken* out_tok);

ElToken _el_pp_advance(ElPreproc* pp);

bool _el_pp_match(ElPreproc* pp, ElTokenType type);
bool _el_pp_expect(ElPreproc* pp, ElTokenType type, ElSourceSpan span, ElStringView what);

void _el_pp_push_frame(ElPreproc* pp, ElTokenStream stream, const ElSourceDocument* doc);
