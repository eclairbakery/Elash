#include <el-ast/unary.h>
#include <el-ast/node.h>

ElAstNode* el_ast_new_unary_expr(ElDynArena* arena, ElAstUnaryExprType type, ElAstNode* operand) {
    ElAstNode* node = EL_DYNARENA_NEW(arena, ElAstNode);
    node->type = EL_AST_NODE_UNARY_EXPR;
    node->as.unary.type = type;
    node->as.unary.operand = operand;
    return node;
}
