#pragma once

typedef struct ElDiagnostic ElDiagnostic;

typedef void ElDiagPrinter(void* ctx, const ElDiagnostic* diag);
