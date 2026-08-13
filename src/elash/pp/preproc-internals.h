#pragma once
#include <elash/pp/preproc.h> // IWYU pragma: export

bool _el_pp_preprocess_directive(ElPreproc* pp, ElToken* out_tok);

bool _el_pp_handle_include(ElPreproc* pp, ElToken* out_tok);
bool _el_pp_handle_embed  (ElPreproc* pp, ElToken* out_tok);
