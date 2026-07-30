#pragma once

#include <elash/util/dynarena.h>
#include <elash/srcdoc/span.h>
#include <elash/defs/int-types.h>

typedef struct ElHirExpr ElHirExpr;
typedef struct ElHirType ElHirType;

typedef enum ElHirLiteralKind {
    EL_HIR_LITERAL_INT,
    EL_HIR_LITERAL_CHAR,
    EL_HIR_LITERAL_BOOL,
    EL_HIR_LITERAL_FLOAT,
} ElHirLiteralKind;

typedef struct ElHirLiteral {
    ElHirLiteralKind kind;
    union {
        int64_t int_; // TODO: use bigints or something like llvm's APInt
        char    char_;
        bool    bool_;
        double float_; // TODO: use arbitrary precision floats
    } of;
} ElHirLiteral;

ElHirExpr* el_hir_new_int_lit(ElDynArena* arena, ElSourceSpan span, int64_t value);
ElHirExpr* el_hir_new_char_lit(ElDynArena* arena, ElSourceSpan span, char value);
ElHirExpr* el_hir_new_bool_lit(ElDynArena* arena, ElSourceSpan span, bool value);
ElHirExpr* el_hir_new_float_lit(ElDynArena* arena, ElSourceSpan span, double value);

ElStringView el_hir_literal_kind_to_string(ElHirLiteralKind lit);
