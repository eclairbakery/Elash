#pragma once
#include <elash/pp/value.h>

typedef enum ElPpSymbolKind {
    EL_PP_SYM_VAR,
    //EL_PP_SYM_FUNC,
    //EL_PP_SYM_MACRO,
} ElPpSymbolKind;

typedef struct ElPpSymbol {
    ElStringView   name;
    ElPpSymbolKind kind;
    union {
        struct {
            ElPpValue* v;
            bool is_mutable;
        } var;
    } as;
} ElPpSymbol;

ElPpSymbol* _el_pp_new_sym_var(ElDynArena* arena, ElStringView name, ElPpValue* value, bool mut);
