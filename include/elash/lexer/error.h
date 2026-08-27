#pragma once

#include <elash/source/span.h>
#include <elash/defs/sv.h>

#include <stddef.h> // For usize
#include <stdio.h>

/// Enumeration of possible error codes returned by the Elash lexer
/// @relates ElLexer
typedef enum ElLexerStatus {
    EL_LEXERR_SUCCESS,         ///< Indicates that no error occurred.

    EL_LEXERR_UNEXPECTED_CHAR, ///< Unknown or invalid character encountered in input
    EL_LEXERR_INVALID_ESCAPE,  ///< Invalid escape sequence in string or character literal
    EL_LEXERR_UNTERM_COMMENT,  ///< Unterminated block comment
    EL_LEXERR_UNTERM_STRING,   ///< Unterminated string literal
    EL_LEXERR_UNTERM_CHAR,     ///< Unterminated character literal
    EL_LEXERR_INVALID_NUMBER,  ///< Invalid number format
    EL_LEXERR_INVALID_UNICODE, ///< Invalid Unicode escape sequence
    EL_LEXERR_INTERNAL_ERROR,  ///< An unexpected internal error occurred within the lexer
    EL_LEXERR_INVALID_ARG,     ///< An invalid argument was provided to a lexer function

    _EL_LEXERR_COUNT,
} ElLexerStatus;

/// Converts an ElLexerStatus to its human-readable string representation.
/// @memberof ElLexerStatus @relates ElLexer
ElStringView el_lexer_err_code_to_string(ElLexerStatus code);

/// Structure containing detailed information about a lexer error.
/// @relates ElLexer @see ElLexerStatus
typedef struct ElLexerResult {
    ElLexerStatus code; ///< The specific error code.
    ElSourceSpan span;  ///< Source span where the error occurred

    /// Additional details about the error.
    union {
        char ch;     ///< Relevant character for errors like @ref EL_LEXERR_UNEXPECTED_CHAR, @ref EL_LEXERR_INVALID_ESCAPE.
        void* other; ///< Placeholder for future, more complex error details.
    } error_details;
} ElLexerResult;

/// Macro to represent a successful lexer result.
/// This is a convenience for initializing @ref ElLexerResult when no error has occurred.
/// @memberof ElLexerResult @relates ElLexer
#define EL_LEXER_RESULT_SUCCESS ((ElLexerResult) { .code = EL_LEXERR_SUCCESS })

/// Formats lexer error details into a fixed-size character buffer.
/// This function attempts to write a human-readable description of the lexer error
/// into the provided buffer. If the description is too long for the buffer, it will be truncated.
/// @memberof ElLexerResult @relates ElLexer
usize el_lexer_result_format(ElLexerResult r, usize n, char buf[static n]);

/// Allocates memory and converts lexer error details into a dynamically allocated string.
/// The caller is responsible for freeing the returned string using `free()`.
/// @memberof ElLexerResult @relates ElLexer
usize el_lexer_result_to_string(ElLexerResult r, char** out);

/// Prints lexer error details to a specified file stream.
/// This function writes a human-readable description of the lexer error to the given `FILE` stream.
/// @memberof ElLexerResult @relates ElLexer
usize el_lexer_result_print(ElLexerResult r, FILE* out);
