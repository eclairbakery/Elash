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
    case EL_PP_VAR_INT:    dst->asInt =   src->asInt;   return true;
    case EL_PP_VAR_FLOAT:  dst->asFloat = src->asFloat; return true;
    case EL_PP_VAR_BOOL:   dst->asBool =  src->asBool;  return true;
    case EL_PP_VAR_CHAR:   dst->asChar =  src->asChar;  return true;
    case EL_PP_VAR_STRING: dst->asStr =   src->asStr;   return true;
    case EL_PP_VAR_ARRAY:  return el_pp_valarr_copy(&src->asArr, &dst->asArr);
    case EL_PP_VAR_TOKENS: return el_token_buf_copy(&src->asToks, &dst->asToks);
    }
    return false;
}

void el_pp_value_move(ElPpValue* src, ElPpValue* dst) {
    if (el_pp_value_type_is_trivial(src->type)) {
        (void)el_pp_value_copy(src, dst);
    }
    if (src->type == EL_PP_VAR_ARRAY) {
        el_pp_valarr_move(&src->asArr, &src->asArr);
    } else if (src->type == EL_PP_VAR_TOKENS) {
        el_token_buf_move(&src->asToks, &dst->asToks);
    }
}

void el_pp_value_free(ElPpValue* val) {
    if (el_pp_value_type_is_trivial(val->type)) return;
    if (val->type == EL_PP_VAR_ARRAY) {
        el_pp_valarr_destroy(&val->asArr);
    } else if (val->type == EL_PP_VAR_TOKENS) {
        el_token_buf_destroy(&val->asToks);
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
