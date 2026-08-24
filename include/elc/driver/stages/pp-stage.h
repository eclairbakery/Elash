#pragma once
#include <elc/pipeline/pipeline.h>

bool elc_preproc_stage_exec(const ElcStage* stage, ElcPipelineContext* ctx, const ElcArtifact* input, ElcArtifact* output);
ElcStage elc_make_preproc_stage();
