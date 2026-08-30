#include <elash/ast/tree/decl.h>
#include <elash/util/assert.h>

ElAstDecl* el_ast_new_var_decl(ElDynArena* arena, ElSourceSpan span, ElAstType* type, ElAstDeclarator* declarators) {
    for (ElAstDeclarator* d = declarators; d != NULL; d = d->next) {
        EL_ASSERT(d->init == NULL, "initializers not allowed in declarations");
    }

    return EL_DYNARENA_NEW_STRUCT(arena, ElAstDecl, {
        .type = EL_AST_DECL_VAR_DECL,
        .span = span,
        .as.var_decl = {
            type, declarators,
        },
    });
}
