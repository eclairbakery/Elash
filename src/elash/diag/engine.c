#include <elash/diag/engine.h>

void el_diag_engine_init(ElDiagEngine* engine, ElDynArena* arena) {
    engine->arena = arena;
    engine->diag_count = 0;
    engine->diag_head = NULL;
    engine->diag_tail = NULL;
}

void el_diag_engine_free(ElDiagEngine* engine) {
    // nothing for now
    (void) engine;
}

static ElDiagMeta _el_diag_clone_meta(ElDynArena* arena, ElDiagMeta meta) {
    if (meta.count == 0) {
        return (ElDiagMeta) { .entries = NULL, .count = 0 };
    }

    ElDiagMetaEntry* entries = EL_DYNARENA_NEW_ARR(arena, ElDiagMetaEntry, meta.count);
    for (usize i = 0; i < meta.count; i++) {
        entries[i].key = meta.entries[i].key;
        entries[i].type = meta.entries[i].type;

        if (entries[i].type == EL_DIAG_META_STRING) {
            entries[i].as.string = el_dynarena_clone_sv(arena, meta.entries[i].as.string);
        } else {
            entries[i].as.integer = meta.entries[i].as.integer;
        }
    }

    return (ElDiagMeta) { .entries = entries, .count = meta.count };
}

void el_diag_report_impl(
    ElDiagEngine* engine,
    ElDiagSeverity sev, ElSourceSpan span,
    ElStringView template, ElDiagMeta meta
) {
    ElDiagnostic* diag = EL_DYNARENA_NEW(engine->arena, ElDiagnostic);
    diag->sev = sev;
    diag->span = span;
    diag->template = el_dynarena_clone_sv(engine->arena, template);
    diag->meta = _el_diag_clone_meta(engine->arena, meta);

    diag->next = NULL;
    diag->prev = engine->diag_tail;

    if (engine->diag_tail) {
        engine->diag_tail->next = diag;
    } else {
        engine->diag_head = diag;
    }

    engine->diag_tail = diag;
    engine->diag_count++;
}
