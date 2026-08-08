#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

typedef struct ElAstStmt ElAstStmt;
typedef struct ElAstBreakStmt {
} ElAstBreakStmt;

ElAstStmt* el_ast_new_break_stmt(ElDynArena* arena, ElSourceSpan span);
