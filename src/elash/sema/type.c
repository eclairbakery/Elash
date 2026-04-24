#include <elash/sema/type.h>

void el_sema_dump_type(const ElType* type, FILE* out) {
    switch (type->kind) {
    case EL_TYPE_PRIM:
        switch (type->as.prim.kind) {
        case EL_PRIMTYPE_INT:  fputs("int", out); break;
        case EL_PRIMTYPE_UINT: fputs("uint", out); break;
        case EL_PRIMTYPE_CHAR: fputs("char", out); break;
        }
        break;
    case EL_TYPE_PTR:
        el_sema_dump_type(type->as.ptr.base, out);
        fputs("*", out);
        break;
    }
}

