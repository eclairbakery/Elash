#include <elash/lowerer/lowerer.h>
#include <elash/util/dynarena.h>
#include <elash/util/assert.h>

ElMirConstant* _el_lowerer_lower_const(ElLowerer* lw, ElHirExpr* expr) {
    EL_ASSERT(expr->kind == EL_HIR_EXPR_CONST || expr->kind == EL_HIR_EXPR_AGGCONST, "Expression must be a constant or array literal");

    ElMirConstant* mirconst = EL_DYNARENA_NEW(lw->arena, ElMirConstant);
    if (expr->kind == EL_HIR_EXPR_CONST) {
        ElHirConstant hir = expr->as.constant;
        switch (expr->type->as.prim.kind) {
        case EL_PRIMTYPE_INT:   mirconst->kind = EL_MIR_CONST_INT;   mirconst->as.int_   = hir.as.int_;           break;
        case EL_PRIMTYPE_CHAR:  mirconst->kind = EL_MIR_CONST_INT;   mirconst->as.int_   = (int64_t)hir.as.char_; break;
        case EL_PRIMTYPE_BOOL:  mirconst->kind = EL_MIR_CONST_INT;   mirconst->as.int_   = hir.as.bool_ ? 1 : 0;  break;
        case EL_PRIMTYPE_FLOAT: mirconst->kind = EL_MIR_CONST_FLOAT; mirconst->as.float_ = hir.as.float_;         break;
        default: EL_UNREACHABLE("invalid hir constant primitive type");      break;
        }
    } else /* EL_HIR_EXPR_AGGCONST */ {
        mirconst->kind = EL_MIR_CONST_ARRAY;
        ElHirAggConst* arrlit = &expr->as.aggconst;
        mirconst->as.array.count = arrlit->count;
        mirconst->as.array.elements = EL_DYNARENA_NEW_ARR(lw->arena, ElMirConstant*, arrlit->count);

        for (usize i = 0; i < arrlit->count; ++i) {
            mirconst->as.array.elements[i] = _el_lowerer_lower_const(lw, arrlit->values[i]);
        }
    }

    return mirconst;
}
