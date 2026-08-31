#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/doc.h>

typedef struct ElAstExpr ElAstExpr;
typedef struct ElAstType ElAstType;

typedef struct ElAstOptType {
    ElAstType* base;
} ElAstOptType;

ElAstType* el_ast_new_type_opt(ElDynArena* arena, ElSourceSpan span, ElAstType* base);

