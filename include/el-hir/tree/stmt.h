#pragma once

#include <el-sema/type.h>
#include <el-hir/symbol.h>
#include <el-util/dynarena.h>

#include "stmt/block.h"
#include "stmt/return.h"

typedef enum ElHirStmtKind {
    EL_HIR_STMT_EXPR,
    EL_HIR_STMT_RETURN,
    EL_HIR_STMT_BLOCK,
} ElHirStmtKind;

typedef struct ElHirStmtNode {
    ElHirStmtKind kind;
    union {
        ElHirExprNode* expr;
        ElHirBlockStmtNode block;
        ElHirReturnStmtNode return_;
    } as;
    ElHirStmtNode* next;
} ElHirStmtNode;

ElHirStmtNode* el_hir_new_expr_stmt(ElDynArena* arena, ElHirExprNode* expr);
void el_hir_stmt_list_append(ElHirStmtNode** head, ElHirStmtNode** tail, ElHirStmtNode* stmt);
