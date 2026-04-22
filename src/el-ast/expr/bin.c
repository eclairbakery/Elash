#include <el-ast/expr/bin.h>
#include <el-ast/expr.h>

#include <el-util/assert.h>

ElAstBinExprNode el_ast_bin_expr(ElAstBinExprType type, ElAstExprNode* left, ElAstExprNode* right) {
    return (ElAstBinExprNode) {
        .left = left,
        .type = type,
        .right = right,
    };
}

ElAstExprNode* el_ast_new_bin_expr(ElDynArena* arena, ElAstBinExprType type, ElAstExprNode* left, ElAstExprNode* right) {
    ElAstExprNode* node = EL_DYNARENA_NEW(arena, ElAstExprNode);
    node->type = EL_AST_EXPR_BINARY;
    node->as.binary = el_ast_bin_expr(type, left, right);
    return node;
}
