#pragma once

#include <elash/hir/symbol.h>
#include <elash/hir/scope.h>

#include <elash/hir/tree/module.h>
#include <elash/hir/tree/toplevel.h>
#include <elash/hir/tree/stmt.h>
#include <elash/hir/tree/expr.h>

#include <elash/ast/module.h>
#include <elash/ast/toplevel.h>
#include <elash/ast/stmt.h>
#include <elash/ast/expr.h>

#include <elash/util/dynarena.h>
#include <elash/diag/engine.h>

typedef struct ElBinder {
    ElDynArena* arena;
    ElDiagEngine* diag;
    ElHirScope* global_scope;
    ElHirScope* current_scope;
    // TODO
} ElBinder;

void el_binder_init(ElBinder* binder, ElDynArena* arena, ElDiagEngine* diag);
void el_binder_free(ElBinder* binder);

ElHirExprNode*     el_binder_bind_expr(ElBinder* binder,   ElAstExprNode* in);
ElHirStmtNode*     el_binder_bind_stmt(ElBinder* binder,   ElAstStmtNode* in);
ElHirTopLevelNode* el_binder_bind_toplvl(ElBinder* binder, ElAstTopLevelNode* in);
ElHirModule*       el_binder_bind_module(ElBinder* binder, ElAstModuleNode* in);
