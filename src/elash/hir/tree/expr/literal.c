#include <elash/hir/tree/expr/literal.h>
#include <elash/hir/tree/expr.h>

#include <elash/util/assert.h>

ElHirExpr* el_hir_new_int_lit(ElDynArena* arena, ElSourceSpan span, int64_t value) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirExpr, {
        .kind = EL_HIR_EXPR_LITERAL,
        .type = NULL,
        .span = span,
        .as.literal = {
            .kind = EL_HIR_LITERAL_INT,
            .of.int_ = value,
        },
    });
}

ElHirExpr* el_hir_new_char_lit(ElDynArena* arena, ElSourceSpan span, char value) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirExpr, {
        .kind = EL_HIR_EXPR_LITERAL,
        .type = NULL,
        .span = span,
        .as.literal = {
            .kind = EL_HIR_LITERAL_CHAR,
            .of.char_ = value,
        },
    });
}

ElHirExpr* el_hir_new_bool_lit(ElDynArena* arena, ElSourceSpan span, bool value) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirExpr, {
        .kind = EL_HIR_EXPR_LITERAL,
        .type = NULL,
        .span = span,
        .as.literal = {
            .kind = EL_HIR_LITERAL_BOOL,
            .of.bool_ = value,
        },
    });
}

ElHirExpr* el_hir_new_float_lit(ElDynArena* arena, ElSourceSpan span, double value) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirExpr, {
        .kind = EL_HIR_EXPR_LITERAL,
        .type = NULL,
        .span = span,
        .as.literal = {
            .kind = EL_HIR_LITERAL_FLOAT,
            .of.float_ = value,
        },
    });
}


ElHirExpr* el_hir_new_str_lit(ElDynArena* arena, ElSourceSpan span, ElStringView value) {
    return EL_DYNARENA_NEW_STRUCT(arena, ElHirExpr, {
        .kind = EL_HIR_EXPR_LITERAL,
        .type = NULL,
        .span = span,
        .as.literal = {
            .kind = EL_HIR_LITERAL_STRING,
            .of.str_ = value,
        },
    });
}

ElStringView el_hir_literal_kind_to_string(ElHirLiteralKind lit) {
    switch (lit) {
    case EL_HIR_LITERAL_INT:    return EL_SV("integer");
    case EL_HIR_LITERAL_CHAR:   return EL_SV("character");
    case EL_HIR_LITERAL_BOOL:   return EL_SV("boolean");
    case EL_HIR_LITERAL_FLOAT:  return EL_SV("float");
    case EL_HIR_LITERAL_STRING: return EL_SV("string");
    }

    EL_UNREACHABLE_ENUM_VAL(ElHirLiteralKind, lit);
}
