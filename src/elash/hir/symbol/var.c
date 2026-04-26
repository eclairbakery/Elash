#include <elash/hir/symbol/var.h>
#include <elash/hir/symbol.h>

ElHirSymbol* el_hir_new_var_symbol(ElDynArena* arena, ElStringView name, ElType* type) {
    ElHirSymbol* sym = EL_DYNARENA_NEW(arena, ElHirSymbol);
    sym->name = name;
    sym->kind = EL_HIR_SYM_VAR;
    sym->as.var = (ElHirVarSymbol) {
        .type = type,
    };
    return sym;
}
