#include <el-ast/expr/literal.h>
#include <el-ast/expr.h>

static ElAstExprNode* el_ast_new_literal_wrapper(ElDynArena* arena, ElAstLiteralNode lit) {
    ElAstExprNode* node = EL_DYNARENA_NEW(arena, ElAstExprNode);
    node->type = EL_AST_EXPR_LITERAL;
    node->as.literal = lit;
    return node;
}

ElAstLiteralNode el_ast_int_literal(int64_t value) {
    return (ElAstLiteralNode) {
        .type = EL_AST_LIT_INT,
        .of.int_.value = value,
    };
}

ElAstLiteralNode el_ast_float_literal(long double value) {
    return (ElAstLiteralNode) {
        .type = EL_AST_LIT_FLOAT,
        .of.float_.value = value,
    };
}

ElAstLiteralNode el_ast_char_literal(char value) {
    return (ElAstLiteralNode) {
        .type = EL_AST_LIT_CHAR,
        .of.char_.value = value,
    };
}

ElAstLiteralNode el_ast_string_literal(ElStringView value) {
    return (ElAstLiteralNode) {
        .type = EL_AST_LIT_STRING,
        .of.str_.value = value,
    };
}

ElAstLiteralNode el_ast_bool_literal(bool value) {
    return (ElAstLiteralNode) {
        .type = EL_AST_LIT_BOOL,
        .of.bool_.value = value,
    };
}

ElAstLiteralNode el_ast_null_literal(void) {
    return (ElAstLiteralNode) {
        .type = EL_AST_LIT_NULL,
    };
}

ElAstExprNode* el_ast_new_int_literal(ElDynArena* arena, int64_t value) {
    return el_ast_new_literal_wrapper(arena, el_ast_int_literal(value));
}

ElAstExprNode* el_ast_new_float_literal(ElDynArena* arena, long double value) {
    return el_ast_new_literal_wrapper(arena, el_ast_float_literal(value));
}

ElAstExprNode* el_ast_new_char_literal(ElDynArena* arena, char value) {
    return el_ast_new_literal_wrapper(arena, el_ast_char_literal(value));
}

ElAstExprNode* el_ast_new_string_literal(ElDynArena* arena, ElStringView value) {
    return el_ast_new_literal_wrapper(arena, el_ast_string_literal(value));
}

ElAstExprNode* el_ast_new_bool_literal(ElDynArena* arena, bool value) {
    return el_ast_new_literal_wrapper(arena, el_ast_bool_literal(value));
}

ElAstExprNode* el_ast_new_null_literal(ElDynArena* arena) {
    return el_ast_new_literal_wrapper(arena, el_ast_null_literal());
}
