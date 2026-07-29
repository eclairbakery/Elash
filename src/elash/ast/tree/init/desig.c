#include <elash/ast/tree/init.h>
#include <elash/ast/tree/init/desig.h>
#include <elash/util/dynarena.h>

ElAstInit* el_ast_new_desig_init(ElDynArena* arena, ElSourceSpan span, ElAstDesigInitElem* head, usize count) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstInit, {
        .kind = EL_AST_INIT_DESIG,
        .span = span,
        .desig = {
            .head = head,
            .count = count,
        },
        .next = NULL,
    });
}

void el_ast_desig_list_append(ElAstDesigInitElem** head, ElAstDesigInitElem** tail, ElAstDesigInitElem* desig) {
    desig->next = NULL;
    if (*tail) {
        (*tail)->next = desig;
        *tail = desig;
    } else {
        *head = *tail = desig;
    }
}
