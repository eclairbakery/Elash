#include <elc/driver/observers/dump-lir.h>
#include <elc/codegen/lir.h>

#include <elash/util/fopen.h>
#include <stdio.h>

void elc_dump_lir_observer_exec(
    void* user_data,
    const ElcPipelineContext* ctx,
    ElcObserverEvent event,
    ElStringView stage_name,
    const ElcArtifact* artifact
) {
    (void) ctx, (void) stage_name;
    ElcDumpLirData* data = user_data;

    if (artifact == NULL || artifact->kind != data->kind) return;
    if (event != ELC_OBS_END) return;

    const ElcLirHandle* lir = &artifact->as.lir;

    bool needs_closing;
    FILE* out = el_open_ofile(data->path, &needs_closing);

    lir->dump(lir, out);

    if (needs_closing) fclose(out);
}

ElcObserver elc_make_dump_lir_observer(ElcDumpLirData* data) {
    return (ElcObserver) {
        .callback = elc_dump_lir_observer_exec,
        .user_data = data
    };
}
