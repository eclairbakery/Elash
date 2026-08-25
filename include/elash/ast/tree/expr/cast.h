#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

#include <elash/sema/cast.h>

typedef struct ElAstExpr ElAstExpr;
typedef struct ElAstType ElAstType;

typedef struct ElAstCastExpr {
    ElCastKind kind;
    ElAstExpr* expr;
    ElAstType* type;
} ElAstCastExpr;

ElAstExpr* el_ast_new_cast_expr(ElDynArena* arena, ElSourceSpan span, ElCastKind kind, ElAstExpr* expr, ElAstType* type);
