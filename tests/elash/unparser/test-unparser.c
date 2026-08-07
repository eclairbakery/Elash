#include <criterion/criterion.h>
#include <elash/defs/int-types.h>

#include <elash/ast/equal.h>
#include <elash/lexer/lexer.h>
#include <elash/lexer/tokbuf.h>
#include <elash/util/dynarena.h>
#include <elash/parser/parser.h>
#include <elash/unparser/unparser.h>

#include <stdio.h>
#include <glob.h>

static ElDynArena arena;
void init() { el_dynarena_init(&arena); }
void fini() { el_dynarena_free(&arena); }

TestSuite(unparser, .init = init, .fini = fini);

// --- integration test ---
Test(unparser, integration_test) {
    // this kinda sucks but i guess will work for now
    glob_t g;
    cr_assert_eq(glob("tests/e2e/positive/*/*.ei", 0, NULL, &g), 0);

    ElDiagEngine diag = {0};
    ElTkBufStream stream;
    ElTokenBuf toks;

    ElUnparser unparser;
    ElParser parser;
    ElLexer lexer;

    el_tkbuf_init(&toks);
    for (usize i = 0; i < g.gl_pathc; ++i) {
        el_tkbuf_clear(&toks);

        ElSourceDocument doc;
        cr_assert_eq(el_srcdoc_init_from_file(&doc, g.gl_pathv[i]), EL_SRCDOC_ERR_SUCCESS);

        el_lexer_init(&lexer, &doc, EL_LEXER_FLAGS_DEFAULT|EL_LF_SKIP_COMMENTS);

        el_parser_init(&parser, el_lexer_as_token_stream(&lexer), &diag, &arena);
        ElAstModule* orig = el_parser_parse_module(&parser);
        el_parser_destroy(&parser);

        el_unparser_init(&unparser, &toks, &arena);
        el_unparser_unparse_module(&unparser, orig);

        el_parser_init(&parser, el_tkbuf_as_stream(&stream, &toks), &diag, &arena);
        ElAstModule* repro = el_parser_parse_module(&parser);
        el_parser_destroy(&parser);

        cr_assert(el_ast_equal_module(orig, repro));
    }

    globfree(&g);
}

// --- normal cases ---
// (maybe someday)
