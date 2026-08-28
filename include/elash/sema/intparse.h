#pragma once

#include <elash/diag/engine.h>
#include <elash/lexer/token.h>
#include <stdint.h>

int64_t el_parse_int_lit(ElDiagEngine* diag, ElToken tok);
