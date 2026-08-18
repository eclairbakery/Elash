#pragma once

#include <elash/defs/int-types.h>

#include <stdbool.h>

typedef struct ElPpValue ElPpValue;
typedef struct ElPpValueArr {
    ElPpValue** data;
    usize count;
    usize cap;
} ElPpValueArr;

bool el_pp_valarr_init(ElPpValueArr* valarr);
void el_pp_valarr_free(ElPpValueArr* valarr);

/// @brief Adds a value to the end of the array.
bool el_pp_valarr_push(ElPpValueArr* valarr, ElPpValue* val);
/// @brief Clears all elements from the array, freeing their resources but not the array's capacity.
void el_pp_valarr_clear(ElPpValueArr* valarr);
