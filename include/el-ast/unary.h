#pragma once

#include <el-util/dynarena.h>
#include <el-defs/sv.h>

typedef struct ElAstNode ElAstNode;

typedef enum ElAstUnaryExprType {
    EL_AST_UNARY_EXPR_POS, // +
    EL_AST_UNARY_EXPR_NEG, // -

    EL_AST_UNARY_EXPR_NOT,    // !
    EL_AST_UNARY_EXPR_BW_NOT, // ~

    EL_AST_UNARY_EXPR_PRE_INC,  // ++x
    EL_AST_UNARY_EXPR_PRE_DEC,  // --x
    EL_AST_UNARY_EXPR_POST_INC, // x++
    EL_AST_UNARY_EXPR_POST_DEC, // x--
} ElAstUnaryExprType;

typedef struct ElAstUnaryExprNode {
    ElAstUnaryExprType type;
    ElAstNode* operand;
} ElAstUnaryExprNode;

ElAstNode* el_ast_new_unary_expr(ElDynArena* arena, ElAstUnaryExprType type, ElAstNode* operand);

ElStringView el_ast_unary_op_to_string(ElAstUnaryExprType type);
