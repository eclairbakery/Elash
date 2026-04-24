#include <stdio.h>  // NOLINT(misc-include-cleaner)
#include <stdlib.h> // NOLINT(misc-include-cleaner)

#if defined(__GNUC__) || defined(__clang__)
    #define EL_UNREACHABLE(STR) do { \
        fprintf(stderr, "REACHED UNREACHABLE: %s (%s:%d)\n", STR, __FILE__, __LINE__); \
        __builtin_unreachable(); \
    } while (0)
#elif defined(_MSC_VER)
    #define EL_UNREACHABLE(STR) do { \
        fprintf(stderr, "REACHED UNREACHABLE: %s (%s:%d)\n", STR, __FILE__, __LINE__); \
        __assume(0); \
    } while (0)
#else
    #define EL_UNREACHABLE(STR) do { \
        fprintf(stderr, "REACHED UNREACHABLE: %s (%s:%d)\n", STR, __FILE__, __LINE__); \
        abort(); \
    } while (0)
#endif

#if defined(__GNUC__) || defined(__clang__)
    #define EL_UNREACHABLE_ENUM_VAL(TYPE, VAL) do { \
        fprintf(stderr, "invalid or not handled value %d for enum %s (%s:%d)\n", \
            (int)(VAL), #TYPE, __FILE__, __LINE__); \
        __builtin_unreachable(); \
    } while (0)
#elif defined(_MSC_VER)
    #define EL_UNREACHABLE_ENUM_VAL(TYPE, VAL) do { \
        fprintf(stderr, "invalid or not handled value %d for enum %s (%s:%d)\n", \
            (int)(VAL), #TYPE, __FILE__, __LINE__); \
        __assume(0); \
    } while (0)
#else
    #define EL_UNREACHABLE_ENUM_VAL(TYPE, VAL) do { \
        fprintf(stderr, "invalid or not handled value %d for enum %s (%s:%d)\n", \
            (int)(VAL), #TYPE, __FILE__, __LINE__); \
        abort(); \
    } while (0)
#endif

#ifdef EL_DEBUG
    #define EL_ASSERT(EXPR, MSG) do { \
        if (!(EXPR)) { \
            fprintf(stderr, "ASSERTION FAILED: %s\nMessage: %s\nLocation: %s:%d\n", \
                #EXPR, MSG, __FILE__, __LINE__); \
            abort(); \
        } \
    } while (0)
#else
    #define EL_ASSERT(EXPR, MSG) ((void)0)
#endif
