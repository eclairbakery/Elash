#pragma once

#define EL_LEXER_SET_ERROR(LEXER, CODE, LOC, DETAILS_INIT)     \
    do {                                                       \
        (LEXER)->last_err_details = (ElLexerErrorDetails) {    \
            .code = (CODE),                                    \
            .error_location = (LOC),                           \
            .error_details = DETAILS_INIT,                     \
        };                                                     \
    } while (0)

#define EL_LEXER_RETURN_ERROR(lexer, code, loc, detail_ch)     \
    do {                                                       \
        EL_LEXER_SET_ERROR(lexer, code, loc, detail_ch);       \
        return (code);                                         \
    } while (0)
