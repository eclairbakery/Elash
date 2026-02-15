#pragma once

#include <el-defs/inttypes.h>

#include <stdbool.h>
#include <stdlib.h>


typedef struct _ElDynArenaChunk _ElDynArenaChunk;
struct _ElDynArenaChunk {
    _ElDynArenaChunk* next;
    usize size;
    uchar data[];
};

// TODO: STUB
typedef struct ElDynArena {
    _ElDynArenaChunk* head;
    _ElDynArenaChunk* current;
} ElDynArena;

//bool el_dynarena_init(ElDynArena* arena);
//void el_dynarena_free(ElDynArena* arena);
//void el_dynarena_reset(ElDynArena* arena);
//
//void* el_dynarena_alloc(ElDynArena* arena, usize size, usize align);
//void* el_dynarena_alloc_zeroed(ElDynArena* arena, usize size, usize align);

static inline bool el_dynarena_init(ElDynArena* _) { return true; }
static inline void el_dynarena_free(ElDynArena* _) {}
static inline void el_dynarena_reset(ElDynArena* _) {}

static inline void* el_dynarena_alloc(ElDynArena* _, usize size, usize _a) { return malloc(size); }
static inline void* el_dynarena_alloc_zeroed(ElDynArena* _, usize size, usize _a) { return calloc(1, size); }

#define EL_DYNARENA_NEW(ARENA, TYPE) \
    (TYPE*)(el_dynarena_alloc((ARENA), sizeof(TYPE), alignof(TYPE)))

#define EL_DYNARENA_NEW_ARR(ARENA, TYPE, SIZE) \
    (TYPE*)(el_dynarena_alloc((ARENA), sizeof(TYPE) * (SIZE), alignof(TYPE)))

#define EL_DYNARENA_NEW_ZEROED(ARENA, TYPE) \
    (TYPE*)(el_dynarena_alloc_zeroed((ARENA), sizeof(TYPE), alignof(TYPE)))

#define EL_DYNARENA_NEW_ARR_ZEROED(ARENA, TYPE, SIZE) \
     (TYPE*)(el_dynarena_alloc_zeroed((ARENA), sizeof(TYPE) * (SIZE), alignof(TYPE)))
