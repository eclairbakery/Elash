#pragma once

#include <elash/util/dynarena.h>
#include <elash/mir/type.h>

typedef struct ElMirValue ElMirValue;
typedef struct ElMirConstant ElMirConstant;

typedef struct ElMirArrConst {
    ElMirConstant** elements;
    usize count;
} ElMirArrConst;

typedef struct ElMirConstant {
    union {
        ElMirArrConst array;

        int64_t int_;
        double float_;
    } as;
} ElMirConstant;

typedef ElMirConstant ElMirConstValue;
ElMirValue* el_mir_new_const(ElDynArena* arena, ElMirType* type, ElMirConstant constant);
