#pragma once

#include <elash/defs/inttypes.h>
#include <stdio.h>

typedef struct ElHirStmtNode ElHirStmtNode;

void el_hir_dump_stmt(ElHirStmtNode* node, usize indent, FILE* out);
