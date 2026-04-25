#include <elash/diag/engine.h>

#include <elash/diag/template.h>
#include <elash/util/strbuf.h>

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
    ElDiagSeverity sev, ElStringView category,
    ElSourceSpan span,
    ElStringView template, ElDiagMeta meta
) {
    ElDiagnostic* diag = EL_DYNARENA_NEW(engine->arena, ElDiagnostic);
    diag->sev = sev;
    diag->category = category;
    diag->span = span;
    diag->template = el_dynarena_clone_sv(engine->arena, template);
    diag->meta = _el_diag_clone_meta(engine->arena, meta);

    if (diag->sev == EL_DIAG_ERROR) engine->summary.total_errors++;
    if (diag->sev == EL_DIAG_WARN)  engine->summary.total_warnings++;
    engine->summary.total_diagnostics++;

    ElStringBuf formatted;
    el_strbuf_init(&formatted);
    if (el_diag_render_template(template, &diag->meta, &formatted)) {
        diag->formatted = el_dynarena_clone_sv(engine->arena, el_strbuf_view(&formatted));
    } else {
        diag->formatted = diag->template;
    }
    el_strbuf_destroy(&formatted);

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

ElDiagSummary el_diag_engine_summary(const ElDiagEngine* engine) {
    return engine->summary;
}

void el_diag_engine_print(const ElDiagEngine* engine, ElDiagPrinter* printer, FILE* out) {
    printer->begin(printer, out);
    for (ElDiagnostic* diag = engine->diag_head; diag != NULL; diag = diag->next) {
        printer->print(printer, out, diag);
    }
    printer->summary(printer, out, &engine->summary);
    printer->finish(printer, out);
}
