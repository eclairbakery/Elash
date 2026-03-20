#include <el-pp/value.h>

bool el_pp_value_type_is_trivial(ElPpValueType type)  {
    return type == EL_PP_VAR_INT
        || type == EL_PP_VAR_FLOAT 
        || type == EL_PP_VAR_BOOL
        || type == EL_PP_VAR_CHAR 
        || type == EL_PP_VAR_STRING; // its just a string-view so its trivially copyable
}

bool el_pp_value_copy(const ElPpValue* src, ElPpValue* dst) {
    dst->type = src->type;
    switch (src->type) {
    case EL_PP_VAR_INT:    dst->as_int =   src->as_int;   return true;
    case EL_PP_VAR_FLOAT:  dst->as_float = src->as_float; return true;
    case EL_PP_VAR_BOOL:   dst->as_bool =  src->as_bool;  return true;
    case EL_PP_VAR_CHAR:   dst->as_char =  src->as_char;  return true;
    case EL_PP_VAR_STRING: dst->as_str =   src->as_str;   return true;
    case EL_PP_VAR_ARRAY:  return el_pp_valarr_copy(&src->as_arr, &dst->as_arr);
    case EL_PP_VAR_TOKENS: return el_tkbuf_copy(&src->as_toks, &dst->as_toks);
    }
    return false;
}

void el_pp_value_move(ElPpValue* src, ElPpValue* dst) {
    if (el_pp_value_type_is_trivial(src->type)) {
        (void)el_pp_value_copy(src, dst);
    }
    if (src->type == EL_PP_VAR_ARRAY) {
        el_pp_valarr_move(&src->as_arr, &src->as_arr);
    } else if (src->type == EL_PP_VAR_TOKENS) {
        el_tkbuf_move(&src->as_toks, &dst->as_toks);
    }
}

void el_pp_value_free(ElPpValue* val) {
    if (el_pp_value_type_is_trivial(val->type)) return;
    if (val->type == EL_PP_VAR_ARRAY) {
        el_pp_valarr_destroy(&val->as_arr);
    } else if (val->type == EL_PP_VAR_TOKENS) {
        el_tkbuf_destroy(&val->as_toks);
    }
}

void el_pp_var_free(ElPpVar* var) {
    el_pp_value_free(&var->value);
}
bool el_pp_var_copy(const ElPpVar* src, ElPpVar* dst) {
    return el_pp_value_copy(&src->value, &dst->value);
}
void el_pp_var_move(ElPpVar* src, ElPpVar* dst) {
    el_pp_value_move(&src->value, &dst->value);
}
