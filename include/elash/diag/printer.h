#pragma once

#include <stdio.h>

typedef struct ElDiagnostic ElDiagnostic;
typedef struct ElDiagSummary ElDiagSummary;
typedef struct ElDiagPrinter ElDiagPrinter;

typedef void ElDiagPrinterBegin  (ElDiagPrinter* self, FILE* out);
typedef void ElDiagPrinterPrint  (ElDiagPrinter* self, FILE* out, const ElDiagnostic* diag);
typedef void ElDiagPrinterSummary(ElDiagPrinter* self, FILE* out, const ElDiagSummary* sum);
typedef void ElDiagPrinterFinish (ElDiagPrinter* self, FILE* out);

struct ElDiagPrinter {
    ElDiagPrinterBegin*   begin;
    ElDiagPrinterPrint*   print;
    ElDiagPrinterSummary* summary;
    ElDiagPrinterFinish*  finish;
    void* ctx;
};
