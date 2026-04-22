#include <el-sema/expr/bin.h>

#include <el-util/assert.h>
#include <el-defs/sv.h>

ElStringView el_sema_bin_op_to_string(ElSemaBinExprType type) {
    switch (type) {
    case EL_SEMA_BIN_EXPR_ADD: return EL_SV("+");
    case EL_SEMA_BIN_EXPR_SUB: return EL_SV("-");
    case EL_SEMA_BIN_EXPR_MUL: return EL_SV("*");
    case EL_SEMA_BIN_EXPR_DIV: return EL_SV("/");
    case EL_SEMA_BIN_EXPR_MOD: return EL_SV("%");

    case EL_SEMA_BIN_EXPR_EQ : return EL_SV("==");
    case EL_SEMA_BIN_EXPR_NEQ: return EL_SV("!=");
    case EL_SEMA_BIN_EXPR_LT : return EL_SV("<");
    case EL_SEMA_BIN_EXPR_LTE: return EL_SV("<=");
    case EL_SEMA_BIN_EXPR_GT : return EL_SV(">");
    case EL_SEMA_BIN_EXPR_GTE: return EL_SV(">=");
    
    case EL_SEMA_BIN_EXPR_AND: return EL_SV("&&");
    case EL_SEMA_BIN_EXPR_OR : return EL_SV("||");
    
    case EL_SEMA_BIN_EXPR_BW_AND: return EL_SV("&");
    case EL_SEMA_BIN_EXPR_BW_OR:  return EL_SV("|");
    case EL_SEMA_BIN_EXPR_BW_XOR: return EL_SV("^");
    case EL_SEMA_BIN_EXPR_SHL:    return EL_SV("<<");
    case EL_SEMA_BIN_EXPR_SHR:    return EL_SV(">>");
    }
    EL_UNREACHABLE_ENUM_VAL(ElSemaBinExprType, type);
}
