#ifndef __S_SUITE_H__
#define __S_SUITE_H__

#if !defined(__S_SUITE_C__) && !defined(__S_HEADER__) &&                       \
    !defined(__S_SRC_FILE__)
#error "Private header, only <stest.h> can be included directly."
#endif

#define __S_SRC_FILE__
#include "slist.h"
#undef __S_SRC_FILE__

typedef struct _ssuite ssuite;

typedef struct _stest stest;

typedef void (*fnptr)(void);

stest *stest_new(const char *name, fnptr ptr);

ssuite *ssuite_new(const char *name);

stest *stest_new(const char *name, fnptr fn);

void ssuite_add_test(ssuite *suite, const char *name, fnptr fn);

/*
 * TODO: put this in public header
 */
#define ssuite_add_test(ssuite, fn) _ssuite_add_test(ssuite, #fn, fn)

void ssuite_run_tests(const ssuite *suite);

void ssuite_free(ssuite *suite, FreeFn free_fn);

void stest_free(stest *test);

#endif
