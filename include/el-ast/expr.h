#pragma once

#include "expr/bin.h"
#include "expr/unary.h"

#include "expr/literal.h"

typedef enum ElAstExprType {
    EL_AST_EXPR_BINARY,
    EL_AST_EXPR_UNARY,
    EL_AST_EXPR_LITERAL,
} ElAstExprType;

typedef struct ElAstExprNode {
    ElAstExprType type;
    union {
        ElAstBinExprNode binary;
        ElAstUnaryExprNode unary;
        ElAstLiteralNode literal;
    } as;
} ElAstExprNode;

// TODO: this sucks
void el_ast_dump_expr(ElAstExprNode* root, FILE* out);
void el_ast_dump_expr_impl(ElAstExprNode* node, size_t ident, FILE* out);
void el_ast_dump_expr_print_ident(size_t ident, FILE* out);

