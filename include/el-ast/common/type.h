#pragma once

#include <el-defs/sv.h>
#include <el-util/dynarena.h>

#include "ident.h"

typedef struct ElAstExprNode ElAstExprNode;

typedef enum ElAstTypeKind {
    EL_AST_TYPE_NAME,
    EL_AST_TYPE_PTR,
} ElAstTypeKind;

typedef struct ElAstTypeNode ElAstTypeNode;
struct ElAstTypeNode {
    ElAstTypeKind kind;
    union {
        ElAstIdentNode* name; // for EL_AST_TYPE_NAME
        ElAstTypeNode* base;  // for EL_AST_TYPE_PTR
    };
};
