#include <el-ast/bin.h>
#include <el-ast/node.h>

ElAstNode* el_ast_new_bin_expr(ElDynArena* arena, ElAstBinExprType type, ElAstNode* left, ElAstNode* right) {
    ElAstNode* node = EL_DYNARENA_NEW(arena, ElAstNode);
    node->type = EL_AST_NODE_BINARY_EXPR;
    node->as.binary.type = type;
    node->as.binary.left = left;
    node->as.binary.right = right;
    return node;
}
