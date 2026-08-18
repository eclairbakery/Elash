#include <elash/pp/scope.h>
#include <elash/pp/valarr.h>

#include <elash/defs/int-types.h>
#include <elash/util/strhash.h>

#include <stdlib.h>

typedef struct _ElPpScopeEntry Entry;

#define LOAD_FACTOR_GROW   0.7
#define LOAD_FACTOR_SHRINK 0.2
#define MIN_CAPACITY 8

static usize next_power_of_two(usize x) {
    usize power = 1;
    while (power < x) power <<= 1;
    return power;
}

bool resize(ElPpScope* vars, usize new_capacity) {
    new_capacity = next_power_of_two(new_capacity);
    if (new_capacity < MIN_CAPACITY)
        new_capacity = MIN_CAPACITY;

    Entry* old_entries = vars->entries;
    usize old_capacity = vars->capacity;

    Entry* new_entries = calloc(new_capacity, sizeof(Entry));
    if (new_entries == NULL)
        return false;

    vars->entries = new_entries;
    vars->capacity = new_capacity;
    vars->num_entries = 0;
    vars->num_tombstones = 0;

    for (usize i = 0; i < old_capacity; ++i) {
        if (old_entries[i].state == _EL_PP_VARS_OCCUPIED) {
           el_pp_scope_assign(vars, old_entries[i].key, old_entries[i].value);
        }
    }

    free(old_entries);
    return true;
}

static bool ensure_capacity_for_new_var(ElPpScope* vars) {
    double load = (double)(vars->num_entries + vars->num_tombstones) / (double)vars->capacity;
    if (load >= LOAD_FACTOR_GROW) {
        return resize(vars, vars->capacity * 2);
    }
    return true;
}

static void maybe_shrink(ElPpScope* vars) {
    double load = (double)vars->num_entries / (double)vars->capacity;
    if (vars->capacity > MIN_CAPACITY && load <= LOAD_FACTOR_SHRINK) {
        resize(vars, vars->capacity / 2);
    }
}

static Entry* find_slot(ElPpScope* vars, ElStringView key, bool* found) {
    usize index = el_hash_string(key) & (vars->capacity - 1);
    Entry* first_tombstone = NULL;

    for (;;) {
        Entry* entry = &vars->entries[index];

        if (entry->state == _EL_PP_VARS_EMPTY) {
            *found = false;
            return first_tombstone != NULL ? first_tombstone : entry;
        }

        if (entry->state == _EL_PP_VARS_TOMBSTONE) {
            if (first_tombstone == NULL)
                first_tombstone = entry;
        } else if (el_sv_eql(entry->key, key)) {
            *found = true;
            return entry;
        }

        index = (index + 1) & (vars->capacity - 1);
    }
}


bool el_pp_scope_init(ElPpScope* vars, ElPpScope* parent, usize initial_capacity) {
    if (!vars) return false;

    initial_capacity = next_power_of_two(initial_capacity);
    if (initial_capacity < MIN_CAPACITY)
        initial_capacity = MIN_CAPACITY;

    vars->entries = calloc(initial_capacity, sizeof(Entry));
    if (!vars->entries)
        return false;

    vars->capacity = initial_capacity;
    vars->num_entries = 0;
    vars->num_tombstones = 0;
    vars->parent = parent;

    return true;
}

void el_pp_scope_destroy(ElPpScope* vars) {
    if (vars == NULL || vars->entries == NULL)
        return;

    free(vars->entries);
    vars->entries = NULL;
    vars->capacity = 0;
    vars->num_entries = 0;
    vars->num_tombstones = 0;
}

bool el_pp_scope_assign(ElPpScope* vars, ElStringView key, ElPpSymbol* value) {
    if (!ensure_capacity_for_new_var(vars))
        return false;

    bool found;
    Entry* slot = find_slot(vars, key, &found);

    if (found) {
        slot->value = value;
        return true;
    }

    if (slot->state == _EL_PP_VARS_TOMBSTONE)
        vars->num_tombstones--;

    slot->key = key;
    slot->value = value;
    slot->state = _EL_PP_VARS_OCCUPIED;
    vars->num_entries++;

    return true;
}

bool el_pp_scope_deassign(ElPpScope* vars, ElStringView key) {
    bool found;
    Entry* slot = find_slot(vars, key, &found);

    if (!found)
        return false;

    slot->state = _EL_PP_VARS_TOMBSTONE;
    vars->num_entries--;
    vars->num_tombstones++;

    maybe_shrink(vars);
    return true;
}

ElPpSymbol* el_pp_scope_lookup_local(ElPpScope* vars, ElStringView key) {
    bool found;
    Entry* slot = find_slot(vars, key, &found);
    if (!found)
        return NULL;

    return slot->value;
}

ElPpSymbol* el_pp_scope_lookup(ElPpScope* vars, ElStringView key) {
    ElPpSymbol* var = el_pp_scope_lookup_local(vars, key);
    if (var != NULL)
        return var;
    if (vars->parent != NULL)
        return el_pp_scope_lookup(vars->parent, key);
    return NULL;
}

bool el_pp_scope_has(ElPpScope* vars, ElStringView key) {
    bool found;
    find_slot(vars, key, &found);
    return found;
}
