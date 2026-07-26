#include <elash/hir/tree/expr.h>

ElHirExpr* el_hir_new_string_lit(
    ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElStringView chars, ElStorageClass scls
) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirExpr, {
        .kind = EL_HIR_EXPR_STRINGLIT,
        .type = type, .span = span,
        .as.string_lit = {
            .chars = chars,
            .scls = scls,
        },
    });
}
