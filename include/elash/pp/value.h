#pragma once

#include <elash/defs/sv.h>
#include <elash/util/strbuf.h>
#include <elash/lexer/tokbuf.h>

typedef enum ElPpType {
    EL_PP_TYPE_INT,
    EL_PP_TYPE_BOOL,
    EL_PP_TYPE_FLOAT,
    EL_PP_TYPE_CHAR,
    EL_PP_TYPE_LIST,
    EL_PP_TYPE_STR,
    EL_PP_TYPE_TOK,
} ElPpType;

typedef struct ElPpValue ElPpValue;

typedef struct ElPpList {
    ElPpValue** values;
    usize count;
} ElPpList;

typedef struct ElPpValue {
    // TODO: use big ints / big floats
    union {
        int64_t      int_;   // EL_PP_TYPE_INT
        double       float_; // EL_PP_TYPE_FLOAT
        bool         bool_;  // EL_PP_TYPE_BOOL
        char         char_;  // EL_PP_TYPE_CHAR
        ElPpList     list_;  // EL_PP_TYPE_LIST
        ElStringView str_;   // EL_PP_TYPE_STRING
        ElToken      tok_;   // EL_PP_TYPE_TOKENS
    } as;
    ElPpType type;
} ElPpValue;
