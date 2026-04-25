#pragma once

#include <elash/util/strbuf.h>
#include <elash/diag/meta.h>

bool el_diag_render_template(ElStringView template, const ElDiagMeta* meta, ElStringBuf* out);
