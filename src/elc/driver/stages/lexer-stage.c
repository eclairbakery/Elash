#include <elc/driver/stages/lexer-stage.h>

#include <elash/lexer/lexer.h>
#include <elash/diag/handle.h>
#include <elash/prof/prof.h>

bool elc_lexer_stage_exec(const ElcStage* stage, ElcPipelineContext* ctx, const ElcArtifact* input, ElcArtifact* output) {
    (void) stage;

    ElLexer* lexer = EL_DYNARENA_NEW(ctx->arena, ElLexer);
    ElLexerStatus status = el_lexer_init_prof(
        lexer, input->as.src, EL_LEXER_FLAGS_DEFAULT,
        ctx->prof, el_prof_current_stage(ctx->prof)
    );

    if (status != EL_LEXERR_SUCCESS) {
        el_diag_handle_lexer_error(ctx->diag, &lexer->last_err_details);
        return false;
    }

    ElTokenStream stream = el_lexer_as_token_stream(lexer);
    if (ctx->token_dump_bufs[ELC_ART_TKS] != NULL) {
        el_tkbuf_capture_stream(&stream, ctx->token_dump_bufs[ELC_ART_TKS], ctx->diag);

        ElTkBufStream* tkbuf_ctx = EL_DYNARENA_NEW(ctx->arena, ElTkBufStream);
        stream = el_tkbuf_as_stream(tkbuf_ctx, ctx->token_dump_bufs[ELC_ART_TKS]);
    }

    output->as.tks = EL_DYNARENA_NEW(ctx->arena, ElTokenStream);
    *output->as.tks = stream;
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
