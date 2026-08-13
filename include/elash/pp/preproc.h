#pragma once

#include <elash/util/dynarena.h>

#include <elash/lexer/tokbuf.h>
#include <elash/lexer/tokque.h>
#include <elash/lexer/token.h>
#include <elash/lexer/tokstream.h>

#include <elash/pp/vars.h>

#include <stdbool.h>

typedef struct ElPreproc {
    ElTokenStream input;
    ElTokenQueue pending;

    ElDynArena* arena;
    ElDiagEngine* diag;

    ElPpVars vars;
} ElPreproc;

bool el_pp_init(ElPreproc* pp, ElTokenStream input, ElDynArena* arena);
void el_pp_destroy(ElPreproc* pp);

bool el_pp_next(ElPreproc* pp, ElToken* out_tok, ElDiagEngine* diag);

ElTokenStream el_pp_as_token_stream(ElPreproc* pp);
