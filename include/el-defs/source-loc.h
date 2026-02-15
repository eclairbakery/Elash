#pragma once

#include <el-defs/inttypes.h>

typedef struct ElSourceLocation {
    u32 line, column;
    u32 offset;
} ElSourceLocation;

#define EL_SOURCE_LOC_ZERO ((ElSourceLocation) { 0 })
