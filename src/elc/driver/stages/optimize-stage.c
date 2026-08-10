#include <elc/driver/stages/optimize-stage.h>
#include <elc/codegen/builtin/llvm-backend.h>

#include <elash/util/todo.h>

bool elc_optimize_stage_exec(const ElcStage* stage, ElcPipelineContext* ctx, const ElcArtifact* input, ElcArtifact* output) {
    (void) stage;

    if (ctx->backend->optimize != NULL)
        // ugly and theoretically unsafe but i have no idea how to do this
        // in any better way
        ctx->backend->optimize(ctx->backend, (ElcLirHandle*)&input->as.lir, ctx->optlevel);

    output->as.lir = input->as.lir;
    return true;
}

ElcStage elc_make_optimize_stage() {
    return (ElcStage) {
        .name = EL_SV("Optimizer"),
        .execute = elc_optimize_stage_exec,

        .input_kind = ELC_ART_LIR,
        .output_kind = ELC_ART_OLIR,
    };
}

