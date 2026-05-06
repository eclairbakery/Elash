#pragma once

#include <elash/util/dynarena.h>
#include <elash/sema/type.h>

#include <elash/sema/symbol.h>

typedef struct ElMirValue ElMirValue;

typedef struct ElMirGlobalValue {
    ElSymbol* sym;
} ElMirGlobalValue;

ElMirValue* el_mir_new_global(ElDynArena* arena, ElType* type, ElSymbol* global);

