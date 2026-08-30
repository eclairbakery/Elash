#include <elash/ast/tree/expr/literal.h>
#include <elash/ast/tree/expr.h>

#define lit(ARENA, SPAN, ...) \
    EL_DYNARENA_NEW_STRUCT(ARENA, ElAstExpr, { \
       .type = EL_AST_EXPR_LITERAL,            \
       .span = SPAN,                           \
       .as.literal = { __VA_ARGS__ }           \
    })

ElAstExpr* el_ast_new_int_lit(ElDynArena* arena, ElSourceSpan span, ElInt128 value) {
    return lit(arena, span, .kind = EL_AST_LIT_INT, .of.int_ = value);
}

ElAstExpr* el_ast_new_float_lit(ElDynArena* arena, ElSourceSpan span, double value) {
    return lit(arena, span, .kind = EL_AST_LIT_FLOAT, .of.float_ = value);
}

ElAstExpr* el_ast_new_char_lit(ElDynArena* arena, ElSourceSpan span, char value) {
    return lit(arena, span, .kind = EL_AST_LIT_CHAR, .of.char_ = value);
}

ElAstExpr* el_ast_new_str_lit(ElDynArena* arena, ElSourceSpan span, ElStringView value) {
    return lit(arena, span, .kind = EL_AST_LIT_STRING, .of.str_ = value);
}

ElAstExpr* el_ast_new_bool_lit(ElDynArena* arena, ElSourceSpan span, bool value) {
    return lit(arena, span, .kind = EL_AST_LIT_BOOL, .of.bool_ = value);
}

ElAstExpr* el_ast_new_null_lit(ElDynArena* arena, ElSourceSpan span) {
    return lit(arena, span, .kind = EL_AST_LIT_NULL);
}
