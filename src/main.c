#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int main(int argc, char **argv) {

    if (argc < 2) {
        printf("usage: elc file\n");
        return 1;
    }

    FILE *f = fopen(argv[1], "r");
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    rewind(f);

    char *buffer = malloc(size + 1);
    fread(buffer, 1, size, f);
    buffer[size] = '\0';
    fclose(f);

    Lexer lexer;
    lexer_init(&lexer, buffer);

    Token t;

    do {
        t = next_token(&lexer);
        printf("type=%d value=%s line=%d col=%d\n",
               t.type, t.value, t.line, t.column);
    } while (t.type != TOKEN_EOF);

    free(buffer);
}

