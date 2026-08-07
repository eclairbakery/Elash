#include <elash/unparser/unparser.h>

#include <elash/util/assert.h>

bool el_unparser_unparse_toe(ElUnparser* unpar, ElAstToE* toe) {
    switch (toe->kind) {
    case EL_AST_TOE_TYPE:
        return el_unparser_unparse_type(unpar, toe->as.type);

    case EL_AST_TOE_EXPR:
        return el_unparser_unparse_expr(unpar, toe->as.expr);

    case EL_AST_TOE_UNR_IDENT:
        return _el_unparser_unparse_ident(unpar, toe->as.unr_ident);

    case EL_AST_TOE_UNR_INDEX:
        if (!el_unparser_unparse_toe(unpar, toe->as.unr_index.base)) return false;
        if (!el_unparser_push_punct(unpar, EL_TT_LBRACKET)) return false;
        if (!el_unparser_unparse_toe(unpar, toe->as.unr_index.index)) return false;
        return el_unparser_push_punct(unpar, EL_TT_RBRACKET);
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstToEKind, toe->kind);
}
