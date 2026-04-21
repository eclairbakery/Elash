#include <el-ast/expr/bin.h>
#include <el-ast/expr.h>

#include <el-util/assert.h>

ElStringView el_ast_bin_op_to_string(ElAstBinExprType type) {
    switch (type) {
    case EL_AST_BIN_EXPR_ADD: return EL_SV("+");
    case EL_AST_BIN_EXPR_SUB: return EL_SV("-");
    case EL_AST_BIN_EXPR_MUL: return EL_SV("*");
    case EL_AST_BIN_EXPR_DIV: return EL_SV("/");
    case EL_AST_BIN_EXPR_MOD: return EL_SV("%");

    case EL_AST_BIN_EXPR_EQ : return EL_SV("==");
    case EL_AST_BIN_EXPR_NEQ: return EL_SV("!=");
    case EL_AST_BIN_EXPR_LT : return EL_SV("<");
    case EL_AST_BIN_EXPR_LTE: return EL_SV("<=");
    case EL_AST_BIN_EXPR_GT : return EL_SV(">");
    case EL_AST_BIN_EXPR_GTE: return EL_SV(">=");
    
    case EL_AST_BIN_EXPR_AND: return EL_SV("&&");
    case EL_AST_BIN_EXPR_OR : return EL_SV("||");
    
    case EL_AST_BIN_EXPR_BW_AND: return EL_SV("&");
    case EL_AST_BIN_EXPR_BW_OR:  return EL_SV("|");
    case EL_AST_BIN_EXPR_BW_XOR: return EL_SV("^");
    case EL_AST_BIN_EXPR_SHL:    return EL_SV("<<");
    case EL_AST_BIN_EXPR_SHR:    return EL_SV(">>");
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstBinExprType, type);
}

ElAstBinExprNode el_ast_bin_expr(ElAstBinExprType type, ElAstExprNode* left, ElAstExprNode* right) {
    return (ElAstBinExprNode) {
        .left = left,
        .type = type,
        .right = right,
    };
}

ElAstExprNode* el_ast_new_bin_expr(ElDynArena* arena, ElAstBinExprType type, ElAstExprNode* left, ElAstExprNode* right) {
    ElAstExprNode* node = EL_DYNARENA_NEW(arena, ElAstExprNode);
    node->type = EL_AST_EXPR_BINARY;
    node->as.binary = el_ast_bin_expr(type, left, right);
    return node;
}
