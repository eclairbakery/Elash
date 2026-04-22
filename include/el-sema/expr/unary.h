#pragma once

#include <el-defs/sv.h>

typedef enum ElSemaUnaryExprType {
    EL_SEMA_UNARY_EXPR_POS, // +
    EL_SEMA_UNARY_EXPR_NEG, // -

    EL_SEMA_UNARY_EXPR_NOT,    // !
    EL_SEMA_UNARY_EXPR_BW_NOT, // ~

    EL_SEMA_UNARY_EXPR_PRE_INC,  // ++x
    EL_SEMA_UNARY_EXPR_PRE_DEC,  // --x
    EL_SEMA_UNARY_EXPR_POST_INC, // x++
    EL_SEMA_UNARY_EXPR_POST_DEC, // x--
} ElSemaUnaryExprType;

ElStringView el_ast_unary_op_to_string(ElSemaUnaryExprType type);
