#pragma once

typedef enum ElParserErrorCode {
    EL_PARSER_ERR_OK,
} ElParserErrorCode;

typedef struct ElParserErrorDetails {
    ElParserErrorCode code;
    // TODO
} ElParserErrorDetails;
