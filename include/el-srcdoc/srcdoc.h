#pragma once

#include <el-lexer/token.h>
#include <el-util/strbuf.h>

#include <el-srcdoc/error.h>

typedef struct ElSourceDocument {
    ElStringBuf content;
} ElSourceDocument;

ElSrcDocErrorCode el_srcdoc_init_empty(ElSourceDocument* srcdoc);
ElSrcDocErrorCode el_srcdoc_init_from_str(ElSourceDocument* srcdoc, ElStringView sv);
ElSrcDocErrorCode el_srcdoc_init_from_file(ElSourceDocument* srcdoc, const char* path);

ElSrcDocErrorCode el_srcdoc_copy(const ElSourceDocument* src, ElSourceDocument* dst);
void              el_srcdoc_move(ElSourceDocument* src, ElSourceDocument* dst);
ElSrcDocErrorCode el_srcdoc_copy_from_strbuf(ElSourceDocument* srcdoc, const ElStringBuf* buf);
void              el_srcdoc_move_from_strbuf(ElSourceDocument* srcdoc, ElStringBuf* buf);

void              el_srcdoc_destroy(ElSourceDocument* srcdoc);
void              el_srcdoc_clear(ElSourceDocument* srcdoc);

ElSrcDocErrorCode el_srcdoc_append_token(ElSourceDocument* srcdoc, ElToken tok);
ElSrcDocErrorCode el_srcdoc_append_str(ElSourceDocument* srcdoc, ElStringView sv);

ElSrcDocErrorCode el_srcdoc_concat(const ElSourceDocument* src1,
                                   const ElSourceDocument* src2,
                                   ElSourceDocument* dst);

ElStringView      el_srcdoc_content(const ElSourceDocument* srcdoc);
usize             el_srcdoc_length(const ElSourceDocument* srcdoc);

