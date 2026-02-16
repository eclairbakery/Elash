#pragma once

#include <el-lexer/token.h>

typedef struct ElTokenBuf {
    ElToken* data;
    usize len;
    usize cap;
} ElTokenBuf;

bool el_tkbuf_init(ElTokenBuf* tkbuf);
void el_tkbuf_destroy(ElTokenBuf* tkbuf);

bool el_tkbuf_copy(const ElTokenBuf* src, ElTokenBuf* dst);
bool el_tkbuf_move(ElTokenBuf* src, ElTokenBuf* dst);

bool el_tkbuf_resize(ElTokenBuf* tkbuf, usize new_size);
bool el_tkbuf_reserve(ElTokenBuf* tkbuf, usize min_cap);
bool el_tkbuf_reserve_exact(ElTokenBuf* tkbuf, usize new_cap);

bool el_tkbuf_push(ElTokenBuf* tkbuf, ElToken tok);

bool el_tkbuf_clear(ElTokenBuf* tkbuf);
