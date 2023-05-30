#include "stest.h"
#include <assert.h>
#include <time.h>
#define __S_TEST_C__
#include "stest_internal.h"
#undef __S_TEST_C__

#include <stdlib.h>

#define PRINT_TEST(filename) \
    fprintf(stdout, "Running test \x1b[1;33m%s\x1b[0m ----> ", filename)
#define PRINT_SUCCESS    fprintf(stdout, "\x1b[1;32msucceeded\x1b[0m [%d]\n", msec)
#define PRINT_FAILURE    fprintf(stdout, "\x1b[1;31mfailed\x1b[0m [%d]\n", msec)
#define PRINT_INVALID    fprintf(stdout, "\x1b[1;91mtest invalid\x1b[0m [%d]\n", msec)
#define PRINT_DESC(desc) fprintf(stdout, "\x1b[1;33mError desc:\x1b[0m %s\n", desc)

#ifndef TABLESIZE
#define TABLESIZE 100
#endif

typedef struct _list list;

typedef struct _Map {
    list** table;
} Map;

Map GlobalTable = { .table = NULL };

typedef void (*FreeFn)(void*);

typedef void (*fnptr)(void);

typedef struct _Test {
    const char* name;
    fnptr       fn;
} Test;

typedef struct _FailEntry {
    unsigned int linen;
    const char*  desc;
} Failentry;

struct _list {
    void*         element;
    struct _list* next;
};

struct _Suite {
    const char* name;
    list*       tests;
};

struct _Runner {
    list* suites;
};

Failentry*
failentry_new(const char* desc, int linen)
{
    Failentry* entry = (Failentry*) malloc(sizeof(Failentry));
    if(entry != NULL) {
        entry->desc = desc;
        entry->linen = linen;
    }
    return entry;
}

list*
list_new(list* next, void* element)
{
    assert(element != NULL);
    list* ls = (list*) malloc(sizeof(list));
    if(ls != NULL) {
        ls->element = element;
        ls->next    = next;
    }
    return ls;
}

long unsigned int
hash(void* val)
{
    long unsigned int    hashvalue;
    const unsigned char* p = (const unsigned char*) val;

    for(hashvalue = 0; *p; val++)
        hashvalue = *p + 31 * hashvalue;

    return hashvalue % TABLESIZE;
}

Map*
map_new(void)
{
    Map* map = (Map*) malloc(sizeof(Map));

    if(map != NULL)
        map->table = NULL;

    return map;
}

void*
map_insert(Map* map, void* key, void* value)
{
    long unsigned int hashvalue = hash((char*) key);
    list*             current;

    if(map->table == NULL
       && (map->table = (list**) calloc(TABLESIZE, sizeof(list*))) == NULL)
        return NULL;

    list* new_head;
    if((new_head = list_new(NULL, value)) == NULL)
        return NULL;

    if((current = map->table[hashvalue]) != NULL)
        new_head->next = current;

    map->table[hashvalue] = new_head;

    return NULL;
}

Test*
test_new(const char* name, fnptr fn)
{
    assert(fn != NULL);
    Test* test = (Test*) malloc(sizeof(Test));
    if(test != NULL) {
        test->name = name;
        test->fn   = fn;
    }
    return test;
}

Suite*
suite_new(const char* name)
{
    if(name != NULL) {
        Suite* suite = (Suite*) malloc(sizeof(Suite));
        if(suite != NULL) {
            suite->name  = name;
            suite->tests = NULL;
        }
    }
    return NULL;
}

Runner*
runner_new(void)
{
    Runner* runner = (Runner*) malloc(sizeof(Runner));

    if(runner != NULL)
        runner->suites = NULL;

    return runner;
}

void
suite_add_test(Suite* suite, const char* name, fnptr fn)
{
    if(suite != NULL && name != NULL && fn != NULL) {
        Test* test = test_new(name, fn);
        if(test == NULL)
            return;

        list* new_head = list_new(NULL, test);
        if(new_head == NULL)
            return;

        new_head->next = suite->tests;
        suite->tests   = new_head;
    }
}

void
suite_run_tests(Suite* suite)
{
    list* current;
    for(current = suite->tests; current != NULL; current = current->next) {
        PRINT_TEST(((Test*) current->element)->name);
        ((Test*) current->element)->fn();
    }
}

void
runner_add_suite(Runner* runner, Suite* suite)
{
    if(runner != NULL && suite != NULL) {
        list* new_head = list_new(NULL, suite);
        if(new_head == NULL)
            return;

        new_head->next = runner->suites;
        runner->suites = new_head;
    }
}

void
runner_add_suites(Runner* runner, Suite** suites, size_t len)
{
    if(runner != NULL && suites != NULL && len != 0)
        while(len--)
            runner_add_suite(runner, *suites++);
}

void
runner_run(Runner* runner)
{
    if(runner != NULL) {
        list* current;

        clock_t tic = clock();

        for(current = runner->suites; current != NULL; current = current->next)
            suite_run_tests(current->element);

        clock_t toc = clock();

        double elapsed_time = (double) (toc - tic) / CLOCKS_PER_SEC;
        fprintf(stdout, "Total elapsed time: %f seconds\n", elapsed_time);
    }
}

void
test_free(Test* test)
{
    test->fn   = NULL;
    test->name = NULL;
    free(test);
}

void
list_free(list* list, FreeFn free_fn)
{
    if(list != NULL) {
        list_free(list->next, free_fn);
        if(free_fn)
            free_fn(list->element);
        free(list);
    }
}

void
suite_free(Suite* suite, FreeFn free_fn)
{
    suite->name = NULL;
    list_free(suite->tests, (FreeFn) test_free);
    free(suite);
}

void
runner_free(Runner* runner)
{
    list_free(runner->suites, (FreeFn) suite_free);
    free(runner);
}

int     __test_start = 0;
int     __test_end   = 0;
int     msec         = 0;
clock_t start_time;

int   errc = 0;
char* errors[MAXERROR];

static char*
atype_to_str(assert_t atype)
{
    switch(atype) {
        case ASSERT:
            return "ASSERT";
        case ASSERT_EQ:
            return "ASSERT_EQ";
        case ASSERT_STREQ:
            return "ASSERT_STREQ";
        case ASSERT_NEQ:
            return "ASSERT_NEQ";
    }
}

void
assert_failed(assert_t atype, char* desc, int line)
{
    char* ds = (desc == NULL) ? "No description" : desc;
    char* t  = atype_to_str(atype);

    sprintf(errors[errc++], "%s failed at line %d: %s\n", t, line, ds);
}

void
start_test(char* filename)
{
    __test_start = 1;
    msec         = 0;
    PRINT_TEST(filename);
    start_time = clock();
}

void
end_test(void)
{
    if(__test_start == 0) {
        PRINT_INVALID;
    } else {
        clock_t difference = clock() - start_time;
        msec               = difference * 1000 / CLOCKS_PER_SEC;
        __test_end         = 1;

        if(errc == 0)
            PRINT_SUCCESS;
        else {
            PRINT_FAILURE;
            for(int i = 0; i < errc; i++)
                PRINT_DESC(errors[errc]);
            errc = 0;
        }
    }
}
