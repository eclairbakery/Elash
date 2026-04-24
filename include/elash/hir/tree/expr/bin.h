#pragma once

#include <elash/sema/expr/bin-op.h>
#include <elash/util/dynarena.h>

typedef struct ElHirExprNode ElHirExprNode;
typedef struct ElType ElType;

typedef struct ElHirBinExprNode {
    ElHirExprNode* left;
    ElSemaBinOp op;
    ElHirExprNode* right;
} ElHirBinExprNode;

ElHirExprNode* el_hir_new_bin_expr(ElDynArena* arena, ElType* type, ElSemaBinOp op, ElHirExprNode* left, ElHirExprNode* right);

