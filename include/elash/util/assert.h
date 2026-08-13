#include <stdio.h>  // IWYU pragma: export
#include <stdlib.h> // IWYU pragma: export

#if defined(EL_DEBUG)
    #define EL_TRAP() abort()
#else
    #if defined(__GNUC__) || defined(__clang__)
        #define EL_TRAP() __builtin_unreachable()
    #elif defined(_MSC_VER)
        #define EL_TRAP() __assume(0)
    #else
        #define EL_TRAP() abort()
    #endif
#endif

#define EL_UNREACHABLE(MSG) do {                                                   \
    fprintf(stderr, "REACHED UNREACHABLE: %s (%s:%d)\n", MSG, __FILE__, __LINE__); \
    EL_TRAP();                                                                     \
} while (0)

#define EL_UNREACHABLE_ENUM_VAL(TYPE, VAL) do {                                                   \
    (void)(TYPE)(VAL);                                                                            \
    fprintf(stderr, "REACHED UNREACHABLE: invalid or not handled value %d for enum %s (%s:%d)\n", \
        (int)(VAL), #TYPE, __FILE__, __LINE__);                                                   \
    EL_TRAP();                                                                                    \
} while (0)

#ifdef EL_DEBUG
    #define EL_ASSERT(EXPR, MSG) do {                                               \
        if (!(EXPR)) {                                                              \
            fprintf(stderr, "ASSERTION FAILED: %s\nMessage: %s\nLocation: %s:%d\n", \
                #EXPR, MSG, __FILE__, __LINE__);                                    \
            abort();                                                                \
        }                                                                           \
    } while (0)
#else
    #define EL_ASSERT(EXPR, MSG) ((void)(EXPR))
#endif
