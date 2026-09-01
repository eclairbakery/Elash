#pragma once

#include <stdbool.h>
#include <string.h>
#include <stdio.h>

static inline FILE* el_open_ifile(const char* path, bool* out_needs_closing) {
    if (strcmp(path, "-") == 0)
        return *out_needs_closing = false, stdin;

    *out_needs_closing = true;
    FILE* f = fopen(path, "rb");
    if (f == NULL) perror("fopen");
    return f;
}

static inline FILE* el_open_ofile(const char* path, bool* out_needs_closing) {
    if (strcmp(path, "-") == 0)
        return *out_needs_closing = false, stdout;

    *out_needs_closing = true;
    FILE* f = fopen(path, "wb");
    if (f == NULL) perror("fopen");
    return f;
}
