#include <el-ast/common/ident.h>
#include <el-ast/expr.h>

ElAstIdentNode el_ast_ident_node(ElStringView name) {
    return (ElAstIdentNode) {
        .name = name,
    };
}

ElAstExprNode* el_ast_new_ident_node(ElDynArena* arena, ElStringView name) {
    ElAstExprNode* node = EL_DYNARENA_NEW(arena, ElAstExprNode);
    node->type = EL_AST_EXPR_IDENT;
    node->as.ident = el_ast_ident_node(name);
    return node;
}
