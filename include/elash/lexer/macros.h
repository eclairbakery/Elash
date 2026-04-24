#pragma once

#define EL_LEXER_SET_ERROR(LEXER, CODE, SPAN, DETAILS_INIT)    \
    do {                                                       \
        (LEXER)->last_err_details = (ElLexerErrorDetails) {    \
            .code = (CODE),                                    \
            .span = (SPAN),                                    \
            .error_details = DETAILS_INIT,                     \
        };                                                     \
    } while (0)

#define EL_LEXER_RETURN_ERROR(lexer, code, span, detail_ch)    \
    do {                                                       \
        EL_LEXER_SET_ERROR(lexer, code, span, detail_ch);      \
        return (code);                                         \
    } while (0)
