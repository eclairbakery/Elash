#pragma once

#include <stdio.h>

typedef struct ElHirSymbol ElHirSymbol;

void el_hir_dump_symbol(ElHirSymbol* symbol, FILE* out);
