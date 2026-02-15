#pragma once

#include <el-defs/sv.h>
#include <el-pp/vararr.h>

typedef enum ElPpVarType {
    EL_PP_VAR_INT,
    EL_PP_VAR_FLOAT,
    EL_PP_VAR_BOOL,
    EL_PP_VAR_CHAR,
    EL_PP_VAR_STRING,
    EL_PP_VAR_ARRAY,
} ElPpVarType;

/// @brief Checks if a given ElPpVarType is a trivial type.
/// @param type The ElPpVarType to check.
/// @return True if the type is trivial (e.g., int, float, bool, char, string view), false otherwise.
bool el_pp_var_type_is_trivial(ElPpVarType type);

typedef union ElPpVarValue {
    int i;              // EL_PP_VAR_INT
    float f;            // EL_PP_VAR_FLOAT
    bool b;             // EL_PP_VAR_BOOL
    char c;             // EL_PP_VAR_CHAR
    ElStringView s;     // EL_PP_VAR_STRING
    ElPpVarArr a;       // EL_PP_VAR_ARRAY
} ElPpVarValue;

typedef struct ElPpVar {
    ElStringView name;
    ElPpVarType type;
    ElPpVarValue value;
} ElPpVar;

/// @brief Frees any dynamically allocated resources held by an ElPpVar.
/// @param var A pointer to the ElPpVar whose resources are to be freed.
void el_pp_var_free(ElPpVar* var);
/// @brief Moves the content of a source ElPpVar to a destination ElPpVar.
/// @param src A pointer to the source ElPpVar, which will be invalidated for non-trivial types.
/// @param dst A pointer to the destination ElPpVar.
void el_pp_var_move(ElPpVar* src, ElPpVar* dst);
/// @brief Creates a deep copy of a source ElPpVar into a destination ElPpVar.
/// @param src A pointer to the source ElPpVar to copy from.
/// @param dst A pointer to the destination ElPpVar to copy to.
/// @return True if the copy is successful, false otherwise (e.g., memory allocation failure for array types).
bool el_pp_var_copy(const ElPpVar* src, ElPpVar* dst);


