#include <el-lexer/tokbuf.h>

#include <stdlib.h>
#include <string.h>

static bool el_token_buf_reallocate(ElTokenBuf* tkbuf, usize new_cap);

bool el_token_buf_init(ElTokenBuf* tkbuf) {
    tkbuf->data = NULL;
    tkbuf->len = 0;
    tkbuf->cap = 0;
    return true;
}

void el_token_buf_destroy(ElTokenBuf* tkbuf) {
    free(tkbuf->data);
    tkbuf->data = NULL;
    tkbuf->len = 0;
    tkbuf->cap = 0;
}

bool el_token_buf_copy(const ElTokenBuf* src, ElTokenBuf* dst) {
    el_token_buf_init(dst);

    if (src->len == 0) {
        return true;
    }

    dst->data = malloc(src->len * sizeof(ElToken));
    if (dst->data == NULL) {
        return false;
    }

    memcpy(dst->data, src->data, src->len * sizeof(ElToken));
    dst->len = src->len;
    dst->cap = src->len;
    return true;
}

bool el_token_buf_move(ElTokenBuf* src, ElTokenBuf* dst) {
    el_token_buf_destroy(dst);

    dst->data = src->data;
    dst->len = src->len;
    dst->cap = src->cap;

    el_token_buf_init(src);
    return true;
}

static bool el_token_buf_reallocate(ElTokenBuf* tkbuf, usize new_cap) {
    if (new_cap == tkbuf->cap) return true;

    ElToken* new_data = realloc(tkbuf->data, new_cap * sizeof(ElToken));
    if (new_data == NULL) return false;

    tkbuf->data = new_data;
    tkbuf->cap = new_cap;
    return true;
}

bool el_token_buf_reserve(ElTokenBuf* tkbuf, usize min_cap) {
    if (tkbuf->cap >= min_cap) {
        return true;
    }

    usize new_cap = tkbuf->cap == 0 ? (min_cap < 4 ? 4 : min_cap) : tkbuf->cap * 2;
    if (new_cap < min_cap) {
        new_cap = min_cap;
    }

    return el_token_buf_reallocate(tkbuf, new_cap);
}

bool el_token_buf_reserve_exact(ElTokenBuf* tkbuf, usize new_cap) {
    if (new_cap < tkbuf->len)  return false;
    if (tkbuf->cap == new_cap) return true;

    return el_token_buf_reallocate(tkbuf, new_cap);
}

bool el_token_buf_resize(ElTokenBuf* tkbuf, usize new_size) {
    if (new_size > tkbuf->cap) {
        if (!el_token_buf_reserve_exact(tkbuf, new_size)) {
            return false;
        }
    }

    if (new_size > tkbuf->len) {
        memset(tkbuf->data + tkbuf->len, 0, (new_size - tkbuf->len) * sizeof(ElToken));
    }

    tkbuf->len = new_size;
    return true;
}

bool el_token_buf_push(ElTokenBuf* tkbuf, ElToken tok) {
    if (tkbuf->len == tkbuf->cap) {
        if (!el_token_buf_reserve(tkbuf, tkbuf->len + 1)) {
            return false;
        }
    }

    tkbuf->data[tkbuf->len++] = tok;
    return true;
}

bool el_token_buf_clear(ElTokenBuf* tkbuf) {
    tkbuf->len = 0;
    return true;
}
