#define __S_SRC_FILE__
#include "smdata.h"
#include "ssuite.h"
#include "stable.h"
#undef __S_SRC_FILE__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct _stest stest;

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
__ssuite_add_test(ssuite* suite, const char* name, fnptr fn)
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

                    slistIterator* entry_iterator = slist_iterator(entry);

                    if(entry_iterator == NULL) {
                        fprintf(stderr, "out of memory\n");
                        exit(EXIT_FAILURE);
                    }

                    const smdata* current;
                    while((current = slistiter_next(entry_iterator)) != NULL)
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
