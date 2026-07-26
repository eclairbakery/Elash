#include <elash/hir/tree/decl.h>

ElHirDecl* el_hir_new_var_def(ElDynArena* arena, ElSourceSpan span, ElHirSymbol* sym, ElHirExpr* init, ElStorageClass scls) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirDecl, {
        .kind = EL_HIR_DECL_VAR_DEF,
        .span = span,
        .as.var_def = {
            .var  = sym,
            .init = init,
            .scls = scls,
        },
    });
}
