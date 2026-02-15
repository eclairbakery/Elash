#include <el-pp/var.h>

bool el_pp_var_type_is_trivial(ElPpVarType type)  {
    return type == EL_PP_VAR_INT
        || type == EL_PP_VAR_FLOAT 
        || type == EL_PP_VAR_BOOL
        || type == EL_PP_VAR_CHAR 
        || type == EL_PP_VAR_STRING; // its just a string-view so its trivially copyable
}

bool el_pp_var_copy(const ElPpVar* src, ElPpVar* dst) {
    dst->name = src->name;
    dst->type = src->type;
    switch (src->type) {
    case EL_PP_VAR_INT:    dst->value.i = src->value.i; return true;
    case EL_PP_VAR_FLOAT:  dst->value.f = src->value.f; return true;
    case EL_PP_VAR_BOOL:   dst->value.b = src->value.b; return true;
    case EL_PP_VAR_CHAR:   dst->value.c = src->value.c; return true;
    case EL_PP_VAR_STRING: dst->value.s = src->value.s; return true;
    case EL_PP_VAR_ARRAY:  return el_pp_vararr_copy(&src->value.a, &dst->value.a);
    }
    return false;
}

void el_pp_var_move(ElPpVar* src, ElPpVar* dst) {
    if (el_pp_var_type_is_trivial(src->type)) {
        (void)el_pp_var_copy(src, dst);
    }
    if (src->type == EL_PP_VAR_ARRAY) {
        el_pp_vararr_move(&src->value.a, &src->value.a);
    }
}

void el_pp_var_free(ElPpVar* var) {
    if (el_pp_var_type_is_trivial(var->type)) return;
    if (var->type == EL_PP_VAR_ARRAY) {
        el_pp_vararr_destroy(&var->value.a);
    }
}
