#include "binder-internals.h"

#include <elash/diag/engine.h>
#include <elash/util/assert.h>
#include <elash/util/todo.h>

#include <elash/hir/type/ref.h>
#include <elash/hir/tree/expr.h>

// to reduce boilerplate.
#define type_eql el_hir_type_eql

static inline bool is_fixed_width(ElHirIntWidth width) {
    return width != EL_HIR_IWIDTH_NATIVE && width != EL_HIR_IWIDTH_EFFICIENT;
}
static inline bool is_fixed_fp_width(ElHirFpWidth width) {
    return width != EL_HIR_FPWIDTH_EFFICIENT;
}

static inline bool is_distinct_conv(ElHirType* from, ElHirType* to) {
    return (from->kind == EL_HIR_TYPE_DISTINCT && el_hir_type_eql(from->as.distinct.orig, to)) ||
            (to->kind == EL_HIR_TYPE_DISTINCT && el_hir_type_eql(to->as.distinct.orig, from));
}

ElHirExpr* _cast_untyped(ElBinder* binder, ElSourceSpan span, ElHirExpr* expr, ElHirType* to);

// NOLINTNEXTLINE(readability-function-cognitive-complexity): it's all good
ElHirExpr* _el_binder_eval_const_cast(ElBinder* binder, ElSourceSpan span, ElHirExpr* expr, ElHirType* to_orig) {
    if (expr == NULL || to_orig == NULL) return NULL;
    if (expr->kind == EL_HIR_EXPR_LITERAL)
        return _cast_untyped(binder, span, expr, to_orig);

    EL_ASSERT(expr->kind == EL_HIR_EXPR_CONST, "eval const cast requires a constant operand");

    ElHirType* to = to_orig;
    if (to->kind == EL_HIR_TYPE_DISTINCT)
        to = el_hir_type_unwrap_distinct(to);

    ElHirType* from = expr->type;
    if (from == NULL) return NULL;
    if (from->kind == EL_HIR_TYPE_DISTINCT)
        from = el_hir_type_unwrap_distinct(from);

    if (to->kind != EL_HIR_TYPE_PRIM || from->kind != EL_HIR_TYPE_PRIM)
        return NULL;

    switch (to->as.prim.kind) {
    case EL_PRIMTYPE_INT:
        switch (from->as.prim.kind) {
        case EL_PRIMTYPE_INT:
            return el_hir_new_int_constant(binder->arena, span, to, expr->as.constant.as.int_);
        case EL_PRIMTYPE_FLOAT:
            return el_hir_new_int_constant(binder->arena, span, to, (int64_t)expr->as.constant.as.float_);
        case EL_PRIMTYPE_BOOL:
        case EL_PRIMTYPE_VOID:
            EL_UNREACHABLE("invalid cast");
        }
        EL_UNREACHABLE_ENUM_VAL(ElHirPrimTypeKind, from->as.prim.kind);
    case EL_PRIMTYPE_BOOL:
        switch (from->as.prim.kind) {
        case EL_PRIMTYPE_BOOL:
            return el_hir_new_bool_constant(binder->arena, span, to, expr->as.constant.as.bool_);
        case EL_PRIMTYPE_FLOAT:
        case EL_PRIMTYPE_INT:
        case EL_PRIMTYPE_VOID:
            EL_UNREACHABLE("invalid cast");
        }
        EL_UNREACHABLE_ENUM_VAL(ElHirPrimTypeKind, from->as.prim.kind);
    case EL_PRIMTYPE_FLOAT:
        switch (from->as.prim.kind) {
        case EL_PRIMTYPE_INT:
            return el_hir_new_float_constant(binder->arena, span, to, (double)expr->as.constant.as.int_);
        case EL_PRIMTYPE_FLOAT:
            return el_hir_new_float_constant(binder->arena, span, to, expr->as.constant.as.float_);
        case EL_PRIMTYPE_BOOL:
        case EL_PRIMTYPE_VOID:
            EL_UNREACHABLE("invalid cast");
        }
        EL_UNREACHABLE_ENUM_VAL(ElHirPrimTypeKind, from->as.prim.kind);
    case EL_PRIMTYPE_VOID:
        EL_UNREACHABLE("invalid cast");
    }
    EL_UNREACHABLE_ENUM_VAL(ElHirPrimTypeKind, to->as.prim.kind);
}

ElHirExpr* _el_binder_explicit_cast(ElBinder* binder, ElSourceSpan span, ElHirExpr* expr, ElHirType* to) {
    EL_ASSERT(expr != NULL, "shouldn't be null here");

    ElHirType* from = expr->type;
    if (from == NULL)
        return _cast_untyped(binder, span, expr, to);

    if (type_eql(from, to)) return expr;

    if (to->kind == EL_HIR_TYPE_DISTINCT) {
        ElHirExpr* casted = _el_binder_implicit_cast(binder, span, expr, to->as.distinct.orig);
        if (casted != NULL) return el_hir_new_cast_expr(binder->arena, expr->span, to, casted);
    }
    if (from->kind == EL_HIR_TYPE_DISTINCT) {
        expr = el_hir_new_cast_expr(binder->arena, expr->span, from->as.distinct.orig, expr);
        from = expr->type;
    }

    if (from->kind == EL_HIR_TYPE_PRIM && to->kind == EL_HIR_TYPE_PRIM) {
        bool is_int_conv = from->as.prim.kind == EL_PRIMTYPE_INT && to->as.prim.kind == EL_PRIMTYPE_INT;
        bool is_float_conv = (from->as.prim.kind == EL_PRIMTYPE_FLOAT || from->as.prim.kind == EL_PRIMTYPE_INT)
                            && (to->as.prim.kind == EL_PRIMTYPE_FLOAT || to->as.prim.kind == EL_PRIMTYPE_INT);
        if (is_int_conv || is_float_conv) {
            return el_hir_new_cast_expr(binder->arena, expr->span, to, expr);
        }
    }

    return _el_binder_implicit_cast(binder, span, expr, to);
}

ElHirExpr* _el_binder_implicit_cast(ElBinder* binder, ElSourceSpan span, ElHirExpr* expr, ElHirType* to) {
    ElHirType* from = expr->type;
    if (from == NULL)
        return _cast_untyped(binder, span, expr, to);

    if (type_eql(from, to)) return expr;

    if (from->kind == EL_HIR_TYPE_ARRAY) {
        if (to->kind == EL_HIR_TYPE_SLICE) {
            return el_hir_new_make_slice_intr(
                binder->arena,
                expr->span,
                _el_binder_implicit_cast(binder, span, expr, el_hir_new_raw_slice_type(binder->arena, from->as.array.base)),
                el_hir_new_int_constant(binder->arena, EL_SRCSPAN_NULL, binder->builtins->type_usize, (int64_t)from->as.array.size)
            );
        } else if (to->kind == EL_HIR_TYPE_RWSLICE) {
            if (type_eql(to->as.rwslice.base, from->as.array.base)) {
                // &(expr)[0] as T[&]
                ElHirType* base_type = from->as.array.base;
                return el_hir_new_cast_expr(binder->arena, expr->span, to,
                    el_hir_new_unary_expr(
                        binder->arena, expr->span,
                        el_hir_new_ref_type(binder->arena, base_type),
                        EL_SEMA_UNARY_OP_ADDROF,
                        el_hir_new_bin_expr(binder->arena, EL_SRCSPAN_NULL, base_type, EL_SEMA_BIN_OP_INDEX,
                            expr, el_hir_new_int_constant(binder->arena, EL_SRCSPAN_NULL, binder->builtins->type_int, 0)
                )));
            }
        } else if (to->kind == EL_HIR_TYPE_REF) {
            // let's give the user some nice error message in this case
            el_diag_report(
                binder->diag, EL_DIAG_ERROR, "sema.invalid-cast", span,
                "invalid cast from array type ${from} to ${to} pointer",
                EL_DIAG_TYPE("from", from), EL_DIAG_TYPE("to", to),
            );
            el_diag_help(
                binder->diag, "did you meant to use a raw slice ('${type}')?",
                EL_DIAG_TYPE("type", el_hir_new_raw_slice_type(binder->arena, to->as.ref.base)),
            );

            return NULL;
        }
    }
    if (from->kind == EL_HIR_TYPE_SLICE) {
        if (to->kind == EL_HIR_TYPE_RWSLICE) {
            if (type_eql(from->as.slice.base, to->as.rwslice.base)) {
                return el_hir_new_slice_data_intr(
                    binder->arena, expr->span,
                    to, expr
                );
            }
        }
    }

    if (from->kind == EL_HIR_TYPE_PRIM && to->kind == EL_HIR_TYPE_PRIM) {
        if (from->as.prim.kind == EL_PRIMTYPE_INT && to->as.prim.kind == EL_PRIMTYPE_INT) {
            // the type of these expressions is an anonymous union
            // and using auto/typeof requires C23 which is not widely
            // supported so let's stick to #define
            #define from_itype (&from->as.prim.as.integral)
            #define to_itype   (&to->as.prim.as.integral)
            bool is_valid = from_itype->is_signed == to_itype->is_signed
                        && (from_itype->width     == to_itype->width
                        || (is_fixed_width(from_itype->width) && is_fixed_width(to_itype->width)
                        &&  from_itype->width     <= to_itype->width));
            if (is_valid) return el_hir_new_cast_expr(binder->arena, expr->span, to, expr);
        } else if (from->as.prim.kind == EL_PRIMTYPE_FLOAT && to->as.prim.kind == EL_PRIMTYPE_FLOAT) {
            // same reason as before, don't blame me plz
            #define from_fptype (&from->as.prim.as.fp)
            #define to_fptype   (&to->as.prim.as.fp)
            bool is_valid = from_fptype->width == to_fptype->width
                        || (is_fixed_fp_width(from_fptype->width) && is_fixed_fp_width(to_fptype->width)
                        &&  from_fptype->width < to_fptype->width);
            if (is_valid) return el_hir_new_cast_expr(binder->arena, expr->span, to, expr);
        }
    }

    if (is_distinct_conv(from, to)) {
        el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.invalid-cast", span,
            "cannot implicitly convert from ${from} to ${to}",
            EL_DIAG_TYPE("from", from), EL_DIAG_TYPE("to", to),
        );
        el_diag_help(
            binder->diag, "distinct types require an explicit cast with 'as'",
        );
        return NULL;
    }

    el_diag_report(
        binder->diag, EL_DIAG_ERROR, "sema.invalid-cast", span,
        "invalid cast from '${from}' to '${to}'",
        EL_DIAG_TYPE("from", from), EL_DIAG_TYPE("to", to),
    );
    return NULL;
}

static ElHirExpr* cast_untyped_compound(ElBinder* binder, ElSourceSpan span, ElHirExpr* expr, ElHirType* to) {
    switch (expr->kind) {
    case EL_HIR_EXPR_BINARY: {
        ElHirBinExpr* bin = &expr->as.binary;
        ElHirExpr* left = _el_binder_implicit_cast(binder, bin->left->span, bin->left, to);
        ElHirExpr* right = _el_binder_implicit_cast(binder, bin->right->span, bin->right, to);
        if (left == NULL || right == NULL) return NULL;

        ElHirType* result_ty = el_sema_bin_op_is_comparison(bin->op)
            ? binder->builtins->type_bool
            : to;
        ElHirExpr* out = el_hir_new_bin_expr(binder->arena, expr->span, result_ty, bin->op, left, right);
        out = _el_binder_simplify_expr(binder, out);
        if (out != NULL && out->type != NULL && !type_eql(out->type, to))
            return _el_binder_implicit_cast(binder, span, out, to);
        return out;
    }
    case EL_HIR_EXPR_UNARY: {
        ElSemaUnaryOp op = expr->as.unary.op;
        if (op == EL_SEMA_UNARY_OP_PRE_INC || op == EL_SEMA_UNARY_OP_PRE_DEC
            || op == EL_SEMA_UNARY_OP_POST_INC || op == EL_SEMA_UNARY_OP_POST_DEC
            || op == EL_SEMA_UNARY_OP_ADDROF || op == EL_SEMA_UNARY_OP_DEREF) {
            return el_diag_report(
                binder->diag, EL_DIAG_ERROR, "sema.invalid-cast", span,
                "operator cannot be used in this context"
            );
        }

        ElHirExpr* operand = _el_binder_implicit_cast(
            binder, expr->as.unary.operand->span, expr->as.unary.operand, to
        );
        if (operand == NULL) return NULL;

        ElHirExpr* out = el_hir_new_unary_expr(binder->arena, expr->span, to, op, operand);
        return _el_binder_simplify_expr(binder, out);
    }
    default:
        return el_diag_report(
            binder->diag, EL_DIAG_ERROR, "sema.invalid-cast",
            span, "untyped expression cannot be converted to type ${to}",
            EL_DIAG_TYPE("to", to),
        );
    }
}

ElHirExpr* _cast_untyped(ElBinder* binder, ElSourceSpan span, ElHirExpr* expr, ElHirType* to) {
    (void) span;
    if (to->kind == EL_HIR_TYPE_DISTINCT) {
        ElHirExpr* casted = _cast_untyped(binder, span, expr, to->as.distinct.orig);
        if (casted == NULL) return NULL;
        return el_hir_new_cast_expr(binder->arena, expr->span, to, casted);
    }

    // Untyped compounds (e.g. 'a' + 1, 1 / 0, --1) also have type == NULL.
    // Only literals may be cast via the literal path below.
    if (expr->kind != EL_HIR_EXPR_LITERAL)
        return cast_untyped_compound(binder, span, expr, to);

    if (to->kind == EL_HIR_TYPE_PRIM) {
        switch (expr->as.literal.kind) {
        case EL_HIR_LITERAL_INT:
            if (to->as.prim.kind == EL_PRIMTYPE_INT) {
                return el_hir_new_int_constant(binder->arena, expr->span, to, expr->as.literal.of.int_);
            } else if (to->as.prim.kind == EL_PRIMTYPE_FLOAT) {
                return el_hir_new_float_constant(binder->arena, expr->span, to, (double)expr->as.literal.of.int_);
            }
            break;
        case EL_HIR_LITERAL_CHAR:
            if (to->as.prim.kind == EL_PRIMTYPE_INT) {
                return el_hir_new_int_constant(binder->arena, expr->span, to, (int64_t)expr->as.literal.of.char_);
            }
            break;
        case EL_HIR_LITERAL_BOOL:
            if (to->as.prim.kind == EL_PRIMTYPE_BOOL) {
                return el_hir_new_bool_constant(binder->arena, expr->span, to, expr->as.literal.of.bool_);
            }
            break;
        case EL_HIR_LITERAL_FLOAT:
            if (to->as.prim.kind == EL_PRIMTYPE_FLOAT) {
                return el_hir_new_float_constant(binder->arena, expr->span, to, expr->as.literal.of.float_);
            } else if (to->as.prim.kind == EL_PRIMTYPE_INT) {
                return el_hir_new_int_constant(binder->arena, expr->span, to, (int64_t)expr->as.literal.of.float_);
            }
            break;
        }
    }

    return el_diag_report(
        binder->diag, EL_DIAG_ERROR, "sema.invalid-cast",
        span, "untyped ${of} literal cannot be converted to type ${to}",
        EL_DIAG_STRING("of", el_hir_literal_kind_to_string(expr->as.literal.kind)),
        EL_DIAG_TYPE("to", to),
    );
}

ElHirExpr* _el_binder_apply_default_type(ElBinder* binder, ElHirExpr* expr) {
    if (expr->type != NULL) return expr;
    if (expr->kind == EL_HIR_EXPR_LITERAL) {
        switch (expr->as.literal.kind) {
        case EL_HIR_LITERAL_INT:
            return el_hir_new_int_constant(binder->arena, expr->span, binder->builtins->type_int, expr->as.literal.of.int_);
        case EL_HIR_LITERAL_CHAR:
            return el_hir_new_char_constant(binder->arena, expr->span, binder->builtins->type_char, expr->as.literal.of.char_);
        case EL_HIR_LITERAL_BOOL:
            return el_hir_new_bool_constant(binder->arena, expr->span, binder->builtins->type_bool, expr->as.literal.of.bool_);
        case EL_HIR_LITERAL_FLOAT:
            return el_hir_new_float_constant(binder->arena, expr->span, binder->builtins->type_float, expr->as.literal.of.float_);
        }
    }
    return expr;
}
