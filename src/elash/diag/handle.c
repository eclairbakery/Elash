#include <elash/diag/handle.h>

#include <elash/util/assert.h>

void el_diag_handle_lexer_error(ElDiagEngine* engine, const ElLexerErrorDetails* err) {
    switch (err->code) {
    case EL_LEXERR_SUCCESS:
    case _EL_LEXERR_COUNT:
        return;

    case EL_LEXERR_INTERNAL_ERROR:
        return el_diag_report(engine, EL_DIAG_ERROR, "internal", err->span, "Internal lexer error");
    case EL_LEXERR_INVALID_ARG:
        return el_diag_report(engine, EL_DIAG_ERROR, "internal", err->span, "Internal lexer error (invalid args passed)");

    case EL_LEXERR_INVALID_ESCAPE:
        return el_diag_report(
            engine, EL_DIAG_ERROR, "syntax.invalid-escape", err->span,
            "Invalid escape: '${char}'", EL_DIAG_CHAR("char", err->error_details.ch),
        );
    case EL_LEXERR_INVALID_NUMBER:
        return el_diag_report(
            engine, EL_DIAG_ERROR, "syntax.invalid-number", err->span,
            "Invalid number literal: ${lit}", EL_DIAG_STRING("lit", el_source_span_to_sv(err->span)),
        );
    case EL_LEXERR_INVALID_UNICODE:
        return el_diag_report(
            engine, EL_DIAG_ERROR, "syntax.invalid-unicode", err->span,
            "Invalid unicode codepoint",
        );

    case EL_LEXERR_UNEXPECTED_CHAR:
        return el_diag_report(
            engine, EL_DIAG_ERROR, "syntax.unexpected-char", err->span,
            "Unexpected character: ${char}", EL_DIAG_CHAR("char", err->error_details.ch),
        );

    case EL_LEXERR_UNTERM_STRING:
        return el_diag_report(
            engine, EL_DIAG_ERROR, "syntax.unterminated", err->span,
            "Unterminated string literal",
        );
    case EL_LEXERR_UNTERM_CHAR:
        return el_diag_report(
            engine, EL_DIAG_ERROR, "syntax.unterminated", err->span,
            "Unterminated char literal",
        );
    case EL_LEXERR_UNTERM_COMMENT:
        return el_diag_report(
            engine, EL_DIAG_ERROR, "syntax.unterminated", err->span,
            "Unterminated multi-line comment",
        );
    }
    EL_UNREACHABLE_ENUM_VAL(ElLexerErrorCode, err->code);
}

