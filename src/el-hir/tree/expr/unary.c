#include <el-hir/tree/expr/unary.h>
#include <el-hir/tree/expr.h>

ElHirExprNode* el_hir_new_unary_expr(ElDynArena* arena, ElType* type, ElSemaUnaryOp op, ElHirExprNode* operand) {
    ElHirExprNode* node = EL_DYNARENA_NEW(arena, ElHirExprNode);
    node->kind = EL_HIR_EXPR_UNARY;
    node->type = type;
    node->as.unary = (ElHirUnaryExprNode) {
        .op = op,
        .operand = operand,
    };
    return node;
}
