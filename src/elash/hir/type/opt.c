#include <elash/hir/type.h>

ElHirType* el_hir_new_opt_type(ElDynArena* arena, ElHirType* base) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirType, {
        .kind = EL_HIR_TYPE_OPT,
        .as.opt = { base },
    });
}

