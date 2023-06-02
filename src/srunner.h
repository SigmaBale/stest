#ifndef __S_RUNNER_H__
#define __S_RUNNER_H__

#if !defined(__S_SRC_FILE__) && !defined(__S_HEADER__)
#error "Private header, only <stest.h> can be included directly"
#endif

#include <stddef.h>

typedef struct _srunner srunner;

typedef struct _ssuite ssuite;

typedef struct _smdata smdata;

typedef void (*fnptr)(void);

srunner *srunner_new(void);

void srunner_add_suite(srunner *runner, ssuite *suite);

void srunner_add_suites(srunner *runner, ssuite **suites, size_t len);

void srunner_run(srunner *runner);

void srunner_free(srunner *runner);

ssuite *ssuite_new(const char *name);

void ssuite_free(ssuite *suite);

smdata *smdata_new(const char *exp, const char *src, const char *dst,
                   const char *file, const int line, const char *fn_name);

void smdata_free(smdata *metadata);

void assert_failed(smdata *metadata);

#endif
