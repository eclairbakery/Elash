#pragma once

typedef enum ElPrimitiveTypeKind {
    // TODO: add more integer types
    EL_PRIMTYPE_INT,
    EL_PRIMTYPE_UINT,
    EL_PRIMTYPE_CHAR,
} ElPrimitiveTypeKind;

typedef struct ElPrimitiveType {
    ElPrimitiveTypeKind kind;
} ElPrimitiveType;
