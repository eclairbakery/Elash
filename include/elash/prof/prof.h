#pragma once

#include <elash/util/dynarena.h>
#include <elash/defs/sv.h>

#include <stdio.h>

typedef struct ElProfStat {
    double wall_sec;
    double user_sec;
    double sys_sec;
} ElProfStat;

typedef struct ElProfStage ElProfStage;
typedef struct ElProfSubstage ElProfSubstage;

struct ElProfSubstage {
    ElStringView name;

    ElProfStat total;
    uint entered_count;

    ElProfStage* pstage;
    ElProfSubstage* next;
};

typedef struct ElProfSubFrame {
    ElProfSubstage*      substage;
    ElProfStat           start;
    struct ElProfSubFrame* parent;
} ElProfSubFrame;

struct ElProfStage {
    ElStringView name;

    ElProfStat start;
    ElProfStat finish;

    struct {
        ElProfSubstage* head;
        ElProfSubstage* tail;
    } sub;
    ElProfStage* next;
};

typedef struct ElProfState {
    ElDynArena*  arena;
    ElProfStage* head;
    ElProfStage* tail;

    ElProfStage* active_stage;
    ElProfSubFrame* active_sub;
} ElProfState;

/// @note Unlike most functions in this codebase, the prof api is no-op/returns NULL
///       if the prof argument is NULL. this is intentional. when the --time-report
///       flag is omitted (most of the time) we don't want to start unnecessary timers
///       and we also don't want to force the caller to do null checks everywhere
///
/// @{

void el_prof_init(ElProfState* prof, ElDynArena* arena);

void el_prof_begin(ElProfState* prof, ElStringView name);
void el_prof_finish(ElProfState* prof);

ElProfSubstage* el_prof_new_sub(ElProfState* prof, ElStringView name);
void el_prof_begin_sub(ElProfState* prof, ElProfSubstage* substage);
void el_prof_finish_sub(ElProfState* prof, ElProfSubstage* substage);

/// @}

void el_prof_print_console(ElProfState* prof, FILE* out);
void el_prof_print_jsonl(ElProfState* prof, FILE* out);
