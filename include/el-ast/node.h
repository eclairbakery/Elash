#pragma once

#include "bin.h"
#include "unary.h"

typedef enum ElAstNodeType {
    EL_AST_NODE_BINARY_EXPR,
    EL_AST_NODE_UNARY_EXPR,
} ElAstNodeType;

typedef struct ElAstNode {
    ElAstNodeType type;
    union {
        ElAstBinExprNode binary;
        ElAstUnaryExprNode unary;
    } as;
} ElAstNode;
