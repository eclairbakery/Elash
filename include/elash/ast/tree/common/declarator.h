#pragma once

#include <elash/util/dynarena.h>
#include <stdbool.h>

typedef struct ElAstType  ElAstType;
typedef struct ElAstDecl  ElAstDecl;
typedef struct ElAstInit  ElAstInit;
typedef struct ElAstIdent ElAstIdent;

typedef struct ElAstDeclarator ElAstDeclarator;

struct ElAstDeclarator {
    ElAstIdent* name;
    ElAstInit*  init; // nullable

    ElAstDeclarator* next;
};

ElAstDeclarator* el_ast_new_declarator(ElDynArena* arena, ElAstIdent* name, ElAstInit* init);
void el_ast_append_declarator(ElAstDeclarator** head, ElAstDeclarator** tail, ElAstDeclarator* declarator);

