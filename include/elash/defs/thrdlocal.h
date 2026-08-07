#pragma once

#if defined(__STDC_VERSION__) && __STDC_VERSION__ >= 202311L
  #define EL_THRD_LOCAL thread_local
#elif defined(__STDC_VERSION__) && __STDC_VERSION__ >= 201112L
  #define EL_THRD_LOCAL _Thread_local
#elif defined(__GNUC__) || defined(__clang__)
  #define EL_THRD_LOCAL __thread
#else
  #error "No thread-local storage support"
#endif
