#pragma once

#include <elash/diag/engine.h>
#include <elash/defs/sv.h>

ElStringView el_parse_str_with_escapes(ElDiagEngine* diag, ElToken tok, char buf[static tok.lexeme.len]);
char el_parse_char_with_escapes(ElDiagEngine* diag, ElToken tok, char buf[static tok.lexeme.len], bool* ok);

