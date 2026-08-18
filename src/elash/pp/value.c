#include <elash/pp/value.h>

ElPpValue* _el_pp_new_int(ElDynArena* arena, int64_t val) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElPpValue, {
        .type = EL_PP_TYPE_INT,
        .as.int_ = val,
    });
}

ElPpValue* _el_pp_new_float(ElDynArena* arena, double val) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElPpValue, {
        .type = EL_PP_TYPE_FLOAT,
        .as.float_ = val,
    });
}

ElPpValue* _el_pp_new_bool(ElDynArena* arena, bool val) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElPpValue, {
        .type = EL_PP_TYPE_BOOL,
        .as.bool_ = val,
    });
}

ElPpValue* _el_pp_new_char(ElDynArena* arena, char val) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElPpValue, {
        .type = EL_PP_TYPE_CHAR,
        .as.char_ = val,
    });
}

ElPpValue* _el_pp_new_str(ElDynArena* arena, ElStringView val) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElPpValue, {
        .type = EL_PP_TYPE_STR,
        .as.str_ = el_dynarena_clone_sv(arena, val),
    });
}

ElPpValue* _el_pp_new_tok(ElDynArena* arena, ElToken tok) {
    tok.lexeme = el_dynarena_clone_sv(arena, tok.lexeme);
    return EL_DYNARENA_NEW_STRUCT(arena, ElPpValue, {
        .type = EL_PP_TYPE_TOK,
        .as.tok_ = tok,
    });
}

ElPpValue* _el_pp_new_list(ElDynArena* arena, ElPpList list) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElPpValue, {
        .type = EL_PP_TYPE_LIST,
        .as.list_ = list,
    });
}
