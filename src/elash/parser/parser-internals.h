#pragma once
#include <elash/parser/parser.h> // IWYU pragma: export

void _el_parser_report_expected(ElParser* parser, ElTokenType expected);
void _el_parser_report_unexpected(ElParser* parser, ElToken tok);

bool _el_parser_lookahead_skip_type(ElParser* parser, usize* idx);
bool _el_parser_is_type_literal(ElParser* parser);

ElAstStmt*  _el_parser_parse_block(ElParser* parser, ElToken lbrace_tok);
ElAstIdent* _el_parser_parse_ident(ElParser* parser);
ElAstType*  _el_parser_parse_type(ElParser* parser);
ElAstType*  _el_parser_parse_type_suffixes(ElParser* parser, ElAstType* type);

ElAstExpr* _el_parser_parse_primary(ElParser* parser);
ElAstExpr* _el_parser_parse_postfix(ElParser* parser);
ElAstExpr* _el_parser_parse_member(ElParser* parser, ElAstExpr* expr);
ElAstExpr* _el_parser_parse_call(ElParser* parser, ElAstExpr* callee);
