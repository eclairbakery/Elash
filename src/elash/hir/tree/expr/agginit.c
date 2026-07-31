#include <elash/hir/tree/expr.h>

ElHirExpr* el_hir_new_agg_init(
    ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElHirExpr** values, usize count, ElStorageClass scls
) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirExpr, {
        .kind = EL_HIR_EXPR_AGGINIT,
        .type = type,
        .span = span,
        .as.agginit = {
            .values = values,
            .count  = count,
            .scls = scls,
        },
    });
}
