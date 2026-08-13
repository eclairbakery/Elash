#include "../preproc-internals.h"

#include <elash/util/todo.h>

bool _el_pp_handle_include(ElPreproc* pp, ElToken* out_tok) {
    (void) pp, (void) out_tok;
    EL_TODO("implement #include");
}

bool _el_pp_handle_embed(ElPreproc* pp, ElToken* out_tok) {
    (void) pp, (void) out_tok;
    EL_TODO("implement #embed");
}
