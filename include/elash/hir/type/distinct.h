#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

typedef struct ElHirType ElHirType;

typedef struct ElHirDistinctType {
    ElHirType*   orig; // null if it's forward declared typedef
    ElStringView name;
} ElHirDistinctType;

ElHirType* el_hir_new_distinct_type(ElDynArena* arena, ElHirType* orig, ElStringView name);
