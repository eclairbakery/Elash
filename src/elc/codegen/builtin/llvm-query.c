#include <elc/codegen/builtin/llvm-backend.h>
#include <elash/util/todo.h>

typedef ElcLLVMBackendCtx Context;

static ElBSType* query_map(const ElBSQuery* self, const ElMirType* mtype) {
    return elc_llvm_map_type(self->ctx, mtype);
}

static usize query_sizeof(const ElBSQuery* self, const ElBSType* type) {
    (void) self, (void) type;
    EL_TODO("sizeof unimplemented");
}
static usize query_alignof(const ElBSQuery* self, const ElBSType* type) {
    (void) self, (void) type;
    EL_TODO("alignof unimplemented");
}


ElBSQuery _elc_llvm_query(ElcLLVMBackendCtx* ctx) {
    if (ctx->cached_query) {
        return ctx->query;
    }

    return ctx->query = (ElBSQuery) {
        .ctx = ctx,

        .map       = query_map,
        .get_size  = query_sizeof,
        .get_align = query_alignof,
    };
}

ElBSQuery elc_llvm_query(ElcCodegenBackend* self) {
    return _elc_llvm_query(self->ctx);
}
