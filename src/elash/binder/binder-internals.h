#pragma once
#include <elash/binder/binder.h> // IWYU pragma: export

ElScope* _el_binder_push_scope(ElBinder* binder);
ElScope* _el_binder_pop_scope(ElBinder* binder);

ElHirBlockStmt _el_binder_bind_block(ElBinder* binder, ElAstBlockStmt* in);
ElHirType*     _el_binder_bind_type(ElBinder* binder, ElAstType* type);

ElHirToE* _el_binder_bind_unresolved(ElBinder* binder, ElAstToE* in, ElAstUnr* unr);

ElHirExpr* _el_binder_simplify_expr(ElBinder* binder, ElHirExpr* expr);
ElHirExpr* _el_binder_explicit_cast(ElBinder* binder, ElSourceSpan span, ElHirExpr* expr, ElHirType* to);
ElHirExpr* _el_binder_implicit_cast(ElBinder* binder, ElSourceSpan span, ElHirExpr* expr, ElHirType* to);
ElHirExpr* _el_binder_eval_const_cast(ElBinder* binder, ElSourceSpan span, ElHirExpr* expr, ElHirType* to);
ElHirExpr* _el_binder_apply_default_type(ElBinder* binder, ElHirExpr* expr);

usize _el_binder_find_field(ElStringView name, const ElHirStructType* type, bool* found);
bool _el_binder_eval_const_index(ElBinder* binder, ElAstExpr* expr, usize* out_idx);
bool _el_binder_is_const(ElBinder* binder, ElHirExpr* expr);

bool _el_binder_stmt_always_returns(ElBinder* binder, ElHirStmt* stmt);
bool _el_binder_block_always_returns(ElBinder* binder, ElHirBlockStmt block);
