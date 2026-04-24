#pragma once

#include <elash/defs/inttypes.h>
#include <elash/sema/type.h>

typedef struct ElHirSymbol ElHirSymbol;

typedef struct ElHirFuncSymbol {
    ElType* ret_type;
    ElHirSymbol** params;
    usize param_count;
} ElHirFuncSymbol;
