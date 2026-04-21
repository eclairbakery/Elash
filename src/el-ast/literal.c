#include <el-ast/expr/literal.h>
#include <el-ast/expr.h>

static ElAstExprNode* el_ast_new_literal(ElDynArena* arena, ElAstLiteralType type) {
    ElAstExprNode* node = EL_DYNARENA_NEW(arena, ElAstExprNode);
    node->type = EL_AST_EXPR_LITERAL;
    node->as.literal.type = type;
    return node;
}

ElAstExprNode* el_ast_new_int_literal(ElDynArena* arena, int64_t value) {
    ElAstExprNode* node = el_ast_new_literal(arena, EL_AST_LIT_INT);
    node->as.literal.of.int_.value = value;
    return node;
}

ElAstExprNode* el_ast_new_float_literal(ElDynArena* arena, long double value) {
    ElAstExprNode* node = el_ast_new_literal(arena, EL_AST_LIT_FLOAT);
    node->as.literal.of.float_.value = value;
    return node;
}

ElAstExprNode* el_ast_new_char_literal(ElDynArena* arena, char value) {
    ElAstExprNode* node = el_ast_new_literal(arena, EL_AST_LIT_CHAR);
    node->as.literal.of.char_.value = value;
    return node;
}

ElAstExprNode* el_ast_new_string_literal(ElDynArena* arena, ElStringView value) {
    ElAstExprNode* node = el_ast_new_literal(arena, EL_AST_LIT_STRING);
    node->as.literal.of.str_.value = value;
    return node;
}

ElAstExprNode* el_ast_new_bool_literal(ElDynArena* arena, bool value) {
    ElAstExprNode* node = el_ast_new_literal(arena, EL_AST_LIT_BOOL);
    node->as.literal.of.bool_.value = value;
    return node;
}

ElAstExprNode* el_ast_new_null_literal(ElDynArena* arena) {
    return el_ast_new_literal(arena, EL_AST_LIT_NULL);
}
