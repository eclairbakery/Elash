#include <el-hir/tree/stmt/return.h>
#include <el-hir/tree/stmt.h>

ElHirStmtNode* el_hir_new_return_stmt(ElDynArena* arena, ElHirExprNode* value) {
    ElHirStmtNode* node = EL_DYNARENA_NEW(arena, ElHirStmtNode);
    node->kind = EL_HIR_STMT_RETURN;
    node->next = NULL;
    node->as.return_ = (ElHirReturnStmtNode) {
        .value = value,
    };
    return node;
}
