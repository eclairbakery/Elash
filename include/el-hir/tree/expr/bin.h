#pragma once

#include <el-sema/expr/bin-op.h>

typedef struct ElHirExprNode ElHirExprNode;

typedef struct ElHirBinExprNode {
    ElHirExprNode* left;
    ElSemaBinOp op;
    ElHirExprNode* right;
} ElHirBinExprNode;

