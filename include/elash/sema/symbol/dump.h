#pragma once

#include <stdio.h>

typedef struct ElSymbol ElSymbol;

void el_sema_dump_symbol(ElSymbol* symbol, FILE* out);
