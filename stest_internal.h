#ifndef __S_TEST_INTERNAL_H__
#define __S_TEST_INTERNAL_H__

#if !defined(__S_TEST_H__) && !defined(__S_TEST_C__)
#error "Only <stest.h> can be included directly."
#endif

typedef struct _Suite Suite;

typedef struct _Runner Runner;

typedef enum { ASSERT, ASSERT_EQ, ASSERT_STREQ, ASSERT_NEQ } assert_t;

#define _ASSERT(exp, desc, line)                                               \
  if (!(exp))                                                                  \
  assert_failed(ASSERT, desc, line)

#define _ASSERT_EQ(src, dst, desc, line)                                       \
  if ((src) != (dst))                                                          \
  assert_failed(ASSERT_EQ, desc, line)

#define _ASSERT_NEQ(src, dst, desc, line)                                      \
  if ((src) == (dst))                                                          \
  assert_failed(ASSERT_NEQ, desc, line)

#define _START_TEST(fn_name) start_test(fn_name)

#define _END_TEST end_test()

#define _PRINT_TEST_MODULE(filename)                                           \
  fprintf(stdout, "\x1b[1;37m[%s]\x1b[0m\n", filename);

void start_test(char *);
void end_test(void);
void assert_failed(assert_t, char *, int);

#endif
