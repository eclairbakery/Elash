#pragma once

#include <el-sema/type.h>
#include <el-hir/symbol.h>

#include "stmt.h"

typedef enum ElHirTopLevelKind {
    EL_HIR_TOPLVL_FUNC_DEF,
} ElHirTopLevelKind;

typedef struct ElHirFuncDefinition {
    ElHirSymbol* symbol;
    ElHirBlockStmtNode* block;
} ElHirFuncDefinition;

typedef struct ElHirTopLevelNode ElHirTopLevelNode;
struct ElHirTopLevelNode {
    ElHirTopLevelKind kind;
    union {
        ElHirFuncDefinition func_def;
    } as;
    ElHirTopLevelNode* next;
};
