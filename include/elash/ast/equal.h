#pragma once

#include <stdbool.h>
#include <elash/ast/tree/module.h>
#include <elash/ast/tree/decl.h>
#include <elash/ast/tree/stmt.h>
#include <elash/ast/tree/expr.h>
#include <elash/ast/tree/type.h>
#include <elash/ast/tree/init.h>

bool el_ast_equal_module(const ElAstModule* a, const ElAstModule* b);
bool el_ast_equal_decl(const ElAstDecl* a, const ElAstDecl* b);
bool el_ast_equal_stmt(const ElAstStmt* a, const ElAstStmt* b);
bool el_ast_equal_expr(const ElAstExpr* a, const ElAstExpr* b);
bool el_ast_equal_type(const ElAstType* a, const ElAstType* b);
bool el_ast_equal_init(const ElAstInit* a, const ElAstInit* b);
