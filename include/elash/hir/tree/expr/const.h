#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>
#include <elash/util/int128.h>

typedef struct ElHirExpr ElHirExpr;
typedef struct ElHirType ElHirType;

ElHirExpr* el_hir_new_int_constant(ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElInt128 value);
ElHirExpr* el_hir_new_char_constant(ElDynArena* arena, ElSourceSpan span, ElHirType* type, char value);
ElHirExpr* el_hir_new_bool_constant(ElDynArena* arena, ElSourceSpan span, ElHirType* type, bool value);
ElHirExpr* el_hir_new_float_constant(ElDynArena* arena, ElSourceSpan span, ElHirType* type, double value);
