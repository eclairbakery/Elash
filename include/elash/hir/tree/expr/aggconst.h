#pragma once

#include <elash/util/dynarena.h>
#include <elash/srcdoc/span.h>
#include <elash/defs/int-types.h>
#include <elash/sema/storage-cls.h>

typedef struct ElHirExpr ElHirExpr;
typedef struct ElHirType ElHirType;

typedef struct ElHirAggConst {
    ElStorageClass scls;
    ElHirExpr** values;
    usize count;
} ElHirAggConst;

ElHirExpr* el_hir_new_agg_const(
    ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElHirExpr** values, usize count, ElStorageClass scls
);
