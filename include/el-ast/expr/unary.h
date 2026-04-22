#pragma once

#include <el-util/dynarena.h>
#include <el-defs/sv.h>

#include <el-sema/expr/unary.h>

typedef struct ElAstExprNode ElAstExprNode;

typedef ElSemaUnaryExprType ElAstUnaryExprType;

typedef struct ElAstUnaryExprNode {
    ElAstUnaryExprType type;
    ElAstExprNode* operand;
} ElAstUnaryExprNode;

ElAstUnaryExprNode el_ast_unary_expr(ElAstUnaryExprType type, ElAstExprNode* operand);
ElAstExprNode* el_ast_new_unary_expr(ElDynArena* arena, ElAstUnaryExprType type, ElAstExprNode* operand);

