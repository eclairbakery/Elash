#pragma once

#include <el-sema/type.h>
#include <el-hir/symbol.h>

#include "expr/bin.h"
#include "expr/unary.h"
#include "expr/literal.h"

typedef enum ElHirExprKind {
    EL_HIR_EXPR_BINARY,
    EL_HIR_EXPR_UNARY,
    EL_HIR_EXPR_LITERAL,
    EL_HIR_EXPR_SYMBOL, // resolved identifier
} ElHirExprKind;

typedef struct ElHirExprNode {
    ElHirExprKind kind;
    ElType* type;
    union {
        ElHirBinExprNode binary;
        ElHirUnaryExprNode unary;
        ElHirLiteral literal;
        ElHirSymbol* symbol;
    } as;
} ElHirExprNode;

