#include <elash/parser/parser.h>
#include <elash/parser/utility.h>

#include <elash/ast/module.h>

ElParserErrorCode _el_parser_parse_module(ElParser* parser, ElAstModuleNode** out) {
    ElAstModuleNode* module = el_ast_new_module(parser->arena);

    while (parser->current.type != EL_TT_EOF) {
        ElAstTopLevelNode* node = NULL;
        ElParserErrorCode err = _el_parser_parse_toplevel(parser, &node);
        if (err != EL_PARSER_ERR_OK) return err;

        if (node) {
            el_ast_module_append(module, node);
        }
    }

    *out = module;

    return _el_parser_ret_ok(parser);
}
