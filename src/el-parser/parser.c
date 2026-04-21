#include <el-parser/parser.h>

#include <el-util/dynarena.h>
#include <el-util/strconv.h>

#include <el-ast/expr.h>
#include <el-ast/expr/bin.h>
#include <el-ast/expr/unary.h>
#include <el-ast/expr/literal.h>

#include <stdlib.h>
#include <string.h>

ElParserErrorCode el_parser_advance(ElParser* parser) {
    while (true) {
        if (el_pp_want_next_token(parser->pp)) {
            ElToken tok;
            ElLexerErrorCode lerr = el_lexer_next_token(parser->lexer, &tok);
            if (lerr != EL_LEXERR_SUCCESS) {
                return EL_PARSER_ERR_UNEXPECTED_TOKEN; 
            }
            el_pp_pass_token(parser->pp, tok);
        }

        ElPpErrorCode pperr = el_pp_preprocess(parser->pp, &parser->current);
        if (pperr == EL_PPERR_SKIPPED) {
            continue;
        }
        
        if (pperr != EL_PPERR_SUCCESS) {
            parser->last_err_details.code = EL_PARSER_ERR_PP_ERROR;
            return EL_PARSER_ERR_PP_ERROR;
        }

        return EL_PARSER_ERR_OK;
    }
}

bool el_parser_match(ElParser* parser, ElTokenType type) {
    if (parser->current.type == type) {
        el_parser_advance(parser);
        return true;
    }
    return false;
}

ElParserErrorCode el_parser_expect(ElParser* parser, ElTokenType type) {
    if (parser->current.type == type) {
        return el_parser_advance(parser);
    }
    parser->last_err_details.code = EL_PARSER_ERR_EXPECTED_TOKEN;
    parser->last_err_details.token = parser->current;
    parser->last_err_details.expected = type;
    return EL_PARSER_ERR_EXPECTED_TOKEN;
}

void el_parser_init(ElParser* parser, ElPreprocessor* pp, ElLexer* lexer, ElDynArena* arena) {
    parser->pp = pp;
    parser->lexer = lexer;
    parser->arena = arena;
    parser->current.type = EL_TT_UNKNOWN;
    memset(&parser->last_err_details, 0, sizeof(parser->last_err_details));
}

void el_parser_destroy(ElParser* parser) {
    // nothing for now
    (void) parser;
}

ElParserErrorCode el_parser_parse(ElParser* parser, ElAstExprNode** out) {
    if (parser->current.type == EL_TT_UNKNOWN) {
        ElParserErrorCode err = el_parser_advance(parser);
        if (err != EL_PARSER_ERR_OK) return err;
    }

    // TODO: parsing statements, top level declarations etc.
    *out = _el_parser_parse_expression(parser);
    if (*out == NULL) {
        // TODO: better error handling
        return EL_PARSER_ERR_UNEXPECTED_TOKEN;
    }

    return EL_PARSER_ERR_OK;
}
