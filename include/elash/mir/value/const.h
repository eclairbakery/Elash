#pragma once

#include <elash/util/dynarena.h>
#include <elash/mir/type.h>
#include <elash/defs/sv.h>

typedef struct ElMirValue ElMirValue;
typedef struct ElMirConstant ElMirConstant;

typedef enum ElMirConstKind {
    EL_MIR_CONST_INT,
    EL_MIR_CONST_FLOAT,
    EL_MIR_CONST_ARRAY,
    EL_MIR_CONST_STRING,
} ElMirConstKind;

typedef struct ElMirArrConst {
    ElMirConstant** elements;
    usize count;
} ElMirArrConst;

typedef struct ElMirStrConst {
    ElStringView val;
} ElMirStrConst;

typedef struct ElMirConstant {
    ElMirConstKind kind;
    union {
        ElMirArrConst array;
        ElMirStrConst string;

        int64_t int_;
        double float_;
    } as;
} ElMirConstant;

typedef ElMirConstant ElMirConstValue;
ElMirValue* el_mir_new_const(ElDynArena* arena, ElMirType* type, ElMirConstant constant);
