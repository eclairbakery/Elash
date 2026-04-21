#pragma once

#include "expr/bin.h"
#include "expr/unary.h"
#include "expr/literal.h"
#include "common/ident.h"

#include <el-defs/inttypes.h>

typedef enum ElAstExprType {
    EL_AST_EXPR_BINARY,
    EL_AST_EXPR_UNARY,
    EL_AST_EXPR_LITERAL,
    EL_AST_EXPR_IDENT,
} ElAstExprType;

typedef struct ElAstExprNode {
    ElAstExprType type;
    union {
        ElAstBinExprNode binary;
        ElAstUnaryExprNode unary;
        ElAstLiteralNode literal;
        ElAstIdentNode ident;
    } as;
} ElAstExprNode;

// TODO: this sucks
void el_ast_dump_expr(ElAstExprNode* node, usize indent, FILE* out);
void el_ast_dump_print_indent(usize ident, FILE* out);

