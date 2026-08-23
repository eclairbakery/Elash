#pragma once

#include <elash/defs/int-types.h>
#include <stdio.h>

typedef struct ElAstUnr ElAstUnr;

void el_ast_dump_unr(ElAstUnr* node, usize indent, FILE* out);
