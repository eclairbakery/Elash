#include <el-hir/dump/toplevel.h>
#include <el-hir/dump/stmt.h>
#include <el-hir/dump/indent.h>
#include <el-hir/dump/symbol.h>

#include <el-hir/tree/toplevel.h>

#include <el-sema/type.h>
#include <el-defs/sv.h>

void el_hir_dump_toplevel(ElHirTopLevelNode* node, usize indent, FILE* out) {
    if (!node) return;

    for (ElHirTopLevelNode* curr = node; curr; curr = curr->next) {
        switch (curr->kind) {
        case EL_HIR_TOPLVL_FUNC_DEF: {
            ElHirFuncDefinition* func = &curr->as.func_def; 
            ElHirFuncSymbol* sym = &func->symbol->as.func;

            el_hir_dump_print_indent(indent, out);
            fputs("define ", out);
            el_sema_dump_type(sym->ret_type, out);
            fputc(' ', out);
            fprintf(out, EL_SV_FMT "(", EL_SV_FARG(func->symbol->name));
            
            for (usize i = 0; i < sym->param_count; ++i) {
                ElHirSymbol* param = sym->params[i];
                if (i > 0) fputs(", ", out);

                el_sema_dump_type(param->as.var.type, out);
                fputc(' ', out);
                el_hir_dump_symbol(param, out);
            }
            fputs(")", out);
            
            if (func->block) {
                fputs(" {\n", out);
                el_hir_dump_stmt(func->block->stmts, indent + 1, out);
                el_hir_dump_print_indent(indent, out);
                fputs("}\n", out);
            } else {
                fputs(";\n", out);
            }
            break;
        }
        }
        if (curr->next) fputs("\n", out);
    }
}
