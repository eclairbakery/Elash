#include <elash/hir/tree/expr/call.h>
#include <elash/hir/tree/expr.h>

ElHirExprNode* el_hir_new_call_expr(
    ElDynArena* arena, ElType* type, ElHirExprNode* callee, ElHirExprNode** args, usize arg_count
) {
    ElHirExprNode* node = EL_DYNARENA_NEW(arena, ElHirExprNode);
    node->kind = EL_HIR_EXPR_CALL;
    node->type = type;
    node->as.call = (ElHirCallExprNode) {
        .callee = callee,
        .args = args,
        .arg_count = arg_count,
    };
    return node;
}
