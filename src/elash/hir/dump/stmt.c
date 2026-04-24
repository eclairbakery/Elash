#include <elash/hir/dump/stmt.h>
#include <elash/hir/dump/expr.h>
#include <elash/hir/dump/indent.h>
#include <elash/hir/tree/stmt.h>

void el_hir_dump_stmt(ElHirStmtNode* node, usize indent, FILE* out) {
    if (!node) return;

    for (ElHirStmtNode* curr = node; curr; curr = curr->next) {
        switch (curr->kind) {
        case EL_HIR_STMT_EXPR:
            el_hir_dump_expr(curr->as.expr, indent, out);
            fputs(";\n", out);
            break;

        case EL_HIR_STMT_RETURN:
            el_hir_dump_print_indent(indent, out);
            fputs("return", out);
            if (curr->as.return_.value) {
                fputs(" ", out);
                el_hir_dump_expr(curr->as.return_.value, 0, out);
            }
            fputs(";\n", out);
            break;

        case EL_HIR_STMT_BLOCK:
            el_hir_dump_print_indent(indent, out);
            fputs("{\n", out);
            el_hir_dump_stmt(curr->as.block.stmts, indent + 1, out);
            el_hir_dump_print_indent(indent, out);
            fputs("}\n", out);
            break;
        }
    }
}
