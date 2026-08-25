#include <elash/hir/tree/expr/cast.h>
#include <elash/hir/tree/expr.h>

ElHirExpr* el_hir_new_cast_expr(ElDynArena* arena, ElSourceSpan span, ElCastKind kind, ElHirType* type, ElHirExpr* expr) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirExpr, {
        .kind = EL_HIR_EXPR_CAST,
        .type = type,
        .span = span,
        .as.cast = {
            .kind = kind,
            .expr = expr,
        },
    });
}

ElHirExpr* el_hir_new_semcast_expr(ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElHirExpr* expr) {
    return el_hir_new_cast_expr(arena, span, EL_SEMCAST, type, expr);
}

ElHirExpr* el_hir_new_bitcast_expr(ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElHirExpr* expr) {
    return el_hir_new_cast_expr(arena, span, EL_BITCAST, type, expr);
}
