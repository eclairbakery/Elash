#pragma once

typedef enum ElPpErrorCode {
    EL_PPERR_SUCCESS,
    EL_PPERR_ALLOC_FAILED,

    EL_PPERR_NO_SUCH_VAR,

    EL_PPERR_INCLUDE_FILE_NOT_FOUND,
    EL_PPERR_EMBED_FILE_NOT_FOUND,
} ElPpErrorCode;

typedef struct ElPpErrorDetails {
    ElPpErrorCode code;
} ElPpErrorDetails;
