#pragma once
#include <elash/prof/prof.h> // IWYU pragma: export

static inline ElProfStat el_prof_stage_stat(const ElProfStage* stage) {
    ElProfStat diff = {
        .wall_sec = stage->finish.wall_sec - stage->start.wall_sec,
        .user_sec = stage->finish.user_sec - stage->start.user_sec,
        .sys_sec  = stage->finish.sys_sec  - stage->start.sys_sec,
    };

    if (diff.wall_sec < 0) diff.wall_sec = 0;
    if (diff.user_sec < 0) diff.user_sec = 0;
    if (diff.sys_sec  < 0) diff.sys_sec  = 0;
    return diff;
}

// other = not captured by any subcategory
static inline ElProfStat el_prof_substage_other(
    const ElProfStage* stage,
    const ElProfStat* stage_stat
) {
    // i have no idea how to name this variable
    // "There is only one hard things in Computer Science: naming things."
    // ~ Phil Karlton (modified by me cause i don't care about cache invalidation.)
    ElProfStat not_other = {0};
    for (ElProfSubstage* sub = stage->sub.head; sub != NULL; sub = sub->next) {
        not_other.wall_sec += sub->total.wall_sec;
        not_other.user_sec += sub->total.user_sec;
        not_other.sys_sec  += sub->total.sys_sec;
    }

    ElProfStat other = {
        .wall_sec = stage_stat->wall_sec - not_other.wall_sec,
        .user_sec = stage_stat->user_sec - not_other.user_sec,
        .sys_sec  = stage_stat->sys_sec  - not_other.sys_sec,
    };
    return other;
}
