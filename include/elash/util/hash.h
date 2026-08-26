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

#define EL_PTR_ALIGN_SHIFT  ((sizeof(void*) >= 8U) ? 3U : 2U)
#define EL_GOLDEN_RATIO_PTR ((sizeof(void*) >= 8U) ? 0x9e3779b97f4a7c15ULL : 0x9e3779b9U)

static inline uhash el_hash_ptr(const void* ptr) {
    return (uhash)(((uintptr_t)ptr >> EL_PTR_ALIGN_SHIFT) * (uintptr_t)EL_GOLDEN_RATIO_PTR);
}

#define EL_HASH_MIX 0x9e3779b97f4a7c15ULL
static inline uhash el_hash_mix(uhash h, uhash val) {
    // NOLINTNEXTLINE(readability-magic-numbers)
    return h ^ (val + EL_HASH_MIX + (h << 6) + (h >> 2));
}
