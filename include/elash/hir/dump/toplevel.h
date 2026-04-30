#pragma once

#include <elash/defs/int-types.h>
#include <stdio.h>

typedef struct ElHirTopLevelNode ElHirTopLevelNode;

void el_hir_dump_toplevel(ElHirTopLevelNode* node, usize indent, FILE* out);
