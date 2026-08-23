#include <elash/ast/tree/toi.h>

#include <elash/ast/tree/toe.h>
#include <elash/util/assert.h>

ElAstToI* el_ast_new_toi_type(ElDynArena* arena, ElAstType* type) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstToI, {
        .kind = EL_AST_TOI_TYPE,
        .span = type->span,
        .as.type = type,
    });
}

ElAstToI* el_ast_new_toi_init(ElDynArena* arena, ElAstInit* init) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstToI, {
        .kind = EL_AST_TOI_INIT,
        .span = init->span,
        .as.init = init,
    });
}

ElAstToI* el_ast_new_toi_unr(ElDynArena* arena, ElAstUnr* unr) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstToI, {
        .kind = EL_AST_TOI_UNR,
        .span = unr->span,
        .as.unr = unr,
    });
}

ElAstType* el_ast_toi_as_type(ElDynArena* arena, ElAstToI* node) {
    if (node == NULL) return NULL;

    switch (node->kind) {
    case EL_AST_TOI_TYPE:
        return node->as.type;
    case EL_AST_TOI_INIT:
        return NULL;
    case EL_AST_TOI_UNR:
        return el_ast_unr_as_type(arena, node->as.unr);
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstToIKind, node->kind);
}

ElAstInit* el_ast_toi_as_init(ElDynArena* arena, ElAstToI* node) {
    if (node == NULL) return NULL;

    switch (node->kind) {
    case EL_AST_TOI_INIT:
        return node->as.init;
    case EL_AST_TOI_TYPE:
        return NULL;
    case EL_AST_TOI_UNR:
        return el_ast_unr_as_init(arena, node->as.unr);
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstToIKind, node->kind);
}

ElAstToI* el_ast_toi_from_toe(ElDynArena* arena, ElAstToE* toe) {
    if (toe == NULL) return NULL;

    switch (toe->kind) {
    case EL_AST_TOE_TYPE:
        return el_ast_new_toi_type(arena, toe->as.type);
    case EL_AST_TOE_EXPR: {
        ElAstInit* init = el_ast_new_init_expr(arena, toe->as.expr);
        return el_ast_new_toi_init(arena, init);
    }
    case EL_AST_TOE_UNR:
        return el_ast_new_toi_unr(arena, toe->as.unr);
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstToEKind, toe->kind);
}

void el_ast_append_toi(ElAstToI** head, ElAstToI** tail, ElAstToI* toi) {
    if (*tail != NULL) {
        (*tail)->next = toi;
        *tail = toi;
    } else {
        *head = *tail = toi;
    }
}
