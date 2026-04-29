#include <elash/hir/symbol/func.h>
#include <elash/hir/symbol.h>

ElHirSymbol* el_hir_new_func_symbol(
    ElDynArena* arena, ElStringView name, ElType* ret_type, ElHirSymbol** params, usize param_count
) {
    ElHirSymbol* sym = EL_DYNARENA_NEW(arena, ElHirSymbol);
    sym->name = name;
    sym->kind = EL_HIR_SYM_FUNC;
    sym->as.func = (ElHirFuncSymbol) {
        .ret_type = ret_type,
        .params = params,
        .param_count = param_count,
    };
    return sym;
}
