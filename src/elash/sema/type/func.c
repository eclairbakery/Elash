#include <elash/sema/type/func.h>
#include <elash/sema/type.h>

ElType* el_sema_new_func_type(ElDynArena* arena, ElType* ret_type, ElType** params, usize param_count) {
    ElType* type = EL_DYNARENA_NEW(arena, ElType);
    type->kind = EL_TYPE_FUNC;
    type->as.func.ret_type = ret_type;
    type->as.func.params = params;
    type->as.func.param_count = param_count;
    return type;
}
