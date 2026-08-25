#pragma once

#include <elash/util/dynarena.h>
#include <elash/source/span.h>

#include <elash/sema/cast.h>

typedef struct ElHirExpr ElHirExpr;
typedef struct ElHirType ElHirType;

typedef struct ElHirCastExpr {
    ElCastKind kind;
    ElHirExpr* expr;
    // NOTE: type is stored in the ElHirExpr struct
    //ElHirType* type;
} ElHirCastExpr;

ElHirExpr* el_hir_new_cast_expr(ElDynArena* arena, ElSourceSpan span, ElCastKind kind, ElHirType* type, ElHirExpr* expr);
ElHirExpr* el_hir_new_semcast_expr(ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElHirExpr* expr);
ElHirExpr* el_hir_new_bitcast_expr(ElDynArena* arena, ElSourceSpan span, ElHirType* type, ElHirExpr* expr);
