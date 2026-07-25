#pragma once

#include <elash/ast/tree/type.h>
#include <elash/ast/tree/init.h>

#include <elash/sema/storage-cls.h>

typedef struct ElAstArrayLit {
    ElStorageClass scls;
    ElAstType*     type;
    ElAstInit*     init;
} ElAstArrayLit;

ElAstExpr* el_ast_new_array_lit(
    ElDynArena* arena, ElSourceSpan span,
    ElStorageClass scls, ElAstType* type, ElAstInit* init
);
