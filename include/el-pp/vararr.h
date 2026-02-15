#pragma once

#include <el-defs/inttypes.h>

#include <stdbool.h>

typedef struct ElPpVar ElPpVar;
typedef struct ElPpVarArr {
    ElPpVar* data;
    usize num_elements;
    usize cap;
} ElPpVarArr;

/// @brief Initializes an ElPpVarArr (dynamic array) structure.
/// @param vararr A pointer to the ElPpVarArr structure to initialize.
/// @return True if initialization is successful.
bool el_pp_vararr_init(ElPpVarArr* vararr);
/// @brief Destroys an ElPpVarArr and frees its allocated resources.
/// @param vararr A pointer to the ElPpVarArr structure to destroy.
void el_pp_vararr_destroy(ElPpVarArr* vararr);

/// @brief Copies the contents of one ElPpVarArr to another.
/// @param src A pointer to the source ElPpVarArr.
/// @param dst A pointer to the destination ElPpVarArr.
/// @return True if the copy is successful, false otherwise.
bool el_pp_vararr_copy(const ElPpVarArr* src, ElPpVarArr* dst);
/// @brief Moves the contents of one ElPpVarArr to another.
/// @param src A pointer to the source ElPpVarArr, which will be invalidated after the move.
/// @param dst A pointer to the destination ElPpVarArr.
void el_pp_vararr_move(ElPpVarArr* src, ElPpVarArr* dst);

/// @brief Adds a copy of an ElPpVar to the end of the array.
/// @param vararr A pointer to the ElPpVarArr structure.
/// @param var A pointer to the ElPpVar to be copied and added.
/// @return True if the push is successful, false otherwise.
bool el_pp_vararr_push(ElPpVarArr* vararr, const ElPpVar* var);
/// @brief Moves an ElPpVar to the end of the array.
/// @param vararr A pointer to the ElPpVarArr structure.
/// @param var A pointer to the ElPpVar to be moved and added.
/// @return True if the move push is successful, false otherwise.
bool el_pp_vararr_move_push(ElPpVarArr* vararr, ElPpVar* var);

/// @brief Clears all elements from the array, freeing their resources but not the array's capacity.
/// @param vararr A pointer to the ElPpVarArr structure.
void el_pp_vararr_clear(ElPpVarArr* vararr);
