#include "binder-internals.h"

#include <elash/diag/engine.h>
#include <elash/util/assert.h>
#include <elash/util/todo.h>

#include <elash/hir/tree/stmt/block.h>
#include <elash/hir/tree/stmt/return.h>

bool _el_binder_is_lvalue(ElHirExpr* expr) {
    if (expr == NULL) return false;
    return (expr->kind == EL_HIR_EXPR_UNARY && expr->as.unary.op == EL_SEMA_UNARY_OP_DEREF)
        || (expr->kind == EL_HIR_EXPR_BINARY && expr->as.binary.op == EL_SEMA_BIN_OP_INDEX)
        || (expr->kind == EL_HIR_EXPR_SYMBOL)
        || (expr->kind == EL_HIR_EXPR_MEMBER)
        || (expr->kind == EL_HIR_EXPR_TMEMBER);
}

ElHirBlockStmt _el_binder_bind_block(ElBinder* binder, ElAstBlockStmt* in) {
    ElHirStmt* head = NULL;
    ElHirStmt* tail = NULL;

    for (ElAstStmt* curr = in->stmts; curr != NULL; curr = curr->next) {
        ElHirStmt* binded = el_binder_bind_stmt(binder, curr);
        if (binded) {
            el_hir_stmt_list_append(&head, &tail, binded);
        }
    }

    return (ElHirBlockStmt) { .stmts = head };
}

ElHirStmt* _el_binder_bind_return(ElBinder* binder, ElAstStmt* in) {
    ElHirExpr* val = NULL;
    if (in->as.return_.value != NULL) {
        val = el_binder_bind_init(
            binder, in->as.return_.value,
            binder->current_func->as.func.type->as.func.ret_type,
            EL_STORAGECLS_LOCAL
        );
        if (val == NULL) goto skip;
    }

    bool is_void_func
        = el_hir_type_eql(binder->current_func->as.func.type->as.func.ret_type, binder->builtins->type_void);

    if (val == NULL) {
        if (!is_void_func) {
            el_diag_report(
                binder->diag, EL_DIAG_ERROR, "sema.return-val-expected",
                in->span,
                "expected return value in non-void function"
            );
        }
    } else {
        bool is_val_void = el_hir_type_eql(val->type, binder->builtins->type_void);
        if (is_void_func) {
            if (!is_val_void) {
                el_diag_report(
                    binder->diag, EL_DIAG_ERROR, "sema.return-val-in-void-func",
                    in->span,
                    "void function should not return a value"
                );
            }
        } else {
            if (is_val_void) {
                el_diag_report(
                    binder->diag, EL_DIAG_ERROR, "sema.returning-void",
                    in->span,
                    "cannot return void value from non-void ction"
                );
            } else {
                val = _el_binder_implicit_cast(binder, in->span, val, binder->current_func->as.func.type->as.func.ret_type);
                if (val == NULL) return NULL;
            }
        }
    }

skip:
    return el_hir_new_return_stmt(binder->arena, in->span, val);
}

#define REPORT_ASSIGN_TO_RVALUE(BINDER, SPAN)                \
    el_diag_report(                                          \
        (BINDER)->diag, EL_DIAG_ERROR, "sema.assign-rvalue", \
        (SPAN), "cannot assign to rvalue",                   \
    )                                                        \

ElHirStmt* _el_binder_bind_assign(ElBinder* binder, ElAstStmt* in, ElAstAssignStmt* assign) {
    ElHirExpr* target = el_binder_bind_expr(binder, assign->target);
    if (!_el_binder_is_lvalue(target)) {
        REPORT_ASSIGN_TO_RVALUE(binder, in->span);
        return NULL;
    }

    ElHirExpr* value = el_binder_bind_init(binder, assign->value, target->type, EL_STORAGECLS_LOCAL);
    if (value == NULL) return NULL;

    return el_hir_new_assign_stmt(
        binder->arena, in->span,
        target, value
    );
}

ElHirStmt* _el_binder_bind_compound_assign(ElBinder* binder, ElAstStmt* in, ElAstCompoundAssignStmt* cassign) {
    ElHirExpr* target = el_binder_bind_expr(binder, cassign->target);
    if (!_el_binder_is_lvalue(target)) {
        REPORT_ASSIGN_TO_RVALUE(binder, in->span);
        return NULL;
    }

    ElHirExpr* value = el_binder_bind_init(binder, cassign->value, target->type, EL_STORAGECLS_LOCAL);
    if (value == NULL) return NULL;

    return el_hir_new_compound_assign_stmt(
        binder->arena, in->span,
        cassign->op, target, value
    );
}

ElHirStmt* el_binder_bind_stmt(ElBinder* binder, ElAstStmt* in) {
    switch (in->type) {
    case EL_AST_STMT_BLOCK: {
        ElHirBlockStmt block = _el_binder_bind_block(binder, &in->as.block);
        return el_hir_new_block_stmt(binder->arena, in->span, block.stmts);
    }
    case EL_AST_STMT_RETURN:
        return _el_binder_bind_return(binder, in);
    case EL_AST_STMT_EXPR: {
        ElHirExpr* expr = el_binder_bind_expr(binder, in->as.expr);
        if (expr != NULL) {
            expr = _el_binder_apply_default_type(binder, expr);
        }
        return el_hir_new_expr_stmt(binder->arena, in->span, expr);
    }

    case EL_AST_STMT_IF: {
        ElHirExpr* cond = el_binder_bind_expr(binder, in->as.if_.cond);
        if (cond == NULL) return NULL;
        cond = _el_binder_implicit_cast(binder, in->as.if_.cond->span, cond, binder->builtins->type_bool);
        if (cond == NULL) return NULL;

        ElHirStmt* then = el_binder_bind_stmt(binder, in->as.if_.then);
        if (then == NULL) return NULL;

        ElHirStmt* else_ = NULL;
        if (in->as.if_.else_ != NULL) {
            else_ = el_binder_bind_stmt(binder, in->as.if_.else_);
            if (else_ == NULL) return NULL;
        }

        return el_hir_new_if_stmt(
            binder->arena, in->span,
            cond, then, else_
        );
    }
    case EL_AST_STMT_WHILE: {
        binder->loop_depth++;
        ElHirExpr* cond = el_binder_bind_expr(binder, in->as.while_.cond);
        if (cond == NULL)
            return binder->loop_depth--, NULL;

        cond = _el_binder_implicit_cast(binder, in->as.while_.cond->span, cond, binder->builtins->type_bool);
        if (cond == NULL)
            return binder->loop_depth--, NULL;

        ElHirStmt* body = el_binder_bind_stmt(binder, in->as.while_.body);
        if (body == NULL)
            return binder->loop_depth--, NULL;

        binder->loop_depth--;

        return el_hir_new_while_stmt(
            binder->arena, in->span,
            cond, body
        );
    }

    case EL_AST_STMT_BREAK:
        if (binder->loop_depth <= 0)
            return el_diag_report(
                binder->diag, EL_DIAG_ERROR, "sema.break-outside-loop",
                in->span, "'break' can only be used inside loops.",
            );
        return el_hir_new_break_stmt(binder->arena, in->span);
    case EL_AST_STMT_CONTINUE:
        if (binder->loop_depth <= 0)
            return el_diag_report(
                binder->diag, EL_DIAG_ERROR, "sema.continue-outside-loop",
                in->span, "'continue' can only be used inside loops.",
            );
        return el_hir_new_continue_stmt(binder->arena, in->span);

    case EL_AST_STMT_ASSIGN:
        return _el_binder_bind_assign(binder, in, &in->as.assign);
    case EL_AST_STMT_COMPOUND_ASSIGN:
        return _el_binder_bind_compound_assign(binder, in, &in->as.cassign);
    case EL_AST_STMT_DECL: {
        ElHirDecl* decl = el_binder_bind_decl(binder, in->as.decl);
        if (!decl) return NULL;
        return el_hir_new_decl_stmt(binder->arena, in->span, decl);
    }
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstStmtType, in->type);
}
