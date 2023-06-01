#define __S_SRC_FILE__
#include "slist.h"
#include "srunner.h"
#include "stable.h"
#undef __S_SRC_FILE__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/***************************************************************************************/
/*                                    OUTPUT FORMAT                                    */
/***************************************************************************************/

static const char* _S_RESET_FORMAT        = "\x1b[0m";
static const char* _S_BRIGHT_RED_BOLD     = "\x1b[1;91m";
static const char* _S_BRIGHT_GREEN_BOLD   = "\x1b[1;92m";
static const char* _S_BRIGHT_YELLOW_BOLD  = "\x1b[1;93m";
static const char* _S_BRIGHT_BLUE_BOLD    = "\x1b[1;94m";
static const char* _S_BRIGHT_MAGENTA_BOLD = "\x1b[1;95m";

#define PRINT_TEST(filename)        \
    fprintf(stdout,                 \
            "\n%s%s%s ---------> ", \
            _S_BRIGHT_YELLOW_BOLD,  \
            filename,               \
            _S_RESET_FORMAT)

#define PRINT_SUCCESS(time)            \
    fprintf(stdout,                    \
            "%ssucceeded%s [%Lf s]\n", \
            _S_BRIGHT_GREEN_BOLD,      \
            _S_RESET_FORMAT,           \
            time)

#define PRINT_FAILURE(time) \
    fprintf(stdout, "%sfailed%s [%Lf s]\n", _S_BRIGHT_RED_BOLD, _S_RESET_FORMAT, time)

/***************************************************************************************/

typedef struct _smdata {
    const char* exp;
    const char* src;
    const char* dst;
    const char* file;
    int         line;
    const char* fn_name;
} smdata;

smdata*
smdata_new(const char* exp,
           const char* src,
           const char* dst,
           const char* file,
           const int   line,
           const char* fn_name)
{
    smdata* metadata = (smdata*) malloc(sizeof(smdata));
    if(metadata != NULL) {
        metadata->exp     = exp;
        metadata->src     = src;
        metadata->dst     = dst;
        metadata->file    = file;
        metadata->line    = line;
        metadata->fn_name = fn_name;
    }
    return metadata;
}

void
smdata_free(smdata* metadata)
{
    if(metadata != NULL)
        free(metadata);
}

void
assert_failed(smdata* metadata)
{
    if(smap_insert(&__S_GLOBAL_TABLE__, (void*) metadata->fn_name, metadata) == NULL) {
        fprintf(stderr, "out of memory\n");
        exit(EXIT_FAILURE);
    }
}

void
smdata_print(const smdata* metadata)
{
    if(metadata->exp != NULL) {
        fprintf(stdout,
                "%s%s failed at:%s %s: line %d -> %sEXPRESSION%s: '%s'\n",
                _S_BRIGHT_YELLOW_BOLD,
                metadata->fn_name,
                _S_RESET_FORMAT,
                metadata->file,
                metadata->line,
                _S_BRIGHT_BLUE_BOLD,
                _S_RESET_FORMAT,
                metadata->exp);
    } else {
        fprintf(stdout,
                "%s%s failed at:%s %s: line %d -> %sLEFT%s: '%s', %sRIGHT%s: '%s'\n",
                _S_BRIGHT_YELLOW_BOLD,
                metadata->fn_name,
                _S_RESET_FORMAT,
                metadata->file,
                metadata->line,
                _S_BRIGHT_BLUE_BOLD,
                _S_RESET_FORMAT,
                metadata->src,
                _S_BRIGHT_MAGENTA_BOLD,
                _S_RESET_FORMAT,
                metadata->dst);
    }
}

typedef struct _stest {
    const char* name;
    fnptr       fn;
} stest;

struct _ssuite {
    const char* name;
    slist*      tests;
};

stest*
stest_new(const char* name, fnptr fn)
{
    stest* test = (stest*) malloc(sizeof(stest));
    if(test != NULL) {
        test->name = name;
        test->fn   = fn;
    }
    return test;
}

ssuite*
ssuite_new(const char* name)
{
    if(name != NULL) {
        ssuite* suite = (ssuite*) malloc(sizeof(ssuite));
        if(suite != NULL) {
            suite->name = name;
            if((suite->tests = slist_new()) == NULL) {
                free(suite);
                return NULL;
            }
        }
        return suite;
    }
    return NULL;
}

void
_ssuite_add_test(ssuite* suite, const char* name, void (*fn)(void))
{
    if(suite != NULL && name != NULL && fn != NULL) {
        stest* test = stest_new(name, fn);
        if(test == NULL)
            return;

        if(slist_insert_front(suite->tests, test) != 0) {
            fprintf(stderr, "out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
}

void
ssuite_run_tests(const ssuite* suite)
{
    if(suite != NULL) {
        slistIterator* iterator = slist_iterator(suite->tests);
        if(iterator != NULL) {
            const stest* test;
            while((test = slistiter_next(iterator)) != NULL) {
                PRINT_TEST(test->name);
                clock_t tic = clock();
                test->fn();
                clock_t toc = clock();

                long double time_elapsed = (double) (toc - tic) / CLOCKS_PER_SEC;

                slist* entry;
                if((entry = smap_key(&__S_GLOBAL_TABLE__, (void*) test->name)) != NULL) {
                    PRINT_FAILURE(time_elapsed);

                    slistIterator* entry_iterator = slist_iterator_rev(entry);

                    if(entry_iterator == NULL) {
                        fprintf(stderr, "out of memory\n");
                        exit(EXIT_FAILURE);
                    }

                    const smdata* current;
                    while((current = slistiter_next_back(entry_iterator)) != NULL)
                        smdata_print(current);
                } else
                    PRINT_SUCCESS(time_elapsed);
            }
        }
    }
}

void
stest_free(stest* test)
{
    if(test != NULL)
        free(test);
}

void
ssuite_free(ssuite* suite)
{
    if(suite != NULL) {
        slist_free(suite->tests, (FreeFn) stest_free);
        free(suite);
    }
}

struct _srunner {
    slist* suites;
};

srunner*
srunner_new(void)
{
    srunner* runner = (srunner*) malloc(sizeof(srunner));

    if(runner != NULL)
        if((runner->suites = slist_new()) == NULL) {
            free(runner);
            return NULL;
        }

    return runner;
}

void
srunner_add_suite(srunner* runner, ssuite* suite)
{
    if(runner != NULL && suite != NULL) {
        if(slist_insert_front(runner->suites, suite) != 0) {
            fprintf(stderr, "out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
}

void
srunner_add_suites(srunner* runner, ssuite** suites, size_t len)
{
    if(runner != NULL && suites != NULL && len != 0)
        while(len--)
            srunner_add_suite(runner, *suites++);
}

void
srunner_run(srunner* runner)
{
    if(runner != NULL) {
        slistIterator* iterator = slist_iterator(runner->suites);
        if(iterator == NULL) {
            fprintf(stdout, "No suites to run...");
            return;
        }

        const ssuite* current;

        fprintf(stdout, "Running all suites...\n");

        clock_t tic = clock();

        while((current = slistiter_next(iterator)) != NULL)
            ssuite_run_tests(current);

        clock_t toc = clock();

        double elapsed_time = (double) (toc - tic) / CLOCKS_PER_SEC;
        fprintf(stdout, "Total elapsed time: %f seconds\n", elapsed_time);
    }
}

void
srunner_free(srunner* runner)
{
    slist_free(runner->suites, (FreeFn) ssuite_free);
    free(runner);
}
