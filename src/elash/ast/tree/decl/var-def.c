#include <elash/ast/tree/decl.h>

ElAstDecl* el_ast_new_var_def(
    ElDynArena* arena, ElSourceSpan span, ElAstType* type, ElAstDeclarator* declarators, bool is_static
) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstDecl, {
        .type = EL_AST_DECL_VAR_DEF,
        .span = span,
        .as.var_def = {
            .is_static   = is_static,
            .type        = type,
            .declarators = declarators,
        },
    });
}
