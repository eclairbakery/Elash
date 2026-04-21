#pragma once

#include <el-util/dynarena.h>

#include "expr.h"

#include "stmt/return.h"

typedef enum ElAstStmtType {
    EL_AST_STMT_EXPR,
    EL_AST_STMT_RETURN,
} ElAstStmtType;

typedef struct ElAstStmtNode {
    ElAstStmtType type;
    union {
        ElAstExprNode* expr;
        ElAstReturnStmtNode return_;
    } as;
} ElAstStmtNode;

ElAstStmtNode el_ast_expr_stmt(ElAstExprNode* expr);
ElAstStmtNode* el_ast_new_expr_stmt(ElDynArena* arena, ElAstExprNode* expr);

void el_ast_dump_stmt(ElAstStmtNode* root, FILE* out);
