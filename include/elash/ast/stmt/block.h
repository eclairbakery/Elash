#pragma once

typedef struct ElAstStmtNode ElAstStmtNode;

typedef struct ElAstBlockStmtNode {
    ElAstStmtNode* stmts;
} ElAstBlockStmtNode;

ElAstBlockStmtNode el_ast_block_stmt(ElAstStmtNode* stmts);
ElAstStmtNode* el_ast_new_block_stmt(ElDynArena* arena, ElAstStmtNode* stmts);
