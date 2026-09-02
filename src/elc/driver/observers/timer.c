#include <elc/driver/observers/timer.h>
#include <elash/prof/prof.h>

void elc_timer_observer_exec(
    void* user_data,
    const ElcPipelineContext* ctx,
    ElcObserverEvent event,
    ElStringView stage_name,
    const ElcArtifact* artifact
) {
    (void)user_data, (void)artifact;

    if (event == ELC_OBS_START) {
        el_prof_begin(ctx->prof, stage_name);
    } else if (event == ELC_OBS_FINISH) {
        ElProfStage* active = el_prof_current_stage(ctx->prof);
        if (active != NULL && el_sv_eql(active->name, stage_name)) {
            el_prof_finish_stage(ctx->prof, active);
        }
    }
}

ElcObserver elc_make_timer_observer() {
    return (ElcObserver) {
        .callback = elc_timer_observer_exec,
        .user_data = NULL,
    };
}
