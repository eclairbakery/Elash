#include <elc/driver/stages/pp-stage.h>

#include <elash/pp/preproc.h>

bool elc_pp_stage_exec(const ElcStage* stage, ElcPipelineContext* ctx, const ElcArtifact* input, ElcArtifact* output) {
    (void) stage;

    ElPreprocessor* pp = EL_DYNARENA_NEW(ctx->arena, ElPreprocessor);
    if (el_pp_init(pp, *input->as.tokens) != EL_PPERR_SUCCESS) {
        // TODO: error handling
        return false;
    }

    ElTokenStream* stream = EL_DYNARENA_NEW(ctx->arena, ElTokenStream);
    *stream = el_pp_as_token_stream(pp);
    
    output->as.tokens = stream;
    return true;
}

ElcStage elc_make_pp_stage() {
    return (ElcStage) {
        .name = EL_SV("Preprocessor"),
        .execute = elc_pp_stage_exec,

        .input_kind = ELC_ART_TOKENS,
        .output_kind = ELC_ART_PP_TOKENS,
    };
}
