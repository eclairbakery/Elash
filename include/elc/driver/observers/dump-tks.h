#pragma once

#include <elc/pipeline/observer.h>
#include <elash/lexer/tokbuf.h>

typedef struct ElcDumpTksData {
    const char* path;
    ElTokenBuf* buffer;
    ElcArtifactKind kind;
} ElcDumpTksData;

void elc_dump_tokens_observer_exec(
    void* user_data,
    const ElcPipelineContext* ctx,
    ElcObserverEvent event,
    ElStringView stage_name,
    const ElcArtifact* artifact
);
ElcObserver elc_make_dump_tokens_observer(ElcDumpTksData* data);
