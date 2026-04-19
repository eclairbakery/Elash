#pragma once

typedef struct ElAstNode ElAstNode;

typedef enum ElAstUnaryExprType {
    POS, // +
    NEG, // -

    NOT,    // !
    BW_NOT, // ~

    PRE_INC,  // ++x
    PRE_DEC,  // --x
    POST_INC, // x++
    POST_DEC, // x--
} ElAstUnaryExprType;

typedef struct ElAstUnaryExprNode {
    ElAstUnaryExprType type;
    ElAstNode* operand;
} ElAstUnaryExprNode;
