#pragma once
#include <elash/prof/prof.h> // IWYU pragma: export

static inline ElProfStat el_prof_stage_stat(const ElProfStage* stage) {
    return stage->total;
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
