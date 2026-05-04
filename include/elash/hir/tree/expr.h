#pragma once

#include <elash/sema/type.h>
#include <elash/hir/symbol.h>
#include <elash/util/dynarena.h>

#include "expr/bin.h"
#include "expr/unary.h"
#include "expr/literal.h"
#include "expr/call.h"

typedef enum ElHirExprKind {
    EL_HIR_EXPR_BINARY,
    EL_HIR_EXPR_UNARY,
    EL_HIR_EXPR_LITERAL,
    EL_HIR_EXPR_SYMBOL, // resolved identifier
    EL_HIR_EXPR_CALL,
} ElHirExprKind;

typedef struct ElHirExprNode {
    ElHirExprKind kind;
    ElType* type;
    union {
        ElHirBinExprNode binary;
        ElHirUnaryExprNode unary;
        ElHirLiteral literal;
        ElHirSymbol* symbol;
        ElHirCallExprNode call;
    } as;
} ElHirExprNode;

ElHirExprNode* el_hir_new_symbol_expr(ElDynArena* arena, ElType* type, ElHirSymbol* symbol);

