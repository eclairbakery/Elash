#pragma once

#include <el-sema/type.h>

#include <el-sema/expr/bin-op.h>
#include <el-sema/expr/unary-op.h>

#include <el-hir/symbol.h>

typedef struct ElHirExprNode ElHirExprNode;

typedef enum ElHirExprKind {
    EL_HIR_EXPR_BINARY,
    EL_HIR_EXPR_UNARY,
    EL_HIR_EXPR_LITERAL,
    EL_HIR_EXPR_SYMBOL, // resolved identifier
} ElHirExprKind;

typedef struct ElHirBinExprNode {
    ElHirExprNode* left;
    ElSemaBinOp op;
    ElHirExprNode* right;
} ElHirBinExprNode;

typedef struct ElHirUnaryExprNode {
    ElSemaUnaryOp op;
    ElHirExprNode* operand;
} ElHirUnaryExprNode;

typedef struct ElHirLiteral {
    // tagged by the type field of ExprNode
    union {
        int64_t int_;
        uint64_t uint_;
        char char_;
    } as;
} ElHirLiteral;

struct ElHirExprNode {
    ElHirExprKind kind;
    ElType* type;
    union {
        ElHirBinExprNode binary;
        ElHirUnaryExprNode unary;
        ElHirLiteral literal;
        ElHirSymbol* symbol;
    } as;
};

