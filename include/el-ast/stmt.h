#pragma once

#include "expr.h"

typedef enum ElAstStmtType {
    EL_AST_STMT_EXPR,
} ElAstStmtType;

typedef struct ElAstStmtNode {
    ElAstStmtType type;
    union {
        ElAstExprNode expr;
    } as;
} ElAstStmtNode;
