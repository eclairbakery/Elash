#include <elash/ast/tree/expr.h>

ElAstExpr* el_ast_new_typedinit(
    ElDynArena* arena, ElSourceSpan span,
    ElStorageClass scls, ElAstType* type, ElAstInit* init
) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstExpr, {
        .type = EL_AST_EXPR_TYPEDINIT,
        .span = span,
        .next = NULL,
        .as.typedinit = {
            .scls = scls,
            .type = type,
            .init = init,
        },
    });
}
