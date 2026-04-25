#pragma once

#include <elash/diag/engine.h>

#include <elash/lexer/error.h>

void el_diag_handle_lexer_error(ElDiagEngine* engine, const ElLexerErrorDetails* err);
