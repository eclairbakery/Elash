#pragma once

#include <elash/defs/sv.h>
#include <elash/util/strbuf.h>
#include <elash/lexer/tokbuf.h>

#include <elash/pp/valarr.h>

typedef enum ElPpValueType {
    EL_PP_VAR_INT,
    EL_PP_VAR_FLOAT,
    EL_PP_VAR_BOOL,
    EL_PP_VAR_CHAR,
    EL_PP_VAR_STRING,
    EL_PP_VAR_ARRAY,
    EL_PP_VAR_TOKEN,
} ElPpValueType;

typedef struct ElPpValue {
    // TODO: use big ints / big floats
    union {
        int64_t      int_;   // EL_PP_VAR_INT
        double       float_; // EL_PP_VAR_FLOAT
        bool         bool_;  // EL_PP_VAR_BOOL
        char         char_;  // EL_PP_VAR_CHAR
        ElStringBuf  str_;   // EL_PP_VAR_STRING
        ElPpValueArr arr_;   // EL_PP_VAR_ARRAY
        ElToken      tok_;   // EL_PP_VAR_TOKENS
    } as;
    ElPpValueType type;
} ElPpValue;

/// @brief Frees any dynamically allocated resources held by an ElPpValue.
void el_pp_value_free(ElPpValue* val);
