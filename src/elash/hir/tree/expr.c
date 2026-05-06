#include <elash/hir/tree/expr.h>

ElHirExprNode* el_hir_new_symbol_expr(ElDynArena* arena, ElType* type, ElSymbol* symbol) {
    ElHirExprNode* node = EL_DYNARENA_NEW(arena, ElHirExprNode);
    node->kind = EL_HIR_EXPR_SYMBOL;
    node->type = type;
    node->as.symbol = symbol;
    return node;
}
