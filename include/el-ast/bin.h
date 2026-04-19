#pragma once

#include <el-util/dynarena.h>

typedef struct ElAstNode ElAstNode;

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
    ElAstNode* left;
    ElAstBinExprType type;
    ElAstNode* right;
} ElAstBinExprNode;

ElAstNode* el_ast_new_bin_expr(ElDynArena* arena, ElAstBinExprType type, ElAstNode* left, ElAstNode* right);
