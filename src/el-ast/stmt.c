#include <el-ast/stmt.h>

ElAstStmtNode el_ast_expr_stmt(ElAstExprNode expr) {
    return (ElAstStmtNode) {
        .type = EL_AST_STMT_EXPR,
        .as.expr = expr,
    };
}

ElAstStmtNode* el_ast_new_expr_stmt(ElDynArena* arena, ElAstExprNode expr) {
    ElAstStmtNode* node = EL_DYNARENA_NEW(arena, ElAstStmtNode);
    *node = el_ast_expr_stmt(expr);
    return node;
}
