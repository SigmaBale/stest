#ifndef __S_RUNNER_H__
#define __S_RUNNER_H__

#if !defined(__S_RUNNER_C__) && !defined(__S_HEADER__)
#error "Private header, only <stest.h> can be included directly"
#endif

#define __S_HEADER__
#include "slist.h"
#include "ssuite.h"
#undef __S_HEADER__
#include <stddef.h>

typedef struct _srunner srunner;

srunner *runner_new(void);

void runner_add_suite(srunner *runner, ssuite *suite);

void runner_add_suites(srunner *runner, ssuite **suites, size_t len);

void runner_run(srunner *runner);

void runner_free(srunner *runner);

#endif
