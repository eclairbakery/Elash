#include <elash/parser/parser.h>
#include <elash/parser/utility.h>

#include <elash/ast/stmt.h>
#include <elash/ast/stmt/return.h>

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

ElParserErrorCode _el_parser_parse_block(ElParser* parser, ElAstStmtNode** out) {
    ElAstStmtNode* head = NULL;
    ElAstStmtNode* tail = NULL;

    while (parser->current.type != EL_TT_RBRACE && parser->current.type != EL_TT_EOF) {
        ElAstStmtNode* stmt;
        ElParserErrorCode result = _el_parser_parse_stmt(parser, &stmt);
        if (result != EL_PARSER_ERR_OK) {
            return result;
        }

        el_ast_stmt_list_append(&head, &tail, stmt);
    }

    ElParserErrorCode result = el_parser_expect(parser, EL_TT_RBRACE);
    if (result != EL_PARSER_ERR_OK) {
        return result;
    }

    *out = el_ast_new_block_stmt(parser->arena, head);
    return _el_parser_ret_ok(parser);
}

ElParserErrorCode _el_parser_parse_stmt(ElParser* parser, ElAstStmtNode** out) {
    if (el_parser_match(parser, EL_TT_KW_RETURN)) {
        return _el_parser_parse_return(parser, out);
    }
    if (el_parser_match(parser, EL_TT_LBRACE)) {
        return _el_parser_parse_block(parser, out);
    }
    return _el_parser_parse_expr_stmt(parser, out);
}
