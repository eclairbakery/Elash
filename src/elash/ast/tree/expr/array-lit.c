#include <elash/ast/tree/expr.h>

ElAstExpr* el_ast_new_array_lit(
    ElDynArena* arena, ElSourceSpan span,
    ElStorageClass scls, ElAstType* type, ElAstInit* init
) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstExpr, {
        .type = EL_AST_EXPR_ARRAYLIT,
        .span = span,
        .next = NULL,
        .as.array_lit = {
            .scls = scls,
            .type = type,
            .init = init,
        },
    });
}
