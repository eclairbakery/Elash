#include <el-ast/expr.h>

#include <inttypes.h>

void el_ast_dump_print_indent(usize indent, FILE* out) {
    for (usize i = 0; i < indent; ++i) {
        fputs("  ", out);
    }
}

void el_ast_dump_expr_literal(ElAstLiteralNode* lit, usize indent, FILE* out) {
    el_ast_dump_print_indent(indent, out);
    switch (lit->type) {
    case EL_AST_LIT_BOOL:
        fprintf(out, "BoolLiteral(%s)\n", lit->of.bool_.value ? "true" : "false");
        break;
    case EL_AST_LIT_NULL:
        fprintf(out, "NullLiteral\n");
        break;
    case EL_AST_LIT_CHAR:
        fprintf(out, "CharLiteral(%c)\n", lit->of.char_.value);
        break;
    case EL_AST_LIT_INT:
        fprintf(out, "IntLiteral(%"PRId64")\n", lit->of.int_.value);
        break;
    case EL_AST_LIT_FLOAT:
        fprintf(out, "FloatLiteral(%Lf)\n", lit->of.float_.value);
        break;
    case EL_AST_LIT_STRING:
        fprintf(out, "StringLiteral(\""EL_SV_FMT"\")\n", EL_SV_FARG(lit->of.str_.value));
        break;
    }
}

void el_ast_dump_expr_ident(ElAstIdentNode* node, usize indent, FILE* out) {
    el_ast_dump_print_indent(indent, out);
    fprintf(out, "IdentExpr(\""EL_SV_FMT"\")\n", EL_SV_FARG(node->name));
}

void el_ast_dump_expr(ElAstExprNode* node, usize indent, FILE* out) {
    switch (node->type) {
    case EL_AST_EXPR_BINARY: {
        ElStringView op = el_ast_bin_op_to_string(node->as.binary.type);
        el_ast_dump_print_indent(indent, out);
        fprintf(out, "BinaryExpr('"EL_SV_FMT"'):\n", EL_SV_FARG(op));
        el_ast_dump_print_indent(indent + 1, out);
        fprintf(out, "left: \n"); el_ast_dump_expr(node->as.binary.left,  indent + 2, out);
        el_ast_dump_print_indent(indent + 1, out);
        fprintf(out, "right: \n"); el_ast_dump_expr(node->as.binary.right, indent + 2, out);
        break;
    }
    case EL_AST_EXPR_UNARY: {
        ElStringView op = el_ast_unary_op_to_string(node->as.unary.type);
        el_ast_dump_print_indent(indent, out);
        fprintf(out, "UnaryExpr('"EL_SV_FMT"'):\n", EL_SV_FARG(op));
        el_ast_dump_expr(node->as.unary.operand, indent + 1, out);
        break;
    }
    case EL_AST_EXPR_LITERAL:
        return el_ast_dump_expr_literal(&node->as.literal, indent, out);
    case EL_AST_EXPR_IDENT:
        return el_ast_dump_expr_ident(&node->as.ident, indent, out);
    }
}

