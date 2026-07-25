#pragma once

typedef enum ElStorageClass {
    EL_STORAGECLS_LOCAL,  // equivalent to 'auto' in C
    EL_STORAGECLS_GLOBAL, // equivalent to 'static' in C
    // EL_STORAGECLS_TLS,
    // note that there is no internal/extern here.
    // in elash, storage classes and linkage specifiers
    // are separate concepts unlike in C
} ElStorageClass;
