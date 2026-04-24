#pragma once

#include <elash/defs/source-loc.h>
#include <elash/defs/sv.h>

typedef struct ElSourceDocument ElSourceDocument;

typedef struct ElSourceSpan {
    const ElSourceDocument* doc;
    ElSourceLocation        start;
    ElSourceLocation        end;
} ElSourceSpan;

#define EL_SOURCE_SPAN_NULL ((ElSourceSpan){ .doc = NULL, .start = EL_SOURCE_LOC_ZERO, .end = EL_SOURCE_LOC_ZERO })

ElSourceSpan el_source_span_make(const ElSourceDocument* doc, ElSourceLocation start, ElSourceLocation end);

/// Returns a string view of the content covered by the span.
ElStringView el_source_span_to_sv(ElSourceSpan span);

/// Merges two spans into one that covers both.
/// Spans must be from the same document.
ElSourceSpan el_source_span_merge(ElSourceSpan a, ElSourceSpan b);

/// Returns true if the span is not null and has a document.
bool el_source_span_is_valid(ElSourceSpan span);

/// Returns true if the span is empty (start == end).
bool el_source_span_is_empty(ElSourceSpan span);
