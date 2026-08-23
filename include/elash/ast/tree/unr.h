#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

#include <elash/ast/tree/common/ident.h>
#include <elash/ast/tree/expr.h>
#include <elash/ast/tree/init.h>
#include <elash/ast/tree/type.h>

typedef struct ElAstUnr ElAstUnr;

typedef enum ElAstUnrKind {
    EL_AST_UNR_IDENT,
    EL_AST_UNR_INDEX,
} ElAstUnrKind;

typedef struct ElAstUnrIndex {
    ElAstUnr*  base;
    ElAstUnr*  index;
    ElAstExpr* index_expr;
} ElAstUnrIndex;

struct ElAstUnr {
    ElAstUnrKind kind;
    ElSourceSpan span;
    union {
        ElAstIdent*   ident;
        ElAstUnrIndex index;
    } as;
};

ElAstUnr* el_ast_new_unr_ident(ElDynArena* arena, ElSourceSpan span, ElAstIdent* ident);
ElAstUnr* el_ast_new_unr_index(
    ElDynArena* arena, ElSourceSpan span,
    ElAstUnr* base, ElAstUnr* index, ElAstExpr* index_expr
);

ElAstType* el_ast_unr_as_type(ElDynArena* arena, ElAstUnr* node);
ElAstExpr* el_ast_unr_as_expr(ElDynArena* arena, ElAstUnr* node);
ElAstInit* el_ast_unr_as_init(ElDynArena* arena, ElAstUnr* node);
