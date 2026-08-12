#include "parser-internals.h"

#include <elash/diag/engine.h>
#include <elash/diag/meta.h>
#include <elash/lexer/token.h>

static ElAstFuncParamList parse_func_params(ElParser* parser) {
    ElAstFuncParamList params = el_ast_make_func_param_list();

    while (parser->current.type != EL_TT_RPAREN && parser->current.type != EL_TT_EOF) {
        ElAstType* p_type = _el_parser_parse_type(parser);
        if (p_type == NULL) {
            el_parser_skip_to(parser, EL_TT_RPAREN);
            return params;
        }

        ElAstIdent* p_name = _el_parser_parse_ident(parser);
        if (p_name == NULL) {
            el_parser_skip_to(parser, EL_TT_RPAREN);
            return params;
        }

        el_ast_func_param_list_append(&params, el_ast_new_func_param(
            parser->arena,
            el_srcspan_merge(p_type->span, p_name->span),
            p_type, p_name
        ));

        if (el_parser_match(parser, EL_TT_COMMA)) {
            continue;
        } else if (parser->current.type != EL_TT_RPAREN) {
            el_parser_expect(parser, EL_TT_RPAREN);
            return params;
        }
    }

    return params;
}

static ElAstFuncSignature parse_func_sig(ElParser* parser) {
    ElAstFuncSignature sig = {0};

    ElAstType* ret_type = _el_parser_parse_type(parser);
    if (ret_type == NULL) return sig;

    ElAstIdent* name = _el_parser_parse_ident(parser);
    if (name == NULL) return sig;

    el_parser_expect(parser, EL_TT_LPAREN);
    if (el_parser_has_errs(parser)) return sig;

    ElAstFuncParamList params = parse_func_params(parser);
    if (el_parser_has_errs(parser)) return sig;

    ElToken rparen_tok = parser->current;
    el_parser_expect(parser, EL_TT_RPAREN);
    if (el_parser_has_errs(parser)) return sig;

    ElSourceSpan span = el_srcspan_merge(ret_type->span, rparen_tok.span);
    return el_ast_func_signature(span, ret_type, name, params);
}

static ElAstDecl* parse_func_internal_decl(ElParser* parser, ElAstFuncSignature sig) {
    if (el_parser_check(parser, EL_TT_LBRACE)) {
        ElToken lbrace_tok = parser->current;
        el_parser_advance(parser);

        ElAstStmt* body_stmt = _el_parser_parse_block(parser, lbrace_tok);
        ElSourceSpan span = el_srcspan_merge(sig.span, body_stmt->span);

        return el_ast_new_func_def(parser->arena, span, sig, &body_stmt->as.block);
    }

    ElToken semi_tok = parser->current;
    el_parser_expect(parser, EL_TT_SEMICOLON);

    ElSourceSpan span = el_srcspan_merge(sig.span, semi_tok.span);
    return el_ast_new_func_decl(parser->arena, span, sig);
}

static ElAstDecl* parse_var_internal_decl(ElParser* parser) {
    bool is_static = el_parser_match(parser, EL_TT_KW_STATIC);

    ElAstType* type = _el_parser_parse_type(parser);
    if (type == NULL) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

    ElAstIdent* name = _el_parser_parse_ident(parser);
    if (name == NULL) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

    ElAstInit* init = NULL;
    if (el_parser_match(parser, EL_TT_ASSIGN)) {
        init = el_parser_parse_init(parser);
        if (init == NULL) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);
    }

    ElToken semi_tok = parser->current;
    el_parser_expect(parser, EL_TT_SEMICOLON);

    ElSourceSpan span = el_srcspan_merge(type->span, semi_tok.span);
    return el_ast_new_var_def(parser->arena, span, type, name, init, is_static);
}

static ElAstDecl* parse_extern_decl(ElParser* parser, ElToken extern_tok) {
    usize idx = 0;
    if (!_el_parser_lookahead_skip_type(parser, &idx) || el_parser_peek_at(parser, idx).type != EL_TT_IDENT) {
        el_parser_expect(parser, EL_TT_IDENT);
        return el_parser_sync(parser, EL_PARSER_SYNC_DECL);
    }

    if (el_parser_peek_at(parser, idx + 1).type == EL_TT_LPAREN) {
        ElAstFuncSignature sig = parse_func_sig(parser);
        if (sig.ret_type == NULL) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

        if (el_parser_check(parser, EL_TT_LBRACE)) {
            el_diag_report(
                parser->diag, EL_DIAG_ERROR, "syntax.extern-body",
                parser->current.span,
                "extern functions cannot have a body"
            );
            el_parser_skip_balanced_braces(parser);
            return NULL;
        }

        ElToken semi_tok = parser->current;
        el_parser_expect(parser, EL_TT_SEMICOLON);

        ElSourceSpan span = el_srcspan_merge(extern_tok.span, semi_tok.span);
        return el_ast_new_func_decl(parser->arena, span, sig);
    } else {
        ElAstType* type = _el_parser_parse_type(parser);
        if (type == NULL) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

        ElAstIdent* name = _el_parser_parse_ident(parser);
        if (name == NULL) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

        if (el_parser_check(parser, EL_TT_ASSIGN)) {
            el_diag_report(
                parser->diag, EL_DIAG_ERROR, "syntax.extern-init",
                parser->current.span,
                "extern variables cannot have an initializer"
            );
            el_parser_advance(parser);
            el_parser_parse_init(parser);
        }

        ElToken semi_tok = parser->current;
        el_parser_expect(parser, EL_TT_SEMICOLON);

        ElSourceSpan span = el_srcspan_merge(extern_tok.span, semi_tok.span);
        return el_ast_new_var_decl(parser->arena, span, type, name);
    }
}

static ElAstDecl* parse_alias_decl(ElParser* parser, ElToken alias_tok) {
    ElToken name_tok = el_parser_expect(parser, EL_TT_IDENT);
    if (el_parser_has_errs(parser)) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

    el_parser_expect(parser, EL_TT_ASSIGN);
    if (el_parser_has_errs(parser)) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

    ElAstToE* target = el_parser_parse_toe(parser);
    if (target == NULL) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

    ElToken semi_tok = el_parser_expect(parser, EL_TT_SEMICOLON);

    ElSourceSpan span = el_srcspan_merge(alias_tok.span, semi_tok.span);
    return el_ast_new_alias(parser->arena, span, name_tok.lexeme, *target);
}

// 100% not just copy pased from the function above (no idea how to dedup this)
static ElAstDecl* parse_typedef_decl(ElParser* parser, ElToken typedef_tok) {
    ElToken name_tok = el_parser_expect(parser, EL_TT_IDENT);
    if (el_parser_has_errs(parser)) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

    el_parser_expect(parser, EL_TT_KW_AS);
    if (el_parser_has_errs(parser)) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

    ElAstType* target = _el_parser_parse_type(parser);
    if (target == NULL) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);

    ElToken semi_tok = el_parser_expect(parser, EL_TT_SEMICOLON);

    ElSourceSpan span = el_srcspan_merge(typedef_tok.span, semi_tok.span);
    return el_ast_new_typedef(parser->arena, span, name_tok.lexeme, target);
}

static ElAstDecl* el_parser_parse_internal_decl(ElParser* parser) {
    if (el_parser_check(parser, EL_TT_KW_ALIAS)) {
        ElToken alias_tok = el_parser_advance(parser);
        return parse_alias_decl(parser, alias_tok);
    }

    if (el_parser_check(parser, EL_TT_KW_TYPEDEF)) {
        ElToken typedef_tok = el_parser_advance(parser);
        return parse_typedef_decl(parser, typedef_tok);
    }

    usize idx = 0;
    if (el_parser_peek_at(parser, idx).type == EL_TT_KW_STATIC) idx++;

    if (!_el_parser_lookahead_skip_type(parser, &idx) || el_parser_peek_at(parser, idx).type != EL_TT_IDENT) {
        el_parser_expect(parser, EL_TT_IDENT);
        return el_parser_sync(parser, EL_PARSER_SYNC_DECL);
    }

    if (el_parser_peek_at(parser, idx + 1).type == EL_TT_LPAREN) {
        ElAstFuncSignature sig = parse_func_sig(parser);
        if (sig.ret_type == NULL) return el_parser_sync(parser, EL_PARSER_SYNC_DECL);
        return parse_func_internal_decl(parser, sig);
    }

    return parse_var_internal_decl(parser);
}

ElAstDecl* el_parser_parse_decl(ElParser* parser) {
    if (el_parser_check(parser, EL_TT_KW_EXTERN)) {
        ElToken extern_tok = parser->current;
        el_parser_advance(parser);
        return parse_extern_decl(parser, extern_tok);
    }

    return el_parser_parse_internal_decl(parser);
}
