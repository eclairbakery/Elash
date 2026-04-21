#pragma once

#include <el-ast/common/ident.h>
#include <el-ast/stmt/block.h>
#include <el-util/dynarena.h>

#include <el-defs/inttypes.h>

typedef struct ElAstTopLevelNode ElAstTopLevelNode;

typedef struct ElAstFuncParam ElAstFuncParam;
struct ElAstFuncParam {
    ElAstIdentNode* type;
    ElAstIdentNode* name;

    ElAstFuncParam* next;
    ElAstFuncParam* prev;
};

typedef struct ElAstFuncParamList {
    ElAstFuncParam* head;
    ElAstFuncParam* tail;
    usize count;
} ElAstFuncParamList;

typedef struct ElAstFuncDefinition {
    ElAstIdentNode* ret_type;
    ElAstIdentNode* name;
    ElAstFuncParamList params;
    ElAstBlockStmtNode* block;
} ElAstFuncDefinition;

ElAstFuncParam el_ast_func_param(ElAstIdentNode* type, ElAstIdentNode* name);
ElAstFuncParam* el_ast_new_func_param(ElDynArena* arena, ElAstIdentNode* type, ElAstIdentNode* name);

ElAstFuncParamList el_ast_make_func_param_list();
void el_ast_func_param_list_append(ElAstFuncParamList* list, ElAstFuncParam* param);

ElAstFuncDefinition el_ast_func_definition(
    ElAstIdentNode* ret_type,
    ElAstIdentNode* name,
    ElAstFuncParamList params,
    ElAstBlockStmtNode* block
);

ElAstTopLevelNode* el_ast_new_func_definition(
    ElDynArena* arena,
    ElAstIdentNode* ret_type,
    ElAstIdentNode* name,
    ElAstFuncParamList params,
    ElAstBlockStmtNode* block
);
