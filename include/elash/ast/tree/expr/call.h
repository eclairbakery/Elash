#pragma once

#include <elash/defs/int-types.h>
#include <elash/util/dynarena.h>
#include <elash/source/span.h>

typedef struct ElAstExpr ElAstExpr;
typedef struct ElAstToI ElAstToI;

typedef struct ElAstCallExpr {
    ElAstExpr* callee;
    ElAstToI* args;
    usize arg_count;
} ElAstCallExpr;

ElAstExpr* el_ast_new_call_expr(
    ElDynArena* arena, ElSourceSpan span, ElAstExpr* callee, ElAstToI* args, usize arg_count
);
