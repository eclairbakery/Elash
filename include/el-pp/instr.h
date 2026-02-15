#pragma once

#include <el-lexer/token.h>

typedef enum ElPpInstr {
    EL_PP_INSTR_INCLUDE,
    EL_PP_INSTR_EMBED,

    EL_PP_INSTR_PRAGMA,
    EL_PP_INSTR_ANNOTE,
    EL_PP_INSTR_ERROR,
    EL_PP_INSTR_WARNING,
    EL_PP_INSTR_NOTE,

    EL_PP_INSTR_DEFINE,
    EL_PP_INSTR_UNDEF,

    EL_PP_INSTR_ASSIGN,
    EL_PP_INSTR_DEASSIGN,
    EL_PP_INSTR_INC,
    EL_PP_INSTR_DEC,

    EL_PP_INSTR_IF,
    EL_PP_INSTR_ELSE,
    EL_PP_INSTR_ELIF,
    EL_PP_INSTR_ENDIF,

    EL_PP_INSTR_WHILE,
    EL_PP_INSTR_ENDWHILE,

    EL_PP_INSTR_FOR,
    EL_PP_INSTR_FOREACH,
    EL_PP_INSTR_ENDFOR,
    
    EL_PP_INSTR_UNKNOWN,
} ElPpInstr;

ElPpInstr el_pp_extract_instr_from_token_type(ElTokenType tt);
