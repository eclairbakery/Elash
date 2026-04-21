#pragma once

#include <el-lexer/lexer.h>
#include <el-pp/preproc.h>

#include <el-ast/expr.h>
#include <el-ast/stmt.h>
#include <el-ast/toplevel.h>
#include <el-ast/common/type.h>

#include <el-parser/error.h>

typedef struct ElParser {
    ElPreprocessor* pp;
    ElLexer* lexer;

    ElDynArena* arena;

    ElToken current;
    ElParserErrorDetails last_err_details;
} ElParser;

void el_parser_init(ElParser* parser, ElPreprocessor* pp, ElLexer* lexer, ElDynArena* arena);
void el_parser_destroy(ElParser* parser);

ElParserErrorCode el_parser_advance(ElParser* parser);
ElParserErrorCode el_parser_expect(ElParser* parser, ElTokenType type);
bool el_parser_match(ElParser* parser, ElTokenType type); 
bool el_parser_check(ElParser* parser, ElTokenType type);

ElParserErrorCode _el_parser_parse_ident(ElParser* parser, ElAstIdentNode** out);
ElParserErrorCode _el_parser_parse_type(ElParser* parser, ElAstTypeNode** out);

ElParserErrorCode _el_parser_parse_expression(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_primary(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_postfix(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_unary(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_multiplicative(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_additive(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_shift(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_relational(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_equality(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_bitwise_and(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_bitwise_xor(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_bitwise_or(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_logical_and(ElParser* parser, ElAstExprNode** out);
ElParserErrorCode _el_parser_parse_logical_or(ElParser* parser, ElAstExprNode** out);

ElParserErrorCode _el_parser_parse_return(ElParser* parser, ElAstStmtNode** out);
ElParserErrorCode _el_parser_parse_expr_stmt(ElParser* parser, ElAstStmtNode** out);
ElParserErrorCode _el_parser_parse_block(ElParser* parser, ElAstStmtNode** out);
ElParserErrorCode _el_parser_parse_stmt(ElParser* parser, ElAstStmtNode** out);

ElParserErrorCode _el_parser_parse_func_def(ElParser* parser, ElAstTopLevelNode** out);
ElParserErrorCode _el_parser_parse_toplevel(ElParser* parser, ElAstTopLevelNode** out);

ElParserErrorCode el_parser_parse(ElParser* parser, ElAstTopLevelNode** out);
