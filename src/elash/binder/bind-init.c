#include <elash/binder/binder.h>
#include <elash/util/assert.h>
#include <elash/util/todo.h>

#include <elash/diag/engine.h>
#include <elash/diag/meta.h>

#include <elash/hir/tree/expr/agginit.h>

static ElHirExpr* bind_init_list_array(ElBinder* binder, ElAstInit* in, ElHirType* expected_type) {
    ElHirArrayType* atype = &el_hir_type_unwrap_distinct(expected_type)->as.array;

    if (in->list.count != atype->size) {
        el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.bad-init-list",
            in->span,
            "unexpected amount of elements in initializer list for array of size ${size}",
            EL_DIAG_INT("size", atype->size)
        );
        return NULL;
    }

    ElHirType* base_type = atype->base;
    ElHirExpr** values = EL_DYNARENA_NEW_ARR(binder->hir_arena, ElHirExpr*, in->list.count);
    usize i = 0;
    for (ElAstInit* node = in->list.head; node != NULL; node = node->next, i++) {
        values[i] = el_binder_bind_init(binder, node, base_type);
        if (values[i] == NULL) return NULL;
    }

    return el_hir_new_agg_init(binder->hir_arena, in->span, expected_type, values, in->list.count, EL_STORAGECLS_LOCAL);
}

static ElHirExpr* bind_init_list_struct(ElBinder* binder, ElAstInit* in, ElHirType* expected_type) {
    const ElHirStructType* stype = &el_hir_type_unwrap_distinct(expected_type)->as.struct_;
    if (in->list.count != stype->count) {
        el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.bad-init-list",
            in->span,
            "unexpected amount of elements in initializer list for struct of field count ${count}",
            EL_DIAG_INT("count", stype->count)
        );
        return NULL;
    }

    ElHirExpr** values = EL_DYNARENA_NEW_ARR(binder->hir_arena, ElHirExpr*, stype->count);
    usize i = 0;
    for (ElAstInit* node = in->list.head; node != NULL; node = node->next, i++) {
        ElHirType* field_type = stype->fields[i].type;
        values[i] = el_binder_bind_init(binder, node, field_type);
        if (values[i] == NULL) return NULL;
    }

    return el_hir_new_agg_init(binder->hir_arena, in->span, expected_type, values, stype->count, EL_STORAGECLS_LOCAL);
}

static ElHirExpr* bind_init_list_tuple(ElBinder* binder, ElAstInit* in, ElHirType* expected_type) {
    ElHirTupleType* ttype = &el_hir_type_unwrap_distinct(expected_type)->as.tuple;
    if (in->list.count != ttype->count) {
        el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.bad-init-list",
            in->span,
            "unexpected amount of elements in initializer list for tuple of element count ${count}",
            EL_DIAG_INT("count", ttype->count)
        );
        return NULL;
    }

    ElHirExpr** values = EL_DYNARENA_NEW_ARR(binder->hir_arena, ElHirExpr*, ttype->count);
    usize i = 0;
    for (ElAstInit* node = in->list.head; node != NULL; node = node->next, i++) {
        ElHirType* elem_type = ttype->elements[i];
        values[i] = el_binder_bind_init(binder, node, elem_type);
        if (values[i] == NULL) return NULL;
    }

    return el_hir_new_agg_init(binder->hir_arena, in->span, expected_type, values, ttype->count, EL_STORAGECLS_LOCAL);
}

ElHirExpr* _el_binder_bind_init_list(ElBinder* binder, ElAstInit* in, ElHirType* expected_type) {
    ElHirType* etype = el_hir_type_unwrap_distinct(expected_type);
    switch (etype->kind) {
    case EL_HIR_TYPE_ARRAY:
        return bind_init_list_array(binder, in, expected_type);
    case EL_HIR_TYPE_STRUCT:
        return bind_init_list_struct(binder, in, expected_type);
    case EL_HIR_TYPE_TUPLE:
        return bind_init_list_tuple(binder, in, expected_type);
    default:
        el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.init-non-aggregate",
            in->span,
            "initializer list can only be used with aggregate types"
        );
        return NULL;
    }
}

ElHirExpr* el_binder_bind_init(ElBinder* binder, ElAstInit* in, ElHirType* expected_type) {
    switch (in->kind) {
    case EL_AST_INIT_EMPTY:
        EL_TODO("handle empty initializer");
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
        return _el_binder_bind_init_list(binder, in, expected_type);
    case EL_AST_INIT_DESIG:
        EL_TODO("implement designed initializers");
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstInitKind, in->kind);
}
