#include <elash/pp/symbol.h>

ElPpSymbol* _el_pp_new_sym_var(ElDynArena* arena, ElStringView name, ElPpValue* value) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElPpSymbol, {
        .name = name,
        .kind = EL_PP_SYM_VAR,
        .as.var = value,
    });
}
