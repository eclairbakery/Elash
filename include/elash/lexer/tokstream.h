#pragma once

#include <elash/lexer/token.h>
#include <elash/diag/engine.h>

typedef struct ElProfState ElProfState;
typedef struct ElProfStage ElProfStage;

typedef struct ElTokenStream ElTokenStream;

typedef ElToken ElTokenStreamNext(ElTokenStream* self, ElDiagEngine* engine);

struct ElTokenStream {
    ElTokenStreamNext* next;
    void* ctx;
    ElProfState* prof;
    ElProfStage* prof_stage;
};
