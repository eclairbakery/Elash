#pragma once

#include <elash/util/dynarena.h>
#include <elash/defs/int-types.h>

typedef struct ElType ElType;

typedef struct ElFunctionType {
    ElType* ret_type;
    ElType** params;
    usize param_count;
} ElFunctionType;

ElType* el_sema_new_func_type(ElDynArena* arena, ElType* ret_type, ElType** params, usize param_count);
