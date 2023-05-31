#ifndef __S_TEST_INTERNAL_H__
#define __S_TEST_INTERNAL_H__

#if !defined(__S_TEST_H__) && !defined(__S_TEST_C__)
#error "Internal header, only <stest.h> can be included directly."
#endif

#define __S_HEADER__
#include "smdata.h"
#include "ssuite.h"
#define __S_HEADER__
#include "srunner.h"
#undef __S_HEADER__

#include <stdlib.h>
#include <string.h>

#define SCOPE_GUARD(block)                                                     \
  do {                                                                         \
    block                                                                      \
  } while (0)

#define _ASSERT(exp, file, line, fn_name)                                      \
  SCOPE_GUARD(if ((exp) == 0) {                                                \
    smdata *metadata;                                                          \
    if ((metadata = smdata_new(ASSERT, #exp, NULL, NULL, file, line,           \
                               fn_namee)) == NULL)                             \
      exit(EXIT_FAILURE);                                                      \
    assert_failed(metadata);                                                   \
  })

#define _ASSERT_EQ(src, dst, file, line, fn_name)                              \
  SCOPE_GUARD(if ((src) != (dst)) {                                            \
    smdata *metadata;                                                          \
    if ((metadata = smdata_new(ASSERT_EQ, NULL, #src, #dst, file, line,        \
                               fn_namee)) == NULL)                             \
      exit(EXIT_FAILURE);                                                      \
    assert_failed(metadata);                                                   \
  })

#define _ASSERT_NEQ(src, dst, file, line, fn_name)                             \
  SCOPE_GUARD(if ((src) == (dst)) {                                            \
    smdata *metadata;                                                          \
    if ((metadata =                                                            \
             smdata_new(ASSERT_NEQ, #src, #dst, file, line, fn_name)) == NULL) \
      exit(EXIT_FAILURE);                                                      \
    assert_failed(metadata);                                                   \
  })

#define _ASSERT_STR_NEQ(src, dst, file, line, fn_name)                         \
  SCOPE_GUARD(if (strcmp(src, dst) == 0) {                                     \
    smdata *metadata;                                                          \
    if ((metadata = smdata_new(ASSERT_STRING_NEQ, #src, #dst, file, line,      \
                               fn_name)) == NULL)                              \
      exit(EXIT_FAILURE);                                                      \
    assert_failed(metadata);                                                   \
  })

#define _ASSERT_STR_EQ(src, dst, file, line, fn_name)                          \
  SCOPE_GUARD(if (strcmp(src, dst) != 0) {                                     \
    smdata *metadata;                                                          \
    if ((metadata = smdata_new(ASSERT_STRING_EQ, #src, #dst, file, line,       \
                               fn_name)) == NULL)                              \
      exit(EXIT_FAILURE);                                                      \
    assert_failed(metadata);                                                   \
  })

#define _ssuite_add_test(ssuite, fn) __ssuite_add_test(ssuite, #fn, fn)

#endif
