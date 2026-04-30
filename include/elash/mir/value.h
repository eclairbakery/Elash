#pragma once

#include <elash/sema/type.h>
#include <elash/hir/tree/expr/literal.h>
#include <elash/hir/symbol.h>
#include <elash/util/dynarena.h>
#include <elash/defs/int-types.h>

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
        struct { uint32_t id; } reg;
        ElHirLiteral constant;
        struct { uint32_t idx; } arg;
        ElHirSymbol* global;
    } as;
} ElMirValue;

ElMirValue* el_mir_new_reg(ElDynArena* arena, ElType* type, uint32_t reg_id);
ElMirValue* el_mir_new_const(ElDynArena* arena, ElType* type, ElHirLiteral constant);
ElMirValue* el_mir_new_arg(ElDynArena* arena, ElType* type, uint32_t arg_idx);
ElMirValue* el_mir_new_global(ElDynArena* arena, ElType* type, ElHirSymbol* global);
