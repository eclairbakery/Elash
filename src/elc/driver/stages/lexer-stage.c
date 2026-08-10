#include <elc/driver/stages/lexer-stage.h>

#include <elash/lexer/lexer.h>
#include <elash/diag/handle.h>

bool elc_lexer_stage_exec(const ElcStage* stage, ElcPipelineContext* ctx, const ElcArtifact* input, ElcArtifact* output) {
    (void) stage;

    ElLexer* lexer = EL_DYNARENA_NEW(ctx->arena, ElLexer);
    if (el_lexer_init(lexer, input->as.src, EL_LEXER_FLAGS_DEFAULT) != EL_LEXERR_SUCCESS) {
        el_diag_handle_lexer_error(ctx->diag, &lexer->last_err_details);
        return false;
    }

    ElTokenStream* stream = EL_DYNARENA_NEW(ctx->arena, ElTokenStream);
    *stream = el_lexer_as_token_stream(lexer);

    output->as.tks = stream;
    return true;
}

ElcStage elc_make_lexer_stage() {
    return (ElcStage) {
        .name = EL_SV("Lexer"),
        .execute = elc_lexer_stage_exec,

        .input_kind = ELC_ART_SRC,
        .output_kind = ELC_ART_TKS,
    };
}
