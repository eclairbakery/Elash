#include <elash/pp/value.h>

bool el_pp_value_type_is_trivial(ElPpValueType type) {
    return type == EL_PP_VAR_INT
        || type == EL_PP_VAR_FLOAT
        || type == EL_PP_VAR_BOOL
        || type == EL_PP_VAR_CHAR
        || type == EL_PP_VAR_TOKEN;
}

bool el_pp_value_copy(const ElPpValue* src, ElPpValue* dst) {
    dst->type = src->type;
    switch (src->type) {
    case EL_PP_VAR_INT:    dst->as.int_   = src->as.int_;   return true;
    case EL_PP_VAR_FLOAT:  dst->as.float_ = src->as.float_; return true;
    case EL_PP_VAR_BOOL:   dst->as.bool_  = src->as.bool_;  return true;
    case EL_PP_VAR_CHAR:   dst->as.char_  = src->as.char_;  return true;
    case EL_PP_VAR_TOKEN:  dst->as.tok_   = src->as.tok_;   return true;
    case EL_PP_VAR_STRING: return el_strbuf_copy(&src->as.str_, &dst->as.str_);
    case EL_PP_VAR_ARRAY:  return el_pp_valarr_copy(&src->as.arr_, &dst->as.arr_);
    }
    return false;
}

void el_pp_value_move(ElPpValue* src, ElPpValue* dst) {
    if (el_pp_value_type_is_trivial(src->type)) {
        (void)el_pp_value_copy(src, dst);
    } else if (src->type == EL_PP_VAR_ARRAY) {
        el_pp_valarr_move(&src->as.arr_, &dst->as.arr_);
    } else if (src->type == EL_PP_VAR_STRING) {
        el_strbuf_move(&src->as.str_, &dst->as.str_);
    }
}

void el_pp_value_free(ElPpValue* val) {
    if (val->type == EL_PP_VAR_ARRAY) {
        el_pp_valarr_destroy(&val->as.arr_);
    } else if (val->type == EL_PP_VAR_STRING) {
        el_strbuf_destroy(&val->as.str_);
    }
}
