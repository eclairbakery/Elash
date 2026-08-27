#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

#include <elash/ast/tree/common/declarator.h>
#include <elash/ast/tree/common/ident.h>
#include <elash/ast/tree/type.h>

typedef struct ElAstDecl ElAstDecl;

typedef struct ElAstVarDecl {
    ElAstDeclarator* declarators;
} ElAstVarDecl;

ElAstDecl* el_ast_new_var_decl(ElDynArena* arena, ElSourceSpan span, ElAstDeclarator* declarators);
