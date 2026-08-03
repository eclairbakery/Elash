#pragma once

#include <elash/util/dynarena.h>
#include <elash/srcdoc/span.h>

typedef struct ElAstInit ElAstInit;
typedef struct ElAstStmt ElAstStmt;

typedef struct ElAstReturnStmt {
    ElAstInit* value; // nullable
} ElAstReturnStmt;

ElAstStmt* el_ast_new_return_stmt(ElDynArena* arena, ElSourceSpan span, ElAstInit* value);
