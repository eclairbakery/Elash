#pragma once

#include <elash/diag/engine.h>
#include <elash/lexer/token.h>
#include <elash/util/int128.h>

ElInt128 el_parse_int_lit(ElDiagEngine* diag, ElToken tok);
