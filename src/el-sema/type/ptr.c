#include <el-sema/type/ptr.h>
#include <el-sema/type.h>

ElType* el_sema_new_ptr_type(ElDynArena* arena, ElType* base) {
    ElType* type = EL_DYNARENA_NEW(arena, ElType);
    type->kind = EL_TYPE_PTR;
    type->as.ptr.base = base;
    return type;
}

