#pragma once

#include <elash/sema/type.h>
#include <elash/util/dynarena.h>
#include <elash/defs/sv.h>

typedef struct ElSymbol ElSymbol;

typedef struct ElTypeSymbol {
    ElType* type;
} ElTypeSymbol;

ElSymbol* el_sema_new_type_symbol(ElDynArena* arena, ElStringView name, ElType* type);
