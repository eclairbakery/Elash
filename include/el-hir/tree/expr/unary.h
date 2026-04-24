#pragma once

#include <el-sema/expr/unary-op.h>

typedef struct ElHirExprNode ElHirExprNode;

typedef struct ElHirUnaryExprNode {
    ElSemaUnaryOp op;
    ElHirExprNode* operand;
} ElHirUnaryExprNode;

