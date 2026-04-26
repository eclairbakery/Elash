#pragma once

#include <elash/sema/type.h>
#include <elash/util/dynarena.h>
#include <elash/defs/sv.h>

typedef struct ElHirSymbol ElHirSymbol;

typedef struct ElHirTypeSymbol {
    ElType* type;
} ElHirTypeSymbol;

ElHirSymbol* el_hir_new_type_symbol(ElDynArena* arena, ElStringView name, ElType* type);
