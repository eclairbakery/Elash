#include <el-sema/expr/unary.h>

#include <el-util/assert.h>
#include <el-defs/sv.h>

ElStringView el_ast_unary_op_to_string(ElSemaUnaryExprType type) {
    switch (type) {
    case EL_SEMA_UNARY_EXPR_POS: return EL_SV("+");
    case EL_SEMA_UNARY_EXPR_NEG: return EL_SV("-");

    case EL_SEMA_UNARY_EXPR_NOT:    return EL_SV("!");
    case EL_SEMA_UNARY_EXPR_BW_NOT: return EL_SV("~");

    case EL_SEMA_UNARY_EXPR_PRE_INC:  return EL_SV("pre  ++x");
    case EL_SEMA_UNARY_EXPR_PRE_DEC:  return EL_SV("pre  --x");
    case EL_SEMA_UNARY_EXPR_POST_INC: return EL_SV("post x++");
    case EL_SEMA_UNARY_EXPR_POST_DEC: return EL_SV("post x--");
    }
    EL_UNREACHABLE_ENUM_VAL(ElSemaUnaryExprType, type);
}

