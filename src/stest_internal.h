#ifndef __S_TEST_INTERNAL_H__
#define __S_TEST_INTERNAL_H__

#include <stdlib.h>
#if !defined(__S_TEST_H__) && !defined(__S_TEST_C__)
#error                                                                         \
    "This is internal header and can be only included in other internal headers or source file."
#endif

#include "stest_impl.h"

#define SCOPE_GUARD(block)                                                     \
  do {                                                                         \
    block                                                                      \
  } while (0)

#define _ASSERT(exp, file, line, fn_name)                                      \
  SCOPE_GUARD(if ((exp) == 0) {                                                \
    Metadata *metadata;                                                        \
    if ((metadata = metadata_new(ASSERT, #exp, NULL, NULL, file, line,         \
                                 fn_namee)) == NULL)                           \
      exit(EXIT_FAILURE);                                                      \
    assert_failed(metadata);                                                   \
  })

#define _ASSERT_EQ(src, dst, file, line, fn_name)                              \
  SCOPE_GUARD(if ((src) != (dst)) {                                            \
    Metadata *metadata;                                                        \
    if ((metadata = metadata_new(ASSERT_EQ, NULL, #src, #dst, file, line,      \
                                 fn_namee)) == NULL)                           \
      exit(EXIT_FAILURE);                                                      \
    assert_failed(metadata);                                                   \
  })

#define _ASSERT_NEQ(src, dst, file, line, fn_name)                             \
  SCOPE_GUARD(if ((src) == (dst)) {                                            \
    Metadata *metadata;                                                        \
    if ((metadata = metadata_new(ASSERT_NEQ, #src, #dst, file, line,           \
                                 fn_name)) == NULL)                            \
      exit(EXIT_FAILURE);                                                      \
    assert_failed(metadata);                                                   \
  })

#endif
