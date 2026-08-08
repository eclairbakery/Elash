#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

#include <elash/ast/tree/common/ident.h>
#include <elash/ast/tree/type.h>
#include <elash/ast/tree/init.h>

typedef struct ElAstDecl ElAstDecl;

typedef struct ElAstVarDef {
    ElAstType*  type;
    ElAstIdent* name;
    ElAstInit*  init; // nullable
    bool        is_global;
} ElAstVarDef;

ElAstDecl* el_ast_new_var_def(ElDynArena* arena, ElSourceSpan span, ElAstType* type, ElAstIdent* name, ElAstInit* init, bool is_global);
