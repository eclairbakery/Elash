#include <elash/binder/binder.h>
#include <elash/util/assert.h>

#include <elash/diag/engine.h>
#include <elash/diag/meta.h>

#include <elash/sema/type/prim.h>

static void _el_binder_register_builtin_type(ElBinder* binder, ElStringView name, ElPrimitiveTypeKind kind) {
    ElType* type = el_sema_new_prim_type(binder->arena, kind);
    ElHirSymbol* sym = el_hir_new_type_symbol(binder->arena, name, type);
    el_hir_scope_insert(binder->builtin_scope, sym);
}

void el_binder_init(ElBinder* binder, ElDynArena* arena, ElDiagEngine* diag) {
    binder->arena = arena;
    binder->diag = diag;

    binder->builtin_scope = el_hir_scope_new(NULL);

    _el_binder_register_builtin_type(binder, EL_SV("int"),  EL_PRIMTYPE_INT);
    _el_binder_register_builtin_type(binder, EL_SV("uint"), EL_PRIMTYPE_UINT);
    _el_binder_register_builtin_type(binder, EL_SV("char"), EL_PRIMTYPE_CHAR);

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

ElType* _el_binder_bind_type(ElBinder* binder, ElAstTypeNode* node) {
    switch (node->kind) {
    case EL_AST_TYPE_PTR: {
        ElType* base = _el_binder_bind_type(binder ,node->base);
        if (base == NULL) return NULL;

        return el_sema_new_ptr_type(binder->arena, base);
    }
    case EL_AST_TYPE_NAME: {
        ElHirSymbol* sym = el_hir_scope_lookup(binder->current_scope, node->name->name);
        if (sym == NULL) return NULL;

        if (sym->kind != EL_HIR_SYM_TYPE) {
            el_diag_report(
                binder->diag, EL_DIAG_ERROR, "sema.",
                EL_SOURCE_SPAN_NULL, // TODO: source span should be attached to AST ig.
                "${type} ${name} used as type",
                EL_DIAG_STRING("type", sym->kind == EL_HIR_SYM_VAR ? EL_SV("Variable") : EL_SV("Function")),
                EL_DIAG_STRING("name", sym->name),
            );
            return NULL;
        }

        return sym->as.type.type;
    }
    }
}

ElHirExprNode* el_binder_bind_expr(ElBinder* binder,   ElAstExprNode* in) {
    (void) binder, (void) in;
    return NULL; // TODO: stub
}

ElHirBlockStmtNode* el_binder_bind_block(ElBinder* binder, ElAstBlockStmtNode* in) {
    (void) in;
    ElType* int_type = el_sema_new_prim_type(binder->arena, EL_PRIMTYPE_INT);
    ElHirExprNode* literal = el_hir_new_int_literal(binder->arena, int_type, 123);
    ElHirStmtNode* ret_stmt = el_hir_new_return_stmt(binder->arena, literal);

    ElHirBlockStmtNode* block = EL_DYNARENA_NEW(binder->arena, ElHirBlockStmtNode);
    block->stmts = ret_stmt;

    return block;
}

ElHirStmtNode* el_binder_bind_stmt(ElBinder* binder,   ElAstStmtNode* in) {
    (void) binder, (void) in;
    return NULL; // TODO: stub
}


ElHirTopLevelNode* el_binder_bind_toplvl(ElBinder* binder, ElAstTopLevelNode* in) {
    switch (in->type) {
    case EL_AST_TOPLVL_FUNC_DEF: {
        ElAstFuncDefinition* def = &in->as.func_def;
        ElHirScope* scope = _el_binder_push_scope(binder);

        ElType* ret_type = _el_binder_bind_type(binder, def->ret_type);
        if (ret_type == NULL) return NULL;

        ElHirSymbol** params = EL_DYNARENA_NEW_ARR(binder->arena, ElHirSymbol*, def->params.count);
        usize i = 0;
        for (ElAstFuncParam* param = def->params.head; param != NULL; param = param->next) {
            ElType* type = _el_binder_bind_type(binder, param->type);
            if (type == NULL) return NULL;
            
            ElHirSymbol* psym = el_hir_new_var_symbol(binder->arena, param->name->name, type);
            el_hir_scope_insert(scope, psym);
            params[i++] = psym;
        }

        ElHirSymbol* sym = el_hir_new_func_symbol(binder->arena, def->name->name, ret_type, params, def->params.count);
        el_hir_scope_insert(binder->current_scope->parent, sym);

        ElHirBlockStmtNode* block = el_binder_bind_block(binder, def->block);
        ElHirTopLevelNode* func = el_hir_new_func_definition(binder->arena, sym, block);
        _el_binder_pop_scope(binder);
        return func;
    }
    }

    EL_UNREACHABLE_ENUM_VAL(ElAstTopLevelType, in->type);
}

ElHirModule* el_binder_bind_module(ElBinder* binder, ElAstModuleNode* in) {
    ElHirModule* mod = el_hir_new_module(binder->arena);
    for (ElAstTopLevelNode* node = in->head; node != NULL; node = node->next) {
        ElHirTopLevelNode* binded = el_binder_bind_toplvl(binder, node);
        if (binded == NULL) return mod;

        el_hir_module_append(mod, binded);
    }
    return mod;
}
