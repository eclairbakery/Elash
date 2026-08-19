#include <elash/pp/preproc.h>
#include <elash/pp/value.h>

#include <elash/util/assert.h>

ElStringView _el_pp_type_name(ElPpType type) {
    switch (type) {
    case EL_PP_TYPE_INT:   return EL_SV("int");
    case EL_PP_TYPE_BOOL:  return EL_SV("bool");
    case EL_PP_TYPE_FLOAT: return EL_SV("float");
    case EL_PP_TYPE_CHAR:  return EL_SV("char");
    case EL_PP_TYPE_LIST:  return EL_SV("list");
    case EL_PP_TYPE_STR:   return EL_SV("string");
    case EL_PP_TYPE_TOK:   return EL_SV("token");
    }
    EL_UNREACHABLE_ENUM_VAL(ElPpType, type);
}

ElPpValue* _el_pp_strcat(ElPreproc* pp, ElPpValue* lhs, ElPpValue* rhs) {
    EL_ASSERT(lhs->type == EL_PP_TYPE_STR && rhs->type == EL_PP_TYPE_STR, "invalid operand types");

    usize len = lhs->as.str_.len + rhs->as.str_.len;
    char* buf = EL_DYNARENA_NEW_ARR(pp->arena, char, len);

    if (len != 0) {
        memcpy(buf, lhs->as.str_.data, lhs->as.str_.len);
        memcpy(buf + lhs->as.str_.len, rhs->as.str_.data, rhs->as.str_.len);
    }
    return _el_pp_new_str(pp->arena, el_sv_from_data_and_len(buf, len));
}

ElPpValue* _el_pp_listcat(ElPreproc* pp, ElPpValue* lhs, ElPpValue* rhs) {
    EL_ASSERT(lhs->type == EL_PP_TYPE_LIST && rhs->type == EL_PP_TYPE_LIST, "invalid operand types");

    usize count = lhs->as.list_.count + rhs->as.list_.count;
    ElPpValue** values = NULL;

    if (count != 0) {
        values = EL_DYNARENA_NEW_ARR(pp->arena, ElPpValue*, count);
        memcpy(values, lhs->as.list_.values, lhs->as.list_.count * sizeof(ElPpValue*));
        memcpy(values + lhs->as.list_.count, rhs->as.list_.values, rhs->as.list_.count * sizeof(ElPpValue*));
    }
    return _el_pp_new_list(pp->arena, (ElPpList) { values, count });
}

/////// constructors ///////
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
