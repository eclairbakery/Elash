#pragma once

#include <elash/defs/int-types.h>
#include <elash/lexer/tokbuf.h>
#include <elash/util/dynarena.h>

#include <elash/ast/tree/expr.h>
#include <elash/ast/tree/stmt.h>
#include <elash/ast/tree/init.h>
#include <elash/ast/tree/decl.h>
#include <elash/ast/tree/module.h>
#include <elash/ast/tree/type.h>
#include <elash/ast/tree/toe.h>
#include <elash/ast/tree/toi.h>
#include <elash/ast/tree/unr.h>

typedef struct ElUnparser {
    ElTokenBuf* out;
    ElDynArena* arena;
} ElUnparser;

void el_unparser_init(ElUnparser* unparser, ElTokenBuf* out, ElDynArena* arena);

bool el_unparser_push(ElUnparser* unparser, ElTokenType type, ElStringView lexeme);
bool el_unparser_push_punct(ElUnparser* unparser, ElTokenType type);
bool el_unparser_push_kw(ElUnparser* unparser, ElTokenType type);
bool el_unparser_push_ident(ElUnparser* unparser, ElStringView name);
bool el_unparser_push_fmt(ElUnparser* unparser, ElTokenType type, const char* fmt, ...);

bool el_unparser_unparse_module(ElUnparser* unparser, ElAstModule* module);
bool el_unparser_unparse_decl(ElUnparser* unparser, ElAstDecl* decl);
bool el_unparser_unparse_stmt(ElUnparser* unparser, ElAstStmt* stmt);
bool el_unparser_unparse_expr(ElUnparser* unparser, ElAstExpr* expr);
bool el_unparser_unparse_type(ElUnparser* unparser, ElAstType* type);
bool el_unparser_unparse_init(ElUnparser* unparser, ElAstInit* init);
bool el_unparser_unparse_unr(ElUnparser* unparser,  ElAstUnr* unr);
bool el_unparser_unparse_toe(ElUnparser* unparser,  ElAstToE* toe);
bool el_unparser_unparse_toi(ElUnparser* unparser,  ElAstToI* toi);

bool _el_unparser_unparse_ident(ElUnparser* unparser, ElAstIdent* ident);
bool _el_unparser_unparse_block(ElUnparser* unparser, ElAstBlockStmt* block);
bool _el_unparser_unparse_expr_prec(ElUnparser* unparser, ElAstExpr* expr, int min_prec);
bool _el_unparser_unparse_type_base(ElUnparser* unparser, ElAstType* type);
bool _el_unparser_unparse_func_sig(ElUnparser* unparser, ElAstFuncSignature* sig);
