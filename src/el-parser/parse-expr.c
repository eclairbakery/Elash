#include <el-parser/parser.h>

#include <el-util/dynarena.h>
#include <el-util/strconv.h>

#include <el-ast/expr.h>
#include <el-ast/expr/bin.h>
#include <el-ast/expr/unary.h>
#include <el-ast/expr/literal.h>

ElAstExprNode* _el_parser_parse_expression(ElParser* parser);

ElAstExprNode* _el_parser_parse_primary(ElParser* parser) {
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
        ElAstExprNode* node = _el_parser_parse_expression(parser);
        el_parser_expect(parser, EL_TT_RPAREN);
        return node;
    }
    
    return NULL;
}

ElAstExprNode* _el_parser_parse_postfix(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_primary(parser);
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

ElAstExprNode* _el_parser_parse_unary(ElParser* parser) {
    if (el_parser_match(parser, EL_TT_PLUS))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_POS, _el_parser_parse_unary(parser));
    if (el_parser_match(parser, EL_TT_MINUS))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_NEG, _el_parser_parse_unary(parser));
    if (el_parser_match(parser, EL_TT_LOGICAL_NOT))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_NOT, _el_parser_parse_unary(parser));
    if (el_parser_match(parser, EL_TT_BITWISE_NOT))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_BW_NOT, _el_parser_parse_unary(parser));
    if (el_parser_match(parser, EL_TT_INC))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_PRE_INC, _el_parser_parse_unary(parser));
    if (el_parser_match(parser, EL_TT_DEC))
        return el_ast_new_unary_expr(parser->arena, EL_AST_UNARY_EXPR_PRE_DEC, _el_parser_parse_unary(parser));

    return _el_parser_parse_postfix(parser);
}

ElAstExprNode* _el_parser_parse_multiplicative(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_unary(parser);
    if (!node) return NULL;
    while (true) {
        ElAstBinExprType type;
        if (el_parser_match(parser, EL_TT_STAR)) type = EL_AST_BIN_EXPR_MUL;
        else if (el_parser_match(parser, EL_TT_SLASH)) type = EL_AST_BIN_EXPR_DIV;
        else if (el_parser_match(parser, EL_TT_PERCENT)) type = EL_AST_BIN_EXPR_MOD;
        else break;

        ElAstExprNode* right = _el_parser_parse_unary(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, type, node, right);
    }
    return node;
}

ElAstExprNode* _el_parser_parse_additive(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_multiplicative(parser);
    if (!node) return NULL;
    while (true) {
        ElAstBinExprType type;
        if (el_parser_match(parser, EL_TT_PLUS)) type = EL_AST_BIN_EXPR_ADD;
        else if (el_parser_match(parser, EL_TT_MINUS)) type = EL_AST_BIN_EXPR_SUB;
        else break;

        ElAstExprNode* right = _el_parser_parse_multiplicative(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, type, node, right);
    }
    return node;
}

ElAstExprNode* _el_parser_parse_shift(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_additive(parser);
    if (!node) return NULL;
    while (true) {
        ElAstBinExprType type;
        if (el_parser_match(parser, EL_TT_SHL)) type = EL_AST_BIN_EXPR_SHL;
        else if (el_parser_match(parser, EL_TT_SHR)) type = EL_AST_BIN_EXPR_SHR;
        else break;

        ElAstExprNode* right = _el_parser_parse_additive(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, type, node, right);
    }
    return node;
}

ElAstExprNode* _el_parser_parse_relational(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_shift(parser);
    if (!node) return NULL;
    while (true) {
        ElAstBinExprType type;
        if (el_parser_match(parser, EL_TT_LT)) type = EL_AST_BIN_EXPR_LT;
        else if (el_parser_match(parser, EL_TT_LTE)) type = EL_AST_BIN_EXPR_LTE;
        else if (el_parser_match(parser, EL_TT_GT)) type = EL_AST_BIN_EXPR_GT;
        else if (el_parser_match(parser, EL_TT_GTE)) type = EL_AST_BIN_EXPR_GTE;
        else break;

        ElAstExprNode* right = _el_parser_parse_shift(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, type, node, right);
    }
    return node;
}

ElAstExprNode* _el_parser_parse_equality(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_relational(parser);
    if (!node) return NULL;
    while (true) {
        ElAstBinExprType type;
        if (el_parser_match(parser, EL_TT_EQL)) type = EL_AST_BIN_EXPR_EQ;
        else if (el_parser_match(parser, EL_TT_NEQ)) type = EL_AST_BIN_EXPR_NEQ;
        else break;

        ElAstExprNode* right = _el_parser_parse_relational(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, type, node, right);
    }
    return node;
}

ElAstExprNode* _el_parser_parse_bitwise_and(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_equality(parser);
    if (!node) return NULL;
    while (el_parser_match(parser, EL_TT_BITWISE_AND)) {
        ElAstExprNode* right = _el_parser_parse_equality(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, EL_AST_BIN_EXPR_BW_AND, node, right);
    }
    return node;
}

ElAstExprNode* _el_parser_parse_bitwise_xor(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_bitwise_and(parser);
    if (!node) return NULL;
    while (el_parser_match(parser, EL_TT_BITWISE_XOR)) {
        ElAstExprNode* right = _el_parser_parse_bitwise_and(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, EL_AST_BIN_EXPR_BW_XOR, node, right);
    }
    return node;
}

ElAstExprNode* _el_parser_parse_bitwise_or(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_bitwise_xor(parser);
    if (!node) return NULL;
    while (el_parser_match(parser, EL_TT_BITWISE_OR)) {
        ElAstExprNode* right = _el_parser_parse_bitwise_xor(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, EL_AST_BIN_EXPR_BW_OR, node, right);
    }
    return node;
}

ElAstExprNode* _el_parser_parse_logical_and(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_bitwise_or(parser);
    if (!node) return NULL;
    while (el_parser_match(parser, EL_TT_LOGICAL_AND)) {
        ElAstExprNode* right = _el_parser_parse_bitwise_or(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, EL_AST_BIN_EXPR_AND, node, right);
    }
    return node;
}

ElAstExprNode* _el_parser_parse_logical_or(ElParser* parser) {
    ElAstExprNode* node = _el_parser_parse_logical_and(parser);
    if (!node) return NULL;
    while (el_parser_match(parser, EL_TT_LOGICAL_OR)) {
        ElAstExprNode* right = _el_parser_parse_logical_and(parser);
        if (!right) return NULL;
        node = el_ast_new_bin_expr(parser->arena, EL_AST_BIN_EXPR_OR, node, right);
    }
    return node;
}

ElAstExprNode* _el_parser_parse_expression(ElParser* parser) {
    return _el_parser_parse_logical_or(parser);
}
