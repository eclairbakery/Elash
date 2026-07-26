#pragma once

#include <elash/srcdoc/span.h>
#include <elash/util/dynarena.h>
#include <elash/defs/int-types.h>
#include <elash/sema/storage-cls.h>

typedef struct ElHirExpr ElHirExpr;
typedef struct ElHirType ElHirType;

typedef struct ElHirStringLit {
    ElStorageClass scls;
    ElStringView chars;
} ElHirStringLit;

ElHirExpr* el_hir_new_string_lit(
    ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElStringView chars, ElStorageClass scls
);
