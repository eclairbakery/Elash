#pragma once

#include <elash/ast/tree/init.h>
#include <elash/ast/tree/type.h>
#include <elash/ast/tree/unr.h>
#include <elash/source/span.h>
#include <elash/util/dynarena.h>

typedef struct ElAstToE ElAstToE;
typedef struct ElAstToI ElAstToI;

typedef enum ElAstToIKind {
    EL_AST_TOI_TYPE,
    EL_AST_TOI_INIT,
    EL_AST_TOI_UNR,
} ElAstToIKind;

struct ElAstToI {
    ElAstToIKind kind;
    ElSourceSpan span;
    union {
        ElAstType* type;
        ElAstInit* init;
        ElAstUnr*  unr;
    } as;

    ElAstToI* next;
};

ElAstToI* el_ast_new_toi_type(ElDynArena* arena, ElAstType* type);
ElAstToI* el_ast_new_toi_init(ElDynArena* arena, ElAstInit* init);
ElAstToI* el_ast_new_toi_unr(ElDynArena* arena, ElAstUnr* unr);

ElAstType* el_ast_toi_as_type(ElDynArena* arena, ElAstToI* node);
ElAstInit* el_ast_toi_as_init(ElDynArena* arena, ElAstToI* node);

ElAstToI* el_ast_toi_from_toe(ElDynArena* arena, ElAstToE* toe);

void el_ast_append_toi(ElAstToI** head, ElAstToI** tail, ElAstToI* toi);
