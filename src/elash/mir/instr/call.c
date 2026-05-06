#include <elash/mir/instr.h>

ElMirInstr* el_mir_new_call_instr(ElDynArena* arena, ElMirValue* result, ElMirValue* callee, ElMirValue** args, uint32_t arg_count) {
    ElMirInstr* instr = EL_DYNARENA_NEW(arena, ElMirInstr);
    *instr = (ElMirInstr) {
        .kind = EL_MIR_INSTR_CALL,
        .result = result,
        .as.call = {
            .callee = callee,
            .args = args,
            .arg_count = arg_count,
        },
        .next = NULL,
        .prev = NULL,
    };
    return instr;
}
