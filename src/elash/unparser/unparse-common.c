#include <elash/unparser/unparser.h>

bool _el_unparser_unparse_ident(ElUnparser* unpar, ElAstIdent* ident) {
    return el_unparser_push_ident(unpar, ident->name);
}

bool _el_unparser_unparse_block(ElUnparser* unpar, ElAstBlockStmt* block) {
    if (!el_unparser_push_punct(unpar, EL_TT_LBRACE)) return false;
    for (ElAstStmt* stmt = block->stmts; stmt != NULL; stmt = stmt->next) {
        if (!el_unparser_unparse_stmt(unpar, stmt)) return false;
    }
    return el_unparser_push_punct(unpar, EL_TT_RBRACE);
}

bool _el_unparser_unparse_func_sig(ElUnparser* unpar, ElAstFuncSignature* sig) {
    if (!el_unparser_unparse_type(unpar, sig->ret_type)) return false;
    if (!_el_unparser_unparse_ident(unpar, sig->name)) return false;
    if (!el_unparser_push_punct(unpar, EL_TT_LPAREN)) return false;

    for (ElAstFuncParam* param = sig->params.head; param != NULL; param = param->next) {
        if (!el_unparser_unparse_type(unpar, param->type)) return false;
        if (!_el_unparser_unparse_ident(unpar, param->name)) return false;
        if (param->next != NULL) {
            if (!el_unparser_push_punct(unpar, EL_TT_COMMA)) return false;
        }
    }

    return el_unparser_push_punct(unpar, EL_TT_RPAREN);
}
