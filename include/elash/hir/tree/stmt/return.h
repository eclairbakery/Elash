#pragma once

#include <elash/util/dynarena.h>

typedef struct ElHirStmtNode ElHirStmtNode;
typedef struct ElHirExprNode ElHirExprNode;

typedef struct ElHirReturnStmtNode {
    ElHirExprNode* value;
} ElHirReturnStmtNode;

ElHirStmtNode* el_hir_new_return_stmt(ElDynArena* arena, ElHirExprNode* value);
