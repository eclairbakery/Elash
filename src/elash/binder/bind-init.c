#include <elash/binder/binder.h>
#include <elash/util/assert.h>
#include <elash/util/todo.h>

ElHirExpr* el_binder_bind_init(ElBinder* binder, ElAstInit* in, ElHirType* expected_type) {
    switch (in->kind) {
    case EL_AST_INIT_EMPTY:
        // TODO: this works for now but it's not the best solution
        return el_binder_bind_init_list(
            binder, el_ast_new_init_list(binder->hir_arena, in->span, NULL, 0), expected_type
        );
    case EL_AST_INIT_EXPR: {
        ElHirExpr* expr = el_binder_bind_expr(binder, in->expr);
        if (expr == NULL) return NULL;

        //if (!el_hir_type_eql(expr->type, expected_type)) {
        //    el_diag_report(
        //        binder->diag, EL_DIAG_ERROR, "sema.type-mismatch",
        //        in->span,
        //        "incompatible type in initializer"
        //    );
        //    return NULL;
        //}
        return _el_binder_implicit_cast(binder, in->span, expr, expected_type);
    }
    case EL_AST_INIT_LIST:
        return el_binder_bind_init_list(binder, in, expected_type);
    case EL_AST_INIT_DESIG:
        return el_binder_bind_designated(binder, in, expected_type);
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstInitKind, in->kind);
}
