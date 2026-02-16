#pragma once

#include <el-lexer/token.h>
#include <el-defs/inttypes.h>

#include <stdbool.h>

typedef struct ElTokenQueue {
    ElToken* data;
    usize cap;
    usize head;
    usize tail;
    usize len;
} ElTokenQueue;

bool el_tkque_init(ElTokenQueue* tkque);
bool el_tkque_init_with_cap(ElTokenQueue* tkque, usize initial_cap);
void el_tkque_destroy(ElTokenQueue* tkque);

bool el_tkque_copy(const ElTokenQueue* src, ElTokenQueue* dst);
void el_tkque_move(ElTokenQueue* src, ElTokenQueue* dst);

bool el_tkque_push(ElTokenQueue* tkque, ElToken tok);
bool el_tkque_pop(ElTokenQueue* tkque, ElToken* out_tok);

bool el_tkque_peek(const ElTokenQueue* tkque, ElToken* out_tok);
bool el_tkque_clear(ElTokenQueue* tkque);

bool el_tkque_reserve(ElTokenQueue* tkque, usize min_additional_cap);
