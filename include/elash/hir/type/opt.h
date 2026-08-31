#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

typedef struct ElHirType ElHirType;

typedef struct ElHirOptType {
    ElHirType* base;
} ElHirOptType;

ElHirType* el_hir_new_opt_type(ElDynArena* arena, ElHirType* base);

