#pragma once

#include <elash/sema/type.h>

typedef struct ElHirSymbol ElHirSymbol;

typedef struct ElHirVarSymbol {
    ElType* type;
} ElHirVarSymbol;
