#pragma once

#include <el-lexer/lexer.h>
#include <el-pp/preproc.h>

#include <el-ast/expr.h>

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
bool el_parser_match(ElParser* parser, ElTokenType type); 
ElParserErrorCode el_parser_expect(ElParser* parser, ElTokenType type);

ElAstExprNode* _el_parser_parse_expression(ElParser* parser);
ElAstExprNode* _el_parser_parse_primary(ElParser* parser);
ElAstExprNode* _el_parser_parse_postfix(ElParser* parser);
ElAstExprNode* _el_parser_parse_unary(ElParser* parser);
ElAstExprNode* _el_parser_parse_multiplicative(ElParser* parser);
ElAstExprNode* _el_parser_parse_additive(ElParser* parser);
ElAstExprNode* _el_parser_parse_shift(ElParser* parser);
ElAstExprNode* _el_parser_parse_relational(ElParser* parser);
ElAstExprNode* _el_parser_parse_equality(ElParser* parser);
ElAstExprNode* _el_parser_parse_bitwise_and(ElParser* parser);
ElAstExprNode* _el_parser_parse_bitwise_xor(ElParser* parser);
ElAstExprNode* _el_parser_parse_bitwise_or(ElParser* parser);
ElAstExprNode* _el_parser_parse_logical_and(ElParser* parser);
ElAstExprNode* _el_parser_parse_logical_or(ElParser* parser);
ElAstExprNode* _el_parser_parse_expression(ElParser* parser);

ElParserErrorCode el_parser_parse(ElParser* parser, ElAstExprNode** out);
