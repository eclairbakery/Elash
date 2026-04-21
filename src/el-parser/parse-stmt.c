#include <el-parser/parser.h>
#include <el-parser/utility.h>

#include <el-ast/stmt.h>
#include <el-ast/stmt/return.h>

ElParserErrorCode _el_parser_parse_return(ElParser* parser, ElAstStmtNode** out) {
    if (el_parser_match(parser, EL_TT_SEMICOLON)) {
        *out = el_ast_new_return_stmt(parser->arena, NULL);
        return _el_parser_ret_ok(parser);
    }

    ElAstExprNode* expr;
    ElParserErrorCode result = _el_parser_parse_expression(parser, &expr);
    if (result != EL_PARSER_ERR_OK) {
        return result;
    }

    *out = el_ast_new_return_stmt(parser->arena, expr);
    return el_parser_expect(parser, EL_TT_SEMICOLON);
}

ElParserErrorCode _el_parser_parse_expr_stmt(ElParser* parser, ElAstStmtNode** out) {
    ElAstExprNode* expr;
    ElParserErrorCode result = _el_parser_parse_expression(parser, &expr);
    if (result != EL_PARSER_ERR_OK) {
        return result;
    }

    *out = el_ast_new_expr_stmt(parser->arena, expr);
    return el_parser_expect(parser, EL_TT_SEMICOLON);
}

ElParserErrorCode _el_parser_parse_stmt(ElParser* parser, ElAstStmtNode** out) {
    if (el_parser_match(parser, EL_TT_KW_RETURN)) {
        return _el_parser_parse_return(parser, out);
    }
    return _el_parser_parse_expr_stmt(parser, out);
}
