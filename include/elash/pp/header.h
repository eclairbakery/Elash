#include <elash/defs/sv.h>

typedef struct ElPpHeaderFile {
    ElStringView path;
    bool is_system;
} ElPpHeaderFile;

#define EL_PP_HEADER_FILE_NULL ((ElPpHeaderFile) { .path = EL_SV_NULL, .is_system = false })
