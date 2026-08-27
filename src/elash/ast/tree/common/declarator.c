#include <elash/ast/tree/common/declarator.h>

void el_ast_append_declarator(ElAstDeclarator** head, ElAstDeclarator** tail, ElAstDeclarator* declarator) {
    if (*tail != NULL) {
        (*tail)->next = declarator;
        *tail = declarator;
    } else {
        *head = *tail = declarator;
    }
}

ElAstDeclarator* el_ast_new_declarator(ElDynArena* arena, ElAstType* type, ElAstIdent* name, ElAstInit* init) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstDeclarator, {
        .type = type,
        .name = name,
        .init = init,
        .next = NULL,
    });
}
