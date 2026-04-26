#include <elash/binder/binder.h>

void el_binder_init(ElBinder* binder, ElDynArena* arena, ElDiagEngine* diag) {
    binder->arena = arena;
    binder->diag = diag;
    // TODO: initialize current scope and global scope
}

void el_binder_free(ElBinder* binder) {
    // no-op for now
    (void) binder;
}

ElHirExprNode* el_binder_bind_expr(ElBinder* binder,   ElAstExprNode* in) {
    (void) binder, (void) in;
    return NULL; // TODO: stub
}

ElHirStmtNode* el_binder_bind_stmt(ElBinder* binder,   ElAstStmtNode* in) {
    (void) binder, (void) in;
    return NULL; // TODO: stub
}

ElHirTopLevelNode* el_binder_bind_toplvl(ElBinder* binder, ElAstTopLevelNode* in) {
    (void) binder, (void) in;
    return NULL; // TODO: stub
}

ElHirModule* el_binder_bind_module(ElBinder* binder, ElAstModuleNode* in) {
    (void) binder, (void) in;
    return el_hir_new_module(binder->arena);
}
