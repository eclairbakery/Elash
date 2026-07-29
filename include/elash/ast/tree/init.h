#pragma once

#include <elash/srcdoc/span.h>
#include <elash/util/dynarena.h>

#include "init/list.h"
#include "init/desig.h"

typedef struct ElAstExpr ElAstExpr;

typedef enum ElAstInitKind {
    EL_AST_INIT_EXPR,
    EL_AST_INIT_LIST,
    EL_AST_INIT_DESIG,
} ElAstInitKind;

typedef struct ElAstInit {
    ElAstInitKind kind;
    ElSourceSpan span;
    union {
        ElAstExpr* expr;
        ElAstInitList list;
        ElAstDesignatedInit desig;
    };
    struct ElAstInit* next;
} ElAstInit;

ElAstInit* el_ast_new_init_expr(ElDynArena* arena, ElAstExpr* expr);
