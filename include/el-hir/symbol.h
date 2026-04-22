#pragma once

#include <el-defs/sv.h>

#include "symbol/var.h"
#include "symbol/func.h"
#include "symbol/type.h"

typedef enum ElHirSymbolKind {
    EL_HIR_SYM_VAR,
    EL_HIR_SYM_FUNC,
    EL_HIR_SYM_TYPE,
} ElHirSymbolKind;

typedef struct ElHirSymbol {
    ElStringView name;
    ElHirSymbolKind kind;
    union {
        ElHirVarSymbol var;
        ElHirFuncSymbol func;
        ElHirTypeSymbol type;
    } as;
} ElHirSymbol;
