#include <el-ast/expr/unary.h>
#include <el-ast/expr.h>

#include <el-util/assert.h>

ElAstUnaryExprNode el_ast_unary_expr(ElAstUnaryOp type, ElAstExprNode* operand) {
    return (ElAstUnaryExprNode) {
        .type = type,
        .operand = operand,
    };
}

ElAstExprNode* el_ast_new_unary_expr(ElDynArena* arena, ElAstUnaryOp type, ElAstExprNode* operand) {
    ElAstExprNode* node = EL_DYNARENA_NEW(arena, ElAstExprNode);
    node->type = EL_AST_EXPR_UNARY;
    node->as.unary = el_ast_unary_expr(type, operand);
    return node;
}
