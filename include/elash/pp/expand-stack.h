#include <elash/pp/expand-state.h>

typedef struct ElPpExpandStack {
    ElPpExpandState* states;
    usize len;
    usize cap;
} ElPpExpandStack;
