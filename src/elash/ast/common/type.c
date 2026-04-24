#include <elash/ast/common/type.h>

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

void el_ast_dump_type(ElAstTypeNode* node, FILE* out) {
    switch (node->kind) {
    case EL_AST_TYPE_NAME:
        el_sv_print(node->name->name, out);
        break;
    case EL_AST_TYPE_PTR:
        el_ast_dump_type(node->base, out);
        fputc('*', out);
        break;
    }
}
