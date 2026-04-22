#pragma once

#include <el-sema/type.h>

// TODO: this should be moved from ast to sema module, probably
#include <el-ast/expr/literal.h>

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

struct ElHirExprNode {
    ElHirExprKind kind;
    ElType* type;
    union {
        ElHirBinExprNode binary;
        ElHirUnaryExprNode unary;
        ElAstLiteralNode literal;
        ElHirSymbol* symbol;
    } as;
};

