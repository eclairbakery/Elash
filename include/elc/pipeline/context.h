#pragma once

#include <elash/util/dynarena.h>
#include <elash/diag/engine.h>

typedef struct ElcDriverContext {
    ElDynArena*   arena;
    ElDiagEngine* diag;
} ElcDriverContext;
