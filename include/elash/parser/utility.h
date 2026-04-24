#include <elash/parser/parser.h>

ElParserErrorCode _el_parser_ret_ok(ElParser* parser);
ElParserErrorCode _el_parser_ret_err_impl(ElParser* parser, const ElParserErrorDetails* d);

#define _el_parser_ret_err(parser, ...) \
    _el_parser_ret_err_impl((parser), (&(const ElParserErrorDetails) { __VA_ARGS__ }))
