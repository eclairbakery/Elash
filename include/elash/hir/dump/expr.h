#pragma once

#include <elash/defs/int-types.h>
#include <stdio.h>

typedef struct ElHirExprNode ElHirExprNode;

void el_hir_dump_expr(ElHirExprNode* node, usize indent, FILE* out);
