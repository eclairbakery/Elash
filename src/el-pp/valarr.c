#include <el-pp/valarr.h>
#include <el-pp/value.h>

#include <stdbool.h>
#include <stdlib.h>

bool el_pp_valarr_reserve(ElPpValueArr* valarr, usize new_cap) {
    if (new_cap <= valarr->cap)
        return true;

    ElPpValue* new_data = realloc(valarr->data, new_cap * sizeof(ElPpValue));
    if (!new_data)
        return false;

    valarr->data = new_data;
    valarr->cap = new_cap;
    return true;
}

bool el_pp_valarr_init(ElPpValueArr* valarr) {
    valarr->data = NULL;
    valarr->num_elements = 0;
    valarr->cap = 0;
    return true;
}

void el_pp_valarr_destroy(ElPpValueArr* valarr) {
    for (usize i = 0; i < valarr->num_elements; ++i)
        el_pp_value_free(&valarr->data[i]);

    free(valarr->data);

    valarr->data = NULL;
    valarr->num_elements = 0;
    valarr->cap = 0;
}

void el_pp_valarr_clear(ElPpValueArr* valarr) {
    for (usize i = 0; i < valarr->num_elements; ++i) {
        el_pp_value_free(&valarr->data[i]);
    }

    valarr->num_elements = 0;
}

bool el_pp_valarr_copy(const ElPpValueArr* src, ElPpValueArr* dst) {
    if (!el_pp_valarr_init(dst))
        return false;

    if (!el_pp_valarr_reserve(dst, src->num_elements))
        return false;

    for (usize i = 0; i < src->num_elements; ++i) {
        if (!el_pp_value_copy(&src->data[i], &dst->data[i])) {
            for (usize j = 0; j < i; ++j) {
                el_pp_value_free(&dst->data[j]);
            }

            free(dst->data);
            dst->data = NULL;
            dst->cap = 0;
            return false;
        }
    }

    dst->num_elements = src->num_elements;
    return true;
}

void el_pp_valarr_move(ElPpValueArr* src, ElPpValueArr* dst) {
    el_pp_valarr_destroy(dst);

    *dst = *src;

    src->data = NULL;
    src->num_elements = 0;
    src->cap = 0;
}

bool el_pp_valarr_push(ElPpValueArr* valarr, const ElPpValue* val) {
    if (valarr->num_elements == valarr->cap) {
        usize new_cap = valarr->cap ? valarr->cap * 2 : 4;
        if (!el_pp_valarr_reserve(valarr, new_cap))
            return false;
    }

    if (!el_pp_value_copy(val, &valarr->data[valarr->num_elements]))
        return false;

    valarr->num_elements++;
    return true;
}

bool el_pp_valarr_move_push(ElPpValueArr* valarr, ElPpValue* val) {
    if (valarr->num_elements == valarr->cap) {
        usize new_cap = valarr->cap ? valarr->cap * 2 : 4;
        if (!el_pp_valarr_reserve(valarr, new_cap)) {
            return false;
        }
    }

    el_pp_value_move(val, &valarr->data[valarr->num_elements]);
    valarr->num_elements++;
    return true;
}
