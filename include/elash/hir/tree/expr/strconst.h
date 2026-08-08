#pragma once

#include <elash/source/span.h>
#include <elash/util/dynarena.h>
#include <elash/defs/int-types.h>
#include <elash/sema/storage-cls.h>

typedef struct ElHirExpr ElHirExpr;
typedef struct ElHirType ElHirType;

typedef struct ElHirStringConst {
    ElStorageClass scls;
    ElStringView chars;
} ElHirStringConst;

ElHirExpr* el_hir_new_string_const(
    ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElStringView chars, ElStorageClass scls
);
