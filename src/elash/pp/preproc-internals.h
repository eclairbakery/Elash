#pragma once
#include <elash/pp/preproc.h> // IWYU pragma: export

ElPpErrorCode _el_pp_ret_error(ElPreprocessor* pp, ElPpErrorCode code);
ElPpErrorCode _el_pp_ret_success(ElPreprocessor* pp);
