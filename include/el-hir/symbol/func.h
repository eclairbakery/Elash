#pragma once

#include <el-defs/inttypes.h>
#include <el-sema/type.h>

typedef struct ElHirSymbol ElHirSymbol;

typedef struct ElHirFuncSymbol {
    ElType* ret_type;
    ElType** param_types;
    usize param_count;
} ElHirFuncSymbol;
