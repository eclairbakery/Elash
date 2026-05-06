#pragma once

#include <elash/mir/value.h>
#include <elash/util/dynarena.h>

typedef struct ElMirInstr ElMirInstr;

typedef struct ElMirRetInstr {
    ElMirValue* value;
} ElMirRetInstr;

ElMirInstr* el_mir_new_ret_instr(ElDynArena* arena, ElMirValue* ret_val);
