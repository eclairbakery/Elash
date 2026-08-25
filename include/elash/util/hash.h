#pragma once

#include <elash/defs/int-types.h>
#include <elash/defs/sv.h>

#define EL_FNV1A_OFFSET 14695981039346656037ULL
#define EL_FNV1A_PRIME  1099511628211ULL

static inline uhash el_hash_string(ElStringView s) {
    uhash hash = EL_FNV1A_OFFSET;
    for (const char* c = s.data; c < s.data + s.len; c++) {
        hash ^= (const unsigned char) *c;
        hash *= EL_FNV1A_PRIME;
    }
    return hash;
}

#define EL_HASH_MIX 0x9e3779b97f4a7c15ULL
static inline uhash el_hash_mix(uhash h, uhash val) {
    // NOLINTNEXTLINE(readability-magic-numbers)
    return h ^ (val + EL_HASH_MIX + (h << 6) + (h >> 2));
}
