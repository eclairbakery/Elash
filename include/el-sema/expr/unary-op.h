#pragma once

#include <el-defs/sv.h>

typedef enum ElSemaUnaryOp {
    EL_SEMA_UNARY_OP_POS, // +
    EL_SEMA_UNARY_OP_NEG, // -

    EL_SEMA_UNARY_OP_NOT,    // !
    EL_SEMA_UNARY_OP_BW_NOT, // ~

    EL_SEMA_UNARY_OP_PRE_INC,  // ++x
    EL_SEMA_UNARY_OP_PRE_DEC,  // --x
    EL_SEMA_UNARY_OP_POST_INC, // x++
    EL_SEMA_UNARY_OP_POST_DEC, // x--
} ElSemaUnaryOp;

ElStringView el_ast_unary_op_to_string(ElSemaUnaryOp type);
