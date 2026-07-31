#include <elash/ast/tree/init.h>
#include <elash/ast/tree/init/desig.h>
#include <elash/util/dynarena.h>

ElAstDesignator* el_ast_new_desig_member(ElDynArena* arena, ElStringView member) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstDesignator, {
        .kind = EL_AST_DESIGNATOR_MEMBER,
        .as.member = member,
        .next = NULL,
    });
}

ElAstDesignator* el_ast_new_desig_tmember(ElDynArena* arena, usize tmember) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstDesignator, {
        .kind = EL_AST_DESIGNATOR_TMEMBER,
        .as.tmember = tmember,
        .next = NULL,
    });
}

ElAstDesignator* el_ast_new_desig_index(ElDynArena* arena, ElAstExpr* index) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstDesignator, {
        .kind = EL_AST_DESIGNATOR_INDEX,
        .as.index = index,
        .next = NULL,
    });
}

ElAstDesigInitElem* el_ast_new_desig_init_elem(ElDynArena* arena, ElAstDesignator* head, usize desig_count, ElAstInit* init) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstDesigInitElem, {
        .head = head,
        .desig_count = desig_count,
        .init = init,
        .next = NULL,
    });
}

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

void el_ast_desig_list_append(ElAstDesignator** head, ElAstDesignator** tail, ElAstDesignator* desig) {
    desig->next = NULL;
    if (*tail) {
        (*tail)->next = desig;
        *tail = desig;
    } else {
        *head = *tail = desig;
    }
}

void el_ast_desig_init_append(ElAstDesigInitElem** head, ElAstDesigInitElem** tail, ElAstDesigInitElem* elem) {
    elem->next = NULL;
    if (*tail) {
        (*tail)->next = elem;
        *tail = elem;
    } else {
        *head = *tail = elem;
    }
}
