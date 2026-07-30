#pragma once

#include <elash/lowerer/builtin.h>

#include <elash/hir/symbol.h>
#include <elash/hir/scope.h>

#include <elash/hir/tree/module.h>
#include <elash/hir/tree/decl.h>
#include <elash/hir/tree/stmt.h>
#include <elash/hir/tree/expr.h>

#include <elash/mir/module.h>
#include <elash/mir/instr.h>
#include <elash/mir/value.h>
#include <elash/mir/block.h>
#include <elash/mir/func.h>
#include <elash/mir/symbol.h>
#include <elash/mir/type.h>

#include <elash/util/dynarena.h>
#include <elash/diag/engine.h>

typedef struct ElLowerer {
    ElDynArena*   arena;
    ElDiagEngine* diag;

    ElLowererBuiltins* builtins;

    uint32_t current_block_id;
    ElMirFunc* current_func;
    ElMirModule* current_mod;
    ElMirInstrBuf ibuf;

    ElMirValue** symbol_map;
    ElMirSymbol** mir_symbol_map;
    uint32_t next_sym_id; // starts at hir->sym_count; used for anonymous symbols

    uint32_t break_target_id;
    uint32_t continue_target_id;
} ElLowerer;

void el_lowerer_init(ElLowerer* lw, ElDynArena* arena, ElDiagEngine* diag, ElLowererBuiltins* builtins);
void el_lowerer_free(ElLowerer* lw);

bool el_lowerer_has_terminator(ElLowerer* lw);
void el_lowerer_emit_block(ElLowerer* lw, uint32_t id);

ElMirValue*  el_lowerer_get_lvalue(ElLowerer* lw, ElHirExpr* hir);

enum {
    EL_MIR_SLICE_FIELD_DATA = 0,
    EL_MIR_SLICE_FIELD_LEN  = 1,
};

ElMirValue*    _el_lowerer_extract_tuple_field(ElLowerer* lw, ElMirValue* tuple, usize index);
ElMirValue*    _el_lowerer_get_tuple_field_ptr(ElLowerer* lw, ElMirValue* tuple_ptr, usize index);
ElMirValue*    _el_lowerer_make_tuple(ElLowerer* lw, ElMirType* tuple_type, ElMirValue** fields);
ElMirConstant* _el_lowerer_lower_const(ElLowerer* lw, ElHirExpr* expr);
ElMirValue*    _el_lowerer_new_anon_global(ElLowerer* lw, ElMirType* type, ElMirConstant* init);
ElMirValue*    _el_lowerer_create_alloca(ElLowerer* lw, ElMirType* type);
void           _el_lowerer_copy_str_to_ptr(ElLowerer* lw, ElMirValue* ptr, ElMirStrConst str);

ElMirValue*  el_lowerer_lower_expr(ElLowerer* lw, ElHirExpr* hir);
void         _el_lowerer_lower_aggconst(ElLowerer* lw, ElMirValue* ptr, ElHirAggConst* aggconst);
void         _el_lowerer_lower_global_decl(ElLowerer* lw, ElHirDecl* decl);
void         _el_lowerer_lower_local_decl(ElLowerer* lw, ElHirDecl* decl);

void         el_lowerer_lower_stmt(ElLowerer* lw, ElHirStmt* hir);
ElMirModule* el_lowerer_lower_module(ElLowerer* lw, ElHirModule* hir);

ElMirType*   el_lowerer_map_type(ElLowerer* lw, const ElHirType* type);
ElMirSymbol* el_lowerer_map_symbol(ElLowerer* lw, ElHirSymbol* sym);
