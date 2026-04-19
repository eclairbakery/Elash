#pragma once

#include "bin.h"
#include "unary.h"

#include "literal.h"

typedef enum ElAstNodeType {
    EL_AST_NODE_BINARY_EXPR,
    EL_AST_NODE_UNARY_EXPR,
    EL_AST_NODE_LITERAL,
} ElAstNodeType;

typedef struct ElAstNode {
    ElAstNodeType type;
    union {
        ElAstBinExprNode binary;
        ElAstUnaryExprNode unary;
        ElAstLiteralNode literal;
    } as;
} ElAstNode;
