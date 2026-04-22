#pragma once

#include <el-util/dynarena.h>
#include <el-defs/sv.h>

#include <el-sema/expr/unary-op.h>

typedef struct ElAstExprNode ElAstExprNode;

typedef ElSemaUnaryOp ElAstUnaryOp;

typedef struct ElAstUnaryExprNode {
    ElAstUnaryOp type;
    ElAstExprNode* operand;
} ElAstUnaryExprNode;

ElAstUnaryExprNode el_ast_unary_expr(ElAstUnaryOp type, ElAstExprNode* operand);
ElAstExprNode* el_ast_new_unary_expr(ElDynArena* arena, ElAstUnaryOp type, ElAstExprNode* operand);

