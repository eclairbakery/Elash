#pragma once

#include <elash/hir/tree/expr/literal.h>

typedef struct ElMirConstValue {
    // TODO: this probably should not use HIR types
    ElHirLiteral lit;
} ElMirConstValue;
