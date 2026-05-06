#pragma once

#include <elash/mir/instr.h>

typedef struct ElMirInstrBuf {
    ElMirInstr** items;
    usize len, cap;
} ElMirInstrBuf;

bool el_mir_ibuf_init(ElMirInstrBuf* ibuf);
void el_mir_ibuf_destroy(ElMirInstrBuf* ibuf);

bool el_mir_ibuf_copy(const ElMirInstrBuf* src, ElMirInstrBuf* dst);
bool el_mir_ibuf_move(ElMirInstrBuf* src, ElMirInstrBuf* dst);

bool el_mir_ibuf_resize(ElMirInstrBuf* ibuf, usize new_size);
bool el_mir_ibuf_reserve(ElMirInstrBuf* ibuf, usize min_cap);
bool el_mir_ibuf_reserve_exact(ElMirInstrBuf* ibuf, usize new_cap);

bool el_mir_ibuf_push(ElMirInstrBuf* ibuf, ElMirInstr* instr);

bool el_mir_ibuf_clear(ElMirInstrBuf* ibuf);
