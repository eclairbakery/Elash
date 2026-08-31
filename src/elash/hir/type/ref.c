#include <elash/hir/type.h>

ElHirType* el_hir_new_ref_type(ElDynArena* arena, ElHirType* base) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirType, {
        .kind = EL_HIR_TYPE_REF,
        .as.ref = { base },
    });
}
