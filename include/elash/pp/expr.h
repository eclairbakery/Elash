#pragma once

#include <elash/pp/value.h>

#include <elash/sema/bin-op.h>
#include <elash/sema/unary-op.h>

typedef struct ElPpExpr ElPpExpr;

typedef struct ElPpBinExpr {
    ElPpExpr*  lhs;
    ElSemaBinOp op;
    ElPpExpr*  rhs;
} ElPpBinaryExpr;

typedef struct ElPpUnaryExpr {
    ElSemaUnaryOp op;
    ElPpExpr* operand;
} ElPpUnaryExpr;

typedef ElPpValue ElPpValueExpr;

typedef enum ElPpExprType {
    EL_PP_EXPR_VALUE,
    EL_PP_EXPR_BINARY,
    EL_PP_EXPR_UNARY,
} ElPpExprType;

typedef struct ElPpExpr {
    ElPpExprType type;
    union {
        ElPpValueExpr  value;
        ElPpUnaryExpr  unary;
        ElPpBinaryExpr bin;
    } as;
} ElPpExpr;
