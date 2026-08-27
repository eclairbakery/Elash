#pragma once

#include <elash/util/dynarena.h>
#include <stdbool.h>

typedef struct ElAstType  ElAstType;
typedef struct ElAstDecl  ElAstDecl;
typedef struct ElAstInit  ElAstInit;
typedef struct ElAstIdent ElAstIdent;

typedef struct ElAstDeclarator ElAstDeclarator;

struct ElAstDeclarator {
    ElAstType*  type;
    ElAstIdent* name;
    ElAstInit*  init; // nullable

    ElAstDeclarator* next;
};

void el_ast_append_declarator(ElAstDeclarator** head, ElAstDeclarator** tail, ElAstDeclarator* declarator);
ElAstDeclarator* el_ast_new_declarator(ElDynArena* arena, ElAstType* type, ElAstIdent* name, ElAstInit* init);

