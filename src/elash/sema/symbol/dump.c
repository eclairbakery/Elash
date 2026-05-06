#include <elash/sema/symbol/dump.h>
#include <elash/sema/symbol.h>

void el_sema_dump_symbol(ElSymbol* symbol, FILE* out) {
    fprintf(out, EL_SV_FMT"#%p", EL_SV_FARG(symbol->name), symbol);
}
