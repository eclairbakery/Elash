#include <elash/parser/utility.h>

ElParserErrorCode _el_parser_ret_ok(ElParser* parser) {
    parser->last_err_details.code = EL_PARSER_ERR_OK;
    return EL_PARSER_ERR_OK;
}

ElParserErrorCode _el_parser_ret_err_impl(ElParser* parser, const ElParserErrorDetails* d) {
    parser->last_err_details = *d;
    return d->code;
}

