#include <el-hir/dump/expr.h>
#include <el-hir/dump/symbol.h>
#include <el-hir/dump/indent.h>

#include <el-hir/tree/expr.h>

#include <el-sema/expr/bin-op.h>
#include <el-sema/expr/unary-op.h>

#include <inttypes.h>
#include <el-defs/sv.h>

void el_hir_dump_expr(ElHirExprNode* node, usize indent, FILE* out) {
    if (!node) return;

    el_hir_dump_print_indent(indent, out);

    fputs("(", out);

    switch (node->kind) {
    case EL_HIR_EXPR_BINARY: {
        ElStringView op = el_sema_bin_op_to_string(node->as.binary.op);
        el_hir_dump_expr(node->as.binary.left, 0, out);
        fprintf(out, " "EL_SV_FMT" ", EL_SV_FARG(op));
        el_hir_dump_expr(node->as.binary.right, 0, out);
        break;
    }

    case EL_HIR_EXPR_UNARY: {
        ElStringView op = el_sema_unary_op_to_string(node->as.unary.op);
        if (!el_sema_unary_op_is_post(node->as.unary.op)) el_sv_print(op, out);
        el_hir_dump_expr(node->as.unary.operand, 0, out);
        if (el_sema_unary_op_is_post(node->as.unary.op))  el_sv_print(op, out);
        break;
    }

    case EL_HIR_EXPR_LITERAL:
        switch (node->as.literal.type) {
        case EL_AST_LIT_INT:    fprintf(out, "%"PRId64, node->as.literal.of.int_.value);                     break;
        case EL_AST_LIT_FLOAT:  fprintf(out, "%Lf", node->as.literal.of.float_.value);                       break;
        case EL_AST_LIT_CHAR:   fprintf(out, "'%c'", node->as.literal.of.char_.value);                       break;
        case EL_AST_LIT_STRING: fprintf(out, "\""EL_SV_FMT"\"", EL_SV_FARG(node->as.literal.of.str_.value)); break;
        case EL_AST_LIT_BOOL:   fputs(node->as.literal.of.bool_.value ? "true" : "false", out);              break;
        case EL_AST_LIT_NULL:   fputs("null", out);                                                          break;
        }
        break;

    case EL_HIR_EXPR_SYMBOL:
        el_hir_dump_symbol(node->as.symbol, out); 
        break;
    }

    fputs(" : ", out);
    el_sema_dump_type(node->type, out);
    fputs(")", out);
}
