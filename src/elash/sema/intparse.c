#include <elash/sema/intparse.h>
#include <elash/util/strconv.h>

#include <ctype.h>

static void report_invalid_prefix(ElDiagEngine* diag, ElSourceSpan span, char c) {
    el_diag_report(
        diag, EL_DIAG_ERROR, "syntax.invalid-number",
        span, "invalid integer base prefix: '0${char}'",
        EL_DIAG_CHAR("char", c),
    );
}

ElInt128 el_parse_int_lit(ElDiagEngine* diag, ElToken tok) {
    ElStringView sv = tok.lexeme;

    // NOLINTBEGIN(readability-magic-numbers): i hate clang-tidy
    uint base = 10;
    usize start = 0;

    if (sv.len > 2 && sv.data[0] == '0') {
        switch (sv.data[1]) {
        case 'x':
            base = 16;
            start = 2;
            break;
        case 'o':
            base = 8;
            start = 2;
            break;
        case 'b':
            base = 2;
            start = 2;
            break;
        case 'B':
        case 'O':
        case 'X':
            report_invalid_prefix(diag, tok.span, sv.data[1]);
            el_diag_help(
                diag, "integer base prefixes in elash must be lower case, did you mean '0${lc}'?",
                EL_DIAG_CHAR("lc", tolower(sv.data[1])),
            );
            return EL_INT128(0);
        default:
            if (!isdigit((unsigned char)sv.data[1])) {
                report_invalid_prefix(diag, tok.span, sv.data[1]);
                return EL_INT128(0);
            }
            break;
        }
    }
    // NOLINTEND(readability-magic-numbers): ok ok

    ElStringView parsed_sv = el_sv_window(sv, start, sv.len - start);
    return el_string_to_i128(diag, parsed_sv, base, tok.span);
}
