#include <elash/diag/severity.h>

#include <elash/util/assert.h>

ElStringView el_diag_severity_to_string(ElDiagSeverity sev) {
    switch (sev) {
    case EL_DIAG_ERROR: return EL_SV("error");
    case EL_DIAG_WARN:  return EL_SV("warning");
    case EL_DIAG_NOTE:  return EL_SV("note");
    case EL_DIAG_TIP:   return EL_SV("tip");
    }
    EL_UNREACHABLE_ENUM_VAL(ElDiagSeverity, sev);
}
