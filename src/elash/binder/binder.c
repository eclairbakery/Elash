#include <elash/binder/binder.h>

void el_binder_init(ElBinder* binder, ElDynArena* arena, ElDiagEngine* diag) {
    binder->arena = arena;
    binder->diag = diag;

    binder->builtin_scope = el_hir_scope_new(NULL);
    binder->global_scope = el_hir_scope_new(binder->builtin_scope);
    binder->current_scope = binder->global_scope;
}

void el_binder_free(ElBinder* binder) {
    el_hir_scope_free(binder->global_scope);
    el_hir_scope_free(binder->builtin_scope);
}

ElHirScope* _el_binder_push_scope(ElBinder* binder) {
    ElHirScope* scope = el_hir_scope_new(binder->current_scope);
    binder->current_scope = scope;
    return scope;
}

ElHirScope* _el_binder_pop_scope(ElBinder* binder) {
    ElHirScope* parent = binder->current_scope->parent;
    el_hir_scope_free(binder->current_scope);
    return binder->current_scope = parent;
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
