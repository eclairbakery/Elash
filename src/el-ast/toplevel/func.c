#include <el-ast/toplevel/func.h>
#include <el-ast/toplevel.h>

#include <stddef.h>

ElAstFuncParam el_ast_func_param(ElAstIdentNode* type, ElAstIdentNode* name) {
    return (ElAstFuncParam) {
        .type = type,
        .name = name,
        .next = NULL,
        .prev = NULL,
    };
}

ElAstFuncParam* el_ast_new_func_param(ElDynArena* arena, ElAstIdentNode* type, ElAstIdentNode* name) {
    ElAstFuncParam* param = EL_DYNARENA_NEW(arena, ElAstFuncParam);
    *param = el_ast_func_param(type, name);
    return param;
}

ElAstFuncParamList el_ast_make_func_param_list() {
    return (ElAstFuncParamList) {
        .head = NULL,
        .tail = NULL,
        .count = 0,
    };
}

void el_ast_func_param_list_append(ElAstFuncParamList* list, ElAstFuncParam* param) {
    if (list->head == NULL) {
        list->head = param;
        list->tail = param;
    } else {
        list->tail->next = param;
        param->prev = list->tail;
        list->tail = param;
    }
    list->count++;
}

// this sucks, but it exists only for consistency with other nodes
ElAstFuncDefinition el_ast_func_definition(
    ElAstIdentNode* ret_type,
    ElAstIdentNode* name,
    ElAstFuncParamList params,
    ElAstBlockStmtNode* block
) {
    return (ElAstFuncDefinition) {
        .ret_type = ret_type,
        .name = name,
        .params = params,
        .block = block,
    };
}

ElAstTopLevelNode* el_ast_new_func_definition(
    ElDynArena* arena,
    ElAstIdentNode* ret_type,
    ElAstIdentNode* name,
    ElAstFuncParamList params,
    ElAstBlockStmtNode* block
) {
    ElAstTopLevelNode* node = EL_DYNARENA_NEW(arena, ElAstTopLevelNode);
    node->type = EL_AST_TOPLEVEL_FUNC;
    node->as.func = el_ast_func_definition(ret_type, name, params, block);
    node->next = NULL;
    return node;
}
