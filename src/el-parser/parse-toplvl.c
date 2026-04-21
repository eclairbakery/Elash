#include <el-parser/parser.h>
#include <el-parser/utility.h>

#include <el-ast/toplevel.h>
#include <el-ast/toplevel/func.h>

#include <el-util/todo.h>

static ElParserErrorCode _el_parser_parse_func_params(ElParser* parser, ElAstFuncParamList* out) {
    ElParserErrorCode result;
    *out = el_ast_make_func_param_list();

    while (parser->current.type != EL_TT_RPAREN) {
        ElToken p_type_tok = parser->current;
        result = el_parser_expect(parser, EL_TT_IDENT);
        if (result != EL_PARSER_ERR_OK) return result;

        ElToken p_name_tok = parser->current;
        result = el_parser_expect(parser, EL_TT_IDENT);
        if (result != EL_PARSER_ERR_OK) return result;

        el_ast_func_param_list_append(out, el_ast_new_func_param(
            parser->arena,
            &el_ast_new_ident_node(parser->arena, p_type_tok.lexeme)->as.ident,
            &el_ast_new_ident_node(parser->arena, p_name_tok.lexeme)->as.ident
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

    ElToken ret_type_tok = parser->current;
    result = el_parser_expect(parser, EL_TT_IDENT);
    if (result != EL_PARSER_ERR_OK) return result;

    ElToken name_tok = parser->current;
    result = el_parser_expect(parser, EL_TT_IDENT);
    if (result != EL_PARSER_ERR_OK) return result;

    result = el_parser_expect(parser, EL_TT_LPAREN);
    if (result != EL_PARSER_ERR_OK) return result;

    ElAstFuncParamList params;
    result = _el_parser_parse_func_params(parser, &params);
    if (result != EL_PARSER_ERR_OK) return result;

    result = el_parser_expect(parser, EL_TT_RPAREN);
    if (result != EL_PARSER_ERR_OK) return result;

    result = el_parser_expect(parser, EL_TT_LBRACE);
    if (result != EL_PARSER_ERR_OK) return result;

    ElAstStmtNode* body_stmt;
    result = _el_parser_parse_block(parser, &body_stmt);
    if (result != EL_PARSER_ERR_OK) return result;

    *out = el_ast_new_func_definition(
        parser->arena,
        &el_ast_new_ident_node(parser->arena, ret_type_tok.lexeme)->as.ident,
        &el_ast_new_ident_node(parser->arena, name_tok.lexeme)->as.ident,
        params,
        &body_stmt->as.block
    );

    return _el_parser_ret_ok(parser);
}

ElParserErrorCode _el_parser_parse_toplevel(ElParser* parser, ElAstTopLevelNode** out) {
    // TODO
    return _el_parser_parse_func_def(parser, out);
}
