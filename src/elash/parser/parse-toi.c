#include "parser-internals.h"

#include <elash/ast/tree/toi.h>

ElAstToI* el_parser_parse_toi(ElParser* parser) {
    if (el_parser_check(parser, EL_TT_LBRACE)) {
        ElAstInit* init = el_parser_parse_init(parser);
        if (init == NULL) return NULL;
        return el_ast_new_toi_init(parser->aarena, init);
    }

    ElAstToE* toe = el_parser_parse_toe(parser);
    return el_ast_toi_from_toe(parser->aarena, toe);
}
