#include <elash/pp/valarr.h>
#include <elash/pp/value.h>

#include <stdbool.h>
#include <stdlib.h>

bool el_pp_valarr_reserve(ElPpValueArr* valarr, usize new_cap) {
    if (new_cap <= valarr->cap)
        return true;

    ElPpValue** new_data = realloc(valarr->data, new_cap * sizeof(ElPpValue*));
    if (new_data == NULL) return false;

    valarr->data = new_data;
    valarr->cap = new_cap;
    return true;
}

bool el_pp_valarr_init(ElPpValueArr* valarr) {
    valarr->data = NULL;
    valarr->count = 0;
    valarr->cap = 0;
    return true;
}

void el_pp_valarr_free(ElPpValueArr* valarr) {
    free(valarr->data);
    valarr->data = NULL;
    valarr->count = 0;
    valarr->cap = 0;
}

void el_pp_valarr_clear(ElPpValueArr* valarr) {
    valarr->count = 0;
}

bool el_pp_valarr_push(ElPpValueArr* valarr, ElPpValue* val) {
    if (valarr->count == valarr->cap) {
        usize new_cap = valarr->cap ? valarr->cap * 2 : 4;
        if (!el_pp_valarr_reserve(valarr, new_cap))
            return false;
    }

    valarr->data[valarr->count] = val;
    valarr->count++;
    return true;
}
