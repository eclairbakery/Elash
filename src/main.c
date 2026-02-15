#include <el-lexer/lexer.h>
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

#ifdef RAM_SHORTAGE
    // Due to the ongoing RAM shortage and our heroic mission 
    // to make our compiler accessible to all, regardless of social 
    // or financial status, the maximum size of this buffer has been 
    // bravely capped at 256 bytes.
    if (size > 256) size = 256;
#endif

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
    ElLexerErrorCode err = el_lexer_init(&lexer, input_sv, EL_LF_SKIP_WHITESPACE);
    if (err != EL_LEXERR_SUCCESS) {
        fprintf(stderr, "Lexer initialization error: %d\n", err);
        free(buffer);
        return 1;
    }

    ElToken t;
    do {
        err = el_lexer_next_token(&lexer, &t);
        if (err != EL_LEXERR_SUCCESS) {
            el_lexer_result_print(lexer.last_err_details, stderr);
            free(buffer);
            el_lexer_destroy(&lexer);
            return 1;
        }

        el_token_print(t, stdout);
        putchar('\n');
    } while (t.type != EL_TT_EOF);

    //free(buffer);
    el_lexer_destroy(&lexer);
    return 0;
}
