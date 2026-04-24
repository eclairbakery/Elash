#include <elash/parser/parser.h>
#include <elash/parser/utility.h>

#include <elash/ast/common/type.h>
#include <elash/ast/common/ident.h>

ElParserErrorCode _el_parser_parse_ident(ElParser* parser, ElAstIdentNode** out) {
    ElToken tok = parser->current;
    ElParserErrorCode result = el_parser_expect(parser, EL_TT_IDENT);
    if (result != EL_PARSER_ERR_OK) return result;

    if (out) {
        *out = el_ast_new_ident_node_raw(parser->arena, tok.lexeme);
    }

    return _el_parser_ret_ok(parser);
}

ElParserErrorCode _el_parser_parse_type(ElParser* parser, ElAstTypeNode** out) {
    ElParserErrorCode result;
    
    ElAstIdentNode* name;
    result = _el_parser_parse_ident(parser, &name);
    if (result != EL_PARSER_ERR_OK) return result;

    ElAstTypeNode* type = el_ast_new_type_name(parser->arena, name);

    while (el_parser_match(parser, EL_TT_STAR)) {
        type = el_ast_new_type_ptr(parser->arena, type);
    }

    if (out) {
        *out = type;
    }

    return _el_parser_ret_ok(parser);
}
