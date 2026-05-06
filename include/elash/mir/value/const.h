#pragma once


#include <elash/util/dynarena.h>
#include <elash/sema/type.h>

#include <elash/hir/tree/expr/literal.h>

typedef struct ElMirValue ElMirValue;

typedef struct ElMirConstValue {
    // TODO: this probably should not use HIR types
    ElHirLiteral lit;
} ElMirConstValue;

ElMirValue* el_mir_new_const(ElDynArena* arena, ElType* type, ElHirLiteral constant);
