#pragma once

#include <elash/defs/inttypes.h>
#include <stdio.h>

typedef struct ElHirModule ElHirModule;

void el_hir_dump_module(ElHirModule* module, usize indent, FILE* out);
