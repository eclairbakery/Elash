#include <elash/mir/instr.h>

ElMirInstr* el_mir_new_unary_instr(ElDynArena* arena, ElMirValue* result, ElSemaUnaryOp op, ElMirValue* operand) {
    ElMirInstr* instr = EL_DYNARENA_NEW(arena, ElMirInstr);
    *instr = (ElMirInstr) {
        .kind = EL_MIR_INSTR_UNARY,
        .result = result,
        .as.unary = {
            .op = op,
            .operand = operand,
        },
        .next = NULL,
        .prev = NULL,
    };
    return instr;
}
