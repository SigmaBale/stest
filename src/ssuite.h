#ifndef __S_SUITE_H__
#define __S_SUITE_H__

#if !defined(__S_HEADER__) && !defined(__S_SRC_FILE__)
#error "Private header, only <stest.h> can be included directly."
#endif

#if !defined(NO_COLOR) && !defined(PRINT_TEST)
/*
 * If NO_COLOR and PRINT_TEST are not defined,
 * stest will use this format to output if test is successfull.
 *
 * Define NO_COLOR if you want the colorless output (default color).
 *
 * Or you can define your PRINT_TEST, make sure the macro
 * can contain a single value as in 'PRINT_TEST(val)'.
 */
#define PRINT_TEST(filename)                                                   \
  fprintf(stdout, "Running test \x1b[1;33m%s\x1b[0m ----> ", filename)
#endif

#if defined(NO_COLOR) && !defined(PRINT_TEST)
#define PRINT_TEST(filename) fprintf(stdout, "Running test %s ----> ", filename)
#endif

#if !defined(NO_COLOR) && !defined(PRINT_SUCCESS)
#define PRINT_SUCCESS(time)                                                    \
  fprintf(stdout, "\x1b[1;32msucceeded\x1b[0m [%Lf]\n", time)
#endif

#if defined(NO_COLOR) && !defined(PRINT_SUCCESS)
#define PRINT_SUCCESS(time) fprintf(stdout, "succeeded [%ld]\n", time)
#endif

#if !defined(NO_COLOR) && !defined(PRINT_FAILURE)
#define PRINT_FAILURE(time)                                                    \
  fprintf(stdout, "\x1b[1;31mfailed\x1b[0m [%Lf]\n", time)
#endif

#if defined(NO_COLOR) && !defined(PRINT_FAILURE)
#define PRINT_FAILURE(time) fprintf(stdout, "failed [%ld]\n", time)
#endif

typedef struct _ssuite ssuite;

typedef void (*fnptr)(void);

ssuite *ssuite_new(const char *name);

void ssuite_add_test(ssuite *suite, const char *name, fnptr fn);

/*
 * Do not use this function, use the macro instead
 * called 'ssuite_add_test'!
 */
void __ssuite_add_test(ssuite *suite, const char *name, fnptr fn);

void ssuite_run_tests(const ssuite *suite);

void ssuite_free(ssuite *suite);

#endif
