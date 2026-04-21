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

ElAstTypeNode el_ast_type_name(ElAstIdentNode* name);
ElAstTypeNode el_ast_type_ptr(ElAstTypeNode* base);

ElAstTypeNode* el_ast_new_type_name(ElDynArena* arena, ElAstIdentNode* name);
ElAstTypeNode* el_ast_new_type_ptr(ElDynArena* arena, ElAstTypeNode* base);

void el_ast_dump_type(ElAstTypeNode* node, FILE* out);
