#pragma once

#include <elash/util/dynarena.h>
#include <elash/defs/sv.h>

#include <elash/sema/expr/unary-op.h>

typedef struct ElAstExprNode ElAstExprNode;

typedef ElSemaUnaryOp ElAstUnaryOp;

typedef struct ElAstUnaryExprNode {
    ElAstUnaryOp op;
    ElAstExprNode* operand;
} ElAstUnaryExprNode;

ElAstUnaryExprNode el_ast_unary_expr(ElAstUnaryOp type, ElAstExprNode* operand);
ElAstExprNode* el_ast_new_unary_expr(ElDynArena* arena, ElAstUnaryOp type, ElAstExprNode* operand);

