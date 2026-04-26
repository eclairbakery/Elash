#include <elash/diag/engine.h>
#include <elash/diag/handle.h>
#include <elash/diag/printer/console.h>

#include <elc/driver/driver.h>

/*
int main(int argc, const char* const* argv) {
    if (argc < 2) {
        printf("usage: elc file\n");
        return 1;
    }

    const char* input_file = argv[1];

    ElSourceDocument input;
    el_srcdoc_init_from_file(&input, input_file);

    ElLexer lexer;
    ElLexerErrorCode err = el_lexer_init(&lexer, &input, EL_LF_ALLOW_UTF8_IDENTS);
    if (err != EL_LEXERR_SUCCESS) {
        fprintf(stderr, "Lexer initialization error: %d\n", err);
        el_srcdoc_destroy(&input);
        return 1;
    }

    ElDynArena arena;
    el_dynarena_init(&arena);

    ElDiagEngine diag_engine;
    el_diag_engine_init(&diag_engine, &arena);

    ElPreprocessor pp;
    el_pp_init(&pp, el_lexer_as_token_stream(&lexer));

    ElSourceDocument preprocessed;
    el_srcdoc_init_empty(&preprocessed, EL_SV("<preprocessed>"));

    ElToken t;
    while (true) {
        ElPpErrorCode pperr = el_pp_next(&pp, &t, &diag_engine);
        if (pperr == EL_PPERR_SKIPPED) continue;
        
        if (t.type == EL_TT_UNKNOWN) {
            ElDiagPrinter printer = el_diag_make_console_printer();
            el_diag_engine_print(&diag_engine, &printer, stdout);

            el_srcdoc_destroy(&input); 
            el_pp_destroy(&pp);
            el_lexer_destroy(&lexer);
            el_srcdoc_destroy(&preprocessed);
            el_diag_engine_free(&diag_engine);
            el_dynarena_free(&arena);
            return 1;
        }

        if (t.type == EL_TT_EOF) break;

        el_srcdoc_append_token(&preprocessed, &t);
    }

    el_srcdoc_print(&preprocessed, stdout);
    putchar('\n');

    el_lexer_reset(&lexer);
    el_lexer_set_document(&lexer, &input);
    el_pp_reset(&pp, el_lexer_as_token_stream(&lexer));

    ElParser parser;
    el_parser_init(&parser, el_pp_as_token_stream(&pp), &diag_engine, &arena);

    ElAstModuleNode* ast = NULL;
    ElParserErrorCode perr = el_parser_parse(&parser, &ast);
    if (perr == EL_PARSER_ERR_OK) {
        printf("--- AST ---\n");
        el_ast_dump_module(ast, 0, stdout);
    } else {
        fprintf(stderr, "Parser error: %d\n", perr);
        ElDiagPrinter printer = el_diag_make_console_printer();
        el_diag_engine_print(&diag_engine, &printer, stdout);
    }

    el_dynarena_free(&arena);
    el_diag_engine_free(&diag_engine);
    el_parser_destroy(&parser);

    el_srcdoc_destroy(&input); 
    el_pp_destroy(&pp);
    el_lexer_destroy(&lexer);
    el_srcdoc_destroy(&preprocessed);
    return 0;
}
*/


int main(int argc, const char* const* argv) {
    if (argc < 2) {
        fprintf(stderr, "usage: %s source-code.ela\n", argv[0]);
        return 1;
    }

    const char* input_path = argv[1];

    ElcDriver driver;
    if (!elc_driver_init(&driver)) {
        fprintf(stderr, "failed to initialize driver\n");
        return 1;
    }

    ElSourceDocument source;
    if (el_srcdoc_init_from_file(&source, input_path) != EL_SRCDOC_ERR_SUCCESS) {
        fprintf(stderr, "failed to load input file: %s\n", input_path);
        elc_driver_free(&driver);
        return 1;
    }

    elc_driver_provide_source(&driver, &source);
    elc_driver_register_stages(&driver);
    elc_driver_register_observers(&driver);

    bool success = elc_driver_run(&driver);

    ElDiagPrinter printer = el_diag_make_console_printer();
    el_diag_engine_print(&driver.diag, &printer, stdout);

    el_srcdoc_destroy(&source);
    elc_driver_free(&driver);

    return success ? 0 : 1;
}
