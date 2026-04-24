#include <elash/hir/tree/stmt/block.h>
#include <elash/hir/tree/stmt.h>

ElHirStmtNode* el_hir_new_block_stmt(ElDynArena* arena, ElHirStmtNode* stmts) {
    ElHirStmtNode* node = EL_DYNARENA_NEW(arena, ElHirStmtNode);
    node->kind = EL_HIR_STMT_BLOCK;
    node->next = NULL;
    node->as.block = (ElHirBlockStmtNode) {
        .stmts = stmts,
    };
    return node;
}
