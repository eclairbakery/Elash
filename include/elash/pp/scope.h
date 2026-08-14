#pragma once

#include <elash/pp/valarr.h>
#include <elash/pp/value.h>

#include <elash/defs/sv.h>

#include <stdbool.h>

struct _ElPpScopeEntry {
    ElStringView key;
    ElPpVar* value;
    enum {
        _EL_PP_VARS_EMPTY,
        _EL_PP_VARS_OCCUPIED,
        _EL_PP_VARS_TOMBSTONE,
    } state;
};

typedef struct ElPpScope ElPpScope;
struct ElPpScope {
    struct _ElPpScopeEntry* entries;
    usize capacity;
    usize num_entries;
    usize num_tombstones;
    ElPpScope* parent;
};

/// @brief Initializes an ElPpScope hash map.
bool el_pp_scope_init(ElPpScope* vars, ElPpScope* parent, usize initial_capacity);
/// @brief Destroys an ElPpScope hash map and frees its resources.
void el_pp_scope_destroy(ElPpScope* vars);

/// @brief Assigns a pointer to a variable to a key in the hash map.
/// @return True if assignment is successful, false otherwise.
bool el_pp_scope_assign(ElPpScope* vars, ElStringView key, ElPpVar* value);
/// @brief Removes a key-value pair from the hash map (does not free the value).
bool el_pp_scope_deassign(ElPpScope* vars, ElStringView key);

/// @brief Retrieves the pointer to the variable associated with a key from this scope only.
ElPpVar* el_pp_scope_lookup_local(ElPpScope* vars, ElStringView key);
/// @brief Retrieves the pointer to the variable associated with a key from this scope or its parents.
ElPpVar* el_pp_scope_lookup(ElPpScope* vars, ElStringView key);
/// @brief Checks if a key exists in the hash map.
bool el_pp_scope_has(ElPpScope* vars, ElStringView key);
