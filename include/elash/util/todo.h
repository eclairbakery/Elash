#pragma once

#include <stdio.h>  // IWYU pragma: export
#include <stdlib.h> // IWYU pragma: export

#define EL_TODO(MSG) do { \
    fprintf(stderr, "TODO: %s (in %s at %s:%d)\n", MSG, __func__, __FILE__, __LINE__); \
    abort(); \
} while (0)
