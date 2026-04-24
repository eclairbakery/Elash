#pragma once

#include "toplevel.h"

typedef struct ElAstModuleNode {
    ElAstTopLevelNode* head;
    ElAstTopLevelNode* tail;
    usize count;
} ElAstModuleNode;

ElAstModuleNode el_ast_module();
ElAstModuleNode* el_ast_new_module(ElDynArena* arena);

void el_ast_module_append(ElAstModuleNode* module, ElAstTopLevelNode* node);

void el_ast_dump_module(ElAstModuleNode* node, usize indent, FILE* out);

