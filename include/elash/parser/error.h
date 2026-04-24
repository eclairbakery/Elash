#pragma once

#include <elash/lexer/token.h>

typedef enum ElParserErrorCode {
    EL_PARSER_ERR_OK = 0,
    EL_PARSER_ERR_UNEXPECTED_TOKEN,
    EL_PARSER_ERR_EXPECTED_TOKEN,
    EL_PARSER_ERR_UNEXPECTED_EOF,
    EL_PARSER_ERR_PP_ERROR,
} ElParserErrorCode;

typedef struct ElParserErrorDetails {
    ElParserErrorCode code;
    union {
        ElToken token;
        ElTokenType expected;
    };
} ElParserErrorDetails;
