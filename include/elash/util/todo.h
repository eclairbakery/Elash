#pragma once

#include <stdio.h>  // NOLINT(misc-include-cleaner)
#include <stdlib.h> // NOLINT(misc-include-cleaner)

#define EL_TODO(MSG) do { \
    fprintf(stderr, "TODO: %s (in %s at %s:%d)\n", MSG, __func__, __FILE__, __LINE__); \
    abort(); \
} while (0)
