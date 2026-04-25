#pragma once

typedef struct ElDiagnostic ElDiagnostic;
typedef struct ElDiagSummary ElDiagSummary;
typedef struct ElDiagPrinter ElDiagPrinter;

typedef void ElDiagPrinterBegin  (ElDiagPrinter* self);
typedef void ElDiagPrinterPrint  (ElDiagPrinter* self, const ElDiagnostic* diag);
typedef void ElDiagPrinterSummary(ElDiagPrinter* self, const ElDiagSummary* sum);
typedef void ElDiagPrinterFinish (ElDiagPrinter* self);

struct ElDiagPrinter {
    ElDiagPrinterBegin*   begin;
    ElDiagPrinterPrint*   print;
    ElDiagPrinterSummary* summary;
    ElDiagPrinterFinish*  finish;
    void* ctx;
};
