#pragma once

typedef struct ElAstNode ElAstNode;

typedef enum ElAstBinExprType {
    ADD, // +
    SUB, // -
    MUL, // *
    DIV, // /
    MOD, // %

    EQ,  // ==
    NEQ, // !=
    LT,  // <
    LTE, // <=
    GT,  // >
    GTE, // >=
    
    AND, // &&
    OR,  // ||

    BW_AND, // &
    BW_OR,  // |
    BW_XOR, // ^
    SHL,    // <<
    SHR,    // >>
} ElAstBinExprType;

typedef struct ElAstBinExprNode {
    ElAstNode* left;
    ElAstBinExprType type;
    ElAstNode* right;
} ElAstBinExprNode;
