#include <elash/hir/tree/expr/aggconst.h>
#include <elash/hir/tree/expr.h>

ElHirExpr* el_hir_new_agg_const(
    ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElHirExpr** values, usize count, ElStorageClass scls
) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirExpr, {
        .kind = EL_HIR_EXPR_AGGCONST,
        .type = type,
        .span = span,
        .as.aggconst = {
            .values = values,
            .count  = count,
            .scls = scls,
        },
    });
}
