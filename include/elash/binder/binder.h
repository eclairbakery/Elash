#pragma once

#include <elash/binder/builtin.h>

#include <elash/hir/symbol.h>
#include <elash/hir/scope.h>
#include <elash/hir/toe.h>

#include <elash/hir/tree/module.h>
#include <elash/hir/tree/decl.h>
#include <elash/hir/tree/stmt.h>
#include <elash/hir/tree/expr.h>

#include <elash/ast/tree/module.h>
#include <elash/ast/tree/decl.h>
#include <elash/ast/tree/stmt.h>
#include <elash/ast/tree/expr.h>

#include <elash/util/dynarena.h>
#include <elash/diag/engine.h>

typedef struct ElBinderInitOpts {
    ElBinderBuiltins* builtins;
    ElDiagEngine* diag;
    ElDynArena* arena;
} ElBinderInitOpts;

typedef struct ElBinder {
    ElDynArena* arena;

    ElDiagEngine* diag;
    ElBinderBuiltins* builtins;

    ElScope* builtin_scope;
    ElScope* global_scope;

    ElScope* current_scope;
    ElHirSymbol* current_func;

    uint32_t sym_id_counter;
    uint32_t loop_depth;
} ElBinder;

#define el_binder_init(BINDER, ...) \
    el_binder_init_opts((BINDER), (ElBinderInitOpts) { __VA_ARGS__ })

void el_binder_init_opts(ElBinder* binder, ElBinderInitOpts opts);
void el_binder_free(ElBinder* binder);

ElHirExpr* el_binder_bind_designated(ElBinder* binder, ElAstInit* in, ElHirType* expected_type, ElStorageClass scls);
ElHirExpr* el_binder_bind_init_list(ElBinder* binder, ElAstInit* in, ElHirType* expected_type, ElStorageClass scls);
ElHirExpr* el_binder_bind_init(ElBinder* binder, ElAstInit* in, ElHirType* expected_type, ElStorageClass scls);

ElHirExpr* el_binder_bind_builtin_call(ElBinder* binder, ElAstExpr* in, ElAstCallExpr* call, ElHirSymbol* builtin);

ElHirToE*      el_binder_bind_toe(ElBinder* binder,    ElAstToE* in);
ElHirExpr*     el_binder_bind_expr(ElBinder* binder,   ElAstExpr* in);
ElHirDecl*     el_binder_bind_decl(ElBinder* binder,   ElAstDecl* in);
ElHirStmt*     el_binder_bind_stmt(ElBinder* binder,   ElAstStmt* in);
ElHirModule*   el_binder_bind_module(ElBinder* binder, ElAstModule* in);
