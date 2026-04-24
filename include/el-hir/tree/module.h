#pragma once

#include "toplevel.h"
#include <el-util/dynarena.h>
#include <el-defs/inttypes.h>

typedef struct ElHirModule {
    ElHirTopLevelNode* head;
    ElHirTopLevelNode* tail;
    usize count;
} ElHirModule;

ElHirModule* el_hir_new_module(ElDynArena* arena);
void el_hir_module_append(ElHirModule* mod, ElHirTopLevelNode* node);
