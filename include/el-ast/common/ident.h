#pragma once

#include <el-defs/sv.h>
#include <el-util/dynarena.h>

typedef struct ElAstExprNode ElAstExprNode;

typedef struct ElAstIdentNode {
    ElStringView name;
} ElAstIdentNode;

ElAstIdentNode el_ast_ident_node(ElStringView name);
ElAstExprNode* el_ast_new_ident_node(ElDynArena* arena, ElStringView name);
