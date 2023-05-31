#define __S_SUITE_C__
#include "ssuite.h"
#undef __S_SUITE_C__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct _stest {
    const char* name;
    fnptr       fn;
};

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

/*
 * Internal only, defined as macro in order
 * to stringify the fn pointer that gets passed.
 */
void
_ssuite_add_test(ssuite* suite, const char* name, fnptr fn)
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
                // print test
                clock_t tic = clock();
                test->fn();
                clock_t toc = clock();
                // if failed then print failed and
                // print all fails
                // else print success
                // fail or success append time to the print
            }
        }
    }
}

void
ssuite_free(ssuite* suite, FreeFn free_fn)
{
    if(suite != NULL) {
        slist_free(suite->tests, free_fn);
        free(suite);
    }
}

void
stest_free(stest* test)
{
    if(test != NULL)
        free(test);
}
