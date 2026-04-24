#include <elash/srcdoc/span.h>

#include <elash/srcdoc/srcdoc.h>
#include <elash/util/assert.h>

ElSourceSpan el_source_span_make(const ElSourceDocument* doc, ElSourceLocation start, ElSourceLocation end) {
    return (ElSourceSpan) {
        .doc   = doc,
        .start = start,
        .end   = end,
    };
}

ElStringView el_source_span_to_sv(ElSourceSpan span) {
    if (!span.doc) return EL_SV_NULL;

    ElStringView full_content = el_srcdoc_content(span.doc);

    if (span.start.offset > span.end.offset) return EL_SV_NULL;
    if (span.end.offset > full_content.len) return EL_SV_NULL;

    return el_sv_slice(full_content, (usize)span.start.offset, (usize)span.end.offset);
}

ElSourceSpan el_source_span_merge(ElSourceSpan a, ElSourceSpan b) {
    if (!a.doc) return b;
    if (!b.doc) return a;

    EL_ASSERT(a.doc == b.doc, "Merging spans from different documents");

    ElSourceLocation start = a.start.offset < b.start.offset ? a.start : b.start;
    ElSourceLocation end   = a.end.offset > b.end.offset ? a.end : b.end;

    return el_source_span_make(a.doc, start, end);
}

bool el_source_span_is_valid(ElSourceSpan span) {
    return span.doc != NULL;
}

bool el_source_span_is_empty(ElSourceSpan span) {
    return span.start.offset == span.end.offset;
}
