#include <elash/binder/binder.h>

bool _el_binder_is_const(ElBinder* binder, ElHirExpr* expr) {
    if (expr == NULL) return false;
    expr = _el_binder_simplify_expr(binder, expr);
    if (expr == NULL) return false;

    switch (expr->kind) {
    case EL_HIR_EXPR_LITERAL:
    case EL_HIR_EXPR_CONST:
        return true;
    case EL_HIR_EXPR_AGGINIT:
        for (usize i = 0; i < expr->as.agginit.count; i++) {
            if (!_el_binder_is_const(binder, expr->as.agginit.values[i])) {
                return false;
            }
        }
        return true;
    default:
        return false;
    }
}
