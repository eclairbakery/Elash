#pragma once


typedef enum ElAstNodeType {
    EL_AST_NODE_BINARY_EXPR,
    EL_AST_NODE_UNARY_EXPR,
} ElAstNodeType;

typedef struct ElAstNode {
    ElAstNodeType type;
} ElAstNode;
