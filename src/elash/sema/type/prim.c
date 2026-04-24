#include <elash/sema/type/prim.h>
#include <elash/sema/type.h>

ElType* el_sema_new_prim_type(ElDynArena* arena, ElPrimitiveTypeKind kind) {
    ElType* type = EL_DYNARENA_NEW(arena, ElType);
    type->kind = EL_TYPE_PRIM;
    type->as.prim.kind = kind;
    return type;
}
