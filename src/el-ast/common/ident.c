#include <el-ast/common/ident.h>
#include <el-ast/expr.h>

ElAstIdentNode el_ast_ident_node(ElStringView name) {
    return (ElAstIdentNode) {
        .name = name,
    };
}

ElAstIdentNode* el_ast_new_ident_node_raw(ElDynArena* arena, ElStringView name) {
    ElAstIdentNode* node = EL_DYNARENA_NEW(arena, ElAstIdentNode);
    *node = el_ast_ident_node(name);
    return node;
}

ElAstExprNode* el_ast_new_ident_node(ElDynArena* arena, ElStringView name) {
    ElAstExprNode* node = EL_DYNARENA_NEW(arena, ElAstExprNode);
    node->type = EL_AST_EXPR_IDENT;
    node->as.ident = el_ast_ident_node(name);
    return node;
}
