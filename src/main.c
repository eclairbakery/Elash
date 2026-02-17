#include <el-srcdoc/srcdoc.h>
#include <el-lexer/lexer.h>
#include <el-pp/preproc.h>

#include <el-defs/sv.h>

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char* const* argv) {
    if (argc < 2) {
        printf("usage: elc file\n");
        return 1;
    }

    FILE* f = fopen(argv[1], "r");
    if (!f) {
        perror("Failed to open file");
        return 1;
    }
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char* buffer = malloc(size + 1);
    if (!buffer) {
        perror("Failed to allocate buffer");
        fclose(f);
        return 1;
    }
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);

    ElLexer lexer;
    ElStringView input_sv = el_sv_from_data_and_len(buffer, size);
    ElLexerErrorCode err = el_lexer_init(&lexer, input_sv, EL_LF_ALLOW_UTF8_IDENTS);
    if (err != EL_LEXERR_SUCCESS) {
        fprintf(stderr, "Lexer initialization error: %d\n", err);
        free(buffer);
        return 1;
    }

    ElPreprocessor pp;
    el_pp_init(&pp);

    ElSourceDocument preprocessed;
    el_srcdoc_init_empty(&preprocessed);

    ElToken t;
    do {
        if (el_pp_want_next_token(&pp)) {
            err = el_lexer_next_token(&lexer, &t);
            if (err != EL_LEXERR_SUCCESS) {
                el_lexer_result_print(lexer.last_err_details, stderr);
                free(buffer);
                el_lexer_destroy(&lexer);
                return 1;
            }
            el_pp_pass_token(&pp, t);
        }
        el_pp_preprocess(&pp, &t);

        el_token_print(&t, stdout);
        el_srcdoc_append_token(&preprocessed, &t);
        putchar('\n');
    } while (t.type != EL_TT_EOF);

    el_srcdoc_print(&preprocessed, stdout);

    free(buffer);
    el_pp_destroy(&pp);
    el_lexer_destroy(&lexer);
    el_srcdoc_destroy(&preprocessed);
    return 0;
}
