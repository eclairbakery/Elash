#pragma once

#include <elash/defs/inttypes.h>
#include <stdio.h>

typedef struct ElHirTopLevelNode ElHirTopLevelNode;

void el_hir_dump_toplevel(ElHirTopLevelNode* node, usize indent, FILE* out);
