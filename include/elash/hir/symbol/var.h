#pragma once

#include <elash/sema/type.h>
#include <elash/util/dynarena.h>
#include <elash/defs/sv.h>

typedef struct ElHirSymbol ElHirSymbol;

typedef struct ElHirVarSymbol {
    ElType* type;
} ElHirVarSymbol;

ElHirSymbol* el_hir_new_var_symbol(ElDynArena* arena, ElStringView name, ElType* type);
