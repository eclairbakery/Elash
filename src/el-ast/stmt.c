#include <el-ast/stmt.h>

#include <stdio.h>

ElAstStmtNode el_ast_expr_stmt(ElAstExprNode* expr) {
    return (ElAstStmtNode) {
        .type = EL_AST_STMT_EXPR,
        .as.expr = expr,
        .next = NULL,
    };
}

ElAstStmtNode* el_ast_new_expr_stmt(ElDynArena* arena, ElAstExprNode* expr) {
    ElAstStmtNode* node = EL_DYNARENA_NEW(arena, ElAstStmtNode);
    *node = el_ast_expr_stmt(expr);
    return node;
}

void el_ast_dump_stmt_impl(ElAstStmtNode* node, size_t ident, FILE* out) {
    switch (node->type) {
    case EL_AST_STMT_EXPR:
        el_ast_dump_expr_print_ident(ident, out);
        fprintf(out, "ExprStmt:\n");
        el_ast_dump_expr_impl(node->as.expr, ident + 1, out);
        break;
    case EL_AST_STMT_RETURN:
        el_ast_dump_expr_print_ident(ident, out);
        fprintf(out, "ReturnStmt:\n");
        if (node->as.return_.value) {
            el_ast_dump_expr_impl(node->as.return_.value, ident + 1, out);
        } else {
            el_ast_dump_expr_print_ident(ident + 1, out);
            fprintf(out, "void\n");
        }
        break;
    }
}

void el_ast_dump_stmt(ElAstStmtNode* root, FILE* out) {
    el_ast_dump_stmt_impl(root, 0, out);
}

