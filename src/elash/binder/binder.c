#include <elash/binder/binder.h>
#include <elash/util/assert.h>

#include <elash/diag/engine.h>
#include <elash/diag/meta.h>

#include <elash/sema/type/prim.h>

#include <elash/hir/tree/stmt/block.h>
#include <elash/hir/tree/stmt/return.h>

static ElType* _el_binder_register_builtin_type(ElBinder* binder, ElStringView name, ElPrimitiveTypeKind kind) {
    ElType* type = el_sema_new_prim_type(binder->arena, kind);
    ElHirSymbol* sym = el_hir_new_type_symbol(binder->arena, name, type);
    el_hir_scope_insert(binder->builtin_scope, sym);
    return type;
}

void el_binder_init(ElBinder* binder, ElDynArena* arena, ElDiagEngine* diag) {
    binder->arena = arena;
    binder->diag = diag;

    binder->builtin_scope = el_hir_scope_new(NULL);

    binder->type_int  = _el_binder_register_builtin_type(binder, EL_SV("int"),  EL_PRIMTYPE_INT);
    binder->type_uint = _el_binder_register_builtin_type(binder, EL_SV("uint"), EL_PRIMTYPE_UINT);
    binder->type_char = _el_binder_register_builtin_type(binder, EL_SV("char"), EL_PRIMTYPE_CHAR);

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
                binder->diag, EL_DIAG_ERROR, "sema.<TODO>", // TODO: idk how to name this
                EL_SOURCE_SPAN_NULL, // TODO: source span should be attached to AST ig.
                "${type} ${name} used as a type",
                EL_DIAG_STRING("type", sym->kind == EL_HIR_SYM_VAR ? EL_SV("Variable") : EL_SV("Function")),
                EL_DIAG_STRING("name", sym->name),
            );
            return NULL;
        }

        return sym->as.type.type;
    }
    }
}

ElHirExprNode* el_binder_bind_expr(ElBinder* binder, ElAstExprNode* in) {
    if (in == NULL) return NULL;

    switch (in->type) {
    case EL_AST_EXPR_BINARY: {
        ElHirExprNode* left = el_binder_bind_expr(binder, in->as.binary.left);
        ElHirExprNode* right = el_binder_bind_expr(binder, in->as.binary.right);
        if (!left || !right) return NULL;

        // TODO: implement type checking 
        return el_hir_new_bin_expr(binder->arena, left->type, in->as.binary.op, left, right);
    }
    case EL_AST_EXPR_UNARY: {
        ElHirExprNode* operand = el_binder_bind_expr(binder, in->as.unary.operand);
        if (!operand) return NULL;

        // TODO: implement type checking 
        return el_hir_new_unary_expr(binder->arena, operand->type, in->as.unary.op, operand);
    }
    case EL_AST_EXPR_LITERAL: {
        switch (in->as.literal.type) {
        case EL_AST_LIT_INT:
            return el_hir_new_int_literal(binder->arena, binder->type_int, in->as.literal.of.int_.value);
        case EL_AST_LIT_CHAR:
            return el_hir_new_char_literal(binder->arena, binder->type_char, in->as.literal.of.char_.value);
        default:
            // TODO: handle other literal types
            el_diag_report(
                binder->diag, EL_DIAG_ERROR, "sema.unsupported-literal",
                EL_SOURCE_SPAN_NULL, // TODO: source span should be attached to the Abstract Syntax Tree
                "unsupported literal type"
            );
            return NULL;
        }
    }
    case EL_AST_EXPR_IDENT: {
        ElHirSymbol* sym = el_hir_scope_lookup(binder->current_scope, in->as.ident.name);
        if (sym == NULL) {
            el_diag_report(
                binder->diag, EL_DIAG_ERROR, "sema.undefined-symbol",
                EL_SOURCE_SPAN_NULL, // TODO: source span should be attached to the Abstract Syntax Tree
                "undefined symbol '${name}'",
                EL_DIAG_STRING("name", in->as.ident.name)
            );
            return NULL;
        }

        switch (sym->kind) {
        case EL_HIR_SYM_VAR:
            return el_hir_new_symbol_expr(binder->arena, sym->as.var.type, sym);
        case EL_HIR_SYM_FUNC:
            EL_UNREACHABLE("Function types not implemented yet");
        case EL_HIR_SYM_TYPE:
            el_diag_report(
                binder->diag, EL_DIAG_ERROR, "sema.type-used-as-expr",
                EL_SOURCE_SPAN_NULL, // TODO: source span should be attached to the Abstract Syntax Tree
                "symbol '${name}' declared as type but used as an expression",
                EL_DIAG_STRING("name", sym->name)
            );
            return NULL;
        }
    }
    }

    return NULL;
}

ElHirBlockStmtNode _el_binder_bind_block(ElBinder* binder, ElAstBlockStmtNode* in) {
    ElHirStmtNode* head = NULL;
    ElHirStmtNode* tail = NULL;

    for (ElAstStmtNode* curr = in->stmts; curr != NULL; curr = curr->next) {
        ElHirStmtNode* binded = el_binder_bind_stmt(binder, curr);
        if (binded) {
            el_hir_stmt_list_append(&head, &tail, binded);
        }
    }

    return (ElHirBlockStmtNode) { .stmts = head };
}

ElHirStmtNode* el_binder_bind_stmt(ElBinder* binder, ElAstStmtNode* in) {
    switch (in->type) {
    case EL_AST_STMT_BLOCK: {
        ElHirBlockStmtNode block = _el_binder_bind_block(binder, &in->as.block);
        return el_hir_new_block_stmt(binder->arena, block.stmts);
    }
    case EL_AST_STMT_RETURN: {
        ElHirExprNode* val = el_binder_bind_expr(binder, in->as.return_.value);
        return el_hir_new_return_stmt(binder->arena, val);
    }
    case EL_AST_STMT_EXPR: {
        ElHirExprNode* expr = el_binder_bind_expr(binder, in->as.expr);
        return el_hir_new_expr_stmt(binder->arena, expr);
    }
    }
    return NULL;
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

        ElHirBlockStmtNode block = _el_binder_bind_block(binder, def->block);
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
