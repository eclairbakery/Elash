#pragma once

#include <stddef.h>

#include <elash/defs/sv.h>

#include <elash/lexer/error.h>
#include <elash/lexer/token.h>
#include <elash/lexer/tokstream.h>

typedef struct ElProfState ElProfState;
typedef struct ElProfStage ElProfStage;

/// Behavior flags for controlling tokenization behavior
/// @memberof ElLexer
typedef enum ElLexerFlags {
    EL_LF_NONE              = 0,
    EL_LF_ALLOW_UNTERM      = 1 << 0, ///< Treat unterminated strings/chars at EOF as valid tokens.
    EL_LF_ALLOW_UTF8_IDENTS = 1 << 1, ///< Allow non-ASCII UTF-8 characters in identifiers.
    EL_LF_SKIP_COMMENTS     = 1 << 2, ///< Silently discard comments.
    EL_LF_SKIP_WHITESPACE   = 1 << 3, ///< Silently discard whitespace.
    EL_LF_SKIP_UNKNOWN      = 1 << 4, ///< Skip unrecognized bytes instead of failing.
} ElLexerFlags;

/// Default lexer flags used by the `elc` compiler to compile Elash source code.
///
/// @warning These flags assume a preprocessor will be used. If not,
///          consider combining them with @ref EL_LF_SKIP_COMMENTS and
///          @ref EL_LF_SKIP_WHITESPACE, as the parser doesn't currently
///          handle whitespaces and comments.
///
/// @relates ElLexer
#define EL_LEXER_FLAGS_DEFAULT (EL_LF_ALLOW_UTF8_IDENTS)

/// State container for pull-based streaming tokenization for the Elash programming language.
///
/// Processes source text on demand in a single pass without allocating intermediate
/// token buffer arrays. Exposes token-at-a-time iteration and wraps directly into
/// generic @ref ElTokenStream pipelines.
///
/// @see ElToken, ElTokenStream, ElSourceDocument
typedef struct ElLexer {
    const ElSourceDocument* doc;
    ElSourceLocation current_loc;
    ElSourceLocation token_start_loc;

    ElProfState* prof;
    ElProfStage* prof_stage;

    ElLexerResult last_err_details;
    ElLexerFlags flags;
} ElLexer;

/// Initializes lexer with a source document and onfiguration flags
/// @memberof ElLexer
ElLexerStatus el_lexer_init(ElLexer* lexer, const ElSourceDocument* doc, ElLexerFlags flags);
ElLexerStatus el_lexer_init_prof(
    ElLexer* lexer, const ElSourceDocument* doc, ElLexerFlags flags,
    ElProfState* prof, ElProfStage* prof_stage
);

/// Rewinds lexer position to the beginning of the active document
/// @memberof ElLexer
ElLexerStatus el_lexer_reset(ElLexer* lexer);

/// Replaces the source document and resets lexing state
/// @memberof ElLexer
ElLexerStatus el_lexer_set_document(ElLexer* lexer, const ElSourceDocument* doc);

/// Scans and returns the next token
/// @memberof ElLexer
ElLexerStatus el_lexer_next_token(ElLexer* lexer, ElToken* out_tok);

/// Wraps lexer into @ref ElTokenStream interface
/// @memberof ElLexer
ElTokenStream el_lexer_as_token_stream(ElLexer* lexer);
