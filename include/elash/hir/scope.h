#pragma once

#include <elash/defs/int-types.h>
#include <elash/defs/sv.h>

#include <elash/hir/symbol.h>

typedef struct ElHirScope ElHirScope;
struct ElHirScope {
    ElHirScope* parent;

    ElHirSymbol** entries;
    usize capacity;
    usize count;
};

ElHirScope* el_hir_scope_new(ElHirScope* parent);
void el_hir_scope_free(ElHirScope* scope);

bool el_hir_scope_insert(ElHirScope* scope, ElHirSymbol* symbol);
ElHirSymbol* el_hir_scope_lookup(ElHirScope* scope, ElStringView name);
ElHirSymbol* el_hir_scope_lookup_local(ElHirScope* scope, ElStringView name);
