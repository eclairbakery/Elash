#pragma once

#include <el-lexer/token.h>

typedef struct ElTokenBuf {
    ElToken* data;
    usize len;
    usize cap;
} ElTokenBuf;

bool el_token_buf_init(ElTokenBuf* tkbuf);
void el_token_buf_destroy(ElTokenBuf* tkbuf);

bool el_token_buf_copy(const ElTokenBuf* src, ElTokenBuf* dst);
bool el_token_buf_move(ElTokenBuf* src, ElTokenBuf* dst);

bool el_token_buf_resize(ElTokenBuf* tkbuf, usize new_size);
bool el_token_buf_reserve(ElTokenBuf* tkbuf, usize min_cap);
bool el_token_buf_reserve_exact(ElTokenBuf* tkbuf, usize new_cap);

bool el_token_buf_push(ElTokenBuf* tkbuf, ElToken tok);

bool el_token_buf_clear(ElTokenBuf* tkbuf);
