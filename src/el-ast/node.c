#include <el-ast/node.h>

#include <inttypes.h>

void el_ast_dump_print_ident(size_t ident, FILE* out) {
    for (size_t i = 0; i < ident; ++i) {
        fputs("    ", out);
    }
}

void el_ast_dump_literal(ElAstLiteralNode* lit, size_t ident, FILE* out) {
    el_ast_dump_print_ident(ident, out);
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

void el_ast_dump_impl(ElAstNode* node, size_t ident, FILE* out) {
    el_ast_dump_print_ident(ident, out);
    switch (node->type) {
    case EL_AST_NODE_BINARY_EXPR: {
        ElStringView op = el_ast_bin_op_to_string(node->as.binary.type);
        fprintf(out, "BinaryExpr('"EL_SV_FMT"'):\n", EL_SV_FARG(op));
        el_ast_dump_print_ident(ident + 1, out);
        fprintf(out, "left:  "); el_ast_dump_impl(node->as.binary.left,  0, out);
        el_ast_dump_print_ident(ident + 1, out);
        fprintf(out, "right: "); el_ast_dump_impl(node->as.binary.right, 0, out);
        fputs("\n", out);
        break;
    }
    case EL_AST_NODE_UNARY_EXPR: {
        ElStringView op = el_ast_unary_op_to_string(node->as.unary.type);
        fprintf(out, "UnaryExpr('"EL_SV_FMT"'):", EL_SV_FARG(op));
        el_ast_dump_impl(node->as.binary.right, ident + 1, out);
        break;
    }
    case EL_AST_NODE_LITERAL:
        return el_ast_dump_literal(&node->as.literal, ident, out);
        break;
    }
}

void el_ast_dump(ElAstNode* root, FILE* out) {
    return el_ast_dump_impl(root, 0, out);    
}
