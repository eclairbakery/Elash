#include <el-pp/vararr.h>
#include <el-pp/var.h>

#include <stdbool.h>
#include <stdlib.h>

bool el_pp_vararr_reserve(ElPpVarArr* vararr, usize new_cap) {
    if (new_cap <= vararr->cap)
        return true;

    ElPpVar* new_data = realloc(vararr->data, new_cap * sizeof(ElPpVar));
    if (!new_data)
        return false;

    vararr->data = new_data;
    vararr->cap = new_cap;
    return true;
}

bool el_pp_vararr_init(ElPpVarArr* vararr) {
    vararr->data = NULL;
    vararr->num_elements = 0;
    vararr->cap = 0;
    return true;
}

void el_pp_vararr_destroy(ElPpVarArr* vararr) {
    for (usize i = 0; i < vararr->num_elements; ++i)
        el_pp_var_free(&vararr->data[i]);

    free(vararr->data);

    vararr->data = NULL;
    vararr->num_elements = 0;
    vararr->cap = 0;
}

void el_pp_vararr_clear(ElPpVarArr* vararr) {
    for (usize i = 0; i < vararr->num_elements; ++i) {
        el_pp_var_free(&vararr->data[i]);
    }

    vararr->num_elements = 0;
}

bool el_pp_vararr_copy(const ElPpVarArr* src, ElPpVarArr* dst) {
    if (!el_pp_vararr_init(dst))
        return false;

    if (!el_pp_vararr_reserve(dst, src->num_elements))
        return false;

    for (usize i = 0; i < src->num_elements; ++i) {
        if (!el_pp_var_copy(&src->data[i], &dst->data[i])) {
            for (usize j = 0; j < i; ++j) {
                el_pp_var_free(&dst->data[j]);
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

void el_pp_vararr_move(ElPpVarArr* src, ElPpVarArr* dst) {
    el_pp_vararr_destroy(dst);

    *dst = *src;

    src->data = NULL;
    src->num_elements = 0;
    src->cap = 0;
}

bool el_pp_vararr_push(ElPpVarArr* vararr, const ElPpVar* var) {
    if (vararr->num_elements == vararr->cap) {
        usize new_cap = vararr->cap ? vararr->cap * 2 : 4;
        if (!el_pp_vararr_reserve(vararr, new_cap))
            return false;
    }

    if (!el_pp_var_copy(var, &vararr->data[vararr->num_elements]))
        return false;

    vararr->num_elements++;
    return true;
}

bool el_pp_vararr_move_push(ElPpVarArr* vararr, ElPpVar* var) {
    if (vararr->num_elements == vararr->cap) {
        usize new_cap = vararr->cap ? vararr->cap * 2 : 4;
        if (!el_pp_vararr_reserve(vararr, new_cap)) {
            return false;
        }
    }

    el_pp_var_move(var, &vararr->data[vararr->num_elements]);
    vararr->num_elements++;
    return true;
}
