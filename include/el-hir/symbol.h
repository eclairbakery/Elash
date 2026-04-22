#pragma once

#include "symbol/var.h"
#include "symbol/func.h"

typedef enum ElHirSymbolKind {
    EL_HIR_SYM_VAR,
    EL_HIR_SYM_FUNC,
} ElHirSymbolKind;

typedef struct ElHirSymbol {
    ElHirSymbolKind kind;
    union {
        ElHirVarSymbol var;
        ElHirFuncSymbol func;
    } as;
} ElHirSymbol;
