#pragma once

#include <el-defs/sv.h>

typedef enum ElSemaBinExprType {
    EL_SEMA_BIN_EXPR_ADD, // +
    EL_SEMA_BIN_EXPR_SUB, // -
    EL_SEMA_BIN_EXPR_MUL, // *
    EL_SEMA_BIN_EXPR_DIV, // /
    EL_SEMA_BIN_EXPR_MOD, // %

    EL_SEMA_BIN_EXPR_EQ,  // ==
    EL_SEMA_BIN_EXPR_NEQ, // !=
    EL_SEMA_BIN_EXPR_LT,  // <
    EL_SEMA_BIN_EXPR_LTE, // <=
    EL_SEMA_BIN_EXPR_GT,  // >
    EL_SEMA_BIN_EXPR_GTE, // >=
    
    EL_SEMA_BIN_EXPR_AND, // &&
    EL_SEMA_BIN_EXPR_OR,  // ||

    EL_SEMA_BIN_EXPR_BW_AND, // &
    EL_SEMA_BIN_EXPR_BW_OR,  // |
    EL_SEMA_BIN_EXPR_BW_XOR, // ^
    EL_SEMA_BIN_EXPR_SHL,    // <<
    EL_SEMA_BIN_EXPR_SHR,    // >>
} ElSemaBinExprType;

ElStringView el_sema_bin_op_to_string(ElSemaBinExprType type);
