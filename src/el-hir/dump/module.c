#include <el-hir/dump/module.h>
#include <el-hir/dump/toplevel.h>
#include <el-hir/dump/indent.h>
#include <el-hir/tree/module.h>

void el_hir_dump_module(ElHirModule* module, usize indent, FILE* out) {
    for (ElHirTopLevelNode* node = module->head; node; node = node->next) {
        el_hir_dump_toplevel(node, indent, out);
        fputs("\n", out);
    }
}
