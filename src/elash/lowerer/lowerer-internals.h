#pragma once
#include <elash/lowerer/lowerer.h>  // IWYU pragma: export

enum {
    EL_MIR_SLICE_FIELD_DATA = 0,
    EL_MIR_SLICE_FIELD_LEN  = 1,
};

ElMirValue*    _el_lowerer_extract_tuple_field(ElLowerer* lw, ElMirValue* tuple, usize index);
ElMirValue*    _el_lowerer_get_tuple_field_ptr(ElLowerer* lw, ElMirValue* tuple_ptr, usize index);
ElMirValue*    _el_lowerer_make_tuple(ElLowerer* lw, ElMirType* tuple_type, ElMirValue** fields);
ElMirConstant* _el_lowerer_lower_const(ElLowerer* lw, ElHirExpr* expr);
ElMirValue*    _el_lowerer_new_anon_global(ElLowerer* lw, ElMirType* type, ElMirConstant* init);
ElMirValue*    _el_lowerer_create_alloca(ElLowerer* lw, ElMirType* type);
void           _el_lowerer_copy_str_to_ptr(ElLowerer* lw, ElMirValue* ptr, ElMirStrConst str);
void           _el_lowerer_lower_agginit(ElLowerer* lw, ElMirValue* ptr, ElHirAggInit* agginit);
