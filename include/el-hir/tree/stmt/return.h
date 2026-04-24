#pragma once

typedef struct ElHirStmtNode ElHirStmtNode;
typedef struct ElHirExprNode ElHirExprNode;

typedef struct ElHirReturnStmtNode {
    ElHirExprNode* value;
} ElHirReturnStmtNode;
