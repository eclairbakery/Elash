#pragma once

#include <elash/util/dynarena.h>
#include <elash/srcdoc/span.h>

#include <elash/ast/tree/toe.h>

typedef struct ElAstDecl ElAstDecl;
typedef struct ElAstTypedef {
    ElStringView name;
    ElAstType* target;
} ElAstTypedef;

ElAstDecl* el_ast_new_typedef(ElDynArena* arena, ElSourceSpan span, ElStringView name, ElAstType* target);
