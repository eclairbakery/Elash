#pragma once

#include <stdint.h>

typedef struct ElHirExprNode ElHirExprNode;

typedef struct ElHirLiteral {
    // tagged by the type field of ExprNode
    union {
        int64_t int_;
        uint64_t uint_;
        char char_;
    } as;
} ElHirLiteral;

