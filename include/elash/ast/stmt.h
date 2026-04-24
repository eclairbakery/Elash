#pragma once

#include <elash/util/dynarena.h>

#include "expr.h"

#include "stmt/return.h"
#include "stmt/block.h"

typedef enum ElAstStmtType {
    EL_AST_STMT_EXPR,
    EL_AST_STMT_RETURN,
    EL_AST_STMT_BLOCK,
} ElAstStmtType;

typedef struct ElAstStmtNode {
    ElAstStmtType type;
    union {
        ElAstExprNode* expr;

        ElAstBlockStmtNode block;
        ElAstReturnStmtNode return_;
    } as;
    ElAstStmtNode* next; // linked list; used in block stmt
} ElAstStmtNode;

ElAstStmtNode el_ast_expr_stmt(ElAstExprNode* expr);
ElAstStmtNode* el_ast_new_expr_stmt(ElDynArena* arena, ElAstExprNode* expr);

void el_ast_stmt_list_append(ElAstStmtNode** head, ElAstStmtNode** tail, ElAstStmtNode* stmt);

void el_ast_dump_stmt(ElAstStmtNode* root, usize indent, FILE* out);
