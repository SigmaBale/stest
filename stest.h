#ifndef __S_TEST_H__
#define __S_TEST_H__

#include "stest_internal.h"
#include <stddef.h>
#include <stdio.h>
#include <time.h>

#ifndef MAXERROR
#define MAXERROR 1000
#endif

#ifndef PRINT_TEST_MODULE
#define PRINT_TEST_MODULE(filename) _PRINT_TEST_MODULE(filename)
#endif

#ifndef START_TEST
#define START_TEST _START_TEST(__func__)
#endif

#ifndef END_TEST
#define END_TEST _END_TEST
#endif

#ifndef ASSERT_EQ
#define ASSERT_EQ(src, dst, desc) _ASSERT_EQ(src, dst, desc, __LINE__)
#endif

#ifndef ASSERT_NEQ
#define ASSERT_NEQ(src, dst, desc) _ASSERT_NEQ(src, dst, desc, __LINE__)
#endif

#ifndef ASSERT
#define ASSERT(exp, desc) _ASSERT(exp, desc, __LINE__)
#endif

#endif
