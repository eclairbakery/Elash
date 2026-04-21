#pragma once

#include <el-util/dynarena.h>

typedef struct ElAstExprNode ElAstExprNode;
typedef struct ElAstStmtNode ElAstStmtNode;

typedef struct ElAstReturnStmtNode {
    ElAstExprNode* value; // nullable
} ElAstReturnStmtNode;

ElAstReturnStmtNode el_ast_return_stmt(ElAstExprNode* value);
ElAstStmtNode* el_ast_new_return_stmt(ElDynArena* arena, ElAstExprNode* value);
