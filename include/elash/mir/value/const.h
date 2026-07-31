#pragma once

#include <elash/util/dynarena.h>
#include <elash/mir/type.h>
#include <elash/defs/sv.h>

typedef struct ElMirValue ElMirValue;
typedef struct ElMirConstant ElMirConstant;

typedef enum ElMirConstKind {
    EL_MIR_CONST_AGG,
    EL_MIR_CONST_INT,
    EL_MIR_CONST_FLOAT,
    EL_MIR_CONST_STRING,
} ElMirConstKind;

typedef struct ElMirAggConst {
    ElMirConstant** elements;
    usize count;
} ElMirAggConst;

typedef struct ElMirStrConst {
    ElStringView val;
} ElMirStrConst;

typedef struct ElMirConstant {
    ElMirConstKind kind;
    union {
        ElMirAggConst agg;
        ElMirStrConst str;

        int64_t int_;
        double float_;
    } as;
} ElMirConstant;

typedef ElMirConstant ElMirConstValue;
ElMirValue* el_mir_new_const(ElDynArena* arena, ElMirType* type, ElMirConstant constant);
