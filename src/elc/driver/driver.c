#include <elc/driver/driver.h>

#include <elc/driver/stages/lexer-stage.h>

bool elc_driver_init(ElcDriver* driver) {
    if (!el_dynarena_init(&driver->arena)) return false;
    el_diag_engine_init(&driver->diag, &driver->arena);
    elc_pipeline_init(&driver->pipeline, &driver->arena, &driver->diag);
    return true;
}

void elc_driver_free(ElcDriver* driver) {
    el_diag_engine_free(&driver->diag);
    el_dynarena_free(&driver->arena);
}

bool elc_driver_register_stages(ElcDriver* driver) {
    elc_pipeline_add_stage(&driver->pipeline, elc_make_lexer_stage());
    return true;
}

bool elc_driver_register_observers(ElcDriver* driver) {
    (void) driver;
    return true;
}

void elc_driver_provide_source(ElcDriver* driver, ElSourceDocument* source) {
    elc_pipeline_provide(&driver->pipeline, (ElcArtifact) {
        .kind = ELC_ART_SOURCE_TEXT,
        .as.source = source
    });
}

bool elc_driver_run(ElcDriver* driver) {
    ElcArtifact out;
    if (!elc_pipeline_request(&driver->pipeline, ELC_ART_TOKENS, &out)) {
        return false;
    }

    ElTokenStream* stream = out.as.tokens;
    while (true) {
        ElToken tok = stream->next(stream, &driver->diag);
        if (tok.type == EL_TT_EOF) break;

        el_token_print(&tok, stdout);
        fputc('\n', stdout);
    }

    return true;
}
