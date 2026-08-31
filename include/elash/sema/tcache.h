#pragma once

#include <elash/hir/type.h>
#include <elash/mir/type.h>
#include <elash/util/ghm.h>
#include <elash/util/dynarena.h>
#include <stdint.h>

#include <elash/sema/backends.h>

typedef struct ElTypeCache {
    ElGHM hir_to_mir; // HIR Type -> MIR Type
    ElGHM mir_to_bst; // MIR Type -> BS Type

    ElMirType* usize_type;
    ElMirType* bool_type;

    ElDynArena* arena;

    ElBSQuery* query;
} ElTypeCache;

// Function declarations
void el_tcache_init(ElTypeCache* cache, ElDynArena* arena, ElBSQuery* query);
void el_tcache_free(ElTypeCache* cache);

ElMirType* el_tcache_get_mir(ElTypeCache* cache, const ElHirType* htype);

ElBSType* el_tcache_get_bst_from_hir(ElTypeCache* cache, const ElHirType* htype);
ElBSType* el_tcache_get_bst_from_mir(ElTypeCache* cache, const ElMirType* mtype);
