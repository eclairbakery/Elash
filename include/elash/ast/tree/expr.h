#pragma once

#include "common/ident.h"

#include "expr/bin.h"
#include "expr/unary.h"
#include "expr/literal.h"
#include "expr/call.h"
#include "expr/cast.h"
#include "expr/member.h"
#include "expr/typed-init.h"

#include "type.h"

#include <elash/defs/int-types.h>
#include <elash/srcdoc/span.h>

typedef enum ElAstExprType {
    EL_AST_EXPR_BINARY,
    EL_AST_EXPR_UNARY,
    EL_AST_EXPR_LITERAL,
    EL_AST_EXPR_TYPEDINIT,
    EL_AST_EXPR_IDENT,
    EL_AST_EXPR_CALL,
    EL_AST_EXPR_CAST,
    EL_AST_EXPR_MEMBER,
    EL_AST_EXPR_TMEMBER,
} ElAstExprType;

typedef struct ElAstExpr {
    ElAstExprType type;
    ElSourceSpan span;
    union {
        ElAstBinExpr     binary;
        ElAstUnaryExpr   unary;
        ElAstLiteral     literal;
        ElAstTypedInit   typedinit;
        ElAstIdent       ident;
        ElAstCallExpr    call;
        ElAstCastExpr    cast;
        ElAstMemberExpr  member;
        ElAstTMemberExpr tmember;
    } as;
    ElAstExpr* next;
} ElAstExpr;

void el_ast_expr_list_append(ElAstExpr** head, ElAstExpr** tail, ElAstExpr* expr);
