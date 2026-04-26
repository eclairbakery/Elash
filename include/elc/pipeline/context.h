#pragma once

#include <elash/util/dynarena.h>
#include <elash/diag/engine.h>

typedef struct ElcPipelineContext {
    ElDynArena*   arena;
    ElDiagEngine* diag;
} ElcPipelineContext;
