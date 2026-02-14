#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include "token.h"

static const char *keywords[] = {
    "if", "else", "while", "return",
    "int", "float", "void",
    NULL
};

static int is_keyword(const char *str) {
    for (int i = 0; keywords[i]; i++) {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

void lexer_init(Lexer *lexer, const char *input) {
    lexer->input = input;
    lexer->pos = 0;
    lexer->current = input[0];
    lexer->line = 1;
    lexer->column = 1;
}

static void advance(Lexer *lexer) {
    if (lexer->current == '\n') {
        lexer->line++;
        lexer->column = 0;
    }

    lexer->pos++;
    lexer->current = lexer->input[lexer->pos];
    lexer->column++;
}

static char peek(Lexer *lexer) {
    return lexer->input[lexer->pos + 1];
}

static void skip_whitespace(Lexer *lexer) {
    while (isspace(lexer->current))
        advance(lexer);
}

static void skip_comment(Lexer *lexer) {
    if (peek(lexer) == '/') {
        while (lexer->current != '\n' && lexer->current != '\0')
            advance(lexer);
    } else if (peek(lexer) == '*') {
        advance(lexer);
        advance(lexer);
        while (!(lexer->current == '*' && peek(lexer) == '/') &&
               lexer->current != '\0') {
            advance(lexer);
        }
        advance(lexer);
        advance(lexer);
    }
}

static Token make_token(Lexer *lexer, TokenType type, const char *value) {
    Token t;
    t.type = type;
    strncpy(t.value, value, sizeof(t.value) - 1);
    t.value[sizeof(t.value) - 1] = '\0';
    t.line = lexer->line;
    t.column = lexer->column;
    return t;
}

static Token identifier(Lexer *lexer) {
    char buffer[256];
    int i = 0;

    while (isalnum(lexer->current) || lexer->current == '_') {
        buffer[i++] = lexer->current;
        advance(lexer);
    }

    buffer[i] = '\0';

    if (is_keyword(buffer))
        return make_token(lexer, TOKEN_KEYWORD, buffer);

    return make_token(lexer, TOKEN_IDENTIFIER, buffer);
}

static Token number(Lexer *lexer) {
    char buffer[256];
    int i = 0;
    int is_float = 0;

    while (isdigit(lexer->current) || lexer->current == '.') {
        if (lexer->current == '.')
            is_float = 1;

        buffer[i++] = lexer->current;
        advance(lexer);
    }

    buffer[i] = '\0';

    if (is_float)
        return make_token(lexer, TOKEN_FLOAT, buffer);

    return make_token(lexer, TOKEN_NUMBER, buffer);
}

static Token string(Lexer *lexer) {
    char buffer[256];
    int i = 0;

    advance(lexer);

    while (lexer->current != '"' && lexer->current != '\0') {
        buffer[i++] = lexer->current;
        advance(lexer);
    }

    advance(lexer);
    buffer[i] = '\0';

    return make_token(lexer, TOKEN_STRING, buffer);
}

Token next_token(Lexer *lexer) {

    while (lexer->current != '\0') {

        if (isspace(lexer->current)) {
            skip_whitespace(lexer);
            continue;
        }

        if (lexer->current == '/' &&
            (peek(lexer) == '/' || peek(lexer) == '*')) {
            skip_comment(lexer);
            continue;
        }

        if (isalpha(lexer->current) || lexer->current == '_')
            return identifier(lexer);

        if (isdigit(lexer->current))
            return number(lexer);

        if (lexer->current == '"')
            return string(lexer);

        switch (lexer->current) {

            case '+': advance(lexer); return make_token(lexer, TOKEN_PLUS, "+");
            case '-': advance(lexer); return make_token(lexer, TOKEN_MINUS, "-");
            case '*': advance(lexer); return make_token(lexer, TOKEN_STAR, "*");
            case '%': advance(lexer); return make_token(lexer, TOKEN_PERCENT, "%");

            case '=':
                advance(lexer);
                if (lexer->current == '=') {
                    advance(lexer);
                    return make_token(lexer, TOKEN_EQUAL, "==");
                }
                return make_token(lexer, TOKEN_ASSIGN, "=");

            case '!':
                advance(lexer);
                if (lexer->current == '=') {
                    advance(lexer);
                    return make_token(lexer, TOKEN_NOT_EQUAL, "!=");
                }
                return make_token(lexer, TOKEN_NOT, "!");

            case '<':
                advance(lexer);
                if (lexer->current == '=') {
                    advance(lexer);
                    return make_token(lexer, TOKEN_LTE, "<=");
                }
                return make_token(lexer, TOKEN_LT, "<");

            case '>':
                advance(lexer);
                if (lexer->current == '=') {
                    advance(lexer);
                    return make_token(lexer, TOKEN_GTE, ">=");
                }
                return make_token(lexer, TOKEN_GT, ">");

            case '&':
                advance(lexer);
                if (lexer->current == '&') {
                    advance(lexer);
                    return make_token(lexer, TOKEN_AND, "&&");
                }
                break;

            case '|':
                advance(lexer);
                if (lexer->current == '|') {
                    advance(lexer);
                    return make_token(lexer, TOKEN_OR, "||");
                }
                break;

            case '(':
                advance(lexer);
                return make_token(lexer, TOKEN_LPAREN, "(");

            case ')':
                advance(lexer);
                return make_token(lexer, TOKEN_RPAREN, ")");

            case '{':
                advance(lexer);
                return make_token(lexer, TOKEN_LBRACE, "{");

            case '}':
                advance(lexer);
                return make_token(lexer, TOKEN_RBRACE, "}");

            case ';':
                advance(lexer);
                return make_token(lexer, TOKEN_SEMICOLON, ";");

            case ',':
                advance(lexer);
                return make_token(lexer, TOKEN_COMMA, ",");
        }

        char unknown[2] = { lexer->current, '\0' };
        advance(lexer);
        return make_token(lexer, TOKEN_UNKNOWN, unknown);
    }

    return make_token(lexer, TOKEN_EOF, "");
}

