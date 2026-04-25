#pragma once

#include <elash/defs/sv.h>

#include <elc/pipeline/artifact.h>
#include <elc/pipeline/context.h>

#include <stdbool.h>

typedef struct ElcStage ElcStage;

typedef bool (*ElcStageExecuteFn)(
    const ElcStage* stage,
    ElcDriverContext* ctx,
    const ElcArtifact* input,
    ElcArtifact* output
);

struct ElcStage {
    ElStringView    name;
    ElcArtifactKind input_kind;
    ElcArtifactKind output_kind;
    
    ElcStageExecuteFn execute;
    void*             priv;
};
