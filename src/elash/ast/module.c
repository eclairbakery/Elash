#include <elash/ast/module.h>
#include <elash/ast/expr.h>

#include <stdio.h>

ElAstModuleNode el_ast_module(ElSourceSpan span) {
    return (ElAstModuleNode) {
        .span = span,
        .head = NULL,
        .tail = NULL,
        .count = 0,
    };
}

ElAstModuleNode* el_ast_new_module(ElDynArena* arena, ElSourceSpan span) {
    ElAstModuleNode* node = EL_DYNARENA_NEW_ZEROED(arena, ElAstModuleNode);
    node->span = span;
    return node;
}

void el_ast_module_append(ElAstModuleNode* module, ElAstTopLevelNode* node) {
    if (module->head == NULL) {
        module->head = node;
        module->tail = node;
    } else {
        module->tail->next = node;
        module->tail = node;
    }
    module->count++;
}

void el_ast_dump_module(ElAstModuleNode* mod, usize indent, FILE* out) {
    el_ast_dump_print_indent(indent, out);
    fprintf(out, "Module(count=%zu):\n", mod->count);
    for (ElAstTopLevelNode* node = mod->head; node != NULL; node = node->next) {
        el_ast_dump_toplevel(node, indent + 1, out);
    }
}
