#include <elash/ast/tree/decl.h>

ElAstDecl* el_ast_new_typedef(ElDynArena* arena, ElSourceSpan span, ElStringView name, ElAstType* target) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstDecl, {
        .type = EL_AST_DECL_TYPEDEF,
        .span = span,
        .as.typedef_ = {
            .name = name,
            .target = target,
        },
    });
}

