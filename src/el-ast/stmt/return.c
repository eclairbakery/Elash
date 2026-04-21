#include <el-ast/stmt.h>

ElAstReturnStmtNode el_ast_return_stmt(ElAstExprNode* value) {
    return (ElAstReturnStmtNode) {
        .value = value,
    };
}

ElAstStmtNode* el_ast_new_return_stmt(ElDynArena* arena, ElAstExprNode* value) {
    ElAstStmtNode* node = EL_DYNARENA_NEW(arena, ElAstStmtNode);
    node->type = EL_AST_STMT_RETURN;
    node->next = NULL;
    node->as.return_ = el_ast_return_stmt(value);
    return node;
}
