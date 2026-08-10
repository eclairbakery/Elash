#include <elc/driver/observers/dump-lir.h>
#include <elc/codegen/lir.h>
#include <stdio.h>
#include <string.h>

void elc_dump_lir_observer_exec(
    void* user_data,
    const ElcPipelineContext* ctx,
    ElcObserverEvent event,
    ElStringView stage_name,
    const ElcArtifact* artifact
) {
    (void) ctx, (void) stage_name;
    DumpLirObserverData* data = (DumpLirObserverData*) user_data;

    if (artifact == NULL || artifact->kind != data->kind) {
        return;
    }

    const ElcLirHandle* lir = &artifact->as.lir;
    switch (event) {
    case ELC_OBS_ARTIFACT_PRODUCED: {
        FILE* out = stdout;
        if (data->path && strcmp(data->path, "-") != 0) {
            out = fopen(data->path, "w");
            if (out == NULL) return;
        }

        lir->dump(lir, out);

        if (out != stdout) fclose(out);
        break;
    }
    default:
        break;
    }
}

ElcObserver elc_make_dump_lir_observer(DumpLirObserverData* data) {
    return (ElcObserver) {
        .callback = elc_dump_lir_observer_exec,
        .user_data = (void*) data
    };
}
