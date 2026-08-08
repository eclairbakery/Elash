#pragma once

#include <elash/hir/symbol.h>
#include <elash/hir/tree/expr.h>
#include <elash/util/dynarena.h>
#include <elash/source/span.h>
#include <elash/sema/storage-cls.h>

typedef struct ElHirDecl ElHirDecl;

typedef struct ElHirVarDef {
    ElHirSymbol*   var;
    ElHirExpr*     init; // nullable
    ElStorageClass scls;
} ElHirVarDef;

ElHirDecl* el_hir_new_var_def(ElDynArena* arena, ElSourceSpan span, ElHirSymbol* sym, ElHirExpr* init, ElStorageClass scls);
