#include "binder-internals.h"

#include <elash/diag/engine.h>
#include <elash/util/assert.h>

static ElHirExpr* len_from_array_type(ElBinder* binder, ElSourceSpan span, ElHirType* type) {
    if (type == NULL) return NULL;
    type = el_hir_type_unwrap_distinct(type);
    if (type->kind != EL_HIR_TYPE_ARRAY) return NULL;

    return el_hir_new_int_constant(
        binder->arena, span, binder->builtins->type_usize, (int64_t)type->as.array.size
    );
}

ElHirExpr* _el_binder_bind_len_call(ElBinder* binder, ElAstExpr* in, ElAstCallExpr* call) {
    if (call->arg_count != 1) {
        return el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.arg-count-mismatch",
            in->span, "expected 1 argument for 'len', but got ${got}",
            EL_DIAG_INT("got", call->arg_count)
        );
    }

    ElAstToI* arg = call->args;
    if (arg == NULL) return NULL;

    if (arg->kind == EL_AST_TOI_INIT && arg->as.init->kind != EL_AST_INIT_EXPR) {
        el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.invalid-builtin-call",
            arg->span, "argument to 'len' must be an expression or a type"
        );
        el_diag_help(
            binder->diag, "non-expression initializers cannot be passed directly to 'len'"
        );
        return NULL;
    }

    ElHirToE* toe = el_binder_bind_toi(binder, arg, NULL, EL_STORAGECLS_LOCAL);
    if (toe == NULL) return NULL;

    if (toe->is_type) {
        ElHirExpr* len = len_from_array_type(binder, in->span, toe->as.type);
        if (len != NULL) return len;

        return el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.type-mismatch",
            arg->span, "argument to 'len' must be an array or slice type"
        );
    }

    ElHirExpr* earg = toe->as.expr;
    if (earg == NULL) return NULL;

    ElHirType* type = el_hir_type_unwrap_distinct(earg->type);

    if (type->kind == EL_HIR_TYPE_ARRAY) {
        return el_hir_new_int_constant(
            binder->arena, in->span, binder->builtins->type_usize,
            (int64_t)type->as.array.size
        );
    }
    if (type->kind == EL_HIR_TYPE_SLICE) {
        return el_hir_new_slice_len_intr(binder->arena, in->span, binder->builtins->type_usize, earg);
    }

    return el_diag_report(
        binder->diag, EL_DIAG_ERROR, "sema.type-mismatch",
        arg->span, "argument to 'len' must be an array or slice"
    );
}

ElHirExpr* _el_binder_bind_mkslice_call(ElBinder* binder, ElAstExpr* in, ElAstCallExpr* call) {
    if (call->arg_count != 2) {
        return el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.arg-count-mismatch",
            in->span, "expected 2 arguments for 'mkslice', but got ${got}",
            EL_DIAG_INT("got", call->arg_count)
        );
    }

    ElAstToI* raw_arg = call->args;
    ElAstToI* len_arg = call->args != NULL ? call->args->next : NULL;
    if (raw_arg == NULL || len_arg == NULL) return NULL;

    if ((raw_arg->kind == EL_AST_TOI_INIT && raw_arg->as.init->kind != EL_AST_INIT_EXPR)
     || (len_arg->kind == EL_AST_TOI_INIT && len_arg->as.init->kind != EL_AST_INIT_EXPR)
    ) {
        el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.invalid-builtin-call",
            in->span,
            "expressions were expected as arguments to the mkslice function"
        );
        return NULL;
    }

    ElHirToE* raw_toe = el_binder_bind_toi(binder, raw_arg, NULL, EL_STORAGECLS_LOCAL);
    ElHirToE* len_toe = el_binder_bind_toi(binder, len_arg, NULL, EL_STORAGECLS_LOCAL);

    if (raw_toe == NULL || len_toe == NULL) return NULL;

    if (raw_toe->is_type || len_toe->is_type) {
        el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.invalid-builtin-call",
            in->span,
            "expressions were expected as arguments to the mkslice function"
        );
        return NULL;
    }

    ElHirExpr* raw = raw_toe->as.expr;
    if (raw == NULL) return NULL;
    if (raw->type->kind != EL_HIR_TYPE_RWSLICE) {
        return el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.type-mismatch",
            raw_arg->span, "first argument to 'mkslice' must be a raw slice"
        );
    }

    ElHirExpr* len = len_toe->as.expr;
    if (len == NULL) return NULL;

    len = _el_binder_implicit_cast(binder, len_arg->span, len, binder->builtins->type_usize);
    if (len == NULL) {
        return el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.type-mismatch",
            len_arg->span, "second argument to 'mkslice' must be convertible to usize"
        );
    }

    return el_hir_new_make_slice_intr(binder->arena, in->span, raw, len);
}

ElHirExpr* el_binder_bind_builtin_call(
    ElBinder* binder, ElAstExpr* in, ElAstCallExpr* call, ElHirSymbol* builtin
) {
    switch (builtin->as.builtin.kind) {
    case EL_BUILTIN_LEN:
        return _el_binder_bind_len_call(binder, in, call);
    case EL_BUILTIN_MKSLICE:
        return _el_binder_bind_mkslice_call(binder, in, call);
    }
    EL_UNREACHABLE_ENUM_VAL(ElBuiltinKind, builtin->as.builtin.kind);
}
