#pragma once

#include <elash/defs/sv.h>
#include <elash/util/strbuf.h>
#include <elash/lexer/tokbuf.h>

typedef enum ElPpValueType {
    EL_PP_VAR_INT,
    EL_PP_VAR_BOOL,
    EL_PP_VAR_FLOAT,
    EL_PP_VAR_CHAR,
    EL_PP_VAR_LIST,
    EL_PP_VAR_STR,
    EL_PP_VAR_TOK,
} ElPpValueType;

typedef struct ElPpValue ElPpValue;

typedef struct ElPpList {
    ElPpValue** values;
    usize count;
} ElPpList;

typedef struct ElPpValue {
    // TODO: use big ints / big floats
    union {
        int64_t      int_;   // EL_PP_VAR_INT
        double       float_; // EL_PP_VAR_FLOAT
        bool         bool_;  // EL_PP_VAR_BOOL
        char         char_;  // EL_PP_VAR_CHAR
        ElPpList     list_;  // EL_PP_VAR_LIST
        ElStringView str_;   // EL_PP_VAR_STRING
        ElToken      tok_;   // EL_PP_VAR_TOKENS
    } as;
    ElPpValueType type;
} ElPpValue;
