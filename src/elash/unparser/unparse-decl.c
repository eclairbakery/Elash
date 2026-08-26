#include <elash/unparser/unparser.h>

#include <elash/util/assert.h>

static bool unparse_var_def(ElUnparser* unpar, ElAstDecl* decl) {
    ElAstVarDef* def = &decl->as.var_def;
    if (def->is_static) {
        if (!el_unparser_push_kw(unpar, EL_TT_KW_STATIC)) return false;
    }

    if (!el_unparser_unparse_type(unpar, def->type))   return false;
    if (!_el_unparser_unparse_ident(unpar, def->name)) return false;
    if (def->init != NULL) {
        if (!el_unparser_push_punct(unpar, EL_TT_ASSIGN)) return false;
        if (!el_unparser_unparse_init(unpar, def->init))  return false;
    }
    return el_unparser_push_punct(unpar, EL_TT_SEMICOLON);
}

static bool unparse_var_decl(ElUnparser* unpar, ElAstDecl* decl) {
    ElAstVarDecl* vd = &decl->as.var_decl;
    if (!el_unparser_push_kw(unpar, EL_TT_KW_EXTERN)) return false;
    if (!el_unparser_unparse_type(unpar, vd->type))   return false;
    if (!_el_unparser_unparse_ident(unpar, vd->name)) return false;
    return el_unparser_push_punct(unpar, EL_TT_SEMICOLON);
}

static bool unparse_func_def(ElUnparser* unpar, ElAstDecl* decl) {
    if (!_el_unparser_unparse_func_sig(unpar, &decl->as.func_def.sig)) return false;
    return _el_unparser_unparse_block(unpar, decl->as.func_def.block);
}

static bool unparse_func_decl(ElUnparser* unpar, ElAstDecl* decl) {
    if (!_el_unparser_unparse_func_sig(unpar, &decl->as.func_decl.sig)) return false;
    return el_unparser_push_punct(unpar, EL_TT_SEMICOLON);
}

static bool unparse_alias(ElUnparser* unpar, ElAstDecl* decl) {
    if (!el_unparser_push_kw(unpar, EL_TT_KW_ALIAS))             return false;
    if (!el_unparser_push_ident(unpar, decl->as.alias.name))     return false;
    if (!el_unparser_push_punct(unpar, EL_TT_ASSIGN))            return false;
    if (!el_unparser_unparse_toe(unpar, &decl->as.alias.target)) return false;
    return el_unparser_push_punct(unpar, EL_TT_SEMICOLON);
}

static bool unparse_typedef(ElUnparser* unpar, ElAstDecl* decl) {
    if (!el_unparser_push_kw(unpar, EL_TT_KW_TYPEDEF))          return false;
    if (!el_unparser_push_ident(unpar, decl->as.typedef_.name)) return false;
    if (decl->as.typedef_.target != NULL) {
        if (!el_unparser_push_kw(unpar, EL_TT_KW_AS))                   return false;
        if (!el_unparser_unparse_type(unpar, decl->as.typedef_.target)) return false;
    }
    return el_unparser_push_punct(unpar, EL_TT_SEMICOLON);
}

bool el_unparser_unparse_decl(ElUnparser* unpar, ElAstDecl* decl) {
    switch (decl->type) {
    case EL_AST_DECL_VAR_DEF:   return unparse_var_def(unpar, decl);
    case EL_AST_DECL_VAR_DECL:  return unparse_var_decl(unpar, decl);
    case EL_AST_DECL_FUNC_DEF:  return unparse_func_def(unpar, decl);
    case EL_AST_DECL_FUNC_DECL: return unparse_func_decl(unpar, decl);
    case EL_AST_DECL_ALIAS:     return unparse_alias(unpar, decl);
    case EL_AST_DECL_TYPEDEF:   return unparse_typedef(unpar, decl);
    }
    EL_UNREACHABLE_ENUM_VAL(ElAstDeclType, decl->type);
}
