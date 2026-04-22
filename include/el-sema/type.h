#pragma once

typedef enum ElTypeKind {
    // TODO: add more integer types
    EL_TYPE_INT,
    EL_TYPE_UINT,
    EL_TYPE_CHAR,

    EL_TYPE_PTR,
} ElTypeKind;

typedef struct ElType ElType;
struct ElType {
    ElTypeKind kind;
    union {
        struct {
            ElType* base;
        } ptr;
    } as;
};
