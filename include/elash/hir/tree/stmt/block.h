#pragma once

#include <elash/util/dynarena.h>

typedef struct ElHirStmtNode ElHirStmtNode;

typedef struct ElHirBlockStmtNode {
    ElHirStmtNode* stmts;
} ElHirBlockStmtNode;

ElHirStmtNode* el_hir_new_block_stmt(ElDynArena* arena, ElHirStmtNode* stmts);

