#pragma once

#include <elash/util/dynarena.h>
#include <elash/mir/value.h>

#include "instr/binary.h"
#include "instr/unary.h"
#include "instr/return.h"
#include "instr/call.h"

typedef enum ElMirInstrKind {
    EL_MIR_INSTR_BIN,
    EL_MIR_INSTR_UNARY,
    EL_MIR_INSTR_RET,
    EL_MIR_INSTR_CALL,
} ElMirInstrKind;

typedef struct ElMirInstr ElMirInstr;

struct ElMirInstr {
    ElMirInstrKind kind;
    ElMirValue* result; 
    
    union {
        ElMirBinInstr   bin;
        ElMirUnaryInstr unary;
        ElMirRetInstr   return_;
        ElMirCallInstr  call;
    } as;

    ElMirInstr* next;
    ElMirInstr* prev;
};
