#pragma once

#include <el-pp/valarr.h>
#include <el-pp/value.h>

#include <el-defs/sv.h>

#include <stdbool.h>

struct _ElPpVarsEntry {
    ElStringView key; 
    ElPpVar value;
    enum {
        _EL_PP_VARS_EMPTY = 0,
        _EL_PP_VARS_OCCUPIED,
        _EL_PP_VARS_TOMBSTONE
    } state;
};
typedef struct ElPpVars {
    struct _ElPpVarsEntry* entries;
    usize capacity;
    usize num_entries;
    usize num_tombstones;
} ElPpVars;

/// @brief Initializes an ElPpVars hash map.
/// @param vars A pointer to the ElPpVars structure to initialize.
/// @param initial_capacity The initial desired capacity for the hash map.
/// @return True if initialization is successful, false otherwise.
bool el_pp_vars_init(ElPpVars* vars, usize initial_capacity);
/// @brief Destroys an ElPpVars hash map and frees its resources.
/// @param vars A pointer to the ElPpVars structure to destroy.
void el_pp_vars_destroy(ElPpVars* vars);

/// @brief Assigns a copy of a variable to a key in the hash map.
/// @param vars A pointer to the ElPpVars structure.
/// @param key The key (string view) to assign the value to.
/// @param value A pointer to the ElPpVar to copy and assign.
/// @return True if assignment is successful, false otherwise.
bool el_pp_vars_assign(ElPpVars* vars, ElStringView key, const ElPpVar* value);
/// @brief Moves a variable to a key in the hash map.
/// @param vars A pointer to the ElPpVars structure.
/// @param key The key (string view) to assign the value to.
/// @param value A pointer to the ElPpVar to move and assign.
/// @return True if assignment is successful, false otherwise.
bool el_pp_vars_assign_move(ElPpVars* vars, ElStringView key, ElPpVar* value);
/// @brief Removes a key-value pair from the hash map.
/// @param vars A pointer to the ElPpVars structure.
/// @param key The key (string view) to deassign.
/// @return True if deassignment is successful, false otherwise.
bool el_pp_vars_deassign(ElPpVars* vars, ElStringView key);

/// @brief Retrieves a copy of the variable associated with a key.
/// @param vars A pointer to the ElPpVars structure.
/// @param key The key (string view) to look up.
/// @param out A pointer to an ElPpVar where the copied value will be stored.
/// @return True if the key is found and the value is copied, false otherwise.
bool el_pp_vars_get(ElPpVars* vars, ElStringView key, ElPpVar* out);
/// @brief Checks if a key exists in the hash map.
/// @param vars A pointer to the ElPpVars structure.
/// @param key The key (string view) to check for.
/// @return True if the key is found, false otherwise.
bool el_pp_vars_has(ElPpVars* vars, ElStringView key);

/// @brief Returns the number of elements in the hash map.
/// @param vars A pointer to the constant ElPpVars structure.
/// @return The number of elements currently stored in the hash map.
usize el_pp_vars_size(const ElPpVars* vars);

////// IMPLEMENTATION DETAILS //////////////////////
// note: those functions are implementation
// details and should not be called directly
// outside internal el-pp implementation.
bool _el_pp_vars_resize(ElPpVars* vars, usize new_capacity);
bool _el_pp_vars_ensure_capacity_for_new_var(ElPpVars* vars);
void _el_pp_vars_maybe_shrink(ElPpVars* vars);
struct _ElPpVarsEntry*
_el_pp_vars_find_slot(ElPpVars* vars, ElStringView key, bool *found);
////////////////////////////////////////////////////
