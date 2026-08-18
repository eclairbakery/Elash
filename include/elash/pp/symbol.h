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
        ElPpValue* var;
    } as;
} ElPpSymbol;
