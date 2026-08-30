#include "parser-internals.h"

#include <elash/ast/tree/toe.h>

ElAstToE* el_parser_parse_toe(ElParser* parser) {
    if (_el_parser_is_complex_expr(parser)) {
        ElAstExpr* expr = el_parser_parse_expr(parser);
        if (expr == NULL) return NULL;
        return el_ast_new_toe_expr(parser->aarena, expr);
    }

    // Type { ... }
    if (_el_parser_is_type_literal(parser)) {
        ElAstExpr* expr = _el_parser_parse_postfix(parser);
        if (expr == NULL) return NULL;
        return el_ast_new_toe_expr(parser->aarena, expr);
    }

    return _el_parser_toe_from_ambig(parser, _el_parser_parse_ambig(parser));
}
