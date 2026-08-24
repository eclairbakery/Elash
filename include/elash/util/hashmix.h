#pragma once
#include <stdint.h>

#define EL_HASH_MIX 0x9e3779b97f4a7c15ULL
static inline uint64_t el_hash_mix(uint64_t h, uint64_t val) {
    // NOLINTNEXTLINE(readability-magic-numbers)
    return h ^ (val + EL_HASH_MIX + (h << 6) + (h >> 2));
}
