#pragma once

#include <elash/defs/sv.h>
#include <elash/diag/engine.h>

uint64_t el_string_to_u64(ElDiagEngine* engine, ElStringView str, uint base, ElSourceSpan span);
int64_t el_string_to_i64(ElDiagEngine* engine, ElStringView str, uint base, ElSourceSpan span);

long double el_string_to_long_double(ElDiagEngine* engine, ElStringView str, ElSourceSpan span);
