#pragma once

#include <el-util/dynarena.h>

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

ElAstStmtNode el_ast_expr_stmt(ElAstExprNode expr);
ElAstStmtNode* el_ast_new_expr_stmt(ElDynArena* arena, ElAstExprNode expr);
