#include <elash/hir/tree/toplevel.h>

#include <stddef.h>

ElHirTopLevelNode* el_hir_new_func_definition(ElDynArena* arena, ElHirSymbol* symbol, ElHirBlockStmtNode block) {
    ElHirTopLevelNode* node = EL_DYNARENA_NEW(arena, ElHirTopLevelNode);
    node->kind = EL_HIR_TOPLVL_FUNC_DEF;
    node->next = NULL;
    node->as.func_def = (ElHirFuncDefinition) {
        .symbol = symbol,
        .block = block,
    };
    return node;
}
