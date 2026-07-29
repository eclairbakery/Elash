#include <elash/hir/type.h>

ElHirType* el_hir_new_distinct_type(ElDynArena* arena, ElHirType* orig, ElStringView name) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirType, {
        .kind = EL_HIR_TYPE_DISTINCT,
        .as.distinct = { orig, name },
    });
}
