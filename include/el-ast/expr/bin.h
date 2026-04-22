#pragma once

#include <el-util/dynarena.h>
#include <el-defs/sv.h>

#include <el-sema/expr/bin.h>

typedef struct ElAstExprNode ElAstExprNode;

typedef ElSemaBinExprType ElAstBinExprType;

typedef struct ElAstBinExprNode {
    ElAstExprNode* left;
    ElAstBinExprType type;
    ElAstExprNode* right;
} ElAstBinExprNode;

ElAstBinExprNode el_ast_bin_expr(ElAstBinExprType type, ElAstExprNode* left, ElAstExprNode* right);
ElAstExprNode* el_ast_new_bin_expr(ElDynArena* arena, ElAstBinExprType type, ElAstExprNode* left, ElAstExprNode* right);
