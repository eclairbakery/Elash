#include <elash/sema/symbol/var.h>
#include <elash/sema/symbol.h>

ElSymbol* el_sema_new_var_symbol(ElDynArena* arena, ElStringView name, ElType* type) {
    ElSymbol* sym = EL_DYNARENA_NEW(arena, ElSymbol);
    sym->name = name;
    sym->kind = EL_SYM_VAR;
    sym->as.var = (ElVarSymbol) {
        .type = type,
    };
    return sym;
}
