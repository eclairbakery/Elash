#pragma once

#include <elc/pipeline/artifact.h>
#include <elc/pipeline/context.h>

typedef enum ElcObserverEvent {
    ELC_OBS_STAGE_BEGIN,
    ELC_OBS_STAGE_END,
    ELC_OBS_ARTIFACT_PRODUCED
} ElcObserverEvent;

typedef void ElcObserverFn(
    void* user_data,
    const ElcDriverContext* ctx,
    ElcObserverEvent event,
    const char* stage_name,
    const ElcArtifact* artifact
);

typedef struct ElcObserver {
    ElcObserverFn* callback;
    void* user_data;
} ElcObserver;
