#include <elash/diag/printer/console.h>
#include <elash/diag/engine.h>

#include <elash/srcdoc/srcdoc.h>
#include <elash/util/ansi.h>

void el_diag_console_printer_begin(ElDiagPrinter* self, FILE* out) {
    (void) self;
    (void) out;
}

ElAnsiStyle _el_diag_console_printer_get_style(ElDiagSeverity sev) {
    ElAnsiColor color = EL_ANSI_CLR_DEFAULT;
    switch (sev) {
    case EL_DIAG_ERROR: color = EL_ANSI_CLR_RED;            break;
    case EL_DIAG_WARN:  color = EL_ANSI_CLR_YELLOW;         break;
    case EL_DIAG_NOTE:  color = EL_ANSI_CLR_BRIGHT_MAGENTA; break;
    case EL_DIAG_TIP:   color = EL_ANSI_CLR_BRIGHT_GREEN;   break;
    }

    return (ElAnsiStyle) {
        .bg_color = EL_ANSI_CLR_DEFAULT,
        .fg_color = color,
        .dec = EL_ANSI_DEC_BOLD, 
    };
}

void _el_diag_console_printer_print_sev(ElDiagSeverity sev, ElStringView cat, FILE* out) {
    ElStringView sev_str = el_diag_severity_to_string(sev);
    ElAnsiStyle style = _el_diag_console_printer_get_style(sev);
    bool ansi = el_ansi_is_supported(out);

    if (ansi) el_ansi_apply_style(style, out);
    fprintf(out, EL_SV_FMT"["EL_SV_FMT"]: ", EL_SV_FARG(sev_str), EL_SV_FARG(cat));
    if (ansi) el_ansi_reset_style(out);
}

void _el_diag_console_printer_print_loc(const ElSourceSpan* span, FILE* out) {
    fprintf(out, EL_SV_FMT": %u:%u: ",
            EL_SV_FARG(span->doc->filename), span->start.line, span->start.column);
}

void el_diag_console_printer_print(ElDiagPrinter* self, FILE* out, const ElDiagnostic* diag) {
    (void) self;
    _el_diag_console_printer_print_loc(&diag->span, out);
    _el_diag_console_printer_print_sev(diag->sev, diag->category, out);
    el_sv_print(diag->formatted, out);
    fputc('\n', out);
}

void el_diag_console_printer_summary(ElDiagPrinter* self, FILE* out, const ElDiagSummary* sum) {
    (void) self;
    if (sum->total_diagnostics != 0) {
        fprintf(out, "Finished with %d diagnostics\n", sum->total_diagnostics);
    }
}

void el_diag_console_printer_finish(ElDiagPrinter* self, FILE* out) {
    (void) self;
    (void) out;
}
