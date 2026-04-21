#include <el-parser/parser.h>
#include <el-parser/utility.h>

#include <el-ast/toplevel.h>
#include <el-ast/toplevel/func.h>

#include <el-util/todo.h>

ElParserErrorCode _el_parser_parse_func_def(ElParser* parser, ElAstTopLevelNode** out) {
    EL_TODO("not implemented");
}

ElParserErrorCode _el_parser_parse_toplevel(ElParser* parser, ElAstTopLevelNode** out) {
    ElAstStmtNode* stmt;
    ElParserErrorCode result = _el_parser_parse_block(parser, &stmt);
    if (result != EL_PARSER_ERR_OK) {
        return result;
    }

    // TODO: stub
    ElAstFuncParamList params = el_ast_make_func_param_list();
    el_ast_func_param_list_append(&params, el_ast_new_func_param(
        parser->arena,
        &el_ast_new_ident_node(parser->arena, EL_SV("int"))->as.ident,
        &el_ast_new_ident_node(parser->arena, EL_SV("argc"))->as.ident
    ));

    ElAstTopLevelNode* main = el_ast_new_func_definition(
        parser->arena,
        &el_ast_new_ident_node(parser->arena, EL_SV("int"))->as.ident,
        &el_ast_new_ident_node(parser->arena, EL_SV("main"))->as.ident,
        params, 
        &stmt->as.block
    );
    *out = main;
    return _el_parser_ret_ok(parser);
}
