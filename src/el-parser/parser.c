#include <el-parser/parser.h>

#include <el-util/dynarena.h>
#include <el-util/strconv.h>

#include <el-ast/node.h>
#include <el-ast/bin.h>
#include <el-ast/unary.h>
#include <el-ast/literal.h>

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

static ElAstNode* parse_expression(ElParser* parser);

static ElAstNode* parse_primary(ElParser* parser) {
    if (parser->current.type == EL_TT_INT_LITERAL) {
        ElToken tok = parser->current;
        el_parser_advance(parser);

        int64_t val = 0;
        if (!el_string_to_i64(tok.lexeme, &val)) {
            // TODO: improve error handling
        }
        return el_ast_new_int_literal(parser->arena, val);
    }
    
    if (parser->current.type == EL_TT_FLOAT_LITERAL) {
        ElToken tok = parser->current;
        el_parser_advance(parser);

        long double val = 0.0L;
        if (!el_string_to_long_double(tok.lexeme, &val)) {
            // TODO: improve error handling
        }
        return el_ast_new_float_literal(parser->arena, val);
    }

    if (parser->current.type == EL_TT_STRING_LITERAL) {
        ElToken tok = parser->current;
        el_parser_advance(parser);
        return el_ast_new_string_literal(parser->arena, tok.lexeme);
    }

    if (parser->current.type == EL_TT_CHAR_LITERAL) {
        ElToken tok = parser->current;
        el_parser_advance(parser);

        char val = 0;
        if (tok.lexeme.len >= 3) {
             val = tok.lexeme.data[1];
        }
        return el_ast_new_char_literal(parser->arena, val);
    }

    if (el_parser_match(parser, EL_TT_TRUE_LITERAL)) {
        return el_ast_new_bool_literal(parser->arena, true);
    }

    if (el_parser_match(parser, EL_TT_FALSE_LITERAL)) {
        return el_ast_new_bool_literal(parser->arena, false);
    }

    if (el_parser_match(parser, EL_TT_NULL_LITERAL)) {
        return el_ast_new_null_literal(parser->arena);
    }

    if (el_parser_match(parser, EL_TT_LPAREN)) {
        ElAstNode* node = parse_expression(parser);
        el_parser_expect(parser, EL_TT_RPAREN);
        return node;
    }
    
    return NULL;
}

static ElAstNode* parse_postfix(ElParser* parser) {
    ElAstNode* node = parse_primary(parser);
    if (!node) return NULL;
    while (true) {
        if (el_parser_match(parser, EL_TT_INC)) {
            node = el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_POST_INC, node);
        } else if (el_parser_match(parser, EL_TT_DEC)) {
            node = el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_POST_DEC, node);
        } else {
            break;
        }
    }
    return node;
}

static ElAstNode* parse_unary(ElParser* parser) {
    if (el_parser_match(parser, EL_TT_PLUS))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_POS, parse_unary(parser));
    if (el_parser_match(parser, EL_TT_MINUS))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_NEG, parse_unary(parser));
    if (el_parser_match(parser, EL_TT_LOGICAL_NOT))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_NOT, parse_unary(parser));
    if (el_parser_match(parser, EL_TT_BITWISE_NOT))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_BW_NOT, parse_unary(parser));
    if (el_parser_match(parser, EL_TT_INC))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_PRE_INC, parse_unary(parser));
    if (el_parser_match(parser, EL_TT_DEC))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_PRE_DEC, parse_unary(parser));

    return parse_postfix(parser);
}

static ElAstNode* parse_multiplicative(ElParser* parser) {
    ElAstNode* node = parse_unary(parser);
    if (!node) return NULL;
    while (true) {
        ElAstBinExprType type;
        if (el_parser_match(parser, EL_TT_STAR)) type = EL_AST_BIN_EXPR_MUL;
        else if (el_parser_match(parser, EL_TT_SLASH)) type = EL_AST_BIN_EXPR_DIV;
        else if (el_parser_match(parser, EL_TT_PERCENT)) type = EL_AST_BIN_EXPR_MOD;
        else break;

        ElAstNode* right = parse_unary(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, type, node, right);
    }
    return node;
}

static ElAstNode* parse_additive(ElParser* parser) {
    ElAstNode* node = parse_multiplicative(parser);
    if (!node) return NULL;
    while (true) {
        ElAstBinExprType type;
        if (el_parser_match(parser, EL_TT_PLUS)) type = EL_AST_BIN_EXPR_ADD;
        else if (el_parser_match(parser, EL_TT_MINUS)) type = EL_AST_BIN_EXPR_SUB;
        else break;

        ElAstNode* right = parse_multiplicative(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, type, node, right);
    }
    return node;
}

static ElAstNode* parse_shift(ElParser* parser) {
    ElAstNode* node = parse_additive(parser);
    if (!node) return NULL;
    while (true) {
        ElAstBinExprType type;
        if (el_parser_match(parser, EL_TT_SHL)) type = EL_AST_BIN_EXPR_SHL;
        else if (el_parser_match(parser, EL_TT_SHR)) type = EL_AST_BIN_EXPR_SHR;
        else break;

        ElAstNode* right = parse_additive(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, type, node, right);
    }
    return node;
}

static ElAstNode* parse_relational(ElParser* parser) {
    ElAstNode* node = parse_shift(parser);
    if (!node) return NULL;
    while (true) {
        ElAstBinExprType type;
        if (el_parser_match(parser, EL_TT_LT)) type = EL_AST_BIN_EXPR_LT;
        else if (el_parser_match(parser, EL_TT_LTE)) type = EL_AST_BIN_EXPR_LTE;
        else if (el_parser_match(parser, EL_TT_GT)) type = EL_AST_BIN_EXPR_GT;
        else if (el_parser_match(parser, EL_TT_GTE)) type = EL_AST_BIN_EXPR_GTE;
        else break;

        ElAstNode* right = parse_shift(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, type, node, right);
    }
    return node;
}

static ElAstNode* parse_equality(ElParser* parser) {
    ElAstNode* node = parse_relational(parser);
    if (!node) return NULL;
    while (true) {
        ElAstBinExprType type;
        if (el_parser_match(parser, EL_TT_EQL)) type = EL_AST_BIN_EXPR_EQ;
        else if (el_parser_match(parser, EL_TT_NEQ)) type = EL_AST_BIN_EXPR_NEQ;
        else break;

        ElAstNode* right = parse_relational(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, type, node, right);
    }
    return node;
}

static ElAstNode* parse_bitwise_and(ElParser* parser) {
    ElAstNode* node = parse_equality(parser);
    if (!node) return NULL;
    while (el_parser_match(parser, EL_TT_BITWISE_AND)) {
        ElAstNode* right = parse_equality(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, EL_AST_BIN_EXPR_BW_AND, node, right);
    }
    return node;
}

static ElAstNode* parse_bitwise_xor(ElParser* parser) {
    ElAstNode* node = parse_bitwise_and(parser);
    if (!node) return NULL;
    while (el_parser_match(parser, EL_TT_BITWISE_XOR)) {
        ElAstNode* right = parse_bitwise_and(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, EL_AST_BIN_EXPR_BW_XOR, node, right);
    }
    return node;
}

static ElAstNode* parse_bitwise_or(ElParser* parser) {
    ElAstNode* node = parse_bitwise_xor(parser);
    if (!node) return NULL;
    while (el_parser_match(parser, EL_TT_BITWISE_OR)) {
        ElAstNode* right = parse_bitwise_xor(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, EL_AST_BIN_EXPR_BW_OR, node, right);
    }
    return node;
}

static ElAstNode* parse_logical_and(ElParser* parser) {
    ElAstNode* node = parse_bitwise_or(parser);
    if (!node) return NULL;
    while (el_parser_match(parser, EL_TT_LOGICAL_AND)) {
        ElAstNode* right = parse_bitwise_or(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, EL_AST_BIN_EXPR_AND, node, right);
    }
    return node;
}

static ElAstNode* parse_logical_or(ElParser* parser) {
    ElAstNode* node = parse_logical_and(parser);
    if (!node) return NULL;
    while (el_parser_match(parser, EL_TT_LOGICAL_OR)) {
        ElAstNode* right = parse_logical_and(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, EL_AST_BIN_EXPR_OR, node, right);
    }
    return node;
}

static ElAstNode* parse_expression(ElParser* parser) {
    return parse_logical_or(parser);
}

ElParserErrorCode el_parser_parse(ElParser* parser, ElAstNode** out) {
    if (parser->current.type == EL_TT_UNKNOWN) {
        ElParserErrorCode err = el_parser_advance(parser);
        if (err != EL_PARSER_ERR_OK) return err;
    }

    // TODO: parsing statements, top level declarations etc.
    *out = parse_expression(parser);
    if (*out == NULL) {
        // TODO: better error handling
        return EL_PARSER_ERR_UNEXPECTED_TOKEN;
    }

    return EL_PARSER_ERR_OK;
}
