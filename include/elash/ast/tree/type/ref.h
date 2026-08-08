#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/doc.h>

typedef struct ElAstExpr ElAstExpr;
typedef struct ElAstType ElAstType;

typedef struct ElAstRefType {
    ElAstType* base;
} ElAstRefType;

ElAstType* el_ast_new_type_ref(ElDynArena* arena, ElSourceSpan span, ElAstType* base);
