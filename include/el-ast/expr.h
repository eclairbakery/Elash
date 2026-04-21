#pragma once

#include "expr/bin.h"
#include "expr/unary.h"

#include "expr/literal.h"

typedef enum ElAstExprType {
    EL_AST_EXPR_BINARY,
    EL_AST_EXPR_UNARY,
    EL_AST_EXPR_LITERAL,
} ElAstExprType;

typedef struct ElAstExprNode {
    ElAstExprType type;
    union {
        ElAstBinExprNode binary;
        ElAstUnaryExprNode unary;
        ElAstLiteralNode literal;
    } as;
} ElAstExprNode;

void el_ast_dump_expr(ElAstExprNode* root, FILE* out);
