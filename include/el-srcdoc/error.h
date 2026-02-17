#pragma once

typedef enum ElSrcDocErrorCode {
    EL_SRCDOC_ERR_SUCCESS,

    EL_SRCDOC_ERR_ALLOC_FAILED,
} ElSrcDocErrorCode;

typedef struct ElSrcDocErrorDetails {
    ElSrcDocErrorCode code;
} ElSrcDocErrorDetails;

#define EL_SRCDOC_ERR_DETAILS_OK \
    ((ElSrcDocErrorDetails) { .code = EL_SRCDOC_ERR_SUCCESS })
