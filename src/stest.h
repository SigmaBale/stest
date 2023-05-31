#ifndef __S_TEST_H__
#define __S_TEST_H__

#include "stest_internal.h"

#ifndef TEST
/*
 * Wrapper around function name for
 * better clarity as to which function
 * is a test.
 */
#define TEST(test_fn) void (test_fn)()
#endif

/*
 * Assert SRC is equivalent to DST.
 */
#ifndef ASSERT_EQ
#define ASSERT_EQ(src, dst) _ASSERT_EQ(src, dst, __FILE__, __LINE__, __func__)
#endif

/*
 * Assert SRC is not equivalent to DST.
 */
#ifndef ASSERT_NEQ
#define ASSERT_NEQ(src, dst) _ASSERT_NEQ(src, dst, __FILE__, __LINE__, __func__)
#endif

/*
 * Assert EXP is 1 (true).
 */
#ifndef ASSERT
#define ASSERT(exp) _ASSERT(exp, __FILE__, __LINE__, __func__)
#endif

/*
 * Assert SRC string is equivalent to DST string.
 */
#ifndef ASSERT_STR_EQ
#define ASSERT_STR_EQ(src, dst)                                                \
  _ASSERT_STR_EQ(src, dst, __FILE__, __LINE__, __func__)
#endif

/*
 * Assert SRC string is not equivalent to DST string.
 */
#ifndef ASSERT_STR_NEQ
#define ASSERT_STR_NEQ(src, dst)                                               \
  _ASSERT_STR_NEQ(src, dst, __FILE__, __LINE__, __func__)
#endif

/*
 * Inserts FN into SSUITE.
 */
#ifndef ssuite_add_test
#define ssuite_add_test(ssuite, fn) _ssuite_add_test(ssuite, fn)
#endif


#endif
