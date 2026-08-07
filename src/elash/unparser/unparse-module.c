#include <elash/unparser/unparser.h>

bool el_unparser_unparse_module(ElUnparser* unpar, ElAstModule* module) {
    for (ElAstDecl* decl = module->head; decl != NULL; decl = decl->next) {
        if (!el_unparser_unparse_decl(unpar, decl)) return false;
    }
    return el_unparser_push_punct(unpar, EL_TT_EOF);
}
