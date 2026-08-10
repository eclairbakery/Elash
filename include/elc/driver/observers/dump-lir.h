#pragma once

#include <elc/pipeline/observer.h>

typedef struct {
    const char* path;
    ElcArtifactKind kind;
} DumpLirObserverData;

void elc_dump_lir_observer_exec(
    void* user_data,
    const ElcPipelineContext* ctx,
    ElcObserverEvent event,
    ElStringView stage_name,
    const ElcArtifact* artifact
);
ElcObserver elc_make_dump_lir_observer(DumpLirObserverData* data);

