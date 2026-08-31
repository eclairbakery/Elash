#include "parser-internals.h"

#include <elash/diag/engine.h>

#include <elash/ast/tree/stmt.h>
#include <elash/ast/tree/stmt/return.h>
#include <elash/ast/tree/stmt/if.h>
#include <elash/ast/tree/stmt/assign.h>
#include <elash/ast/tree/stmt/cassign.h>

ElAstStmt* _el_parser_parse_return(ElParser* parser, ElToken return_tok) {
    if (el_parser_check(parser, EL_TT_SEMICOLON)) {
        ElToken semi_tok = el_parser_advance(parser);
        return el_ast_new_return_stmt(parser->aarena, el_srcspan_merge(return_tok.span, semi_tok.span), NULL);
    }

    ElAstInit* init = el_parser_parse_init(parser);
    if (el_parser_has_errs(parser)) return NULL;

    ElToken semi_tok = el_parser_expect(parser, EL_TT_SEMICOLON);
    if (el_parser_has_errs(parser)) return NULL;

    return el_ast_new_return_stmt(parser->aarena, el_srcspan_merge(return_tok.span, semi_tok.span), init);
}

ElAstStmt* _el_parser_parse_if(ElParser* parser, ElToken if_tok) {
    ElSourceSpan end_span;

    el_parser_expect(parser, EL_TT_LPAREN);
    if (el_parser_has_errs(parser))
        return el_parser_sync(parser, EL_PARSER_SYNC_STMT);

    ElAstExpr* cond = el_parser_parse_expr(parser);
    if (el_parser_has_errs(parser))
        return el_parser_sync(parser, EL_PARSER_SYNC_STMT);

    if (!el_parser_check(parser, EL_TT_RPAREN)) {
        el_parser_expect(parser, EL_TT_RPAREN);
        el_parser_skip_to(parser, EL_TT_RPAREN);
        if (el_parser_check(parser, EL_TT_RPAREN)) {
            el_parser_advance(parser);
        }
    } else {
        el_parser_advance(parser);
    }

    ElAstStmt* then_stmt = el_parser_parse_stmt(parser);
    if (el_parser_has_errs(parser)) return NULL;

    end_span = then_stmt->span;

    ElAstStmt* else_stmt = NULL;
    if (el_parser_match(parser, EL_TT_KW_ELSE)) {
        else_stmt = el_parser_parse_stmt(parser);
        if (el_parser_has_errs(parser)) return NULL;
        end_span = else_stmt->span;
    }

    return el_ast_new_if_stmt(
        parser->aarena,
        el_srcspan_merge(if_tok.span, end_span),
        cond, then_stmt, else_stmt
    );
}

static ElAstStmt* _el_parser_parse_while(ElParser* parser, ElToken while_tok) {
    el_parser_expect(parser, EL_TT_LPAREN);
    if (el_parser_has_errs(parser))
        return el_parser_sync(parser, EL_PARSER_SYNC_STMT);

    ElAstExpr* cond = el_parser_parse_expr(parser);
    if (el_parser_has_errs(parser))
        return el_parser_sync(parser, EL_PARSER_SYNC_STMT);

    if (!el_parser_check(parser, EL_TT_RPAREN)) {
        el_parser_expect(parser, EL_TT_RPAREN);
        el_parser_skip_to(parser, EL_TT_RPAREN);
        if (el_parser_check(parser, EL_TT_RPAREN)) {
            el_parser_advance(parser);
        }
    } else {
        el_parser_advance(parser);
    }

    ElAstStmt* body_stmt = el_parser_parse_stmt(parser);
    if (el_parser_has_errs(parser)) return NULL;

    return el_ast_new_while_stmt(
        parser->aarena,
        el_srcspan_merge(while_tok.span, body_stmt->span),
        cond, body_stmt
    );
}

static bool match_compound_op(ElParser* parser, ElSemaBinOp* op) {
    if      (el_parser_match(parser, EL_TT_ADD_ASSIGN))         { return *op = EL_SEMA_BIN_OP_ADD,    true; }
    else if (el_parser_match(parser, EL_TT_SUB_ASSIGN))         { return *op = EL_SEMA_BIN_OP_SUB,    true; }
    else if (el_parser_match(parser, EL_TT_MUL_ASSIGN))         { return *op = EL_SEMA_BIN_OP_MUL,    true; }
    else if (el_parser_match(parser, EL_TT_DIV_ASSIGN))         { return *op = EL_SEMA_BIN_OP_DIV,    true; }
    else if (el_parser_match(parser, EL_TT_MOD_ASSIGN))         { return *op = EL_SEMA_BIN_OP_MOD,    true; }
    else if (el_parser_match(parser, EL_TT_BITWISE_AND_ASSIGN)) { return *op = EL_SEMA_BIN_OP_BW_AND, true; }
    else if (el_parser_match(parser, EL_TT_BITWISE_OR_ASSIGN))  { return *op = EL_SEMA_BIN_OP_BW_OR,  true; }
    else if (el_parser_match(parser, EL_TT_BITWISE_XOR_ASSIGN)) { return *op = EL_SEMA_BIN_OP_BW_XOR, true; }
    else if (el_parser_match(parser, EL_TT_LOGICAL_AND_ASSIGN)) { return *op = EL_SEMA_BIN_OP_AND,    true; }
    else if (el_parser_match(parser, EL_TT_LOGICAL_OR_ASSIGN))  { return *op = EL_SEMA_BIN_OP_OR,     true; }
    else if (el_parser_match(parser, EL_TT_LOGICAL_IMP_ASSIGN)) { return *op = EL_SEMA_BIN_OP_IMP,    true; }
    else if (el_parser_match(parser, EL_TT_BITWISE_IMP_ASSIGN)) { return *op = EL_SEMA_BIN_OP_BW_IMP, true; }
    else if (el_parser_match(parser, EL_TT_OPT_FB_ASSIGN))      { return *op = EL_SEMA_BIN_OP_OPT_FB, true; }
    else if (el_parser_match(parser, EL_TT_SHL_ASSIGN))         { return *op = EL_SEMA_BIN_OP_SHL,    true; }
    else if (el_parser_match(parser, EL_TT_SHR_ASSIGN))         { return *op = EL_SEMA_BIN_OP_SHR,    true; }
    return false;
}

ElAstStmt* _el_parser_parse_expr_stmt(ElParser* parser) {
    ElAstExpr* expr = el_parser_parse_expr(parser);
    if (expr == NULL) return NULL;

    if (el_parser_match(parser, EL_TT_ASSIGN)) {
        ElAstInit* value = el_parser_parse_init(parser);
        if (value == NULL) return NULL;

        ElToken semi_tok = el_parser_expect(parser, EL_TT_SEMICOLON);
        if (el_parser_has_errs(parser)) return NULL;

        return el_ast_new_assign_stmt(parser->aarena, el_srcspan_merge(expr->span, semi_tok.span), expr, value);
    }

    ElSemaBinOp op;
    bool is_compound = match_compound_op(parser, &op);

    if (is_compound) {
        ElAstInit* value = el_parser_parse_init(parser);
        if (value == NULL) return NULL;

        ElToken semi_tok = el_parser_expect(parser, EL_TT_SEMICOLON);
        if (el_parser_has_errs(parser)) {
            return NULL;
        }

        return el_ast_new_compound_assign_stmt(parser->aarena, el_srcspan_merge(expr->span, semi_tok.span), op, expr, value);
    }

    ElToken semi_tok = el_parser_expect(parser, EL_TT_SEMICOLON);
    if (el_parser_has_errs(parser)) {
        return NULL;
    }

    return el_ast_new_expr_stmt(parser->aarena, el_srcspan_merge(expr->span, semi_tok.span), expr);
}

ElAstStmt* _el_parser_parse_block(ElParser* parser, ElToken lbrace_tok) {
    ElAstStmt* head = NULL;
    ElAstStmt* tail = NULL;

    while (parser->current.type != EL_TT_RBRACE && parser->current.type != EL_TT_EOF) {
        ElAstStmt* stmt = el_parser_parse_stmt(parser);
        if (el_parser_has_errs(parser)) {
            el_parser_sync(parser, EL_PARSER_SYNC_STMT);
            continue;
        }

        if (stmt != NULL) {
            el_ast_stmt_list_append(&head, &tail, stmt);
        }
    }

    ElToken rbrace_tok = parser->current;
    if (el_parser_check(parser, EL_TT_RBRACE)) {
        el_parser_advance(parser);
    } else {
        el_parser_expect(parser, EL_TT_RBRACE);
        el_parser_sync(parser, EL_PARSER_SYNC_BLOCK);
        if (el_parser_check(parser, EL_TT_RBRACE)) {
            rbrace_tok = el_parser_advance(parser);
        }
    }

    return el_ast_new_block_stmt(parser->aarena, el_srcspan_merge(lbrace_tok.span, rbrace_tok.span), head);
}

ElAstStmt* el_parser_parse_stmt(ElParser* parser) {
    if (el_parser_check(parser, EL_TT_KW_RETURN)) {
        ElToken return_tok = el_parser_advance(parser);
        return _el_parser_parse_return(parser, return_tok);
    }
    if (el_parser_check(parser, EL_TT_KW_IF)) {
        ElToken if_tok = el_parser_advance(parser);
        return _el_parser_parse_if(parser, if_tok);
    }
    if (el_parser_check(parser, EL_TT_KW_WHILE)) {
        ElToken while_tok = el_parser_advance(parser);
        return _el_parser_parse_while(parser, while_tok);
    }
    if (el_parser_check(parser, EL_TT_LBRACE)) {
        ElToken lbrace_tok = el_parser_advance(parser);
        return _el_parser_parse_block(parser, lbrace_tok);
    }

    if (el_parser_check(parser, EL_TT_KW_BREAK)) {
        ElToken break_tok = el_parser_advance(parser);
        ElToken semi_tok = el_parser_expect(parser, EL_TT_SEMICOLON);
        return el_ast_new_break_stmt(parser->aarena, el_srcspan_merge(break_tok.span, semi_tok.span));
    }
    if (el_parser_check(parser, EL_TT_KW_CONTINUE)) {
        ElToken continue_tok = el_parser_advance(parser);
        ElToken semi_tok = el_parser_expect(parser, EL_TT_SEMICOLON);
        return el_ast_new_continue_stmt(parser->aarena, el_srcspan_merge(continue_tok.span, semi_tok.span));
    }

    usize lookahead_idx = 0;
    bool is_decl = el_parser_check(parser, EL_TT_KW_EXTERN)
                || el_parser_check(parser, EL_TT_KW_STATIC)
                || el_parser_check(parser, EL_TT_KW_ALIAS)
                || el_parser_check(parser, EL_TT_KW_TYPEDEF);
    if (!is_decl && _el_parser_lookahead_skip_type(parser, &lookahead_idx)) {
        is_decl = el_parser_peek_at(parser, lookahead_idx).type == EL_TT_IDENT;
    }

    if (is_decl) {
        ElAstDecl* decl = el_parser_parse_decl(parser);
        if (decl == NULL) return NULL;
        return el_ast_new_decl_stmt(parser->aarena, decl->span, decl);
    }

    return _el_parser_parse_expr_stmt(parser);
}
