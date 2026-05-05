#include <elash/hir/symbol/func.h>
#include <elash/hir/symbol.h>
#include <elash/sema/type/func.h>

ElHirSymbol* el_hir_new_func_symbol(
    ElDynArena* arena, ElStringView name, ElType* ret_type, ElHirSymbol** params, usize param_count
) {
    ElType** param_types = EL_DYNARENA_NEW_ARR(arena, ElType*, param_count);
    for (usize i = 0; i < param_count; i++) {
        param_types[i] = params[i]->as.var.type;
    }
    ElType* type = el_sema_new_func_type(arena, ret_type, param_types, param_count);

    ElHirSymbol* sym = EL_DYNARENA_NEW(arena, ElHirSymbol);
    sym->name = name;
    sym->kind = EL_HIR_SYM_FUNC;
    sym->as.func = (ElHirFuncSymbol) {
        .type = type,
        .ret_type = ret_type,
        .params = params,
        .param_count = param_count,
    };
    return sym;
}
