#include <elash/pp/value.h>

void el_pp_value_free(ElPpValue* val) {
    if (val->type == EL_PP_VAR_ARRAY) {
        el_pp_valarr_free(&val->as.arr_);
    } else if (val->type == EL_PP_VAR_STRING) {
        el_strbuf_destroy(&val->as.str_);
    }
}
