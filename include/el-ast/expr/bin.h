#pragma once

#include <el-util/dynarena.h>
#include <el-defs/sv.h>

typedef struct ElAstExprNode ElAstExprNode;

typedef enum ElAstBinExprType {
    EL_AST_BIN_EXPR_ADD, // +
    EL_AST_BIN_EXPR_SUB, // -
    EL_AST_BIN_EXPR_MUL, // *
    EL_AST_BIN_EXPR_DIV, // /
    EL_AST_BIN_EXPR_MOD, // %

    EL_AST_BIN_EXPR_EQ,  // ==
    EL_AST_BIN_EXPR_NEQ, // !=
    EL_AST_BIN_EXPR_LT,  // <
    EL_AST_BIN_EXPR_LTE, // <=
    EL_AST_BIN_EXPR_GT,  // >
    EL_AST_BIN_EXPR_GTE, // >=
    
    EL_AST_BIN_EXPR_AND, // &&
    EL_AST_BIN_EXPR_OR,  // ||

    EL_AST_BIN_EXPR_BW_AND, // &
    EL_AST_BIN_EXPR_BW_OR,  // |
    EL_AST_BIN_EXPR_BW_XOR, // ^
    EL_AST_BIN_EXPR_SHL,    // <<
    EL_AST_BIN_EXPR_SHR,    // >>
} ElAstBinExprType;

typedef struct ElAstBinExprNode {
    ElAstExprNode* left;
    ElAstBinExprType type;
    ElAstExprNode* right;
} ElAstBinExprNode;

ElAstBinExprNode el_ast_bin_expr(ElAstBinExprType type, ElAstExprNode* left, ElAstExprNode* right);
ElAstExprNode* el_ast_new_bin_expr(ElDynArena* arena, ElAstBinExprType type, ElAstExprNode* left, ElAstExprNode* right);

ElStringView el_ast_bin_op_to_string(ElAstBinExprType type);
