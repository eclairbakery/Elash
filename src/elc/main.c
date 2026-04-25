#include <elash/srcdoc/srcdoc.h>
#include <elash/lexer/lexer.h>
#include <elash/pp/preproc.h>
#include <elash/parser/parser.h>
#include <elash/hir/dump/toplevel.h>
#include <elash/util/dynarena.h>

#include <elash/diag/engine.h>
#include <elash/diag/handle.h>
#include <elash/diag/printer/console.h>

#include <elash/defs/sv.h>

#include <stdio.h>

int main(int argc, const char* const* argv) {
    if (argc < 2) {
        printf("usage: elc file\n");
        return 1;
    }

    const char* input_file = argv[1];

    ElSourceDocument input;
    ElSrcDocErrorCode derr = el_srcdoc_init_from_file(&input, input_file);
    printf("%d", derr);

    ElLexer lexer;
    ElLexerErrorCode err = el_lexer_init(&lexer, &input, EL_LF_ALLOW_UTF8_IDENTS);
    if (err != EL_LEXERR_SUCCESS) {
        fprintf(stderr, "Lexer initialization error: %d\n", err);
        el_srcdoc_destroy(&input);
        return 1;
    }

    ElPreprocessor pp;
    el_pp_init(&pp);

    ElSourceDocument preprocessed;
    el_srcdoc_init_empty(&preprocessed, EL_SV("<preprocessed>"));

    ElDynArena arena;
    el_dynarena_init(&arena);

    ElToken t;
    do {
        if (el_pp_want_next_token(&pp)) {
            err = el_lexer_next_token(&lexer, &t);
            if (err != EL_LEXERR_SUCCESS) {
                ElDiagEngine engine;
                el_diag_engine_init(&engine, &arena);
                el_diag_handle_lexer_error(&engine, &lexer.last_err_details);
                ElDiagPrinter printer = el_diag_make_console_printer();
                el_diag_engine_print(&engine, &printer, stdout);

                el_srcdoc_destroy(&input); 
                el_pp_destroy(&pp);
                el_lexer_destroy(&lexer);
                el_srcdoc_destroy(&preprocessed);
                return 1;
            }
            el_pp_pass_token(&pp, t);
        }


        ElPpErrorCode pperr = el_pp_preprocess(&pp, &t);
        if (pperr == EL_PPERR_SKIPPED) continue;
        if (pperr != EL_PPERR_SUCCESS) {
            // TODO: handle error
        }

        // el_token_print(&t, stdout);
        el_srcdoc_append_token(&preprocessed, &t);
        // putchar('\n');
    } while (t.type != EL_TT_EOF);

    el_srcdoc_print(&preprocessed, stdout);
    putchar('\n');

    el_lexer_set_document(&lexer, &input);
    el_pp_reset(&pp);

    ElParser parser;
    el_parser_init(&parser, &pp, &lexer, &arena);

    ElAstModuleNode* ast = NULL;
    ElParserErrorCode perr = el_parser_parse(&parser, &ast);
    if (perr == EL_PARSER_ERR_OK) {
        printf("--- AST ---\n");
        el_ast_dump_module(ast, 0, stdout);
    } else {
        fprintf(stderr, "Parser error: %d\n", perr);
    }

    el_dynarena_free(&arena);
    el_parser_destroy(&parser);

    el_srcdoc_destroy(&input); 
    el_pp_destroy(&pp);
    el_lexer_destroy(&lexer);
    el_srcdoc_destroy(&preprocessed);
    return 0;
}
