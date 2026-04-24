#pragma once

#include <elash/defs/sv.h>
#include <elash/util/dynarena.h>

typedef struct ElAstExprNode ElAstExprNode;

typedef struct ElAstIdentNode {
    ElStringView name;
} ElAstIdentNode;

ElAstIdentNode el_ast_ident_node(ElStringView name);
ElAstIdentNode* el_ast_new_ident_node_raw(ElDynArena* arena, ElStringView name);
ElAstExprNode* el_ast_new_ident_node(ElDynArena* arena, ElStringView name);
