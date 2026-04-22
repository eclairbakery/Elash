#include <el-sema/expr/unary.h>

#include <el-util/assert.h>
#include <el-defs/sv.h>

ElStringView el_ast_unary_op_to_string(ElSemaUnaryOp type) {
    switch (type) {
    case EL_SEMA_UNARY_OP_POS: return EL_SV("+");
    case EL_SEMA_UNARY_OP_NEG: return EL_SV("-");

    case EL_SEMA_UNARY_OP_NOT:    return EL_SV("!");
    case EL_SEMA_UNARY_OP_BW_NOT: return EL_SV("~");

    case EL_SEMA_UNARY_OP_PRE_INC:  return EL_SV("pre  ++x");
    case EL_SEMA_UNARY_OP_PRE_DEC:  return EL_SV("pre  --x");
    case EL_SEMA_UNARY_OP_POST_INC: return EL_SV("post x++");
    case EL_SEMA_UNARY_OP_POST_DEC: return EL_SV("post x--");
    }
    EL_UNREACHABLE_ENUM_VAL(ElSemaUnaryOp, type);
}

