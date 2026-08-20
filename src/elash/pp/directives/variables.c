#include "../preproc-internals.h"

static ElPpSymbol* _el_pp_get_var_symbol(ElPreproc* pp, ElStringView dname, ElSourceSpan dspan, ElToken* out_name_tok) {
    if (!_el_pp_read(pp, out_name_tok)) {
        return el_diag_report(
            pp->diag, EL_DIAG_ERROR, "pp.unexpected-token",
            dspan, "expected identifier after #${dir} directive",
            EL_DIAG_STRING("dir", dname)
        );
    }

    if (out_name_tok->type != EL_TT_IDENT) {
         return el_diag_report(
            pp->diag, EL_DIAG_ERROR, "pp.unexpected-token",
            out_name_tok->span, "expected identifier after #${dir} directive",
            EL_DIAG_STRING("dir", dname)
        );
    }

    ElPpSymbol* sym = el_pp_scope_lookup(pp->current_scope, out_name_tok->lexeme);
    if (sym == NULL) {
        return el_diag_report(
            pp->diag, EL_DIAG_ERROR, "pp.undeclared",
            out_name_tok->span, "undeclared identifier ${name} in #${dir} directive",
            EL_DIAG_STRING("name", out_name_tok->lexeme),
            EL_DIAG_STRING("dir", dname)
        );
    }

    if (sym->kind != EL_PP_SYM_VAR) {
        return el_diag_report(
            pp->diag, EL_DIAG_ERROR, "pp.sym-kind",
            out_name_tok->span, "expected a variable name"
        );
    }

    return sym;
}

bool _el_pp_handle_var(ElPreproc* pp, ElSourceSpan dspan) {
    ElToken name_tok;
    if (!_el_pp_read(pp, &name_tok) || name_tok.type != EL_TT_IDENT) {
        return el_diag_report(
            pp->diag, EL_DIAG_ERROR, "pp.unexpected-token",
            dspan, "expected identifier after #var directive"
        );
    }

    if (el_pp_scope_lookup_local(pp->current_scope, name_tok.lexeme)) {
        return el_diag_report(
            pp->diag, EL_DIAG_ERROR, "pp.redefinition",
            name_tok.span, "redefinition of variable ${name}",
            EL_DIAG_STRING("name", name_tok.lexeme)
        );
    }

    ElToken next; ElPpValue* value;
    if (_el_pp_peek(pp, &next) && next.type == EL_TT_ASSIGN) {
        _el_pp_advance(pp); // '='

        value = _el_pp_eval(pp);
        if (value == NULL) return false;

    } else {
        value = _el_pp_new_null(pp->arena);
    }

    ElPpSymbol* sym = _el_pp_new_sym_var(pp->arena, name_tok.lexeme, value);
    return el_pp_scope_assign(pp->current_scope, sym->name, sym);
}

bool _el_pp_handle_set(ElPreproc* pp, ElSourceSpan dspan) {
    ElToken name_tok;
    ElPpSymbol* sym = _el_pp_get_var_symbol(pp, EL_SV("set"), dspan, &name_tok);
    if (sym == NULL) return false;

    if (!_el_pp_match(pp, EL_TT_ASSIGN)) {
        ElToken tok; _el_pp_peek(pp, &tok);
        return el_diag_report(
            pp->diag, EL_DIAG_ERROR, "pp.unexpected-token",
            tok.span, "expected '=' after variable name"
        );
    }

    ElPpValue* value = _el_pp_eval(pp);
    if (value == NULL) return false;

    sym->as.var = value;
    return true;
}

static bool _el_pp_handle_incdec(ElPreproc* pp, ElStringView dname, bool increment, ElSourceSpan dspan) {
    ElToken name_tok;
    ElPpSymbol* sym = _el_pp_get_var_symbol(pp, dname, dspan, &name_tok);
    if (sym == NULL) return false;

    ElPpValue* val = sym->as.var;
    if (val->type == EL_PP_TYPE_INT) {
        if (increment) val->as.int_++;   else val->as.int_--;
    } else if (val->type == EL_PP_TYPE_FLOAT) {
        if (increment) val->as.float_++; else val->as.float_--;
    } else if (val->type == EL_PP_TYPE_CHAR) {
        if (increment) val->as.char_++;  else val->as.char_--;
    } else {
        return el_diag_report(
            pp->diag, EL_DIAG_ERROR, "pp.invalid-type",
            name_tok.span, "cannot ${op} variable of type ${type}",
            EL_DIAG_STRING("op", increment ? EL_SV("increment") : EL_SV("decrement")),
            EL_DIAG_STRING("type", _el_pp_type_name(val->type))
        );
    }
    return true;
}

bool _el_pp_handle_inc(ElPreproc* pp, ElSourceSpan dspan) {
    return _el_pp_handle_incdec(pp, EL_SV("inc"), true, dspan);
}

bool _el_pp_handle_dec(ElPreproc* pp, ElSourceSpan dspan) {
    return _el_pp_handle_incdec(pp, EL_SV("dec"), false, dspan);
}
