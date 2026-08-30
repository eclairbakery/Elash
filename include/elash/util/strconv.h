#pragma once

#include <elash/defs/sv.h>
#include <elash/diag/engine.h>

#include <elash/util/int128.h>

ElUint128 el_string_to_u128(ElDiagEngine* engine, ElStringView str, uint base, ElSourceSpan span);
ElInt128 el_string_to_i128(ElDiagEngine* engine, ElStringView str, uint base, ElSourceSpan span);

double el_string_to_double(ElDiagEngine* engine, ElStringView str, ElSourceSpan span);
