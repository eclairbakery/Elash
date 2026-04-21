#pragma once

#include "bin.h"
#include "unary.h"

#include "literal.h"

typedef enum ElAstExprNodeType {
    EL_AST_EXPR_BINARY,
    EL_AST_EXPR_UNARY,
    EL_AST_EXPR_LITERAL,
} ElAstExprNodeType;

typedef struct ElAstExprNode {
    ElAstExprNodeType type;
    union {
        ElAstBinExprNode binary;
        ElAstUnaryExprNode unary;
        ElAstLiteralNode literal;
    } as;
} ElAstExprNode;

void el_ast_dump(ElAstExprNode* root, FILE* out);
