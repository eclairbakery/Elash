#pragma once

#include <elash/util/dynarena.h>
#include <elash/diag/engine.h>

#include <elash/lexer/tokbuf.h>

#include <elash/lowerer/builtin.h>
#include <elash/binder/builtin.h>

#include <elc/codegen/backend.h>

typedef struct ElcPipelineContext {
    ElDynArena*   arena;
    ElDiagEngine* diag;

    ElBinderBuiltins*  binder_builtins;
    ElLowererBuiltins* lowerer_builtins;
    ElcCodegenBackend* backend;

    ElcOptLevel optlevel;

    ElTokenBuf* token_dump_bufs[ELC_ART_MAX];
} ElcPipelineContext;
