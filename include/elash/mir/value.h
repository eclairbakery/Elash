#pragma once

#include <elash/util/dynarena.h>
#include <elash/sema/type.h>

#include "value/global.h"
#include "value/const.h"
#include "value/reg.h"
#include "value/arg.h"

typedef enum ElMirValueKind {
    EL_MIR_VAL_REG,
    EL_MIR_VAL_CONST,
    EL_MIR_VAL_ARG,
    EL_MIR_VAL_GLOBAL,
} ElMirValueKind;

typedef struct ElMirValue {
    ElMirValueKind kind;
    ElType* type;
    union {
        ElMirRegValue    reg;
        ElMirArgValue    arg;
        ElMirConstValue  constant;
        ElMirGlobalValue global;
    } as;
} ElMirValue;

ElMirValue* el_mir_new_reg(ElDynArena* arena, ElType* type, uint32_t reg_id);
ElMirValue* el_mir_new_const(ElDynArena* arena, ElType* type, ElHirLiteral constant);
ElMirValue* el_mir_new_arg(ElDynArena* arena, ElType* type, uint32_t arg_idx);
ElMirValue* el_mir_new_global(ElDynArena* arena, ElType* type, ElHirSymbol* global);
