#include "binder-internals.h"

#include <elash/diag/engine.h>
#include <elash/util/assert.h>
#include <elash/util/todo.h>

#include <elash/hir/toe.h>

ElHirToE* el_binder_bind_toe(ElBinder* binder, ElAstToE* in) {
    if (in == NULL) return NULL;

    switch (in->kind) {
    case EL_AST_TOE_TYPE: {
        ElHirType* type = el_binder_bind_type(binder, in->as.type);
        if (type == NULL) return NULL;
        return el_hir_new_toe_type(binder->arena, type);
    }
    case EL_AST_TOE_EXPR: {
        ElHirExpr* expr = el_binder_bind_expr(binder, in->as.expr);
        if (expr == NULL) return NULL;
        return el_hir_new_toe_expr(binder->arena, expr);
    }
    case EL_AST_TOE_UNR:
        return _el_binder_bind_unresolved(binder, in, in->as.unr);
    }

    EL_UNREACHABLE_ENUM_VAL(ElAstToEKind, in->kind);
}
