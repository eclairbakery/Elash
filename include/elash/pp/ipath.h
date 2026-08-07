#pragma once

#include <elash/defs/sv.h>

typedef struct ElPpIncPath ElPpIncPath;
struct ElPpIncPath {
    bool is_system;
    ElStringView name;
    ElStringView path;
    ElPpIncPath* next;
};

typedef struct ElPpIncPaths {
    ElPpIncPath* head;
    ElPpIncPath* tail;
} ElPpIncPaths;

static inline void el_pp_add_ipath(ElPpIncPaths* paths, ElPpIncPath* new) {
    if (paths->head == NULL) {
        paths->head = new;
        paths->tail = new;
    } else {
        paths->tail->next = new;
        paths->tail = new;
    }
}
