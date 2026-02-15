#pragma once

#include <el-pp/value.h>

struct _ElPpExpr;
typedef struct _ElPpExpr _ElPpExpr;

typedef struct _ElPpValueExpr {
    ElPpValue value;
} _ElPpValueExpr;

typedef enum _ElPpBinaryOp {
    _EL_PP_BINARY_OP_ADD,
    _EL_PP_BINARY_OP_SUB,
    _EL_PP_BINARY_OP_MUL,
    _EL_PP_BINARY_OP_DIV,
} _ElPpBinaryOp;
typedef struct _ElPpBinaryExpr {
    _ElPpExpr* lhs;
    _ElPpBinaryOp op;
    _ElPpExpr* rhs;
} _ElPpBinaryExpr;

typedef enum _ElPpUnaryOp {
    _EL_PP_UNARY_OP_NEG,
    _EL_PP_UNARY_OP_PLUS,

    _EL_PP_UNARY_NOT,
    _EL_PP_UNARY_BITWISE_NOT,
} _ElPpUnaryOp;
typedef struct _ElPpUnaryExpr {
    _ElPpUnaryOp op;
    _ElPpExpr* arg;
} _ElPpUnaryExpr;

typedef enum _ElPpExprType {
    _EL_PP_EXPR_VALUE,
    _EL_PP_EXPR_BINARY,
    _EL_PP_EXPR_UNARY,
} _ElPpExprType;
typedef union _ElPpExprData {
    _ElPpValueExpr value;
    _ElPpBinaryExpr binary;
    _ElPpUnaryExpr unary;
} _ElPpExprData;

struct _ElPpExpr {
    _ElPpExprType type;
    _ElPpExprData data; 
};
