#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>
#include <elash/defs/sv.h>

#include <elash/util/int128.h>

typedef struct ElAstExpr ElAstExpr;

typedef enum ElAstLiteralKind {
    EL_AST_LIT_INT,
    EL_AST_LIT_FLOAT,
    EL_AST_LIT_CHAR,
    EL_AST_LIT_STRING,
    EL_AST_LIT_BOOL,
    EL_AST_LIT_NULL,
} ElAstLiteralKind;

typedef struct ElAstLiteral {
    ElAstLiteralKind kind;
    union {
        ElInt128     int_;
        double       float_;
        bool         bool_;
        char         char_;
        ElStringView str_;
    } of;
} ElAstLiteral;

ElAstExpr* el_ast_new_int_lit(ElDynArena* arena, ElSourceSpan span, ElInt128 value);
ElAstExpr* el_ast_new_float_lit(ElDynArena* arena, ElSourceSpan span, double value);
ElAstExpr* el_ast_new_char_lit(ElDynArena* arena, ElSourceSpan span, char value);
ElAstExpr* el_ast_new_str_lit(ElDynArena* arena, ElSourceSpan span, ElStringView value);
ElAstExpr* el_ast_new_bool_lit(ElDynArena* arena, ElSourceSpan span, bool value);
ElAstExpr* el_ast_new_null_lit(ElDynArena* arena, ElSourceSpan span);
