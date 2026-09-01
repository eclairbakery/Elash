#if !defined(_POSIX_C_SOURCE) && !defined(_XOPEN_SOURCE)
    #define _POSIX_C_SOURCE 200809L
#endif

#include "prof-internals.h"

#include <elash/defs/platform.h>
#include <elash/util/assert.h>
#include <elash/util/todo.h>

#if EL_PLATFORM_IS_WINDOWS
    #if EL_COMPILER_IS_MSVC
        #pragma comment(lib, "psapi.lib")
    #endif
    #include <windows.h>
    #include <psapi.h>
#elif EL_PLATFORM_IS_POSIX
    #include <sys/resource.h>
    #include <sys/time.h>
    #include <time.h>
#endif

// TODO: handle errors, somehow

#define NSEC_PER_SEC 1000000000.0
#define USEC_PER_SEC 1000000.0

// does this unit even have a name?
// when will microsoft learn what "standarization" is?
// right, never. i guess they haven't discovered nanoseconds yet.
#define WEIRD_WINDOWS_UNIT_PER_SEC 10000000.0

#if EL_PLATFORM_IS_POSIX
    static ElProfStat stat_now() {
        ElProfStat stat;

        struct rusage usage;
        struct timespec now;

        if (clock_gettime(CLOCK_MONOTONIC, &now) == 0) {
            stat.wall_sec = (double)now.tv_sec + ((double)now.tv_nsec / NSEC_PER_SEC);
        } else {
            EL_TODO("handle error");
        }

        if (getrusage(RUSAGE_SELF, &usage) == 0) {
            stat.user_sec = (double)usage.ru_utime.tv_sec +
                ((double)usage.ru_utime.tv_usec / USEC_PER_SEC);
            stat.sys_sec = (double)usage.ru_stime.tv_sec +
                ((double)usage.ru_stime.tv_usec / USEC_PER_SEC);
        } else {
            EL_TODO("handle error");
        }

        return stat;
    }
#elif EL_PLATFORM_IS_WINDOWS
    // written by ai; don't ask me how this works.
    static ElProfStat stat_now() {
        ElProfStat stat;

        static LARGE_INTEGER frequency;
        static bool has_frequency;

        LARGE_INTEGER counter;
        FILETIME creation;
        FILETIME exit;
        FILETIME kernel;
        FILETIME user;

        if (!has_frequency) {
            QueryPerformanceFrequency(&frequency);
            has_frequency = true;
        }

        if (QueryPerformanceCounter(&counter) && frequency.QuadPart != 0) {
            stat.wall_sec = (double)counter.QuadPart / (double)frequency.QuadPart;
        } else {
            EL_TODO("handle error");
        }

        if (GetProcessTimes(GetCurrentProcess(), &creation, &exit, &kernel, &user)) {
            ULARGE_INTEGER kernel_ticks = {
                .LowPart = kernel.dwLowDateTime,
                .HighPart = kernel.dwHighDateTime,
            };
            ULARGE_INTEGER user_ticks = {
                .LowPart = user.dwLowDateTime,
                .HighPart = user.dwHighDateTime,
            };

            stat.sys_sec = (double)kernel_ticks.QuadPart / WEIRD_WINDOWS_UNIT_PER_SEC;
            stat.user_sec = (double)user_ticks.QuadPart / WEIRD_WINDOWS_UNIT_PER_SEC;
        } else {
            EL_TODO("handle error");
        }

        return stat;
    }
#endif

static ElProfStat stat_diff(ElProfStat end, ElProfStat start) {
    ElProfStat diff = {
        .wall_sec = end.wall_sec - start.wall_sec,
        .user_sec = end.user_sec - start.user_sec,
        .sys_sec  = end.sys_sec  - start.sys_sec,
    };

    if (diff.wall_sec < 0) diff.wall_sec = 0;
    if (diff.user_sec < 0) diff.user_sec = 0;
    if (diff.sys_sec  < 0) diff.sys_sec  = 0;
    return diff;
}

static void stat_add(ElProfStat* total, ElProfStat delta) {
    total->wall_sec += delta.wall_sec;
    total->user_sec += delta.user_sec;
    total->sys_sec  += delta.sys_sec;
}

void el_prof_init(ElProfState* prof, ElDynArena* arena) {
    *prof = (ElProfState) {
        .arena = arena,
    };
}

void el_prof_begin(ElProfState* prof, ElStringView name) {
    if (prof == NULL) return;
    EL_ASSERT(prof->active_stage == NULL,
              "a benchmark stage is already active");

    ElProfStage* stage = EL_DYNARENA_NEW_STRUCT(prof->arena, ElProfStage, {
        .name  = name,
        .start = stat_now(),
    });

    if (prof->tail != NULL) {
        prof->tail->next = stage;
    } else {
        prof->head = stage;
    }

    prof->tail = stage;
    prof->active_stage = stage;
}

void el_prof_finish(ElProfState* prof) {
    if (prof == NULL) return;
    EL_ASSERT(prof->active_stage != NULL,
              "no benchmark stage is active");
    EL_ASSERT(prof->active_sub == NULL,
              "benchmark substages are still active");

    ElProfStage* stage = prof->active_stage;
    stage->finish = stat_now();
    prof->active_stage = NULL;
}

ElProfSubstage* el_prof_new_sub(ElProfState* prof, ElStringView name) {
    if (prof == NULL) return NULL;
    EL_ASSERT(prof->active_stage != NULL,
              "a substage needs an active stage");

    ElProfSubstage* substage = EL_DYNARENA_NEW_STRUCT(prof->arena, ElProfSubstage, {
        .name   = name,
        .pstage = prof->active_stage,
    });

    // advanced linked list append
    ElProfStage* stage = prof->active_stage;
    if (stage->sub.tail != NULL) {
        stage->sub.tail->next = substage;
    } else {
        stage->sub.head = substage;
    }

    stage->sub.tail = substage;
    return substage;
}

void el_prof_begin_sub(ElProfState* prof, ElProfSubstage* substage) {
    if (prof == NULL) return;
    EL_ASSERT(substage->pstage == prof->active_stage,
              "substage belongs to another stage");

    ElProfStat now = stat_now();
    if (prof->active_sub != NULL) {
        stat_add(&prof->active_sub->substage->total,
                 stat_diff(now, prof->active_sub->start));
    }

    substage->entered_count++;

    ElProfSubFrame* frame = EL_DYNARENA_NEW_STRUCT(prof->arena, ElProfSubFrame, {
        .substage = substage,
        .start    = now,
        .parent   = prof->active_sub,
    });
    prof->active_sub = frame;
}

void el_prof_finish_sub(ElProfState* prof, ElProfSubstage* substage) {
    if (prof == NULL) return;
    EL_ASSERT(prof->active_sub != NULL,
              "no benchmark substage is active");
    EL_ASSERT(prof->active_sub->substage == substage,
              "you're using this function wrong, chud");

    ElProfSubFrame* frame = prof->active_sub;
    ElProfStat now = stat_now();
    stat_add(&substage->total, stat_diff(now, frame->start));

    prof->active_sub = frame->parent;
    if (prof->active_sub != NULL) {
        prof->active_sub->start = now;
    }
}
