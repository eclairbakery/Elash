#pragma once

#include <el-util/dynarena.h>
#include <el-defs/sv.h>

#include <el-sema/expr/bin-op.h>

typedef struct ElAstExprNode ElAstExprNode;

typedef ElSemaBinOp ElAstBinOp;

typedef struct ElAstBinExprNode {
    ElAstExprNode* left;
    ElAstBinOp op;
    ElAstExprNode* right;
} ElAstBinExprNode;

ElAstBinExprNode el_ast_bin_expr(ElAstBinOp type, ElAstExprNode* left, ElAstExprNode* right);
ElAstExprNode* el_ast_new_bin_expr(ElDynArena* arena, ElAstBinOp type, ElAstExprNode* left, ElAstExprNode* right);
