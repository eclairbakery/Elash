#pragma once

#include <elash/defs/sv.h>

typedef enum ElDiagSeverity {
    EL_DIAG_TIP,
    EL_DIAG_NOTE,
    EL_DIAG_WARN,
    EL_DIAG_ERROR,
} ElDiagSeverity;

ElStringView el_diag_severity_to_string(ElDiagSeverity sev);
