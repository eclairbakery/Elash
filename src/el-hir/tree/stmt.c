#include <el-hir/tree/stmt.h>

#include <stddef.h>

ElHirStmtNode* el_hir_new_expr_stmt(ElDynArena* arena, ElHirExprNode* expr) {
    ElHirStmtNode* node = EL_DYNARENA_NEW(arena, ElHirStmtNode);
    node->kind = EL_HIR_STMT_EXPR;
    node->next = NULL;
    node->as.expr = expr;
    return node;
}

void el_hir_stmt_list_append(ElHirStmtNode** head, ElHirStmtNode** tail, ElHirStmtNode* stmt) {
    if (*head == NULL) {
        *head = stmt;
        *tail = stmt;
    } else {
        (*tail)->next = stmt;
        *tail = stmt;
    }
}
