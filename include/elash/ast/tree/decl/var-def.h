#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

#include <elash/ast/tree/common/declarator.h>
#include <elash/ast/tree/common/ident.h>
#include <elash/ast/tree/type.h>
#include <elash/ast/tree/init.h>

typedef struct ElAstDecl ElAstDecl;

typedef struct ElAstVarDef {
    bool is_static;
    ElAstType* type;
    ElAstDeclarator* declarators;
} ElAstVarDef;

ElAstDecl* el_ast_new_var_def(
    ElDynArena* arena, ElSourceSpan span, ElAstType* type, ElAstDeclarator* declarators, bool is_static
);
