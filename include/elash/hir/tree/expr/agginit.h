#pragma once

#include <elash/util/dynarena.h>
#include <elash/srcdoc/span.h>
#include <elash/defs/int-types.h>
#include <elash/sema/storage-cls.h>

typedef struct ElHirExpr ElHirExpr;
typedef struct ElHirType ElHirType;

typedef struct ElHirAggInit {
    ElStorageClass scls;
    ElHirExpr** values;
    usize count;
} ElHirAggInit;

ElHirExpr* el_hir_new_agg_init(
    ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElHirExpr** values, usize count, ElStorageClass scls
);
