#pragma once

#include <stddef.h>

#include "token.h"

typedef struct {
    const char *input;
    int pos;
    char current;
    int line;
    int column;
} Lexer;

void lexer_init(Lexer *lexer, const char *input);
Token next_token(Lexer *lexer);

