#pragma once

#include <el-defs/inttypes.h>
#include <el-sema/type.h>

typedef struct ElHirSymbol ElHirSymbol;

typedef struct ElHirFuncSymbol {
    ElType* ret_type;
    ElHirSymbol** params;
    usize param_count;
} ElHirFuncSymbol;
