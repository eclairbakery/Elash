#include <el-ast/stmt.h>

ElAstBlockStmtNode el_ast_block_stmt(ElAstStmtNode* stmts) {
    return (ElAstBlockStmtNode) {
        .stmts = stmts,
    };
}

ElAstStmtNode* el_ast_new_block_stmt(ElDynArena* arena, ElAstStmtNode* stmts) {
    ElAstStmtNode* node = EL_DYNARENA_NEW(arena, ElAstStmtNode);
    node->type = EL_AST_STMT_BLOCK;
    node->next = NULL;
    node->as.block = el_ast_block_stmt(stmts);
    return node;
}
