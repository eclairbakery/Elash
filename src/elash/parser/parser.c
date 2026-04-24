#include <elash/parser/parser.h>
#include <elash/parser/utility.h>

#include <elash/util/dynarena.h>
#include <elash/util/strconv.h>

#include <elash/ast/expr.h>
#include <elash/ast/expr/bin.h>
#include <elash/ast/expr/unary.h>
#include <elash/ast/expr/literal.h>

ElParserErrorCode el_parser_advance(ElParser* parser) {
    while (true) {
        if (el_pp_want_next_token(parser->pp)) {
            ElToken tok;
            ElLexerErrorCode lerr = el_lexer_next_token(parser->lexer, &tok);
            if (lerr != EL_LEXERR_SUCCESS) {
                return _el_parser_ret_err(parser, .code = EL_PARSER_ERR_UNEXPECTED_TOKEN, .token = tok); 
            }
            el_pp_pass_token(parser->pp, tok);
        }

        ElPpErrorCode pperr = el_pp_preprocess(parser->pp, &parser->current);
        if (pperr == EL_PPERR_SKIPPED) {
            continue;
        }
        
        if (pperr != EL_PPERR_SUCCESS) {
            return _el_parser_ret_err(parser, .code = EL_PARSER_ERR_PP_ERROR);
        }

        return _el_parser_ret_ok(parser);
    }
}

bool el_parser_match(ElParser* parser, ElTokenType type) {
    if (el_parser_check(parser, type)) {
        el_parser_advance(parser);
        return true;
    }
    return false;
}

bool el_parser_check(ElParser* parser, ElTokenType type) {
    return parser->current.type == type;
}

ElParserErrorCode el_parser_expect(ElParser* parser, ElTokenType type) {
    if (el_parser_check(parser, type)) {
        return el_parser_advance(parser);
    }
    return _el_parser_ret_err(parser,
        .code = EL_PARSER_ERR_EXPECTED_TOKEN,
        .expected = type
    );
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

ElParserErrorCode el_parser_parse(ElParser* parser, ElAstModuleNode** out) {
    if (parser->current.type == EL_TT_UNKNOWN) {
        ElParserErrorCode err = el_parser_advance(parser);
        if (err != EL_PARSER_ERR_OK) return err;
    }
    
    return _el_parser_parse_module(parser, out);
}
