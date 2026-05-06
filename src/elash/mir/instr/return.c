#include <elash/mir/instr.h>

ElMirInstr* el_mir_new_ret_instr(ElDynArena* arena, ElMirValue* ret_val) {
    ElMirInstr* instr = EL_DYNARENA_NEW(arena, ElMirInstr);
    *instr = (ElMirInstr) {
        .kind = EL_MIR_INSTR_RET,
        .result = NULL,
        .as.return_ = {
            .value = ret_val,
        },
        .next = NULL,
        .prev = NULL,
    };
    return instr;
}
