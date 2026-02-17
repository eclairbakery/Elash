#include <el-pp/instr.h>

ElPpInstr el_pp_extract_instr_from_token_type(ElTokenType tt) {
    switch (tt) {
        case EL_TT_PP_INCLUDE:  return EL_PP_INSTR_INCLUDE;
        case EL_TT_PP_EMBED:    return EL_PP_INSTR_EMBED;

        case EL_TT_PP_PRAGMA:   return EL_PP_INSTR_PRAGMA;
        case EL_TT_PP_ANNOTE:   return EL_PP_INSTR_ANNOTE;
        case EL_TT_PP_ERROR:    return EL_PP_INSTR_ERROR;
        case EL_TT_PP_WARNING:  return EL_PP_INSTR_WARNING;
        case EL_TT_PP_NOTE:     return EL_PP_INSTR_NOTE;

        case EL_TT_PP_DEFINE:   return EL_PP_INSTR_DEFINE;
        case EL_TT_PP_ENDDEF:   return EL_PP_INSTR_ENDDEF;
        case EL_TT_PP_UNDEF:    return EL_PP_INSTR_UNDEF;

        case EL_TT_PP_ASSIGN:   return EL_PP_INSTR_ASSIGN;
        case EL_TT_PP_DEASSIGN: return EL_PP_INSTR_DEASSIGN;
        case EL_TT_PP_INC:      return EL_PP_INSTR_INC;
        case EL_TT_PP_DEC:      return EL_PP_INSTR_DEC;

        case EL_TT_PP_IF:       return EL_PP_INSTR_IF;
        case EL_TT_PP_ELSE:     return EL_PP_INSTR_ELSE;
        case EL_TT_PP_ELIF:     return EL_PP_INSTR_ELIF;
        case EL_TT_PP_ENDIF:    return EL_PP_INSTR_ENDIF;

        case EL_TT_PP_WHILE:    return EL_PP_INSTR_WHILE;
        case EL_TT_PP_ENDWHILE: return EL_PP_INSTR_ENDWHILE;

        case EL_TT_PP_FOR:      return EL_PP_INSTR_FOR;
        case EL_TT_PP_FOREACH:  return EL_PP_INSTR_FOREACH;
        case EL_TT_PP_ENDFOR:   return EL_PP_INSTR_ENDFOR;

        default: return EL_PP_INSTR_UNKNOWN;
    }
}

bool el_pp_is_instr_token_type(ElTokenType tt) {
    return el_pp_extract_instr_from_token_type(tt) != EL_PP_INSTR_UNKNOWN;
}
