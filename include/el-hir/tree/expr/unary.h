#pragma once

#include <el-sema/expr/unary-op.h>
#include <el-util/dynarena.h>

typedef struct ElHirExprNode ElHirExprNode;
typedef struct ElType ElType;

typedef struct ElHirUnaryExprNode {
    ElSemaUnaryOp op;
    ElHirExprNode* operand;
} ElHirUnaryExprNode;

ElHirExprNode* el_hir_new_unary_expr(ElDynArena* arena, ElType* type, ElSemaUnaryOp op, ElHirExprNode* operand);

