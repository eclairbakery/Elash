#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

typedef struct ElHirStmt ElHirStmt;
typedef struct ElHirBreakStmt {
} ElHirBreakStmt;

ElHirStmt* el_hir_new_break_stmt(ElDynArena* arena, ElSourceSpan span);
