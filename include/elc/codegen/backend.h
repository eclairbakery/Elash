#pragma once

#include <elash/sema/backends.h>

#include <elash/defs/semver.h>
#include <elash/defs/sv.h>

#include <elash/mir/module.h>

#include <elc/codegen/lir.h>
#include <elc/optlevel.h>

typedef struct ElcCodegenBackend ElcCodegenBackend;

typedef enum ElcCodegenResult {
    ELC_CODEGEN_OK,
    ELC_CODEGEN_INTERNAL_ERROR,
    // TODO: add more error variants
} ElcCodegenResult;

typedef ElcCodegenResult ElcBackendCompileFn(
    ElcCodegenBackend* self,
    const ElMirModule* input,
    ElcLirHandle* output
);
typedef ElBSQuery ElcBackendQueryFn(
    ElcCodegenBackend* self
);

typedef void ElcBackendOptimizeFn(
    ElcCodegenBackend* self,
    ElcLirHandle* inout,
    ElcOptLevel level
);
typedef void ElcBackendCleanupFn(ElcCodegenBackend* self);

struct ElcCodegenBackend {
    ElStringView name;
    ElSemVersion version;

    void* ctx;
    ElcBackendCompileFn*  compile;  // always non-null
    ElcBackendQueryFn*    query;    // always non-null
    ElcBackendOptimizeFn* optimize; // NULL if optimizations are unsupported
    ElcBackendCleanupFn*  cleanup;  // NULL if no cleanup is needed
};
