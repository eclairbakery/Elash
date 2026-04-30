#pragma once

#include <elash/defs/int-types.h>

typedef struct ElSourceLocation {
    uint line, column;
    uint offset;
} ElSourceLocation;

#define EL_SOURCE_LOC_ZERO ((ElSourceLocation) { 0 })
