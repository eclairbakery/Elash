#include <el-hir/tree/expr/bin.h>
#include <el-hir/tree/expr.h>

ElHirExprNode* el_hir_new_bin_expr(ElDynArena* arena, ElType* type, ElSemaBinOp op, ElHirExprNode* left, ElHirExprNode* right) {
    ElHirExprNode* node = EL_DYNARENA_NEW(arena, ElHirExprNode);
    node->kind = EL_HIR_EXPR_BINARY;
    node->type = type;
    node->as.binary = (ElHirBinExprNode) {
        .left = left,
        .op = op,
        .right = right,
    };
    return node;
}
