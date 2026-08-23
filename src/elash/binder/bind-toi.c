#include "binder-internals.h"
#include <elash/ast/tree/toi.h>
#include <elash/hir/toe.h>
#include <elash/diag/engine.h>

ElHirToE* el_binder_bind_toi(ElBinder* binder, ElAstToI* in, ElHirType* expected_type, ElStorageClass scls) {
    if (in == NULL) return NULL;

    ElHirToE* toe = NULL;
    switch (in->kind) {
    case EL_AST_TOI_TYPE: {
        ElHirType* type = _el_binder_bind_type(binder, in->as.type);
        if (type == NULL) return NULL;
        return el_hir_new_toe_type(binder->arena, type);
    }
    case EL_AST_TOI_INIT: {
        if (expected_type == NULL && in->as.init->kind == EL_AST_INIT_EXPR) {
            ElHirExpr* expr = el_binder_bind_expr(binder, in->as.init->expr);
            if (expr == NULL) return NULL;
            toe = el_hir_new_toe_expr(binder->arena, expr);
        } else {
            ElHirExpr* expr = el_binder_bind_init(binder, in->as.init, expected_type, scls);
            if (expr == NULL) return NULL;
            return el_hir_new_toe_expr(binder->arena, expr);
        }
        break;
    }
    case EL_AST_TOI_UNR:
        toe = _el_binder_bind_unresolved(binder, el_ast_new_toe_unr(binder->arena, in->as.unr), in->as.unr);
        break;
    }

    if (toe == NULL) return NULL;

    if (!toe->is_type && expected_type != NULL) {
        ElHirExpr* casted = _el_binder_implicit_cast(binder, in->span, toe->as.expr, expected_type);
        if (casted == NULL) return NULL;
        toe->as.expr = _el_binder_simplify_expr(binder, casted);
    }

    return toe;
}
