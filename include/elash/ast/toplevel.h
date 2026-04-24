#pragma once

#include "toplevel/func.h"

typedef enum ElAstTopLevelType {
    EL_AST_TOPLVL_FUNC_DEF,
} ElAstTopLevelType;

typedef struct ElAstTopLevelNode {
    ElAstTopLevelType type;
    union {
        ElAstFuncDefinition func_def;
    } as;
    ElAstTopLevelNode* next;
} ElAstTopLevelNode;

void el_ast_dump_toplevel(ElAstTopLevelNode* node, usize indent, FILE* out);
