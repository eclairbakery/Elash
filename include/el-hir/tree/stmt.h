#include "expr.h"

typedef struct ElHirStmtNode ElHirStmtNode;

typedef enum ElHirStmtKind {
    EL_HIR_STMT_EXPR,
    EL_HIR_STMT_RETURN,
    EL_HIR_STMT_BLOCK,
} ElHirStmtKind;

typedef struct ElHirBlockStmtNode {
    ElHirStmtNode* stmts;
} ElHirBlockStmtNode;

typedef struct ElHirReturnStmtNode {
    ElHirExprNode* value; // nullable
} ElHirReturnStmtNode;

struct ElHirStmtNode {
    ElHirStmtKind kind;
    union {
        ElHirExprNode* expr;
        ElHirBlockStmtNode block;
        ElHirReturnStmtNode return_;
    } as;
    ElHirStmtNode* next;
};

