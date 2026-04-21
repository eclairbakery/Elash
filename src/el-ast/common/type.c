#include <el-ast/common/type.h>

ElAstTypeNode el_ast_type_name(ElAstIdentNode* name) {
    return (ElAstTypeNode) {
        .kind = EL_AST_TYPE_NAME,
        .name = name,
    };
}

ElAstTypeNode el_ast_type_ptr(ElAstTypeNode* base) {
    return (ElAstTypeNode) {
        .kind = EL_AST_TYPE_PTR,
        .base = base,
    };
}

ElAstTypeNode* el_ast_new_type_name(ElDynArena* arena, ElAstIdentNode* name) {
    ElAstTypeNode* node = EL_DYNARENA_NEW(arena, ElAstTypeNode);
    *node = el_ast_type_name(name);
    return node;
}

ElAstTypeNode* el_ast_new_type_ptr(ElDynArena* arena, ElAstTypeNode* base) {
    ElAstTypeNode* node = EL_DYNARENA_NEW(arena, ElAstTypeNode);
    *node = el_ast_type_ptr(base);
    return node;
}
