#include <elash/sema/strparse.h>
#include <elash/lexer/token.h>

#include <elash/util/todo.h>

static ElSourceSpan get_escape_span(ElToken tok, usize offset) {
    ElSourceLocation loc = tok.span.ranges[0].start;
    loc.offset += offset;
    loc.column += offset;

    ElSourceSpan span = tok.span;
    span.ranges[0].start = loc;
    span.ranges[0].end = loc;
    span.ranges[0].end.offset += 2;
    span.ranges[0].end.column += 2;
    return span;
}

ElStringView el_parse_str_with_escapes(ElDiagEngine* diag, ElToken tok, char buf[static tok.lexeme.len]) {
    ElStringView input = tok.lexeme;

    usize bidx = 0;
    for (usize i = 0; i < input.len; ++i) {
        if (input.data[i] == '\\') {
            if (i + 1 >= input.len) {
                el_diag_report(
                    diag, EL_DIAG_ERROR, "sema.invalid-literal",
                    get_escape_span(tok, i), "trailing backslash in string/char literal"
                );
                return EL_SV_NULL;
            }

            char next = input.data[++i];
            switch (next) {
            case 'a':  buf[bidx++] = '\a';   break;
            case 'b':  buf[bidx++] = '\b';   break;
            case 'e':  buf[bidx++] = '\033'; break;
            case 'f':  buf[bidx++] = '\f';   break;
            case 'n':  buf[bidx++] = '\n';   break;
            case 'r':  buf[bidx++] = '\r';   break;
            case 't':  buf[bidx++] = '\t';   break;
            case 'v':  buf[bidx++] = '\v';   break;
            case '0':  buf[bidx++] = '\0';   break;

            case '\\': buf[bidx++] = '\\'; break;
            case '\"': buf[bidx++] = '\"'; break;
            case '\'': buf[bidx++] = '\''; break;

            case 'x':
                EL_TODO("implement \\x escape sequence");
            case 'o':
                EL_TODO("implement \\o escape sequence");
            case 'u':
                EL_TODO("implement \\u escape sequence");

            default:
                el_diag_report(
                    diag, EL_DIAG_ERROR, "syntax.unknown-escape",
                    get_escape_span(tok, i - 1), "unknown escape sequence: \\{char}",
                    EL_DIAG_CHAR("char", next)
                );
                return EL_SV_NULL;
            }
        } else {
            buf[bidx++] = input.data[i];
        }
    }

    return el_sv_from_data_and_len(buf, bidx);
}

char el_parse_char_with_escapes(ElDiagEngine* diag, ElToken tok, char buf[static tok.lexeme.len], bool* ok) {
    ElStringView str = el_parse_str_with_escapes(diag, tok, buf);
    if (el_sv_is_null(str)) {
        *ok = false;
        return 'Q';
    }

    if (str.len != 1) {
        el_diag_report(
            diag, EL_DIAG_ERROR, "syntax.invalid-literal",
            tok.span, "character literal may only contain one character"
        );
        el_diag_help(
            diag, "if you meant a string literal, use double quotes instead"
        );

        *ok = false;
        return 'X';
    }

    *ok = true;
    return str.data[0];
}
