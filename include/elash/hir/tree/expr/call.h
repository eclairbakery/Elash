#pragma once

#include <elash/util/dynarena.h>
#include <elash/defs/int-types.h>

typedef struct ElHirExprNode ElHirExprNode;
typedef struct ElType ElType;

typedef struct ElHirCallExprNode {
    ElHirExprNode* callee;
    ElHirExprNode** args;
    usize arg_count;
} ElHirCallExprNode;

ElHirExprNode* el_hir_new_call_expr(
    ElDynArena* arena, ElType* type, ElHirExprNode* callee, ElHirExprNode** args, usize arg_count
);
