#include <el-hir/scope.h>

#include <stdlib.h>

#define INITIAL_CAPACITY 16
#define LOAD_FACTOR 0.75

static ulong hash_name(ElStringView name) {
    ulong hash = 5381;
    for (usize i = 0; i < name.len; i++) {
        hash = ((hash << 5) + hash) + (uchar)name.data[i];
    }
    return hash;
}

ElHirScope* el_hir_scope_new(ElHirScope* parent) {
    ElHirScope* scope = malloc(sizeof(ElHirScope));
    if (!scope) return NULL;

    scope->parent = parent;
    scope->capacity = INITIAL_CAPACITY;
    scope->count = 0;
    scope->entries = calloc(scope->capacity, sizeof(ElHirSymbol*));
    
    if (!scope->entries) {
        free(scope);
        return NULL;
    }

    return scope;
}

void el_hir_scope_free(ElHirScope* scope) {
    if (!scope) return;
    free(scope->entries);
    free(scope);
}

static bool resize(ElHirScope* scope) {
    usize old_capacity = scope->capacity;
    ElHirSymbol** old_entries = scope->entries;

    scope->capacity *= 2;
    scope->entries = calloc(scope->capacity, sizeof(ElHirSymbol*));
    if (!scope->entries) {
        scope->entries = old_entries;
        scope->capacity = old_capacity;
        return false;
    }

    scope->count = 0;
    for (usize i = 0; i < old_capacity; i++) {
        if (old_entries[i]) {
            el_hir_scope_insert(scope, old_entries[i]);
        }
    }

    free(old_entries);
    return true;
}

bool el_hir_scope_insert(ElHirScope* scope, ElHirSymbol* symbol) {
    if ((double)scope->count / scope->capacity >= LOAD_FACTOR) {
        if (!resize(scope)) return false;
    }

    ulong hash = hash_name(symbol->name);
    usize index = hash % scope->capacity;

    while (scope->entries[index]) {
        if (el_sv_eql(scope->entries[index]->name, symbol->name)) {
            return false; // already exists
        }
        index = (index + 1) % scope->capacity;
    }

    scope->entries[index] = symbol;
    scope->count++;
    return true;
}

ElHirSymbol* el_hir_scope_lookup_local(ElHirScope* scope, ElStringView name) {
    if (scope->count == 0) return NULL;

    ulong hash = hash_name(name);
    usize index = hash % scope->capacity;

    while (scope->entries[index]) {
        if (el_sv_eql(scope->entries[index]->name, name)) {
            return scope->entries[index];
        }
        index = (index + 1) % scope->capacity;
    }

    return NULL;
}

ElHirSymbol* el_hir_scope_lookup(ElHirScope* scope, ElStringView name) {
    while (scope) {
        ElHirSymbol* symbol = el_hir_scope_lookup_local(scope, name);
        if (symbol) return symbol;
        scope = scope->parent;
    }
    return NULL;
}
