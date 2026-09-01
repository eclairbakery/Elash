#include <elc/driver/observers/dump-tks.h>
#include <elash/lexer/token.h>

#include <elash/util/fopen.h>
#include <stdio.h>

void elc_dump_tokens_observer_exec(
    void* user_data,
    const ElcPipelineContext* ctx,
    ElcObserverEvent event,
    ElStringView stage_name,
    const ElcArtifact* artifact
) {
    (void) ctx, (void) stage_name;
    ElcDumpTksData* data = user_data;

    if (artifact == NULL || artifact->kind != data->kind) return;
    if (event != ELC_OBS_END) return;

    bool needs_closing;
    FILE* out = el_open_ofile(data->path, &needs_closing);

    if (data->buffer != NULL) {
        for (usize i = 0; i < data->buffer->len; ++i) {
            el_token_print(&data->buffer->data[i], out);
            fprintf(out, "\n");
        }
    }

    if (needs_closing) fclose(out);
}

ElcObserver elc_make_dump_tokens_observer(ElcDumpTksData* data) {
    return (ElcObserver) {
        .callback = elc_dump_tokens_observer_exec,
        .user_data = data
    };
}
