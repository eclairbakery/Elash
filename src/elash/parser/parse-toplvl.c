#include <elash/parser/parser.h>
#include <elash/parser/utility.h>

#include <elash/ast/toplevel.h>
#include <elash/ast/toplevel/func.h>

#include <elash/util/todo.h>

static ElParserErrorCode _el_parser_parse_func_params(ElParser* parser, ElAstFuncParamList* out) {
    ElParserErrorCode result;
    *out = el_ast_make_func_param_list();

    while (parser->current.type != EL_TT_RPAREN) {
        ElAstTypeNode* p_type;
        result = _el_parser_parse_type(parser, &p_type);
        if (result != EL_PARSER_ERR_OK) return result;

        ElAstIdentNode* p_name;
        result = _el_parser_parse_ident(parser, &p_name);
        if (result != EL_PARSER_ERR_OK) return result;

        el_ast_func_param_list_append(out, el_ast_new_func_param(
            parser->arena,
            el_source_span_merge(p_type->span, p_name->span),
            p_type,
            p_name
        ));

        if (parser->current.type == EL_TT_COMMA) {
            el_parser_advance(parser);
        } else if (parser->current.type != EL_TT_RPAREN) {
            return _el_parser_ret_err(parser, .code = EL_PARSER_ERR_EXPECTED_TOKEN, .expected = EL_TT_RPAREN);
        }
    }

    return _el_parser_ret_ok(parser);
}

ElParserErrorCode _el_parser_parse_func_def(ElParser* parser, ElAstTopLevelNode** out) {
    ElParserErrorCode result;

    ElAstTypeNode* ret_type;
    result = _el_parser_parse_type(parser, &ret_type);
    if (result != EL_PARSER_ERR_OK) return result;

    ElAstIdentNode* name;
    result = _el_parser_parse_ident(parser, &name);
    if (result != EL_PARSER_ERR_OK) return result;

    result = el_parser_expect(parser, EL_TT_LPAREN);
    if (result != EL_PARSER_ERR_OK) return result;

    ElAstFuncParamList params;
    result = _el_parser_parse_func_params(parser, &params);
    if (result != EL_PARSER_ERR_OK) return result;

    result = el_parser_expect(parser, EL_TT_RPAREN);
    if (result != EL_PARSER_ERR_OK) return result;

    ElToken lbrace_tok = parser->current;
    result = el_parser_expect(parser, EL_TT_LBRACE);
    if (result != EL_PARSER_ERR_OK) return result;

    ElAstStmtNode* body_stmt;
    result = _el_parser_parse_block(parser, lbrace_tok, &body_stmt);
    if (result != EL_PARSER_ERR_OK) return result;

    *out = el_ast_new_func_definition(
        parser->arena,
        el_source_span_merge(ret_type->span, body_stmt->span),
        ret_type,
        name,
        params,
        &body_stmt->as.block
    );

    return _el_parser_ret_ok(parser);
}

ElParserErrorCode _el_parser_parse_toplevel(ElParser* parser, ElAstTopLevelNode** out) {
    // TODO
    return _el_parser_parse_func_def(parser, out);
}
