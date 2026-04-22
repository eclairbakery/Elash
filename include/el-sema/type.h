#pragma once

#include "type/prim.h"
#include "type/ptr.h"

typedef enum ElTypeKind {
    EL_TYPE_PRIM,
    EL_TYPE_PTR,
} ElTypeKind;

typedef struct ElType ElType;
struct ElType {
    ElTypeKind kind;
    union {
        ElPrimitiveType prim;
        ElPointerType ptr;
    } as;
};
