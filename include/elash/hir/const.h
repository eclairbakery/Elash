#pragma once

#include <elash/util/int128.h>
#include <stdbool.h>

typedef struct ElHirConstant {
    // tagged by the type field of Expr
    union {
        ElInt128 int_;
        bool     bool_;
        double   float_; // TODO: use arbitrary precision floats here
    } as;
} ElHirConstant;
