#include <elash/hir/dump/indent.h>

void el_hir_dump_print_indent(usize indent, FILE* out) {
    for (usize i = 0; i < indent; ++i) {
        fputs("  ", out);
    }
}
