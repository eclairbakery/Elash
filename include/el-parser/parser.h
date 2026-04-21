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

ElParserErrorCode el_parser_parse(ElParser* parser, ElAstExprNode** out);
