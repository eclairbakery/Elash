#include <elash/ast/tree/toe.h>

#include <elash/util/assert.h>

ElAstToE* el_ast_new_toe_type(ElDynArena* arena, ElAstType* type) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstToE, {
        .kind = EL_AST_TOE_TYPE,
        .span = type->span,
        .as.type = type,
    });
}

ElAstToE* el_ast_new_toe_expr(ElDynArena* arena, ElAstExpr* expr) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstToE, {
        .kind = EL_AST_TOE_EXPR,
        .span = expr->span,
        .as.expr = expr,
    });
}

ElAstToE* el_ast_new_toe_unr(ElDynArena* arena, ElAstUnr* unr) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElAstToE, {
        .kind = EL_AST_TOE_UNR,
        .span = unr->span,
        .as.unr = unr,
    });
}

ElAstType* el_ast_toe_as_type(ElDynArena* arena, ElAstToE* node) {
    if (node == NULL) return NULL;

    switch (node->kind) {
    case EL_AST_TOE_TYPE:
        return node->as.type;
    case EL_AST_TOE_EXPR:
        return NULL;
    case EL_AST_TOE_UNR:
        return el_ast_unr_as_type(arena, node->as.unr);
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstToEKind, node->kind);
}

ElAstExpr* el_ast_toe_as_expr(ElDynArena* arena, ElAstToE* node) {
    if (node == NULL) return NULL;

    switch (node->kind) {
    case EL_AST_TOE_EXPR:
        return node->as.expr;
    case EL_AST_TOE_TYPE:
        return NULL;
    case EL_AST_TOE_UNR:
        return el_ast_unr_as_expr(arena, node->as.unr);
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstToEKind, node->kind);
}
