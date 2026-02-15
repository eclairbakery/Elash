#pragma once

#include <el-defs/inttypes.h>

#include <stdbool.h>

typedef struct ElPpValue ElPpValue;
typedef struct ElPpValueArr {
    ElPpValue* data;
    usize num_elements;
    usize cap;
} ElPpValueArr;

/// @brief Initializes an ElPpValueArr (dynamic array) structure.
/// @param valarr A pointer to the ElPpValueArr structure to initialize.
/// @return True if initialization is successful.
bool el_pp_valarr_init(ElPpValueArr* valarr);
/// @brief Destroys an ElPpValueArr and frees its allocated resources.
/// @param valarr A pointer to the ElPpValueArr structure to destroy.
void el_pp_valarr_destroy(ElPpValueArr* valarr);

/// @brief Copies the contents of one ElPpValueArr to another.
/// @param src A pointer to the source ElPpValueArr.
/// @param dst A pointer to the destination ElPpValueArr.
/// @return True if the copy is successful, false otherwise.
bool el_pp_valarr_copy(const ElPpValueArr* src, ElPpValueArr* dst);
/// @brief Moves the contents of one ElPpValueArr to another.
/// @param src A pointer to the source ElPpValueArr, which will be invalidated after the move.
/// @param dst A pointer to the destination ElPpValueArr.
void el_pp_valarr_move(ElPpValueArr* src, ElPpValueArr* dst);

/// @brief Adds a copy of an ElPpValue to the end of the array.
/// @param valarr A pointer to the ElPpValueArr structure.
/// @param var A pointer to the ElPpValue to be copied and added.
/// @return True if the push is successful, false otherwise.
bool el_pp_valarr_push(ElPpValueArr* valarr, const ElPpValue* val);
/// @brief Moves an ElPpValue to the end of the array.
/// @param valarr A pointer to the ElPpValueArr structure.
/// @param var A pointer to the ElPpValue to be moved and added.
/// @return True if the move push is successful, false otherwise.
bool el_pp_valarr_move_push(ElPpValueArr* valarr, ElPpValue* val);

/// @brief Clears all elements from the array, freeing their resources but not the array's capacity.
/// @param valarr A pointer to the ElPpValueArr structure.
void el_pp_valarr_clear(ElPpValueArr* valarr);
