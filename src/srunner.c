#define __S_SRC_FILE__
#include "slist.h"
#include "srunner.h"
#undef __S_SRC_FILE__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct _srunner {
    slist* suites;
};

srunner*
runner_new(void)
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
runner_add_suite(srunner* runner, ssuite* suite)
{
    if(runner != NULL && suite != NULL) {
        if(slist_insert_front(runner->suites, suite) != 0) {
            fprintf(stderr, "out of memory\n");
            exit(EXIT_FAILURE);
        }
    }
}

void
runner_add_suites(srunner* runner, ssuite** suites, size_t len)
{
    if(runner != NULL && suites != NULL && len != 0)
        while(len--)
            runner_add_suite(runner, *suites++);
}

void
runner_run(srunner* runner)
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
runner_free(srunner* runner)
{
    slist_free(runner->suites, (FreeFn) ssuite_free);
    free(runner);
}
