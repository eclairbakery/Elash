#pragma once

#include <elash/defs/int-types.h>
#include <elash/sema/type.h>
#include <elash/util/dynarena.h>
#include <elash/defs/sv.h>

typedef struct ElHirSymbol ElHirSymbol;

typedef struct ElHirFuncSymbol {
    ElType* type;
    ElType* ret_type;
    ElHirSymbol** params;
    usize param_count;
} ElHirFuncSymbol;

ElHirSymbol* el_hir_new_func_symbol(
    ElDynArena* arena, ElStringView name, ElType* ret_type, ElHirSymbol** params, usize param_count
);
