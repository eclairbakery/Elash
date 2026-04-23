#include <el-hir/dump/symbol.h>
#include <el-hir/symbol.h>

void el_hir_dump_symbol(ElHirSymbol* symbol, FILE* out) {
    fprintf(out, EL_SV_FMT"#%p", EL_SV_FARG(symbol->name), symbol);
}
