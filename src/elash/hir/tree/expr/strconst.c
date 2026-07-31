#include <elash/hir/tree/expr.h>

ElHirExpr* el_hir_new_string_const(
    ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElStringView chars, ElStorageClass scls
) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirExpr, {
        .kind = EL_HIR_EXPR_STRCONST,
        .type = type, .span = span,
        .as.strconst = {
            .chars = chars,
            .scls = scls,
        },
    });
}
