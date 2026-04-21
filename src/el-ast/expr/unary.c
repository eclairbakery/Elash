#include <el-ast/expr/unary.h>
#include <el-ast/expr.h>

#include <el-util/assert.h>

ElStringView el_ast_unary_op_to_string(ElAstUnaryExprType type) {
    switch (type) {
    case EL_AST_UNARY_EXPR_POS: return EL_SV("+");
    case EL_AST_UNARY_EXPR_NEG: return EL_SV("-");

    case EL_AST_UNARY_EXPR_NOT:    return EL_SV("!");
    case EL_AST_UNARY_EXPR_BW_NOT: return EL_SV("~");

    case EL_AST_UNARY_EXPR_PRE_INC:  return EL_SV("pre  ++x");
    case EL_AST_UNARY_EXPR_PRE_DEC:  return EL_SV("pre  --x");
    case EL_AST_UNARY_EXPR_POST_INC: return EL_SV("post x++");
    case EL_AST_UNARY_EXPR_POST_DEC: return EL_SV("post x--");
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstUnaryExprType, type);
}

ElAstUnaryExprNode el_ast_unary_expr(ElAstUnaryExprType type, ElAstExprNode* operand) {
    return (ElAstUnaryExprNode) {
        .type = type,
        .operand = operand,
    };
}

ElAstExprNode* el_ast_new_unary_expr(ElDynArena* arena, ElAstUnaryExprType type, ElAstExprNode* operand) {
    ElAstExprNode* node = EL_DYNARENA_NEW(arena, ElAstExprNode);
    node->type = EL_AST_EXPR_UNARY;
    node->as.unary = el_ast_unary_expr(type, operand);
    return node;
}
