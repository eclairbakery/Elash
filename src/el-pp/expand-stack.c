#include <el-pp/expand-stack.h>

#include <stdlib.h>
#include <string.h>

static bool el_expstack_reallocate(ElPpExpandStack* expstack, usize new_cap);

bool el_expstack_init(ElPpExpandStack* expstack) {
    expstack->states = NULL;
    expstack->len = 0;
    expstack->cap = 0;
    return true;
}

void el_expstack_destroy(ElPpExpandStack* expstack) {
    free(expstack->states);
    expstack->states = NULL;
    expstack->len = 0;
    expstack->cap = 0;
}

bool el_expstack_copy(const ElPpExpandStack* src, ElPpExpandStack* dst) {
    el_expstack_init(dst);

    if (src->len == 0) {
        return true;
    }

    dst->states = malloc(src->len * sizeof(ElPpExpandState));
    if (dst->states == NULL) {
        return false;
    }

    memcpy(dst->states, src->states, src->len * sizeof(ElPpExpandState));
    dst->len = src->len;
    dst->cap = src->len;
    return true;
}

bool el_expstack_move(ElPpExpandStack* src, ElPpExpandStack* dst) {
    el_expstack_destroy(dst);

    dst->states = src->states;
    dst->len = src->len;
    dst->cap = src->cap;

    el_expstack_init(src);
    return true;
}

static bool el_expstack_reallocate(ElPpExpandStack* expstack, usize new_cap) {
    if (new_cap == expstack->cap) return true;

    ElPpExpandState* new_states = realloc(expstack->states, new_cap * sizeof(ElToken));
    if (new_states == NULL) return false;

    expstack->states = new_states;
    expstack->cap = new_cap;
    return true;
}

bool el_expstack_reserve(ElPpExpandStack* expstack, usize min_cap) {
    if (expstack->cap >= min_cap) {
        return true;
    }

    usize new_cap = expstack->cap == 0 ? (min_cap < 4 ? 4 : min_cap) : expstack->cap * 2;
    if (new_cap < min_cap) {
        new_cap = min_cap;
    }

    return el_expstack_reallocate(expstack, new_cap);
}

bool el_expstack_reserve_exact(ElPpExpandStack* expstack, usize new_cap) {
    if (new_cap < expstack->len)  return false;
    if (expstack->cap == new_cap) return true;

    return el_expstack_reallocate(expstack, new_cap);
}

bool el_expstack_resize(ElPpExpandStack* expstack, usize new_size) {
    if (new_size > expstack->cap) {
        if (!el_expstack_reserve_exact(expstack, new_size)) {
            return false;
        }
    }

    if (new_size > expstack->len) {
        memset(expstack->states + expstack->len, 0, (new_size - expstack->len) * sizeof(ElPpExpandState));
    }

    expstack->len = new_size;
    return true;
}

bool el_expstack_push(ElPpExpandStack* expstack, ElPpExpandState state) {
    if (expstack->len == expstack->cap) {
        if (!el_expstack_reserve(expstack, expstack->len + 1)) {
            return false;
        }
    }

    expstack->states[expstack->len++] = state;
    return true;
}

bool el_expstack_clear(ElPpExpandStack* expstack) {
    expstack->len = 0;
    return true;
}

