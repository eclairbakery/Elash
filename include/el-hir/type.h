#pragma once

typedef enum ElHirTypeKind {
    // TODO: add more integer types
    EL_HIR_TYPE_INT,
    EL_HIR_TYPE_UINT,
    EL_HIR_TYPE_CHAR,

    EL_HIR_TYPE_PTR,
} ElHirTypeKind;

typedef struct ElHirType ElHirType;
struct ElHirType {
    ElHirTypeKind kind;
    union {
        struct {
            ElHirType* base;
        } ptr;
    } as;
};
