#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

typedef struct ElHirStmt ElHirStmt;
typedef struct ElHirContinueStmt {
} ElHirContinueStmt;

ElHirStmt* el_hir_new_continue_stmt(ElDynArena* arena, ElSourceSpan span);
