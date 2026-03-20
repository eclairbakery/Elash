#include "el-pp/expr.h"
#include <el-pp/instr.h>
#include <el-pp/value.h>
#include <el-pp/header.h>

#include <el-lexer/tokbuf.h>

#include <el-defs/sv.h>

typedef struct ElPpLogInstrExpandState {
    ElStringView str;
} ElPpLogInstrExpandState;

typedef struct ElPpExpandState {
    ElPpInstr instr;
    union {
        struct {
            ElPpHeaderFile header;
        } for_include;
        struct {
            ElPpHeaderFile header;
        } for_embed;
        struct {
            ElStringView pragma;
        } for_pragma;
        struct {
            ElStringView body;
        } for_annote;

        ElPpLogInstrExpandState for_error;
        ElPpLogInstrExpandState for_warning;
        ElPpLogInstrExpandState for_note;

        struct {
            ElStringView macro;
            ElTokenBuf toks;
        } for_define;
        struct {} for_enddef;
        struct {} for_undef;
        
        struct {
            ElStringView var;
            ElPpExpr expr;
        } for_assign;
        struct { ElStringView var; } for_deassign;
        struct { ElStringView var; } for_inc;
        struct { ElStringView var; } for_dec;
        
        struct {
            ElPpExpr cond;
        } for_if;
        struct {
            ElPpExpr cond;
        } for_elif;
        struct {} for_else;
        struct {} for_endif;
        
        struct {
            ElPpExpr cond;
        } for_while;
        struct {} for_endwhile;
        
        // TODO
        struct {
        } for_for;
        struct {
        } for_foreach;
        struct {
        } for_endfor;
    };
} ElPpExpandState;
