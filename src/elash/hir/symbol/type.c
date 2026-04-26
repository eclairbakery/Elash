#include <elash/hir/symbol/type.h>
#include <elash/hir/symbol.h>

ElHirSymbol* el_hir_new_type_symbol(ElDynArena* arena, ElStringView name, ElType* type) {
    ElHirSymbol* sym = EL_DYNARENA_NEW(arena, ElHirSymbol);
    sym->name = name;
    sym->kind = EL_HIR_SYM_TYPE;
    sym->as.type = (ElHirTypeSymbol) {
        .type = type,
    };
    return sym;
}
