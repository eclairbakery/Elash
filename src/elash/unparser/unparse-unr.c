#include <elash/unparser/unparser.h>

#include <elash/util/assert.h>

bool el_unparser_unparse_unr(ElUnparser* unpar, ElAstUnr* unr) {
    switch (unr->kind) {
    case EL_AST_UNR_IDENT:
        return _el_unparser_unparse_ident(unpar, unr->as.ident);

    case EL_AST_UNR_INDEX:
        if (!el_unparser_unparse_unr(unpar, unr->as.index.base)) return false;
        if (!el_unparser_push_punct(unpar, EL_TT_LBRACKET)) return false;
        if (unr->as.index.index != NULL) {
            if (!el_unparser_unparse_unr(unpar, unr->as.index.index)) return false;
        } else {
            if (!el_unparser_unparse_expr(unpar, unr->as.index.index_expr)) return false;
        }
        return el_unparser_push_punct(unpar, EL_TT_RBRACKET);
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstUnrKind, unr->kind);
}
