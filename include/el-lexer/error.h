#pragma once

#include <el-defs/source-loc.h>
#include <el-defs/sv.h>

#include <stddef.h> // For usize
#include <stdio.h>

/// @brief Enumeration of possible error codes returned by the Elash lexer.
///
/// These codes indicate specific issues encountered during the lexical analysis
/// phase of parsing Elash source code.
typedef enum ElLexerErrorCode {
    EL_LEXERR_SUCCESS = 0, ///< Indicates that no error occurred.

    /// @brief Unknown or invalid character encountered in input.
    /// The unexpected character is stored in @ref ElLexerErrorDetails.error_details.ch.
    EL_LEXERR_UNEXPECTED_CHAR,

    /// @brief Invalid escape sequence in string or character literal.
    /// The unexpected escape sequence (e.g., the character following '\')
    /// is stored in @ref ElLexerErrorDetails.error_details.ch.
    EL_LEXERR_INVALID_ESCAPE,

    /// @brief Unterminated block comment.
    /// This error occurs when a block comment (e.g., `/* ... `) is not closed.
    /// @warning This error is not reported when the lexer is configured
    ///          with the @ref EL_LF_ALLOW_UNTERM flag (if such a flag exists).
    EL_LEXERR_UNTERM_COMMENT,

    /// @brief Unterminated string literal.
    /// This error occurs when a string literal (e.g., `"abc`) is not closed.
    /// @warning This error is not reported when the lexer is configured
    ///          with the @ref EL_LF_ALLOW_UNTERM flag (if such a flag exists).
    EL_LEXERR_UNTERM_STRING,

    /// @brief Unterminated character literal.
    /// This error occurs when a character literal (e.g., `'a`) is not closed.
    /// @warning This error is not reported when the lexer is configured
    ///          with the @ref EL_LF_ALLOW_UNTERM flag (if such a flag exists).
    EL_LEXERR_UNTERM_CHAR,

    /// @brief Invalid number format.
    /// This error occurs when a numeric literal (integer or float) is malformed.
    EL_LEXERR_INVALID_NUMBER,

    /// @brief Invalid Unicode escape sequence.
    /// This error occurs when a Unicode escape sequence (e.g., `\uXXXX`) is malformed
    /// or represents an invalid Unicode code point.
    EL_LEXERR_INVALID_UNICODE,

    /// @brief An unexpected internal error occurred within the lexer.
    /// This typically indicates a bug in the lexer's implementation.
    EL_LEXERR_INTERNAL_ERROR,

    /// @brief An invalid argument was provided to a lexer function.
    /// This error is usually reported by lexer utility functions when
    /// their input parameters are incorrect.
    EL_LEXERR_INVALID_ARG,

    EL_LEXERR_UNTERM_ANGLE_HEADER, // Unterminated angle bracket header in preprocessor context
    _EL_LEXERR_COUNT, // Keep this last to count the number of error codes.
} ElLexerErrorCode;

/// @brief Converts an ElLexerErrorCode to its human-readable string representation.
///
/// @param code The error code to convert.
/// @return A constant string describing the error code, or EL_SV_NULL if the code is out of range.
ElStringView el_lexer_err_code_to_string(ElLexerErrorCode code);

/// @brief Structure containing detailed information about a lexer error.
typedef struct ElLexerErrorDetails {
    ElLexerErrorCode code; ///< The specific error code.

    /// @brief Source location where the error occurred.
    /// This field is undefined when `code` is @ref EL_LEXERR_SUCCESS.
    ElSourceLocation error_location;

    /// @brief Additional details about the error.
    /// The interpretation of this union depends on the `code` field.
    union {
        char ch; ///< Relevant character for errors like @ref EL_LEXERR_UNEXPECTED_CHAR, @ref EL_LEXERR_INVALID_ESCAPE.
        void* other; ///< Placeholder for future, more complex error details.
    } error_details;
} ElLexerErrorDetails;

/// @brief Macro to represent a successful lexer result.
/// This is a convenience for initializing @ref ElLexerErrorDetails when no error has occurred.
#define EL_LEXER_RESULT_SUCCESS ((ElLexerErrorDetails) { .code = EL_LEXERR_SUCCESS })

/// @brief Formats lexer error details into a fixed-size character buffer.
/// This function attempts to write a human-readable description of the lexer error
/// into the provided buffer. If the description is too long for the buffer, it will be truncated.
///
/// @param r The lexer error details to format.
/// @param n The size of the output buffer.
/// @param buf The character buffer to write the formatted error into.
/// @return The number of characters that would have been written if `buf` was large enough,
///         not including the null terminator.
usize el_lexer_result_format(ElLexerErrorDetails r, usize n, char buf[static n]);

/// @brief Allocates memory and converts lexer error details into a dynamically allocated string.
/// The caller is responsible for freeing the returned string using `free()`.
///
/// @param r The lexer error details to convert.
/// @param out A pointer to a char pointer, which will be updated to point to the newly allocated string.
/// @return The length of the formatted string, not including the null terminator. Returns 0 on allocation failure.
usize el_lexer_result_to_string(ElLexerErrorDetails r, char** out);

/// @brief Prints lexer error details to a specified file stream.
/// This function writes a human-readable description of the lexer error to the given `FILE` stream.
///
/// @param r The lexer error details to print.
/// @param out The file stream to print to (e.g., `stdout`, `stderr`).
/// @return The number of characters printed.
usize el_lexer_result_print(ElLexerErrorDetails r, FILE* out);
