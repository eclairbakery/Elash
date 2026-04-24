#define _POSIX_C_SOURCE 200809L // for fileno
#include <elash/util/ansi.h>

#ifdef _WIN32
    #include <io.h>
    #define ISATTY _isatty
    #define FILENO _fileno
#else
    #include <unistd.h>
    #define ISATTY isatty
    #define FILENO fileno
#endif

bool el_ansi_is_supported() {
    return ISATTY(FILENO(stdout));
}

void el_ansi_apply_style(ElAnsiStyle style, FILE* out) {
    if (!el_ansi_is_supported()) return;

    // start with reset to ensure a clean state.
    fprintf(out, "\033[0");

    if (style.dec & EL_ANSI_DEC_BOLD) {
        fprintf(out, ";1");
    }
    if (style.dec & EL_ANSI_DEC_ITALIC) {
        fprintf(out, ";3");
    }
    if (style.dec & EL_ANSI_DEC_UNDERLINE) {
        fprintf(out, ";4");
    }

    if (style.fg_color != EL_ANSI_CLR_DEFAULT) {
        fprintf(out, ";%d", (int)style.fg_color);
    }
    if (style.bg_color != EL_ANSI_CLR_DEFAULT) {
        fprintf(out, ";%d", (int)style.bg_color + 10);
    }

    fprintf(out, "m");
}

void el_ansi_reset_style(FILE* out) {
    fputs("\033[0", out);
}
