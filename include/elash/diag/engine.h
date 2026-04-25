#pragma once

#include <elash/util/dynarena.h>

#include <elash/srcdoc/span.h>

#include <elash/diag/severity.h>
#include <elash/diag/meta.h>

typedef struct ElDiagnostic {
    ElDiagSeverity sev;
    ElSourceSpan span;
    ElStringView template;
    ElDiagMeta meta;
    
    struct ElDiagnostic* prev;
    struct ElDiagnostic* next;
} ElDiagnostic;

typedef struct ElDiagEngine {
    ElDynArena* arena;
    ElDiagnostic* diag_head;
    ElDiagnostic* diag_tail;
    usize diag_count;
} ElDiagEngine;

void el_diag_engine_init(ElDiagEngine* engine, ElDynArena* arena);
void el_diag_engine_free(ElDiagEngine* engine);

void el_diag_report_impl(
    ElDiagEngine* engine,
    ElDiagSeverity sev, ElSourceSpan span,
    ElStringView template, ElDiagMeta meta
);

#define el_diag_report(engine, sev, span, template, ...) \
    el_diag_report_impl(engine, sev, span, EL_SV(template), EL_DIAG_META(__VA_ARGS__))

