#include <elc/codegen/builtin/llvm-backend.h>

#include <elash/util/dynarena.h>
#include <elash/util/assert.h>
#include <elash/util/todo.h>

#include <llvm-c/Core.h>
#include <llvm-c/Error.h>
#include <llvm-c/Target.h>

#ifdef EL_DEBUG
#include <llvm-c/Analysis.h>
#endif

#include <llvm-c/TargetMachine.h>
#include <llvm-c/Transforms/PassBuilder.h>

#include <stdlib.h>
#include <string.h>

void elc_llvm_lir_free(ElcLirHandle* handle) {
    ElcLLVMLir* data = handle->data;
    if (data->module != NULL) {
        LLVMDisposeModule(data->module);
    }
}

void elc_llvm_lir_dump(const ElcLirHandle* handle, FILE* out) {
    ElcLLVMLir* data = handle->data;
    char* ir = LLVMPrintModuleToString(data->module);
    fputs(ir, out);
    LLVMDisposeMessage(ir);
}

LLVMTargetMachineRef elc_llvm_create_target_machine(char** out_triple) {
    char* triple = LLVMGetDefaultTargetTriple();
    LLVMTargetRef target;
    char* error = NULL;
    if (LLVMGetTargetFromTriple(triple, &target, &error)) {
        LLVMDisposeMessage(error);
        LLVMDisposeMessage(triple);
        return NULL;
    }

    LLVMTargetMachineRef machine = LLVMCreateTargetMachine(
        target, triple, "generic", "",
        LLVMCodeGenLevelDefault, LLVMRelocDefault, LLVMCodeModelDefault
    );

    if (out_triple != NULL) {
        *out_triple = triple;
    } else {
        LLVMDisposeMessage(triple);
    }

    return machine;
}

void elc_llvm_setup_module_layout(LLVMModuleRef module, LLVMTargetMachineRef machine, const char* triple) {
    LLVMSetTarget(module, triple);
    LLVMTargetDataRef target_data = LLVMCreateTargetDataLayout(machine);
    char* layout_str = LLVMCopyStringRepOfTargetData(target_data);
    LLVMSetDataLayout(module, layout_str);
    LLVMDisposeMessage(layout_str);
    LLVMDisposeTargetData(target_data);
}

ElcCodegenBuffer elc_llvm_lir_emit_to_buffer(const ElcLirHandle* handle, LLVMCodeGenFileType file_type) {
    ElcLLVMLir* data = handle->data;
    char* triple = NULL;
    LLVMTargetMachineRef machine = elc_llvm_create_target_machine(&triple);
    if (machine == NULL) EL_TODO("Handle error");

    elc_llvm_setup_module_layout(data->module, machine, triple);

    LLVMMemoryBufferRef buffer_ref;
    char* error = NULL;
    if (LLVMTargetMachineEmitToMemoryBuffer(machine, data->module, file_type, &error, &buffer_ref)) {
        EL_TODO("Handle error");
    }

    ElcCodegenBuffer buffer = {
        .size = LLVMGetBufferSize(buffer_ref),
        .data = malloc(LLVMGetBufferSize(buffer_ref))
    };

    if (buffer.data != NULL) {
        memcpy(buffer.data, LLVMGetBufferStart(buffer_ref), buffer.size);
    } else {
        buffer.size = 0;
    }

    LLVMDisposeMemoryBuffer(buffer_ref);
    LLVMDisposeTargetMachine(machine);
    LLVMDisposeMessage(triple);

    return buffer;
}

ElcCodegenBuffer elc_llvm_lir_emit_obj(const ElcLirHandle* handle) {
    return elc_llvm_lir_emit_to_buffer(handle, LLVMObjectFile);
}
ElcCodegenBuffer elc_llvm_lir_emit_asm(const ElcLirHandle* handle) {
    return elc_llvm_lir_emit_to_buffer(handle, LLVMAssemblyFile);
}

void elc_llvm_lir_free_buffer(const ElcLirHandle* handle, ElcCodegenBuffer buffer) {
    (void) handle;
    free(buffer.data);
}

void elc_llvm_cleanup(ElcCodegenBackend* self) {
    ElcLLVMBackendCtx* ctx = self->ctx;
    LLVMDisposeBuilder(ctx->builder);
    LLVMContextDispose(ctx->context);
}

ElcLirHandle elc_llvm_make_lir_handle(ElcLLVMLir* data) {
    return (ElcLirHandle) {
        .ir_name     = EL_SV("llvm-ir"),
        .data        = data,
        .dump        = elc_llvm_lir_dump,
        .emit_asm    = elc_llvm_lir_emit_asm,
        .emit_obj    = elc_llvm_lir_emit_obj,
        .free        = elc_llvm_lir_free,
        .free_buffer = elc_llvm_lir_free_buffer,
    };
}

void elc_llvm_optimize(ElcCodegenBackend* self, ElcLirHandle* lir, ElcOptLevel level) {
    // just in case
    if (level == ELC_OPT_UNSPEC || level == ELC_OPT_O0) return;
    (void) self;

    ElcLLVMLir* data = lir->data;
    LLVMModuleRef module = data->module;
    LLVMPassBuilderOptionsRef pbo = LLVMCreatePassBuilderOptions();

    const char* pass_str = NULL;
    switch (level) {
    case ELC_OPT_O1: pass_str = "default<O1>"; break;
    case ELC_OPT_O2: pass_str = "default<O2>"; break;
    case ELC_OPT_O3: pass_str = "default<O3>"; break;
    case ELC_OPT_Og: pass_str = "default<Og>"; break;
    case ELC_OPT_Os: pass_str = "default<Os>"; break;
    case ELC_OPT_Oz: pass_str = "default<Oz>"; break;
    case ELC_OPT_Of: pass_str = "default<O3>"; break;
    default: EL_UNREACHABLE("shouldn't get here");
    }

    LLVMErrorRef err = LLVMRunPasses(module, pass_str, NULL, pbo);
    if (err != LLVMErrorSuccess) {
        char* msg = LLVMGetErrorMessage(err);
        // TODO: better error handling
        fprintf(stderr, "LLVMRunPasses failed: %s\n", msg);
        LLVMDisposeErrorMessage(msg);
        LLVMDisposePassBuilderOptions(pbo);
        return;
    }

#ifdef EL_DEBUG
    LLVMVerifyModule(data->module, LLVMPrintMessageAction, NULL);
#endif

    LLVMDisposePassBuilderOptions(pbo);
}

ElcCodegenBackend elc_make_llvm_codegen(ElDynArena* arena, ElTypeCache* tcache) {
    ElcLLVMBackendCtx* ctx = EL_DYNARENA_NEW(arena, ElcLLVMBackendCtx);
    ctx->context = LLVMContextCreate();
    ctx->builder = LLVMCreateBuilderInContext(ctx->context);

    ctx->arena  = arena;
    ctx->tcache = tcache;

    ctx->globals = NULL;
    ctx->globals_count = 0;

    LLVMInitializeNativeTarget();
    LLVMInitializeNativeAsmPrinter();

    return (ElcCodegenBackend) {
        .name = EL_SV("llvm"),
        .version = EL_SEM_VER(0, 1, 0),
        .ctx = ctx,
        .compile  = elc_llvm_compile,
        .query    = elc_llvm_query,
        .optimize = elc_llvm_optimize,
        .cleanup  = elc_llvm_cleanup,
    };
}
