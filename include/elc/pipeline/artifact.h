#pragma once

#include <elash/source/doc.h>
#include <elash/lexer/tokstream.h>
#include <elash/ast/tree/module.h>
#include <elash/hir/tree/module.h>
#include <elash/mir/module.h>

#include <elc/codegen/lir.h>

typedef enum ElcArtifactKind {
    ELC_ART_NONE,
    ELC_ART_SRC,   // ElSourceDocument
    ELC_ART_TKS,   // ElTokenStream
    ELC_ART_PPTKS, // ElTokenStream (Post-Preprocessor)
    ELC_ART_AST,   // ElAstModule
    ELC_ART_HIR,   // ElHirModule
    ELC_ART_MIR,   // ElMirModule
    ELC_ART_LIR,   // ElcLirHandle
    ELC_ART_OLIR,  // ElcLirHandle (Optimized)
    ELC_ART_ASM,   // ElcCodegenBuffer
    ELC_ART_OBJ,   // ElcCodegenBuffer

    ELC_ART_MAX,
} ElcArtifactKind;

typedef struct ElcArtifact {
    ElcArtifactKind kind;
    union {
        ElSourceDocument*   src;
        ElTokenStream*      tks;
        ElAstModule*        ast;
        ElHirModule*        hir;
        ElMirModule*        mir;
        ElcLirHandle        lir;
        ElcCodegenBuffer    asm;
        ElcCodegenBuffer    obj;
    } as;
} ElcArtifact;

ElStringView elc_artifact_kind_to_string(ElcArtifactKind art);
ElcArtifactKind elc_artifact_kind_from_string(ElStringView str);
void elc_artifact_free(ElcArtifact* art);
