#pragma once

#include <elash/mir/value.h>
#include <elash/util/dynarena.h>
#include <stdint.h>

typedef struct ElMirInstr ElMirInstr;

typedef struct ElMirCallInstr {
    ElMirValue* callee;
    ElMirValue** args;
    uint32_t arg_count;
} ElMirCallInstr;

ElMirInstr* el_mir_new_call_instr(ElDynArena* arena, ElMirValue* result, ElMirValue* callee, ElMirValue** args, uint32_t arg_count);
